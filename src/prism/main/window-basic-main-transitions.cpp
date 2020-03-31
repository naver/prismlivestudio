/******************************************************************************
    Copyright (C) 2016 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <QWidgetAction>
#include <QToolTip>
#include <util/dstr.hpp>
#include "window-basic-main.hpp"
#include "display-helpers.hpp"
#include "window-namedialog.hpp"
#include "menu-button.hpp"
#include "qt-wrappers.hpp"
#include "spinbox.hpp"
#include "PLSSceneDataMgr.h"
#include "main-view.hpp"
#include "pls-common-define.hpp"

#include "obs-hotkey.h"

using namespace std;

Q_DECLARE_METATYPE(OBSScene);
Q_DECLARE_METATYPE(OBSSource);

void PLSBasic::InitDefaultTransitions()
{
	std::vector<OBSSource> transitions;
	size_t idx = 0;
	const char *id;

	/* automatically add transitions that have no configuration (things
	 * such as cut/fade/etc) */
	while (obs_enum_transition_types(idx++, &id)) {
		if (!obs_is_source_configurable(id)) {
			const char *name = obs_source_get_display_name(id);

			obs_source_t *tr = obs_source_create_private(id, name, NULL);

			ui->scenesFrame->InitTransition(tr);
			transitions.emplace_back(tr);

			if (strcmp(id, "cut_transition") == 0)
				cutTransition = tr;

			obs_source_release(tr);
		}
	}
	ui->scenesFrame->AddTransitionsItem(transitions);
}

static inline OBSSource GetTransitionComboItem(QComboBox *combo, int idx)
{
	return combo->itemData(idx).value<OBSSource>();
}

void PLSBasic::SetCutTransition(obs_source_t *source)
{
	cutTransition = source;
}

void PLSBasic::TransitionToScene(OBSScene scene, bool force)
{
	obs_source_t *source = obs_scene_get_source(scene);
	TransitionToScene(source, force);
}

void PLSBasic::TransitionStopped()
{
	if (swapScenesMode) {
		OBSSource scene = OBSGetStrongRef(swapScene);
		if (scene)
			SetCurrentScene(scene);

		// Make sure we re-enable the transition button
		ui->previewTitle->transApply->setEnabled(true);
	}

	if (api) {
		api->on_event(OBS_FRONTEND_EVENT_TRANSITION_STOPPED);
		api->on_event(OBS_FRONTEND_EVENT_SCENE_CHANGED);
	}

	swapScene = nullptr;
}

void PLSBasic::OverrideTransition(OBSSource transition)
{
	obs_source_t *oldTransition = obs_get_output_source(0);

	if (transition != oldTransition) {
		obs_transition_swap_begin(transition, oldTransition);
		obs_set_output_source(0, transition);
		obs_transition_swap_end(transition, oldTransition);
	}

	obs_source_release(oldTransition);
}

void PLSBasic::TransitionFullyStopped()
{
	if (overridingTransition) {
		OverrideTransition(ui->scenesFrame->GetCurrentTransition());
		overridingTransition = false;
	}
}

void PLSBasic::TransitionToScene(OBSSource source, bool force, bool quickTransition, int quickDuration, bool black)
{
	obs_scene_t *scene = obs_scene_from_source(source);
	bool usingPreviewProgram = IsPreviewProgramMode();
	if (!scene)
		return;

	OBSWeakSource lastProgramScene;

	if (usingPreviewProgram) {
		lastProgramScene = programScene;
		programScene = OBSGetWeakRef(source);

		if (swapScenesMode && !force && !black) {
			OBSSource newScene = OBSGetStrongRef(lastProgramScene);

			if (!sceneDuplicationMode && newScene == source)
				return;

			if (newScene && newScene != GetCurrentSceneSource())
				swapScene = lastProgramScene;
		}
	}

	if (usingPreviewProgram && sceneDuplicationMode) {
		scene = obs_scene_duplicate(scene, obs_source_get_name(obs_scene_get_source(scene)), editPropertiesMode ? OBS_SCENE_DUP_PRIVATE_COPY : OBS_SCENE_DUP_PRIVATE_REFS);
		source = obs_scene_get_source(scene);
	}

	OBSSource transition = obs_get_output_source(0);
	obs_source_release(transition);

	float t = obs_transition_get_time(transition);
	bool stillTransitioning = t < 1.0f && t > 0.0f;
	//bool stillTransitioning = obs_transition_get_time(transition) < 1.0f;

	// If actively transitioning, block new transitions from starting
	if (usingPreviewProgram && stillTransitioning)
		goto cleanup;

	if (force) {
		obs_transition_set(transition, source);
		if (api)
			api->on_event(OBS_FRONTEND_EVENT_SCENE_CHANGED);
	} else {
		/* check for scene override */
		OBSData data = obs_source_get_private_settings(source);
		obs_data_release(data);

		const char *trOverrideName = obs_data_get_string(data, "transition");
		int duration = ui->scenesFrame->GetTransitionDurationValue();

		if (trOverrideName && *trOverrideName && !quickTransition && !overridingTransition) {
			OBSSource trOverride = ui->scenesFrame->FindTransition(trOverrideName);
			if (trOverride) {
				transition = trOverride;

				obs_data_set_default_int(data, "transition_duration", 300);

				duration = (int)obs_data_get_int(data, "transition_duration");
				OverrideTransition(trOverride);
				overridingTransition = true;
			}
		}

		if (black && !prevFTBSource) {
			source = nullptr;
			prevFTBSource = obs_transition_get_active_source(transition);
			obs_source_release(prevFTBSource);
		} else if (black && prevFTBSource) {
			source = prevFTBSource;
			prevFTBSource = nullptr;
		} else if (!black) {
			prevFTBSource = nullptr;
		}

		if (quickTransition)
			duration = quickDuration;

		bool success = obs_transition_start(transition, OBS_TRANSITION_MODE_AUTO, duration, source);
		if (!success)
			TransitionFullyStopped();
	}

	// If transition has begun, disable Transition button
	if (usingPreviewProgram && stillTransitioning) {
		ui->previewTitle->transApply->setEnabled(false);
	}

cleanup:
	if (usingPreviewProgram && sceneDuplicationMode)
		obs_scene_release(scene);
}

void PLSBasic::OnTransitionAdded()
{
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_TRANSITION_LIST_CHANGED);
}

void PLSBasic::onTransitionRemoved(OBSSource source)
{
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_TRANSITION_LIST_CHANGED);
	this->DeletePropertiesWindow(source);
}

void PLSBasic::OnTransitionRenamed()
{
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_TRANSITION_LIST_CHANGED);
}

void PLSBasic::OnTransitionSet()
{
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_TRANSITION_CHANGED);
}

void PLSBasic::OnTransitionDurationValueChanged(int value)
{
	if (api)
		api->on_event(pls_frontend_event::PLS_FRONTEND_EVENT_TRANSITION_DURATION_CHANGED, {value});
}

void PLSBasic::SetCurrentScene(obs_scene_t *scene, bool force)
{
	obs_source_t *source = obs_scene_get_source(scene);
	SetCurrentScene(source, force);
}

template<typename T> static T GetPLSRef(QListWidgetItem *item)
{
	return item->data(static_cast<int>(QtDataRole::PLSRef)).value<T>();
}

void PLSBasic::SetCurrentScene(OBSSource scene, bool force)
{
	if (!IsPreviewProgramMode()) {
		TransitionToScene(scene, force);
	} else {
		OBSSource actualLastScene = OBSGetStrongRef(lastScene);
		if (actualLastScene != scene) {
			if (scene)
				obs_source_inc_showing(scene);
			if (actualLastScene)
				obs_source_dec_showing(actualLastScene);
			lastScene = OBSGetWeakRef(scene);
		}
	}

	if (obs_scene_get_source(GetCurrentScene()) != scene) {
		SceneDisplayVector data = PLSSceneDataMgr::Instance()->GetDisplayVector();
		for (auto iter = data.begin(); iter != data.end(); ++iter) {
			PLSSceneItemView *item = iter->second;
			OBSScene itemScene = item->GetData();
			obs_source_t *source = obs_scene_get_source(itemScene);

			if (source == scene) {
				isCopyScene = false;
				ui->scenesFrame->SetCurrentItem(item);
				if (api)
					api->on_event(OBS_FRONTEND_EVENT_PREVIEW_SCENE_CHANGED);
				break;
			}
		}
	}

	UpdateSceneSelection(scene);

	bool userSwitched = (!force && !disableSaving);
	blog(LOG_INFO, "%s to scene '%s'", userSwitched ? "User switched" : "Switched", obs_source_get_name(scene));
}

void PLSBasic::CreateProgramDisplay()
{
	program = new PLSQTDisplay(ui->previewContainer);
	program->setObjectName(OBJECT_MAIN_PREVIEW);
	PLSDpiHelper::dpiDynamicUpdate(program.data());

	program->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(program.data(), &QWidget::customContextMenuRequested, this, &PLSBasic::on_program_customContextMenuRequested);

	auto displayResize = [this]() {
		struct obs_video_info ovi;

		if (obs_get_video_info(&ovi))
			ResizeProgram(ovi.base_width, ovi.base_height);
	};

	auto adjustResize = [this](QLabel *scr, QLabel *view, bool &handled) {
		struct obs_video_info ovi;
		if (obs_get_video_info(&ovi)) {
			int x = 0;
			int y = 0;
			QSize targetSize = GetPixelSize(program);
			GetScaleAndCenterPos(int(ovi.base_width), int(ovi.base_height), targetSize.width() - PREVIEW_EDGE_SIZE * 2, targetSize.height() - PREVIEW_EDGE_SIZE * 2, x, y, programScale);

			x += float(PREVIEW_EDGE_SIZE);
			y += float(PREVIEW_EDGE_SIZE);

			int cx = int(programScale * float(ovi.base_width));
			int cy = int(programScale * float(ovi.base_height));

			view->setGeometry(x, y, cx, cy);
			handled = true;
		}
	};

	auto addDisplay = [this](PLSQTDisplay *window) {
		obs_display_add_draw_callback(window->GetDisplay(), PLSBasic::RenderProgram, this);

		struct obs_video_info ovi;
		if (obs_get_video_info(&ovi))
			ResizeProgram(ovi.base_width, ovi.base_height);
	};

	connect(program.data(), &PLSQTDisplay::DisplayResized, displayResize);
	connect(program.data(), &PLSQTDisplay::AdjustResizeView, adjustResize);
	connect(program.data(), &PLSQTDisplay::DisplayCreated, addDisplay);

	program->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void PLSBasic::TransitionClicked()
{
	PLS_UI_STEP(MAINFRAME_MODULE, "Apply Button", ACTION_CLICK);
	if (previewProgramMode)
		TransitionToScene(GetCurrentScene());
}

QMenu *PLSBasic::CreatePerSceneTransitionMenu()
{
	OBSSource scene = GetCurrentSceneSource();
	QMenu *menu = new QMenu(QTStr("TransitionOverride"), ui->scenesFrame);

	QAction *action;

	OBSData data = obs_source_get_private_settings(scene);
	obs_data_release(data);

	obs_data_set_default_int(data, "transition_duration", 300);

	const char *curTransition = obs_data_get_string(data, "transition");
	int curDuration = (int)obs_data_get_int(data, "transition_duration");

	QSpinBox *duration = new PLSSpinBox(menu);
	duration->setMinimum(50);
	duration->setSuffix("ms");
	duration->setMaximum(20000);
	duration->setSingleStep(50);
	duration->setValue(curDuration);

	auto setTransition = [this](QAction *action) {
		PLS_UI_STEP(MAINMENU_MODULE, "Per Scene Transition", ACTION_CLICK);
		int idx = action->property("transition_index").toInt();
		OBSSource scene = GetCurrentSceneSource();
		OBSData data = obs_source_get_private_settings(scene);
		obs_data_release(data);

		if (idx == -1) {
			obs_data_set_string(data, "transition", "");
			return;
		}

		OBSSource tr = ui->scenesFrame->GetTransitionComboItem(idx);
		const char *name = obs_source_get_name(tr);

		obs_data_set_string(data, "transition", name);
	};

	auto setDuration = [this](int duration) {
		PLS_UI_STEP(MAINMENU_MODULE, "Per Scene Transition Duration", ACTION_CLICK);

		OBSSource scene = GetCurrentSceneSource();
		OBSData data = obs_source_get_private_settings(scene);
		obs_data_release(data);

		obs_data_set_int(data, "transition_duration", duration);
	};

	connect(duration, (void (QSpinBox::*)(int)) & QSpinBox::valueChanged, setDuration);

	for (int i = -1; i < ui->scenesFrame->GetTransitionComboBoxCount(); i++) {
		const char *name = "";

		if (i >= 0) {
			OBSSource tr;
			tr = ui->scenesFrame->GetTransitionComboItem(i);
			name = obs_source_get_name(tr);
		}

		bool match = (name && strcmp(name, curTransition) == 0);

		if (!name || !*name)
			name = Str("None");

		action = menu->addAction(QT_UTF8(name));
		action->setProperty("transition_index", i);
		action->setCheckable(true);
		action->setChecked(match);

		connect(action, &QAction::triggered, std::bind(setTransition, action));
	}

	QWidgetAction *durationAction = new QWidgetAction(menu);
	durationAction->setDefaultWidget(duration);

	menu->addSeparator();
	menu->addAction(durationAction);
	return menu;
}

void PLSBasic::SetPreviewProgramMode(bool enabled)
{
	if (IsPreviewProgramMode() == enabled)
		return;

	// update main menu studio mode state
	mainView->setStudioMode(enabled);
	// update right bar studio mode button state
	ui->actionStudioMode->setText(tr(enabled ? "Basic.MainMenu.View.StudioModeOff" : "Basic.MainMenu.View.StudioModeOn"));
	ui->actionStudioMode->setChecked(enabled);

	os_atomic_set_bool(&previewProgramMode, enabled);
	ui->previewTitle->OnStudioModeStatus(enabled);

	if (IsPreviewProgramMode()) {
		if (!previewEnabled)
			EnablePreviewDisplay(true);

		CreateProgramDisplay();

		OBSScene curScene = GetCurrentScene();

		obs_scene_t *dup;
		if (sceneDuplicationMode) {
			dup = obs_scene_duplicate(curScene, obs_source_get_name(obs_scene_get_source(curScene)), editPropertiesMode ? OBS_SCENE_DUP_PRIVATE_COPY : OBS_SCENE_DUP_PRIVATE_REFS);
		} else {
			dup = curScene;
			obs_scene_addref(dup);
		}

		obs_source_t *transition = obs_get_output_source(0);
		obs_source_t *dup_source = obs_scene_get_source(dup);
		obs_transition_set(transition, dup_source);
		obs_source_release(transition);
		obs_scene_release(dup);

		if (curScene) {
			obs_source_t *source = obs_scene_get_source(curScene);
			obs_source_inc_showing(source);
			lastScene = OBSGetWeakRef(source);
			programScene = OBSGetWeakRef(source);
		}

		ui->perviewLayoutHrz->addWidget(program);

		if (api)
			api->on_event(OBS_FRONTEND_EVENT_STUDIO_MODE_ENABLED);

		blog(LOG_INFO, "Switched to Preview/Program mode");
		blog(LOG_INFO, "------------------------------------------------");
	} else {
		OBSSource actualProgramScene = OBSGetStrongRef(programScene);
		if (!actualProgramScene)
			actualProgramScene = GetCurrentSceneSource();
		else
			SetCurrentScene(actualProgramScene, true);
		TransitionToScene(actualProgramScene, true);

		delete program;

		if (lastScene) {
			OBSSource actualLastScene = OBSGetStrongRef(lastScene);
			if (actualLastScene)
				obs_source_dec_showing(actualLastScene);
			lastScene = nullptr;
		}

		programScene = nullptr;
		swapScene = nullptr;

		if (!previewEnabled)
			EnablePreviewDisplay(false);

		if (api)
			api->on_event(OBS_FRONTEND_EVENT_STUDIO_MODE_DISABLED);

		blog(LOG_INFO, "Switched to regular Preview mode");
		blog(LOG_INFO, "-----------------------------"
			       "-------------------");
	}

	ResetUI();
	UpdateTitleBar();
}

void PLSBasic::RenderProgram(void *data, uint32_t cx, uint32_t cy)
{
	GS_DEBUG_MARKER_BEGIN(GS_DEBUG_COLOR_DEFAULT, "RenderProgram");

	PLSBasic *window = static_cast<PLSBasic *>(data);
	obs_video_info ovi;

	obs_get_video_info(&ovi);

	window->programCX = int(window->programScale * float(ovi.base_width));
	window->programCY = int(window->programScale * float(ovi.base_height));

	gs_viewport_push();
	gs_projection_push();

	/* --------------------------------------- */

	gs_ortho(0.0f, float(ovi.base_width), 0.0f, float(ovi.base_height), -100.0f, 100.0f);
	gs_set_viewport(window->programX, window->programY, window->programCX, window->programCY);

	obs_render_main_texture_src_color_only();
	gs_load_vertexbuffer(nullptr);

	/* --------------------------------------- */

	gs_projection_pop();
	gs_viewport_pop();

	GS_DEBUG_MARKER_END();

	UNUSED_PARAMETER(cx);
	UNUSED_PARAMETER(cy);
}

void PLSBasic::ResizeProgram(uint32_t cx, uint32_t cy)
{
	QSize targetSize;

	/* resize program panel to fix to the top section of the window */
	targetSize = GetPixelSize(program);
	GetScaleAndCenterPos(int(cx), int(cy), targetSize.width() - PREVIEW_EDGE_SIZE * 2, targetSize.height() - PREVIEW_EDGE_SIZE * 2, programX, programY, programScale);

	programX += float(PREVIEW_EDGE_SIZE);
	programY += float(PREVIEW_EDGE_SIZE);
}
