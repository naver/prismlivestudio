/******************************************************************************
 Copyright (C) 2014 by John R. Bradley <jrb@turrettech.com>
 Copyright (C) 2018 by Hugh Bailey ("Jim") <jim@obsproject.com>

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

#include "text-motion-source.hpp"
#include "browser-client.hpp"
#include "browser-scheme.hpp"
#include "wide-string.hpp"
#include <util/threading.h>
#include <QApplication>
#include <util/dstr.h>
#include <functional>
#include <thread>
#include <mutex>
#include "obs-frontend-api.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <QTimer>
#include <QFile>
#include <qcolor.h>
#include <QDir>
#include <QUrl>
#include <qdebug.h>

#ifdef USE_QT_LOOP
#include <QEventLoop>
#include <QThread>
#endif

using namespace std;

#define S_FONT "font"
#define S_TEXT "text"
#define S_COLOR "color"
#define S_BKCOLOR "bk_color"
#define S_FACE "face"
#define S_TM_TEXT "TextMotion.Text"
#define S_TM_TEXT_CONTENT "textmotion.text.content"
#define S_TM_TAB "textmotion.tab"
#define S_TM_TEMPLATE_TAB "textmotion.template.tab"
#define S_TM_TEMPLATE_LIST "textmotion.template.list"
#define S_TM_COLOR "textmotion.color"
#define S_TM_MOTION "textmotion.motion"

extern bool QueueCEFTask(std::function<void()> task);
extern "C" void obs_browser_initialize(void);
extern void DispatchJSEvent(BrowserSource *source, std::string eventName,
			    std::string jsonString);

static mutex browser_list_mutex;
static TextMotionSource *first_browser = nullptr;
struct TMTemplateAttr;
static QMap<int, TMTemplateAttr> g_TMTemplateJsons;
static bool isFirstShow = true;
void browser_source_get_defaults(obs_data_t *settings);
obs_properties_t *browser_source_get_properties(void *data);

void SendBrowserVisibility(CefRefPtr<CefBrowser> browser, bool isVisible);
void ExecuteOnAllBrowsers(BrowserFunc func);
void DispatchJSEvent(std::string eventName, std::string jsonString);

static inline uint32_t rgb_to_bgr(uint32_t rgb)
{
	return ((rgb & 0xFF) << 16) | (rgb & 0xFF00) | ((rgb & 0xFF0000) >> 16);
}
static int getItemId(const QString &id)
{
	if (id.contains('_')) {
		return id.split('_').last().toInt();
	}
	return 0;
}
static QString getTextAlign(const int index)
{
	switch (index) {
	case 0:
		return "left";
	case 1:
		return "center";
	case 2:
		return "right";
	default:
		return "";
	}
}
static int getTextAlignIndex(const QString &alignStr)
{
	if (0 == alignStr.compare("left", Qt::CaseInsensitive)) {
		return 0;
	} else if (0 == alignStr.compare("center", Qt::CaseInsensitive)) {
		return 1;
	} else if (0 == alignStr.compare("right", Qt::CaseInsensitive)) {
		return 2;
	} else {
		return -1;
	}
}
static QString getLetter(const int index)
{
	switch (index) {
	case 0:
		return "uppercase";
	case 1:
		return "lowercase";
	default:
		return "none";
		break;
	}
}
static int getLetterIndex(const QString &letterStr)
{
	if (0 == letterStr.compare("up", Qt::CaseInsensitive)) {
		return 0;
	} else if (0 == letterStr.compare("down", Qt::CaseInsensitive)) {
		return 1;
	} else {
		return -1;
	}
}

static inline long long color_to_int(const QString &colorStr)
{
	QString colorStr_(colorStr.trimmed());
	if (colorStr_.isEmpty() || colorStr_ == "#000000") {
		return 0;
	}
	QColor color(colorStr_);
	auto shift = [&](unsigned val, int shift) {
		return ((val & 0xff) << shift);
	};

	return shift(color.red(), 0) | shift(color.green(), 8) |
	       shift(color.blue(), 16) | shift(color.alpha(), 24);
}
using TMTemplateAttr = struct TMTemplateAttr {
	QString name;
	int textCount;
	QString firstText;
	QString secText;
	bool isFont;
	QString fontFamily;
	QString fontWeight;
	bool isHorizontalAlignment;
	int hAlignValue;
	bool isSize;
	int fontSizeValue;
	int letterValue;
	bool isTextColor;
	int textColorValue;
	bool isTextTransparent;
	int textTransparentValue;
	bool isBackgroundOn;
	bool isBackgroundColor;
	int backgroundColorValue;
	bool isBackgroundTransparent;
	int backgroundTransparentValue;
	bool isStrokeOn;
	bool isStrokeColor;
	int strokeColorValue;
	int strokeLineValue;
	bool isRotate;
	bool isMotion;
	int motionValue;
};
static void attachTMInfo()
{
	if (!g_TMTemplateJsons.isEmpty())
		return;
	typedef int(__cdecl * PfnGetConfigPath)(char *path, size_t size,
						const char *name);
	extern PfnGetConfigPath GetConfigPath;

	char configPath[1024];
	int ret = GetConfigPath(
		configPath, sizeof(configPath),
		"PRISMLiveStudio/textmotion/textMotionTemplate.json");
	QString templatePath(configPath);
	if (!QFileInfo(templatePath).exists()) {
		QDir appDir(qApp->applicationDirPath());
		templatePath = appDir.absoluteFilePath(
			"data/prism-studio/text_motion/textMotionTemplate.json");
	}
	QFile file(templatePath);
	if (!file.open(QFile::ReadOnly | QIODevice::Text)) {
		return;
	}
	QByteArray array = file.readAll();
	file.close();
	QString lang = QString(obs_get_locale()).split('-')[0];

	QJsonArray category =
		QJsonDocument::fromJson(array).object().value("group").toArray();
	for (auto cate : category) {
		QJsonArray itemObj = cate.toObject().value("items").toArray();
		for (auto item : itemObj) {
			QJsonObject itemObj = item.toObject();
			QJsonObject itemAttrObj =
				itemObj.value("properties")
					.toObject()
					.value("template")
					.toObject()
					.value("baseInfoPC")
					.toObject()
					.value("adjustableAttribute")
					.toObject();
			QJsonObject contentObj = itemObj.value("properties")
							 .toObject()
							 .value("template")
							 .toObject()
							 .value(lang)
							 .toObject();
			QJsonObject fontObj =
				itemAttrObj.value(lang).toObject();

			TMTemplateAttr templateAttr;
			templateAttr.name = itemObj.value("title").toString();
			templateAttr.firstText =
				contentObj.value("textInfo1").toString();
			templateAttr.secText =
				contentObj.value("textInfo2").toString();
			templateAttr.textCount =
				templateAttr.secText.isEmpty() ? 1 : 2;
			templateAttr.isFont = true;
			templateAttr.fontFamily =
				fontObj.value("font").toString().simplified();
			templateAttr.fontWeight =
				fontObj.value("fontStyle").toString();
			templateAttr.isHorizontalAlignment =
				itemAttrObj.value("isHorizontalAlignment")
					.toBool();
			templateAttr.hAlignValue = getTextAlignIndex(
				itemAttrObj.value("horizontalAlignment")
					.toString());
			templateAttr.isSize = true;
			templateAttr.fontSizeValue =
				itemAttrObj.value("fontSize").toInt();
			templateAttr.letterValue = getLetterIndex(
				itemAttrObj.value("fontUpperCase").toString());
			templateAttr.isTextColor = true;
			templateAttr.textColorValue = color_to_int(
				itemAttrObj.value("fontColor").toString());
			templateAttr.isTextTransparent = true;
			templateAttr.textTransparentValue = 100;

			templateAttr.isBackgroundOn =
				itemAttrObj.value("isBackgroundColor").toBool();
			templateAttr.isBackgroundColor =
				itemAttrObj.value("isBackgroundOn").toBool();
			templateAttr.backgroundColorValue = color_to_int(
				itemAttrObj.value("backgroundColor").toString());
			templateAttr.isBackgroundTransparent = true;
			templateAttr.backgroundTransparentValue = 100;

			templateAttr.isStrokeOn = true;
			templateAttr.isStrokeColor =
				itemAttrObj.value("isOutlineColor").toBool();
			templateAttr.strokeColorValue = color_to_int(
				itemAttrObj.value("outLineColor").toString());
			templateAttr.strokeLineValue =
				itemAttrObj.value("outLineSize")
					.toString()
					.toInt();

			templateAttr.isRotate =
				itemAttrObj.value("isAgain").toBool();
			templateAttr.isMotion = true;

			templateAttr.motionValue = 50;

			g_TMTemplateJsons.insert(
				getItemId(itemObj.value("itemId").toString()),
				templateAttr);
		}
	}
}
void set_tm_text_content_data(obs_data_t *settings,
			      const TMTemplateAttr &tm_attr, bool is_default)
{
	obs_data_t *tm_text_content_obj = obs_data_create();
	if (is_default) {
		obs_data_set_default_bool(tm_text_content_obj,
					  "text-content-change-1", false);
		obs_data_set_default_bool(tm_text_content_obj,
					  "text-content-change-2", false);
		obs_data_set_default_int(tm_text_content_obj, "text-count",
					 tm_attr.textCount);
		obs_data_set_default_string(tm_text_content_obj,
					    "text-content-1",
					    tm_attr.firstText.toUtf8());
		obs_data_set_default_string(tm_text_content_obj,
					    "text-content-2",
					    tm_attr.secText.toUtf8());
		obs_data_set_default_obj(settings, S_TM_TEXT_CONTENT,
					 tm_text_content_obj);
	} else {
		obs_data_t *val = obs_data_get_obj(settings, S_TM_TEXT_CONTENT);
		QString firText = obs_data_get_string(val, "text-content-1");
		QString secText = obs_data_get_string(val, "text-content-2");
		bool isFirstTextChange =
			obs_data_get_bool(val, "text-content-change-1");
		bool isSecTextChange =
			obs_data_get_bool(val, "text-content-change-2");

		obs_data_set_int(tm_text_content_obj, "text-count",
				 tm_attr.textCount);
		if (isFirstTextChange) {
			obs_data_set_string(tm_text_content_obj,
					    "text-content-1", firText.toUtf8());
		} else {
			obs_data_set_string(tm_text_content_obj,
					    "text-content-1",
					    tm_attr.firstText.toUtf8());
		}

		if (isSecTextChange) {
			obs_data_set_string(tm_text_content_obj,
					    "text-content-2", secText.toUtf8());
		} else {
			obs_data_set_string(tm_text_content_obj,
					    "text-content-2",
					    tm_attr.secText.toUtf8());
		}
		obs_data_set_bool(tm_text_content_obj, "text-content-change-1",
				  isFirstTextChange);
		obs_data_set_bool(tm_text_content_obj, "text-content-change-2",
				  isSecTextChange);
		obs_data_set_obj(settings, S_TM_TEXT_CONTENT,
				 tm_text_content_obj);
		obs_data_release(val);
	}
	obs_data_release(tm_text_content_obj);
}
void set_tm_text_data(obs_data_t *settings, const TMTemplateAttr &tm_attr,
		      bool is_default)
{
	obs_data_t *tm_text_obj = obs_data_create();
	if (is_default) {
		obs_data_set_default_bool(tm_text_obj, "is-font",
					  tm_attr.isFont);
		obs_data_set_default_string(tm_text_obj, "font-family",
					    tm_attr.fontFamily.toUtf8());
		obs_data_set_default_string(tm_text_obj, "font-weight",
					    tm_attr.fontWeight.toUtf8());
		obs_data_set_default_bool(tm_text_obj, "is-h-aligin",
					  tm_attr.isHorizontalAlignment);
		obs_data_set_default_int(tm_text_obj, "h-aligin",
					 tm_attr.hAlignValue);
		obs_data_set_default_bool(tm_text_obj, "is-font-size",
					  tm_attr.isSize);
		obs_data_set_default_int(tm_text_obj, "font-size",
					 tm_attr.fontSizeValue);
		obs_data_set_default_int(tm_text_obj, "letter",
					 tm_attr.letterValue);
		obs_data_set_default_obj(settings, S_TM_TEXT, tm_text_obj);

	} else {
		obs_data_set_bool(tm_text_obj, "is-font", tm_attr.isFont);
		obs_data_set_string(tm_text_obj, "font-family",
				    tm_attr.fontFamily.toUtf8());
		obs_data_set_string(tm_text_obj, "font-weight",
				    tm_attr.fontWeight.toUtf8());
		obs_data_set_bool(tm_text_obj, "is-h-aligin",
				  tm_attr.isHorizontalAlignment);
		obs_data_set_int(tm_text_obj, "h-aligin", tm_attr.hAlignValue);
		obs_data_set_bool(tm_text_obj, "is-font-size", tm_attr.isSize);
		obs_data_set_int(tm_text_obj, "font-size",
				 tm_attr.fontSizeValue);
		obs_data_set_int(tm_text_obj, "letter", tm_attr.letterValue);
		obs_data_set_obj(settings, S_TM_TEXT, tm_text_obj);
		//setting box height/width
		obs_data_set_int(settings, "width", 720);
		obs_data_set_int(settings, "height", 210);
	}
	obs_data_release(tm_text_obj);
}
void set_tm_text_color_data(obs_data_t *settings, const TMTemplateAttr &tm_attr,
			    bool is_default)
{
	obs_data_t *tm_color = obs_data_create();

	if (is_default) {
		obs_data_set_default_bool(tm_color, "text-color-change", false);

		obs_data_set_default_int(tm_color, "color-tab", 0);

		obs_data_set_default_bool(tm_color, "is-color",
					  tm_attr.isTextColor);
		obs_data_set_default_int(tm_color, "text-color",
					 tm_attr.textColorValue);
		obs_data_set_default_bool(tm_color, "is-text-color-alpha",
					  tm_attr.isTextTransparent);
		obs_data_set_default_int(tm_color, "text-color-alpha",
					 tm_attr.textTransparentValue);
		obs_data_set_default_bool(tm_color, "is-bk-init-color",
					  tm_attr.isBackgroundColor);
		obs_data_set_default_bool(tm_color, "is-bk-color-on",
					  tm_attr.isBackgroundOn);
		obs_data_set_default_bool(tm_color, "is-bk-color",
					  tm_attr.isBackgroundColor);
		obs_data_set_default_int(tm_color, "bk-color",
					 tm_attr.backgroundColorValue);
		obs_data_set_default_bool(tm_color, "is-bk-color-alpha",
					  tm_attr.isBackgroundTransparent);
		obs_data_set_default_int(tm_color, "bk-color-alpha",
					 tm_attr.backgroundTransparentValue);

		obs_data_set_default_bool(tm_color, "is-outline-color-on",
					  tm_attr.isStrokeOn);
		obs_data_set_default_bool(tm_color, "is-outline-color",
					  tm_attr.isStrokeColor);
		obs_data_set_default_int(tm_color, "outline-color",
					 tm_attr.strokeColorValue);
		obs_data_set_default_int(tm_color, "outline-color-line",
					 tm_attr.strokeLineValue);

		obs_data_set_default_obj(settings, S_TM_COLOR, tm_color);

	} else {
		obs_data_t *val = obs_data_get_obj(settings, S_TM_COLOR);
		long long textColor = obs_data_get_int(val, "text-color");
		int textColorAlaph = obs_data_get_int(val, "text-color-alpha");
		bool textColorChange =
			obs_data_get_bool(val, "text-color-change");

		if (textColorChange) {
			obs_data_set_int(tm_color, "text-color", textColor);
			obs_data_set_int(tm_color, "text-color-alpha",
					 textColorAlaph);

		} else {
			obs_data_set_int(tm_color, "text-color",
					 tm_attr.textColorValue);
			obs_data_set_int(tm_color, "text-color-alpha",
					 tm_attr.textTransparentValue);
		}

		obs_data_set_bool(tm_color, "text-color-change",
				  textColorChange);
		obs_data_set_bool(tm_color, "is-color", tm_attr.isTextColor);
		obs_data_set_bool(tm_color, "is-text-color-alpha",
				  tm_attr.isTextTransparent);
		obs_data_set_bool(tm_color, "is-bk-color-on",
				  tm_attr.isBackgroundOn);
		obs_data_set_bool(tm_color, "is-bk-init-color",
				  tm_attr.isBackgroundColor);
		obs_data_set_bool(tm_color, "is-bk-color",
				  tm_attr.isBackgroundColor);
		obs_data_set_int(tm_color, "bk-color",
				 tm_attr.backgroundColorValue);
		obs_data_set_bool(tm_color, "is-bk-color-alpha",
				  tm_attr.isBackgroundTransparent);
		obs_data_set_int(tm_color, "bk-color-alpha",
				 tm_attr.backgroundTransparentValue);

		obs_data_set_bool(tm_color, "is-outline-color-on",
				  tm_attr.isStrokeOn);
		obs_data_set_bool(tm_color, "is-outline-color",
				  tm_attr.isStrokeColor);
		obs_data_set_int(tm_color, "outline-color",
				 tm_attr.strokeColorValue);
		obs_data_set_int(tm_color, "outline-color-line",
				 tm_attr.strokeLineValue);

		obs_data_set_obj(settings, S_TM_COLOR, tm_color);
		obs_data_release(val);
	}
	obs_data_release(tm_color);
}
void set_tm_text_motion_data(obs_data_t *settings,
			     const TMTemplateAttr &tm_attr, bool is_default)
{
	obs_data_t *tm_motion = obs_data_create();
	if (is_default) {
		obs_data_set_default_int(tm_motion, "text-motion",
					 tm_attr.isRotate);
		obs_data_set_default_bool(tm_motion, "is-text-motion-speed",
					  tm_attr.isMotion);

		obs_data_set_default_int(tm_motion, "text-motion-speed",
					 tm_attr.motionValue);

		obs_data_set_default_obj(settings, S_TM_MOTION, tm_motion);
	} else {
		obs_data_set_int(tm_motion, "text-motion", tm_attr.isRotate);
		obs_data_set_bool(tm_motion, "is-text-motion-speed",
				  tm_attr.isMotion);

		obs_data_set_int(tm_motion, "text-motion-speed",
				 tm_attr.motionValue);

		obs_data_set_obj(settings, S_TM_MOTION, tm_motion);
	}
	obs_data_release(tm_motion);
}

static bool tm_tab_changed(obs_properties_t *props, obs_property_t *,
			   obs_data_t *settings)
{
	int index = obs_data_get_int(settings, S_TM_TAB);
	obs_property_t *tm_template_tab =
		obs_properties_get(props, S_TM_TEMPLATE_TAB);
	obs_property_t *tm_template_list =
		obs_properties_get(props, S_TM_TEMPLATE_LIST);
	obs_property_t *tm_text_content =
		obs_properties_get(props, S_TM_TEXT_CONTENT);
	obs_property_t *tm_text = obs_properties_get(props, S_TM_TEXT);
	obs_property_t *tm_color = obs_properties_get(props, S_TM_COLOR);
	obs_property_t *tm_motion = obs_properties_get(props, S_TM_MOTION);

	obs_property_set_visible(tm_template_tab, false);
	obs_property_set_visible(tm_template_list, false);
	obs_property_set_visible(tm_text, false);
	obs_property_set_visible(tm_color, false);
	obs_property_set_visible(tm_motion, false);

	switch (index) {
	case 0:
		obs_property_set_visible(tm_template_tab, true);
		obs_property_set_visible(tm_template_list, true);
		break;
	case 1:
		obs_property_set_visible(tm_text, true);
		obs_property_set_visible(tm_color, true);
		obs_property_set_visible(tm_motion, true);
		break;
	default:
		break;
	}
	return true;
}
static bool tm_template_list_changed(obs_properties_t *props, obs_property_t *,
				     obs_data_t *settings)
{
	static int currentIndex = -1;
	int index = obs_data_get_int(settings, S_TM_TEMPLATE_LIST);

	if ((currentIndex == index) || isFirstShow) {
		currentIndex = index;
		isFirstShow = false;
		return false;
	}
	//template change content and text color value not need change
	if (g_TMTemplateJsons.find(index) != g_TMTemplateJsons.end()) {
		currentIndex = index;
		TMTemplateAttr templateAttr = g_TMTemplateJsons.value(index);

		set_tm_text_content_data(settings, templateAttr, false);
		set_tm_text_data(settings, templateAttr, false);
		set_tm_text_color_data(settings, templateAttr, false);
		set_tm_text_motion_data(settings, templateAttr, false);
	}
	return true;
}

obs_properties_t *text_motion_source_get_properties(void *data)
{
	isFirstShow = true;
	obs_properties_t *properties = obs_properties_create();

	obs_properties_add_tm_content(properties, S_TM_TEXT_CONTENT,
				      obs_module_text(S_TM_TEXT_CONTENT));
	obs_property_t *tm_tab_prop =
		obs_properties_add_tm_tab(properties, S_TM_TAB);
	obs_property_set_modified_callback(tm_tab_prop, tm_tab_changed);

	obs_properties_add_tm_template_tab(properties, S_TM_TEMPLATE_TAB);

	obs_property_t *tm_template_list_prop =
		obs_properties_add_tm_template_list(properties,
						    S_TM_TEMPLATE_LIST);
	obs_property_set_modified_callback(tm_template_list_prop,
					   tm_template_list_changed);
	obs_properties_add_tm_text(properties, S_TM_TEXT,
				   obs_module_text(S_TM_TEXT), 6, 72, 1);
	obs_properties_add_tm_color(properties, S_TM_COLOR,
				    obs_module_text(S_TM_COLOR), 0, 100, 1);

	obs_properties_add_tm_motion(properties, S_TM_MOTION,
				     obs_module_text(S_TM_MOTION), 1, 100, 1);

	return properties;
}

void text_motion_source_get_defaults(obs_data_t *settings)
{
	attachTMInfo();
	browser_source_get_defaults(settings);
	QDir appDir(qApp->applicationDirPath());

	obs_data_set_default_int(settings, "width", 720);
	obs_data_set_default_int(settings, "height", 210);
	obs_data_set_default_string(
		settings, "url",
		QUrl::fromLocalFile(
			appDir.absoluteFilePath(
				"data/prism-studio/text_motion/index.html"))
			.toString()
			.toUtf8());
	if (g_TMTemplateJsons.count() <= 0)
		return;
	obs_data_set_default_int(settings, S_TM_TAB, 0);
	obs_data_set_default_int(settings, S_TM_TEMPLATE_TAB, 0);
	obs_data_set_default_int(settings, S_TM_TEMPLATE_LIST,
				 g_TMTemplateJsons.firstKey());

	set_tm_text_content_data(settings, g_TMTemplateJsons.first(), true);

	set_tm_text_data(settings, g_TMTemplateJsons.first(), true);

	set_tm_text_color_data(settings, g_TMTemplateJsons.first(), true);

	set_tm_text_motion_data(settings, g_TMTemplateJsons.first(), true);
}

void register_prism_text_motion_source()
{
	struct obs_source_info info = {};
	info.id = "prism_text_motion_source";
	info.type = OBS_SOURCE_TYPE_INPUT;
	info.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW |
			    OBS_SOURCE_DO_NOT_DUPLICATE;
	info.get_properties = text_motion_source_get_properties;
	info.get_defaults = text_motion_source_get_defaults;

	info.get_name = [](void *) {
		return obs_module_text(obs_module_text("text.template"));
	};
	info.create = [](obs_data_t *settings, obs_source_t *source) -> void * {
		obs_browser_initialize();
		return new TextMotionSource(settings, source);
	};
	info.destroy = [](void *data) {
		delete static_cast<TextMotionSource *>(data);
	};
	info.update = [](void *data, obs_data_t *settings) {
		static_cast<TextMotionSource *>(data)->Update(settings);
	};
	info.get_width = [](void *data) {
		return (uint32_t) static_cast<TextMotionSource *>(data)->width;
	};
	info.get_height = [](void *data) {
		return (uint32_t) static_cast<TextMotionSource *>(data)->height;
	};
	info.video_tick = [](void *data, float) {
		static_cast<TextMotionSource *>(data)->Tick();
	};
	info.video_render = [](void *data, gs_effect_t *) {
		static_cast<TextMotionSource *>(data)->Render();
	};
#if CHROME_VERSION_BUILD >= 3683
	info.audio_mix = [](void *data, uint64_t *ts_out,
			    struct audio_output_data *audio_output,
			    size_t channels, size_t sample_rate) {
		return static_cast<TextMotionSource *>(data)->AudioMix(
			ts_out, audio_output, channels, sample_rate);
	};
	info.enum_active_sources = [](void *data, obs_source_enum_proc_t cb,
				      void *param) {
		static_cast<TextMotionSource *>(data)->EnumAudioStreams(cb,
									param);
	};
#endif
	/*
	info.mouse_click = [](void *data, const struct obs_mouse_event *event,
			      int32_t type, bool mouse_up,
			      uint32_t click_count) {
		static_cast<TextMotionSource *>(data)->SendMouseClick(
			event, type, mouse_up, click_count);
	};
	info.mouse_move = [](void *data, const struct obs_mouse_event *event,
			     bool mouse_leave) {
		static_cast<TextMotionSource *>(data)->SendMouseMove(
			event, mouse_leave);
	};
	info.mouse_wheel = [](void *data, const struct obs_mouse_event *event,
			      int x_delta, int y_delta) {
		static_cast<TextMotionSource *>(data)->SendMouseWheel(
			event, x_delta, y_delta);
	};
	info.focus = [](void *data, bool focus) {
		static_cast<TextMotionSource *>(data)->SendFocus(focus);
	};
	info.key_click = [](void *data, const struct obs_key_event *event,
			    bool key_up) {
		static_cast<TextMotionSource *>(data)->SendKeyClick(event,
								    key_up);
	};*/

	info.set_private_data = BrowserSource::SetBrowserData;
	info.get_private_data = BrowserSource::GetBrowserData;

	info.show = [](void *data) {
		static_cast<TextMotionSource *>(data)->SetShowing(true);
	};
	info.hide = [](void *data) {
		static_cast<TextMotionSource *>(data)->SetShowing(false);
	};
	info.activate = [](void *data) {
		TextMotionSource *bs = static_cast<TextMotionSource *>(data);
		if (bs->restart)
			bs->Refresh();
		bs->SetActive(true);
	};
	info.deactivate = [](void *data) {
		static_cast<TextMotionSource *>(data)->SetActive(false);
	};
	info.properties_edit_start = [](void *data, obs_data_t *settings) {
		static_cast<TextMotionSource *>(data)->propertiesEditStart(
			settings);
	};

	info.icon_type = OBS_ICON_TYPE_TEXT_MOTION;

	obs_register_source(&info);
}

void release_prism_text_motion_source() {}

void TextMotionSource::Update(obs_data_t *settings)
{
	BrowserSource::Update(settings);

	int index = obs_data_get_int(settings, S_TM_TEMPLATE_LIST);

	if (g_TMTemplateJsons.find(index) != g_TMTemplateJsons.end()) {
		TMTemplateAttr templateAttr = g_TMTemplateJsons.value(index);
		type = templateAttr.name;
	}
	obs_data_t *tmTextVal = obs_data_get_obj(settings, S_TM_TEXT);
	obs_data_t *tmColorVal = obs_data_get_obj(settings, S_TM_COLOR);
	obs_data_t *tmContentVal =
		obs_data_get_obj(settings, S_TM_TEXT_CONTENT);
	obs_data_t *tmMotionVal = obs_data_get_obj(settings, S_TM_MOTION);

	bool isBackground = obs_data_get_bool(tmColorVal, "is-bk-color");
	bool isOutLine = obs_data_get_bool(tmColorVal, "is-outline-color");
	long long textColorInt =
		rgb_to_bgr(obs_data_get_int(tmColorVal, "text-color"));
	long long textColorAlaph =
		obs_data_get_int(tmColorVal, "text-color-alpha") * 0.01 * 255;

	textColor = QString("%1%2")
			    .arg(QColor(textColorInt).name(QColor::HexRgb))
			    .arg(QString::asprintf("%02x", textColorAlaph));

	textFamily = QString("%1").arg(
		obs_data_get_string(tmTextVal, "font-family"));
	textStyle = obs_data_get_string(tmTextVal, "font-weight");
	textFontSize =
		QString("%1pt").arg(obs_data_get_int(tmTextVal, "font-size"));
	if (isBackground) {
		long long bkColorInt =
			rgb_to_bgr(obs_data_get_int(tmColorVal, "bk-color"));
		long long bkColorAlaph =
			obs_data_get_int(tmColorVal, "bk-color-alpha") * 0.01 *
			255;
		bkColor = QString("%1%2")
				  .arg(QColor(bkColorInt).name(QColor::HexRgb))
				  .arg(QString::asprintf("%02x", bkColorAlaph));

	} else {
		bkColor.clear();
	}
	if (isOutLine) {
		long long outlineColorInt = rgb_to_bgr(
			obs_data_get_int(tmColorVal, "outline-color"));
		textLineColor = QString("%1").arg(
			QColor(outlineColorInt).name(QColor::HexRgb));
		textLineSize = QString("%1").arg(
			obs_data_get_int(tmColorVal, "outline-color-line"));
	} else {
		textLineColor.clear();
		textLineSize.clear();
	}

	content = obs_data_get_string(tmContentVal, "text-content-1");
	subTitle = obs_data_get_string(tmContentVal, "text-content-2");
	hAlign = obs_data_get_int(tmTextVal, "h-aligin");
	letter = obs_data_get_int(tmTextVal, "letter");
	isMotionOnce = qAbs(obs_data_get_int(tmMotionVal, "text-motion") - 1);
	animateDuration = obs_data_get_int(tmMotionVal, "text-motion-speed");
	if (animateDuration <= 50) {
		animateDuration = animateDuration * 0.02;
	} else {
		animateDuration = animateDuration * 0.04;
	}
	obs_data_release(tmTextVal);
	obs_data_release(tmColorVal);
	obs_data_release(tmContentVal);
	obs_data_release(tmMotionVal);
	QByteArray updateTextTemplateInfo(toJsonStr().toUtf8());

	blog(LOG_INFO, "update text template info:%s",
	     updateTextTemplateInfo.constData());
	//send event to Web
	DispatchJSEvent(this, "textMotion", updateTextTemplateInfo.constData());
}

QString TextMotionSource::toJsonStr()
{
	QJsonObject root;
	QJsonObject data;

	data.insert("type", type);
	data.insert("isOnce", isMotionOnce);
	data.insert("animateRate", animateDuration);
	data.insert("align", getTextAlign(hAlign));
	data.insert("textTransform", getLetter(letter));
	data.insert("fontColor", textColor);
	data.insert("background", bkColor);
	data.insert("fontFamily", textFamily);
	data.insert("fontStyle", textStyle);
	data.insert("fontSize", textFontSize);
	data.insert("textLineColor", textLineColor);
	data.insert("textLineSize", textLineSize);
	data.insert("content", content.replace('\n', "\\n"));
	data.insert("subContent", subTitle.replace('\n', "\\n"));
	QString json = QJsonDocument(data).toJson(QJsonDocument::Compact);
	return json;
}

void TextMotionSource::onBrowserLoadEnd()
{
	//send event to Web
	DispatchJSEvent(this, "textMotion", toJsonStr().toUtf8().constData());
}

void TextMotionSource::propertiesEditStart(obs_data_t *settings)
{
	//send event to Web
	DispatchJSEvent(this, "textMotion", toJsonStr().toUtf8().constData());
}
