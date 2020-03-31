#include <QAction>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QMenu>
#include "pls-app.hpp"
#include "window-basic-main.hpp"
#include "display-helpers.hpp"
#include "qt-wrappers.hpp"
#include "platform.hpp"

const int LABEL_MARGIN = 10;

static QList<PLSProjector *> windowedProjectors;
static QList<PLSProjector *> multiviewProjectors;
static bool updatingMultiview = false, drawLabel, drawSafeArea, mouseSwitching, transitionOnDoubleClick;
static MultiviewLayout multiviewLayout;
static size_t maxSrcs, numSrcs;

PLSProjector::PLSProjector(QWidget *widget, obs_source_t *source_, int monitor, QString title, ProjectorType type_)
	: PLSQTDisplay(widget, Qt::Window), source(source_), removedSignal(obs_source_get_signal_handler(source), "remove", OBSSourceRemoved, this)
{
	setMouseTracking(true);
	setCursor(Qt::ArrowCursor);

	projectorTitle = std::move(title);
	savedMonitor = monitor;
	isWindow = savedMonitor < 0;
	type = type_;

	if (isWindow) {
		UpdateProjectorTitle(projectorTitle);
		windowedProjectors.push_back(this);
	}

	auto addDrawCallback = [this]() {
		bool isMultiview = type == ProjectorType::Multiview;
		obs_display_add_draw_callback(GetDisplay(), isMultiview ? PLSRenderMultiview : PLSRender, this);
		obs_display_set_background_color(GetDisplay(), 0x000000);
	};

	auto adjustResize = [this](QLabel *scr, QLabel *view, bool &handled) {
		QSize size = GetPixelSize(scr);
		view->setGeometry(0, 0, size.width(), size.height());
		handled = true;
	};

	connect(this, &PLSQTDisplay::DisplayCreated, addDrawCallback);
	connect(this, &PLSQTDisplay::AdjustResizeView, adjustResize);

	if (type == ProjectorType::Multiview) {
		obs_enter_graphics();

		// All essential action should be placed inside this area
		gs_render_start(true);
		gs_vertex2f(actionSafePercentage, actionSafePercentage);
		gs_vertex2f(actionSafePercentage, 1 - actionSafePercentage);
		gs_vertex2f(1 - actionSafePercentage, 1 - actionSafePercentage);
		gs_vertex2f(1 - actionSafePercentage, actionSafePercentage);
		gs_vertex2f(actionSafePercentage, actionSafePercentage);
		actionSafeMargin = gs_render_save();

		// All graphics should be placed inside this area
		gs_render_start(true);
		gs_vertex2f(graphicsSafePercentage, graphicsSafePercentage);
		gs_vertex2f(graphicsSafePercentage, 1 - graphicsSafePercentage);
		gs_vertex2f(1 - graphicsSafePercentage, 1 - graphicsSafePercentage);
		gs_vertex2f(1 - graphicsSafePercentage, graphicsSafePercentage);
		gs_vertex2f(graphicsSafePercentage, graphicsSafePercentage);
		graphicsSafeMargin = gs_render_save();

		// 4:3 safe area for widescreen
		gs_render_start(true);
		gs_vertex2f(fourByThreeSafePercentage, graphicsSafePercentage);
		gs_vertex2f(1 - fourByThreeSafePercentage, graphicsSafePercentage);
		gs_vertex2f(1 - fourByThreeSafePercentage, 1 - graphicsSafePercentage);
		gs_vertex2f(fourByThreeSafePercentage, 1 - graphicsSafePercentage);
		gs_vertex2f(fourByThreeSafePercentage, graphicsSafePercentage);
		fourByThreeSafeMargin = gs_render_save();

		gs_render_start(true);
		gs_vertex2f(0.0f, 0.5f);
		gs_vertex2f(lineLength, 0.5f);
		leftLine = gs_render_save();

		gs_render_start(true);
		gs_vertex2f(0.5f, 0.0f);
		gs_vertex2f(0.5f, lineLength);
		topLine = gs_render_save();

		gs_render_start(true);
		gs_vertex2f(1.0f, 0.5f);
		gs_vertex2f(1 - lineLength, 0.5f);
		rightLine = gs_render_save();
		obs_leave_graphics();

		solid = obs_get_base_effect(OBS_EFFECT_SOLID);
		color = gs_effect_get_param_by_name(solid, "color");

		UpdateMultiview();

		multiviewProjectors.push_back(this);
	}

	App()->IncrementSleepInhibition();

	if (source)
		obs_source_inc_showing(source);

	ready = true;
}

PLSProjector::~PLSProjector()
{
	bool isMultiview = type == ProjectorType::Multiview;
	obs_display_remove_draw_callback(GetDisplay(), isMultiview ? PLSRenderMultiview : PLSRender, this);

	if (source)
		obs_source_dec_showing(source);

	if (isMultiview) {
		for (OBSWeakSource &weakSrc : multiviewScenes) {
			OBSSource src = OBSGetStrongRef(weakSrc);
			if (src)
				obs_source_dec_showing(src);
		}

		obs_enter_graphics();
		gs_vertexbuffer_destroy(actionSafeMargin);
		gs_vertexbuffer_destroy(graphicsSafeMargin);
		gs_vertexbuffer_destroy(fourByThreeSafeMargin);
		gs_vertexbuffer_destroy(leftLine);
		gs_vertexbuffer_destroy(topLine);
		gs_vertexbuffer_destroy(rightLine);
		obs_leave_graphics();
	}

	if (type == ProjectorType::Multiview)
		multiviewProjectors.removeAll(this);

	if (isWindow)
		windowedProjectors.removeAll(this);

	for (auto tex : multiLabelTexture) {
		gs_texture_destroy(tex->render_texture);
		gs_texture_destroy(tex->shader_input_texture);
		delete tex;
		tex = nullptr;
	}
	multiLabelTexture.clear();

	App()->DecrementSleepInhibition();
}

static OBSSource CreateLabel(const char *name, size_t h)
{
	obs_data_t *settings = obs_data_create();
	obs_data_t *font = obs_data_create();

	std::string text;
	text += " ";
	text += name;
	text += " ";

#if defined(_WIN32)
	obs_data_set_string(font, "face", "Arial");
#elif defined(__APPLE__)
	obs_data_set_string(font, "face", "Helvetica");
#else
	obs_data_set_string(font, "face", "Monospace");
#endif
	obs_data_set_int(font, "flags", 1); // Bold text
	obs_data_set_int(font, "size", int(h / 9.81));

	obs_data_set_obj(settings, "font", font);
	obs_data_set_string(settings, "text", text.c_str());
	obs_data_set_bool(settings, "outline", false);

#ifdef _WIN32
	const char *text_source_id = "text_gdiplus";
#else
	const char *text_source_id = "text_ft2_source";
#endif

	OBSSource txtSource = obs_source_create_private(text_source_id, name, settings);
	obs_source_release(txtSource);

	obs_data_release(font);
	obs_data_release(settings);

	return txtSource;
}

static inline uint32_t labelOffset(obs_source_t *label, uint32_t cx)
{
	uint32_t w = obs_source_get_width(label);

	int n; // Number of scenes per row
	switch (multiviewLayout) {
	case MultiviewLayout::HORIZONTAL_TOP_24_SCENES:
		n = 6;
		break;
	default:
		n = 4;
		break;
	}

	w = uint32_t(w * ((1.0f) / n));
	int32_t offset = cx * 0.5f - w;
	return offset > 0 ? offset : 0;
}

static inline bool isLabelOverRegion(obs_source_t *label, uint32_t cx)
{
	uint32_t w = obs_source_get_width(label);

	int n; // Number of scenes per row
	switch (multiviewLayout) {
	case MultiviewLayout::HORIZONTAL_TOP_24_SCENES:
		n = 6;
		break;
	default:
		n = 4;
		break;
	}

	w = uint32_t(w * ((1.0f) / n));
	if (cx * 0.5f > w) {
		return false;
	}
	return true;
}

static inline void startRegion(int vX, int vY, int vCX, int vCY, float oL, float oR, float oT, float oB)
{
	gs_projection_push();
	gs_viewport_push();
	gs_set_viewport(vX, vY, vCX, vCY);
	gs_ortho(oL, oR, oT, oB, -100.0f, 100.0f);
}

static inline void endRegion()
{
	gs_viewport_pop();
	gs_projection_pop();
}

static inline void set_render_size(uint32_t width, uint32_t height)
{
	gs_enable_depth_test(false);
	gs_set_cull_mode(GS_NEITHER);

	gs_ortho(0.0f, (float)width, 0.0f, (float)height, -100.0f, 100.0f);
	gs_set_viewport(0, 0, width, height);
}

static inline void check_label_texture_valid(gs_texture **tex, int width, int height, uint32_t flag)
{
	if (!(*tex)) {
		*tex = gs_texture_create(width, height, GS_RGBA, 1, NULL, flag);
		return;
	}

	if (gs_texture_get_width(*tex) != width || gs_texture_get_height(*tex) != height) {
		gs_texture_destroy(*tex);
		*tex = gs_texture_create(width, height, GS_RGBA, 1, NULL, flag);
	}
}

void PLSProjector::PLSRenderMultiview(void *data, uint32_t cx, uint32_t cy)
{
	PLSProjector *window = (PLSProjector *)data;

	if (updatingMultiview || !window->ready)
		return;

	PLSBasic *main = (PLSBasic *)obs_frontend_get_main_window();
	uint32_t targetCX, targetCY;
	int x, y;
	float scale;

	targetCX = (uint32_t)window->fw;
	targetCY = (uint32_t)window->fh;

	GetScaleAndCenterPos(targetCX, targetCY, cx, cy, x, y, scale);

	OBSSource previewSrc = main->GetCurrentSceneSource();
	OBSSource programSrc = main->GetProgramSource();
	bool studioMode = main->IsPreviewProgramMode();

	auto renderVB = [&](gs_vertbuffer_t *vb, int cx, int cy, uint32_t colorVal) {
		if (!vb)
			return;

		matrix4 transform;
		matrix4_identity(&transform);
		transform.x.x = cx;
		transform.y.y = cy;

		gs_load_vertexbuffer(vb);

		gs_matrix_push();
		gs_matrix_mul(&transform);

		gs_effect_set_color(window->color, colorVal);
		while (gs_effect_loop(window->solid, "Solid"))
			gs_draw(GS_LINESTRIP, 0, 0);

		gs_matrix_pop();
	};

	auto drawBox = [&](float cx, float cy, uint32_t colorVal) {
		gs_effect_set_color(window->color, colorVal);
		while (gs_effect_loop(window->solid, "Solid"))
			gs_draw_sprite(nullptr, 0, (uint32_t)cx, (uint32_t)cy);
	};

	auto setRegion = [&](float bx, float by, float cx, float cy) {
		float vX = int(x + bx * scale);
		float vY = int(y + by * scale);
		float vCX = int(cx * scale);
		float vCY = int(cy * scale);

		float oL = bx;
		float oT = by;
		float oR = (bx + cx);
		float oB = (by + cy);

		startRegion(vX, vY, vCX, vCY, oL, oR, oT, oB);
	};

	auto calcBaseSource = [&](size_t i) {
		switch (multiviewLayout) {
		case MultiviewLayout::HORIZONTAL_TOP_24_SCENES:
			window->sourceX = (i % 6) * window->scenesCX;
			window->sourceY = window->pvwprgCY + (i / 6) * window->scenesCY;
			break;
		case MultiviewLayout::VERTICAL_LEFT_8_SCENES:
			window->sourceX = window->pvwprgCX;
			window->sourceY = (i / 2) * window->scenesCY;
			if (i % 2 != 0)
				window->sourceX += window->scenesCX;
			break;
		case MultiviewLayout::VERTICAL_RIGHT_8_SCENES:
			window->sourceX = 0;
			window->sourceY = (i / 2) * window->scenesCY;
			if (i % 2 != 0)
				window->sourceX = window->scenesCX;
			break;
		case MultiviewLayout::HORIZONTAL_BOTTOM_8_SCENES:
			if (i < 4) {
				window->sourceX = (float(i) * window->scenesCX);
				window->sourceY = 0;
			} else {
				window->sourceX = (float(i - 4) * window->scenesCX);
				window->sourceY = window->scenesCY;
			}
			break;
		default: // MultiviewLayout::HORIZONTAL_TOP_8_SCENES:
			if (i < 4) {
				window->sourceX = (float(i) * window->scenesCX);
				window->sourceY = window->pvwprgCY;
			} else {
				window->sourceX = (float(i - 4) * window->scenesCX);
				window->sourceY = window->pvwprgCY + window->scenesCY;
			}
		}
		window->siX = window->sourceX + window->thickness;
		window->siY = window->sourceY + window->thickness;
	};

	auto calcPreviewProgram = [&](bool program) {
		switch (multiviewLayout) {
		case MultiviewLayout::HORIZONTAL_TOP_24_SCENES:
			window->sourceX = window->thickness + window->pvwprgCX / 2;
			window->sourceY = window->thickness;
			window->labelX = window->offset + window->pvwprgCX / 2;
			window->labelY = window->pvwprgCY * 0.85f;
			if (program) {
				window->sourceX += window->pvwprgCX;
				window->labelX += window->pvwprgCX;
			}
			break;
		case MultiviewLayout::VERTICAL_LEFT_8_SCENES:
			window->sourceX = window->thickness;
			window->sourceY = window->pvwprgCY + window->thickness;
			window->labelX = window->offset;
			window->labelY = window->pvwprgCY * 1.85f;
			if (program) {
				window->sourceY = window->thickness;
				window->labelY = window->pvwprgCY * 0.85f;
			}
			break;
		case MultiviewLayout::VERTICAL_RIGHT_8_SCENES:
			window->sourceX = window->pvwprgCX + window->thickness;
			window->sourceY = window->pvwprgCY + window->thickness;
			window->labelX = window->pvwprgCX + window->offset;
			window->labelY = window->pvwprgCY * 1.85f;
			if (program) {
				window->sourceY = window->thickness;
				window->labelY = window->pvwprgCY * 0.85f;
			}
			break;
		case MultiviewLayout::HORIZONTAL_BOTTOM_8_SCENES:
			window->sourceX = window->thickness;
			window->sourceY = window->pvwprgCY + window->thickness;
			window->labelX = window->offset;
			window->labelY = window->pvwprgCY * 1.85f;
			if (program) {
				window->sourceX += window->pvwprgCX;
				window->labelX += window->pvwprgCX;
			}
			break;
		default: // MultiviewLayout::HORIZONTAL_TOP_8_SCENES:
			window->sourceX = window->thickness;
			window->sourceY = window->thickness;
			window->labelX = window->offset;
			window->labelY = window->pvwprgCY * 0.85f;
			if (program) {
				window->sourceX += window->pvwprgCX;
				window->labelX += window->pvwprgCX;
			}
		}
	};

	auto paintAreaWithColor = [&](float tx, float ty, float cx, float cy, uint32_t color) {
		gs_matrix_push();
		gs_matrix_translate3f(tx, ty, 0.0f);
		drawBox(cx, cy, color);
		gs_matrix_pop();
	};

	// Define the whole usable region for the multiview
	startRegion(x, y, targetCX * scale, targetCY * scale, 0.0f, window->fw, 0.0f, window->fh);

	// Change the background color to highlight all sources
	drawBox(window->fw, window->fh, outerColor);

	/* ----------------------------- */
	/* draw sources                  */

	for (size_t i = 0; i < maxSrcs; i++) {
		// Handle all the offsets
		calcBaseSource(i);

		if (i >= numSrcs) {
			// Just paint the background and continue
			paintAreaWithColor(window->sourceX, window->sourceY, window->scenesCX, window->scenesCY, outerColor);
			paintAreaWithColor(window->siX, window->siY, window->siCX, window->siCY, backgroundColor);
			continue;
		}

		OBSSource src = OBSGetStrongRef(window->multiviewScenes[i]);

		// We have a source. Now chose the proper highlight color
		uint32_t colorVal = outerColor;
		if (src == programSrc)
			colorVal = programColor;
		else if (src == previewSrc)
			colorVal = studioMode ? previewColor : programColor;

		// Paint the background
		paintAreaWithColor(window->sourceX, window->sourceY, window->scenesCX, window->scenesCY, colorVal);
		paintAreaWithColor(window->siX, window->siY, window->siCX, window->siCY, backgroundColor);

		/* ----------- */

		// Render the source
		gs_matrix_push();
		gs_matrix_translate3f(window->siX, window->siY, 0.0f);
		gs_matrix_scale3f(window->siScaleX, window->siScaleY, 1.0f);
		setRegion(window->siX, window->siY, window->siCX, window->siCY);
		obs_source_video_render(src);
		endRegion();
		gs_matrix_pop();

		/* ----------- */

		// Render the label
		if (!drawLabel)
			continue;

		obs_source *label = window->multiviewLabels[i + 2];
		if (!label)
			continue;

		window->offset = labelOffset(label, window->scenesCX);
		bool bOverRegion = isLabelOverRegion(label, window->scenesCX);
		if (!bOverRegion) {
			gs_rect rt;
			gs_get_viewport(&rt);
			gs_matrix_push();
			gs_matrix_translate3f(window->sourceX + window->offset, (window->scenesCY * 0.85f) + window->sourceY, 0.0f);
			gs_matrix_scale3f(window->ppiScaleX, window->ppiScaleY, 1.0f);
			drawBox(obs_source_get_width(label), obs_source_get_height(label) + int(window->sourceY * 0.015f), labelColor);
			obs_source_video_render(label);
			gs_matrix_pop();
		} else {
			label_texture *label_tex = window->multiLabelTexture[i + 2];
			if (!label_tex) {
				continue;
			}
			int src_width = obs_source_get_width(label);
			int src_height = obs_source_get_height(label);
			check_label_texture_valid(&label_tex->render_texture, src_width, src_height, GS_RENDER_TARGET);
			check_label_texture_valid(&label_tex->shader_input_texture, src_width, src_height, GS_DYNAMIC);

			gs_texture_t *preTarget = gs_get_render_target();
			gs_set_render_target(label_tex->render_texture, NULL);
			struct vec4 clear_color;
			vec4_set(&clear_color, 0.0f, 0.0f, 0.0f, 0.0f);
			gs_clear(GS_CLEAR_COLOR, &clear_color, 1.0f, 0);
			gs_viewport_push();
			gs_projection_push();
			set_render_size(src_width, src_height);

			obs_source_video_render(label);

			gs_set_render_target(preTarget, NULL);
			gs_matrix_pop();
			gs_viewport_pop();
			gs_projection_pop();

			gs_matrix_push();
			gs_matrix_translate3f(window->sourceX + window->offset + LABEL_MARGIN, (window->scenesCY * 0.85f) + window->sourceY, 0.0f);
			gs_matrix_scale3f(window->ppiScaleX, window->ppiScaleY, 1.0f);
			drawBox(window->pvwprgCX - LABEL_MARGIN * 2, obs_source_get_height(label) + int(window->sourceY * 0.015f), labelColor);
			gs_matrix_pop();

			gs_matrix_push();
			gs_matrix_translate3f(window->sourceX + window->offset + LABEL_MARGIN, (window->scenesCY * 0.85f) + window->sourceY + int(window->sourceY * 0.015f * 0.5f), 0.0f);
			gs_matrix_scale3f(window->ppiScaleX, window->ppiScaleY, 1.0f);

			gs_effect_t *effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
			gs_technique_t *tech = gs_effect_get_technique(effect, "Draw");

			gs_technique_begin(tech);
			gs_technique_begin_pass(tech, 0);

			gs_copy_texture(label_tex->shader_input_texture, label_tex->render_texture);

			gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"), label_tex->shader_input_texture);
			gs_draw_sprite_subregion(label_tex->shader_input_texture, 0, 0, 0, window->pvwprgCX - LABEL_MARGIN * 3, src_height);

			gs_technique_end_pass(tech);
			gs_technique_end(tech);

			gs_matrix_pop();
		}
	}

	/* ----------------------------- */
	/* draw preview                  */

	obs_source_t *previewLabel = window->multiviewLabels[0];
	window->offset = labelOffset(previewLabel, window->pvwprgCX);
	calcPreviewProgram(false);

	// Paint the background
	paintAreaWithColor(window->sourceX, window->sourceY, window->ppiCX, window->ppiCY, backgroundColor);

	// Scale and Draw the preview
	gs_matrix_push();
	gs_matrix_translate3f(window->sourceX, window->sourceY, 0.0f);
	gs_matrix_scale3f(window->ppiScaleX, window->ppiScaleY, 1.0f);
	setRegion(window->sourceX, window->sourceY, window->ppiCX, window->ppiCY);
	if (studioMode)
		obs_source_video_render(previewSrc);
	else
		obs_render_main_texture();
	if (drawSafeArea) {
		renderVB(window->actionSafeMargin, targetCX, targetCY, outerColor);
		renderVB(window->graphicsSafeMargin, targetCX, targetCY, outerColor);
		renderVB(window->fourByThreeSafeMargin, targetCX, targetCY, outerColor);
		renderVB(window->leftLine, targetCX, targetCY, outerColor);
		renderVB(window->topLine, targetCX, targetCY, outerColor);
		renderVB(window->rightLine, targetCX, targetCY, outerColor);
	}
	endRegion();
	gs_matrix_pop();

	/* ----------- */

	// Draw the Label
	if (drawLabel) {
		gs_matrix_push();
		gs_matrix_translate3f(window->labelX, window->labelY, 0.0f);
		gs_matrix_scale3f(window->ppiScaleX, window->ppiScaleY, 1.0f);
		drawBox(obs_source_get_width(previewLabel), obs_source_get_height(previewLabel) + int(window->pvwprgCX * 0.015f), labelColor);
		obs_source_video_render(previewLabel);
		gs_matrix_pop();
	}

	/* ----------------------------- */
	/* draw program                  */

	obs_source_t *programLabel = window->multiviewLabels[1];
	window->offset = labelOffset(programLabel, window->pvwprgCX);
	calcPreviewProgram(true);

	paintAreaWithColor(window->sourceX, window->sourceY, window->ppiCX, window->ppiCY, backgroundColor);

	// Scale and Draw the program
	gs_matrix_push();
	gs_matrix_translate3f(window->sourceX, window->sourceY, 0.0f);
	gs_matrix_scale3f(window->ppiScaleX, window->ppiScaleY, 1.0f);
	setRegion(window->sourceX, window->sourceY, window->ppiCX, window->ppiCY);
	obs_render_main_texture();
	endRegion();
	gs_matrix_pop();

	/* ----------- */

	// Draw the Label
	if (drawLabel) {
		gs_matrix_push();
		gs_matrix_translate3f(window->labelX, window->labelY, 0.0f);
		gs_matrix_scale3f(window->ppiScaleX, window->ppiScaleY, 1.0f);
		drawBox(obs_source_get_width(programLabel), obs_source_get_height(programLabel) + int(window->pvwprgCX * 0.015f), labelColor);
		obs_source_video_render(programLabel);
		gs_matrix_pop();
	}

	// Region for future usage with additional info.
	if (multiviewLayout == MultiviewLayout::HORIZONTAL_TOP_24_SCENES) {
		// Just paint the background for now
		paintAreaWithColor(window->thickness, window->thickness, window->siCX, window->siCY * 2 + window->thicknessx2, backgroundColor);
		paintAreaWithColor(window->thickness + 2.5 * (window->thicknessx2 + window->ppiCX), window->thickness, window->siCX, window->siCY * 2 + window->thicknessx2, backgroundColor);
	}

	endRegion();
}

void PLSProjector::PLSRender(void *data, uint32_t cx, uint32_t cy)
{
	PLSProjector *window = reinterpret_cast<PLSProjector *>(data);

	if (!window->ready)
		return;

	PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
	OBSSource source = window->source;

	uint32_t targetCX;
	uint32_t targetCY;
	int x, y;
	int newCX, newCY;
	float scale;

	if (source) {
		targetCX = std::max(obs_source_get_width(source), 1u);
		targetCY = std::max(obs_source_get_height(source), 1u);
	} else {
		struct obs_video_info ovi;
		obs_get_video_info(&ovi);
		targetCX = ovi.base_width;
		targetCY = ovi.base_height;
	}

	GetScaleAndCenterPos(targetCX, targetCY, cx, cy, x, y, scale);

	newCX = int(scale * float(targetCX));
	newCY = int(scale * float(targetCY));

	startRegion(x, y, newCX, newCY, 0.0f, float(targetCX), 0.0f, float(targetCY));

	if (window->type == ProjectorType::Preview && main->IsPreviewProgramMode()) {
		OBSSource curSource = main->GetCurrentSceneSource();

		if (source != curSource) {
			obs_source_dec_showing(source);
			obs_source_inc_showing(curSource);
			source = curSource;
			window->source = source;
		}
	} else if (window->type == ProjectorType::Preview && !main->IsPreviewProgramMode()) {
		window->source = nullptr;
	}

	if (source)
		obs_source_video_render(source);
	else
		obs_render_main_texture();

	endRegion();
}

void PLSProjector::OBSSourceRemoved(void *data, calldata_t *params)
{
	PLSProjector *window = reinterpret_cast<PLSProjector *>(data);
	window->EscapeTriggered();

	UNUSED_PARAMETER(params);
}

static int getSourceByPosition(int x, int y, float ratio)
{
	int pos = -1;
	QWidget *rec = QApplication::activeWindow();
	if (!rec)
		return pos;
	int cx = rec->width();
	int cy = rec->height();
	int minX = 0;
	int minY = 0;
	int maxX = cx;
	int maxY = cy;

	switch (multiviewLayout) {
	case MultiviewLayout::HORIZONTAL_TOP_24_SCENES:
		if (float(cx) / float(cy) > ratio) {
			int validX = cy * ratio;
			minX = (cx / 2) - (validX / 2);
			maxX = (cx / 2) + (validX / 2);
			minY = cy / 3;
		} else {
			int validY = cx / ratio;
			maxY = (cy / 2) + (validY / 2);
			minY = (cy / 2) - (validY / 6);
		}

		if (x < minX || x > maxX || y < minY || y > maxY)
			break;

		pos = (x - minX) / ((maxX - minX) / 6);
		pos += ((y - minY) / ((maxY - minY) / 4)) * 6;

		break;
	case MultiviewLayout::VERTICAL_LEFT_8_SCENES:
		if (float(cx) / float(cy) > ratio) {
			int validX = cy * ratio;
			maxX = (cx / 2) + (validX / 2);
		} else {
			int validY = cx / ratio;
			minY = (cy / 2) - (validY / 2);
			maxY = (cy / 2) + (validY / 2);
		}

		minX = cx / 2;

		if (x < minX || x > maxX || y < minY || y > maxY)
			break;

		pos = 2 * ((y - minY) / ((maxY - minY) / 4));
		if (x > minX + ((maxX - minX) / 2))
			pos++;
		break;
	case MultiviewLayout::VERTICAL_RIGHT_8_SCENES:
		if (float(cx) / float(cy) > ratio) {
			int validX = cy * ratio;
			minX = (cx / 2) - (validX / 2);
		} else {
			int validY = cx / ratio;
			minY = (cy / 2) - (validY / 2);
			maxY = (cy / 2) + (validY / 2);
		}

		maxX = (cx / 2);

		if (x < minX || x > maxX || y < minY || y > maxY)
			break;

		pos = 2 * ((y - minY) / ((maxY - minY) / 4));
		if (x > minX + ((maxX - minX) / 2))
			pos++;
		break;
	case MultiviewLayout::HORIZONTAL_BOTTOM_8_SCENES:
		if (float(cx) / float(cy) > ratio) {
			int validX = cy * ratio;
			minX = (cx / 2) - (validX / 2);
			maxX = (cx / 2) + (validX / 2);
		} else {
			int validY = cx / ratio;
			minY = (cy / 2) - (validY / 2);
		}

		maxY = (cy / 2);

		if (x < minX || x > maxX || y < minY || y > maxY)
			break;

		pos = (x - minX) / ((maxX - minX) / 4);
		if (y > minY + ((maxY - minY) / 2))
			pos += 4;
		break;
	default: // MultiviewLayout::HORIZONTAL_TOP_8_SCENES
		if (float(cx) / float(cy) > ratio) {
			int validX = cy * ratio;
			minX = (cx / 2) - (validX / 2);
			maxX = (cx / 2) + (validX / 2);
		} else {
			int validY = cx / ratio;
			maxY = (cy / 2) + (validY / 2);
		}

		minY = (cy / 2);

		if (x < minX || x > maxX || y < minY || y > maxY)
			break;

		pos = (x - minX) / ((maxX - minX) / 4);
		if (y > minY + ((maxY - minY) / 2))
			pos += 4;
	}

	return pos;
}

void PLSProjector::mouseDoubleClickEvent(QMouseEvent *event)
{
	PLSQTDisplay::mouseDoubleClickEvent(event);

	if (!mouseSwitching)
		return;

	if (!transitionOnDoubleClick)
		return;

	PLSBasic *main = (PLSBasic *)obs_frontend_get_main_window();
	if (!main->IsPreviewProgramMode())
		return;

	if (event->button() == Qt::LeftButton) {
		int pos = getSourceByPosition(event->x(), event->y(), ratio);
		if (pos < 0 || pos >= (int)numSrcs)
			return;
		OBSSource src = OBSGetStrongRef(multiviewScenes[pos]);
		if (!src)
			return;

		if (main->GetProgramSource() != src)
			main->TransitionToScene(src);
	}
}

void PLSProjector::mouseMoveEvent(QMouseEvent *event)
{
	bool shouldHideCursor = (!isWindow && type != ProjectorType::Multiview && config_get_bool(GetGlobalConfig(), "BasicWindow", "HideProjectorCursor"));
	if (cursorHidden != shouldHideCursor) {
		cursorHidden = shouldHideCursor;
		setCursor(shouldHideCursor ? Qt::BlankCursor : Qt::ArrowCursor);
	}

	__super::mouseMoveEvent(event);
}

void PLSProjector::mousePressEvent(QMouseEvent *event)
{
	PLSQTDisplay::mousePressEvent(event);

	if (event->button() == Qt::RightButton) {
		QMenu popup(this);
		popup.addAction(QTStr("Close"), this, SLOT(EscapeTriggered()));
		popup.exec(QCursor::pos());
	}

	if (!mouseSwitching)
		return;

	if (event->button() == Qt::LeftButton) {
		int pos = getSourceByPosition(event->x(), event->y(), ratio);
		if (pos < 0 || pos >= (int)numSrcs)
			return;
		OBSSource src = OBSGetStrongRef(multiviewScenes[pos]);
		if (!src)
			return;

		PLSBasic *main = (PLSBasic *)obs_frontend_get_main_window();
		if (main->GetCurrentSceneSource() != src)
			main->SetCurrentScene(src, false);
	}
}

void PLSProjector::EscapeTriggered()
{
	PLSDialogView *dialogView = dynamic_cast<PLSDialogView *>(pls_get_toplevel_view(this));
	dialogView->deleteLater();
}

void PLSProjector::UpdateMultiview()
{
	multiviewScenes.clear();
	multiviewLabels.clear();

	for (auto tex : multiLabelTexture) {
		gs_texture_destroy(tex->render_texture);
		gs_texture_destroy(tex->shader_input_texture);
		delete tex;
		tex = nullptr;
	}
	multiLabelTexture.clear();

	struct obs_video_info ovi;
	obs_get_video_info(&ovi);

	uint32_t w = ovi.base_width;
	uint32_t h = ovi.base_height;
	fw = float(w);
	fh = float(h);
	ratio = fw / fh;

	struct obs_frontend_source_list scenes = {};
	obs_frontend_get_scenes(&scenes);

	multiviewLabels.emplace_back(CreateLabel("EDIT", h / 2));
	multiviewLabels.emplace_back(CreateLabel("LIVE", h / 2));

	label_texture *label_tex = new label_texture;
	multiLabelTexture.emplace_back(label_tex);
	label_tex = new label_texture;
	multiLabelTexture.emplace_back(label_tex);

	multiviewLayout = static_cast<MultiviewLayout>(config_get_int(GetGlobalConfig(), "BasicWindow", "MultiviewLayout"));

	drawLabel = config_get_bool(GetGlobalConfig(), "BasicWindow", "MultiviewDrawNames");

	drawSafeArea = config_get_bool(GetGlobalConfig(), "BasicWindow", "MultiviewDrawAreas");

	mouseSwitching = config_get_bool(GetGlobalConfig(), "BasicWindow", "MultiviewMouseSwitch");

	transitionOnDoubleClick = config_get_bool(GetGlobalConfig(), "BasicWindow", "TransitionOnDoubleClick");

	switch (multiviewLayout) {
	case MultiviewLayout::HORIZONTAL_TOP_24_SCENES:
		pvwprgCX = fw / 3;
		pvwprgCY = fh / 3;

		maxSrcs = 24;
		break;
	default:
		pvwprgCX = fw / 2;
		pvwprgCY = fh / 2;

		maxSrcs = 8;
	}

	ppiCX = pvwprgCX - thicknessx2;
	ppiCY = pvwprgCY - thicknessx2;
	ppiScaleX = (pvwprgCX - thicknessx2) / fw;
	ppiScaleY = (pvwprgCY - thicknessx2) / fh;

	scenesCX = pvwprgCX / 2;
	scenesCY = pvwprgCY / 2;
	siCX = scenesCX - thicknessx2;
	siCY = scenesCY - thicknessx2;
	siScaleX = (scenesCX - thicknessx2) / fw;
	siScaleY = (scenesCY - thicknessx2) / fh;

	numSrcs = 0;
	size_t i = 0;
	while (i < scenes.sources.num && numSrcs < maxSrcs) {
		obs_source_t *src = scenes.sources.array[i++];
		OBSData data = obs_source_get_private_settings(src);
		obs_data_release(data);

		obs_data_set_default_bool(data, "show_in_multiview", true);
		if (!obs_data_get_bool(data, "show_in_multiview"))
			continue;

		// We have a displayable source.
		numSrcs++;

		multiviewScenes.emplace_back(OBSGetWeakRef(src));
		obs_source_inc_showing(src);

		std::string name = std::to_string(numSrcs) + " - " + obs_source_get_name(src);
		multiviewLabels.emplace_back(CreateLabel(name.c_str(), h / 3));

		label_tex = new label_texture;
		multiLabelTexture.emplace_back(label_tex);
	}

	obs_frontend_source_list_free(&scenes);
}

void PLSProjector::UpdateProjectorTitle(QString name)
{
	projectorTitle = name;

	QString title = nullptr;
	switch (type) {
	case ProjectorType::Scene:
		title = QTStr("SceneWindow") + " - " + name;
		break;
	case ProjectorType::Source:
		title = QTStr("SourceWindow") + " - " + name;
		break;
	default:
		title = name;
		break;
	}

	setWindowTitle(title);
}

OBSSource PLSProjector::GetSource()
{
	return source;
}

ProjectorType PLSProjector::GetProjectorType()
{
	return type;
}

int PLSProjector::GetMonitor()
{
	return savedMonitor;
}

void PLSProjector::UpdateMultiviewProjectors()
{
	obs_enter_graphics();
	updatingMultiview = true;
	obs_leave_graphics();

	for (auto &projector : multiviewProjectors)
		projector->UpdateMultiview();

	obs_enter_graphics();
	updatingMultiview = false;
	obs_leave_graphics();
}

void PLSProjector::RenameProjector(QString oldName, QString newName)
{
	for (auto &projector : windowedProjectors)
		if (projector->projectorTitle == oldName)
			projector->UpdateProjectorTitle(newName);
}
