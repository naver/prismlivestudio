#ifndef PLSBEAUTYDEFINE_H
#define PLSBEAUTYDEFINE_H

#include <QString>

static const int maxFilerIdLength = 20;
const int SKIN_SMOOTH_DEFALUT_VALUE = 100;
static const char *GEOMETRY_DATA = "geometryBeauty"; //key of beauty window geometry in global ini
static const char *MAXIMIZED_STATE = "isMaxState";   //key if the beauty window is maximized in global ini
static const char *SHOW_MADE = "showMode";           //key if the beauty window is shown in global ini

//defines of beuaty data key.
static const char *IS_CURRENT_SOURCE = "is_current_source";
static const char *BEAUTY_NODE = "beauty";
static const char *BASE_ID = "base_id";
static const char *FILTER_ID = "filter_id";
static const char *BEAUTY_ON = "beauty_on";
static const char *FILTER_INDEX = "filter_index";
static const char *IS_CURRENT = "is_current";
static const char *IS_CUSTOM = "is_custom";
static const char *FILTER_TYPE = "filter_type";
static const char *CUSTOM_NODE = "custom";
static const char *CHEEK_VALUE = "cheek_value";
static const char *CHEEKBONE_VALUE = "cheekbone_value";
static const char *CHIN_VALUE = "chin_value";
static const char *EYE_VALUE = "eye_value";
static const char *NOSE_VALUE = "nose_value";
static const char *SKIN_VALUE = "skin_value";
static const char *DEFAULT_NODE = "default";

enum BeautyConfigType { PresetConfig = 0, CustomConfig };

struct BeautyToken {
	QString strID;    //cute natural sharp
	QString category; //system, cute/natrual/sharp
};

struct SkinSmoothModel {
	int default_value{SKIN_SMOOTH_DEFALUT_VALUE};
	int dymamic_value;

	bool IsChanged() { return default_value != dymamic_value; }
};

struct BeautyParam {
	int chin;
	int cheek;
	int cheekbone;
	int eyes;
	int nose;
};

struct BeautyModel {
	BeautyToken token;
	BeautyParam defaultParam;
	BeautyParam dynamicParam;

	bool IsChanged()
	{
		return !(defaultParam.cheek == dynamicParam.cheek && defaultParam.cheekbone == dynamicParam.cheekbone && defaultParam.chin == dynamicParam.chin &&
			 defaultParam.eyes == dynamicParam.eyes && defaultParam.nose == dynamicParam.nose);
	}
};

struct BeautyStatus {
	bool beauty_enable{false};
};

struct BeautyConfig {
	BeautyStatus beautyStatus;
	SkinSmoothModel smoothModel;
	BeautyModel beautyModel;
	bool isCurrent{false};
	bool isCustom{false};
	int filterIndex{0};
	int filterType{0};
};

#endif // PLSBEAUTYDEFINE_H
