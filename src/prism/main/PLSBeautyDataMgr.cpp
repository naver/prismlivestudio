#include "PLSBeautyDataMgr.h"
#include "pls-common-define.hpp"

#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "liblog.h"
#include "log/module_names.h"

PLSBeautyDataMgr *PLSBeautyDataMgr::Instance()
{
	static PLSBeautyDataMgr instance;
	return &instance;
}

int PLSBeautyDataMgr::GetBeautyFaceSizeBySourceId(OBSSceneItem item)
{
	auto iter = beautySourceIdData.find(item);
	if (iter != beautySourceIdData.end()) {
		BeautyConfigMap config = iter->second;
		return static_cast<int>(config.size());
	}

	return 0;
}

int PLSBeautyDataMgr::GetCustomFaceSizeBySourceId(OBSSceneItem item)
{
	int count = 0;
	auto iter = beautySourceIdData.find(item);
	if (iter == beautySourceIdData.end()) {
		return count;
	}

	BeautyConfigMap config = iter->second;
	for (auto iter = config.begin(); iter != config.end(); ++iter) {
		BeautyConfig config = iter->second;
		if (config.isCustom) {
			count += 1;
		}
	}

	return count;
}

BeautySourceIdMap PLSBeautyDataMgr::GetData()
{
	return beautySourceIdData;
}

BeautyConfigMap PLSBeautyDataMgr::GetBeautyConfigBySourceId(OBSSceneItem item)
{
	auto iter = beautySourceIdData.find(item);
	if (iter != beautySourceIdData.end()) {
		return iter->second;
	}

	return BeautyConfigMap();
}

bool PLSBeautyDataMgr::GetBeautyConfig(OBSSceneItem item, const QString &filterId, BeautyConfig &config)
{
	return FindFilterId(item, filterId, config);
}

bool PLSBeautyDataMgr::ResetBeautyConfig(OBSSceneItem item, const QString &filterId)
{
	BeautyConfig config;
	if (FindFilterId(item, filterId, config)) {
		config.beautyModel.dynamicParam = config.beautyModel.defaultParam;
		config.smoothModel.dymamic_value = config.smoothModel.default_value;
		SetBeautyConfig(item, filterId, config);
		return true;
	}
	return false;
}

bool PLSBeautyDataMgr::DeleteBeautyFilterConfig(const QString &filterId)
{
	if (beautySourceIdData.empty()) {
		return false;
	}

	for (auto iter = beautySourceIdData.begin(); iter != beautySourceIdData.end(); ++iter) {
		BeautyConfigMap &configMap = iter->second;
		for (auto filterIter = configMap.begin(); filterIter != configMap.end();) {
			if (filterIter->first == filterId) {
				filterIter = configMap.erase(filterIter);
				continue;
			}
			filterIter++;
		}
	}
	return true;
}

bool PLSBeautyDataMgr::DeleteBeautyCustomConfig(const QString &filterId)
{
	if (filterId.isEmpty())
		return false;
	auto iter = beautyCustomConfig.begin();
	while (iter != beautyCustomConfig.end()) {
		if (iter->first == filterId) {
			beautyCustomConfig.erase(iter);
			break;
		}
		iter++;
	}
	return true;
}

bool PLSBeautyDataMgr::DeleteBeautySourceConfig(OBSSceneItem item)
{
	auto iter = beautySourceIdData.find(item);
	if (iter == beautySourceIdData.end()) {
		return true;
	}

	beautySourceIdData.erase(iter);
	return true;
}

void PLSBeautyDataMgr::ClearBeautyConfig()
{
	beautySourceIdData.clear();
	beautyCustomConfig.clear();
}

bool PLSBeautyDataMgr::CopyBeautyConfig(OBSSceneItem srcItem, OBSSceneItem destItem)
{
	auto iter = beautySourceIdData.find(destItem);
	if (iter != beautySourceIdData.end()) {
		return true;
	}

	iter = beautySourceIdData.begin();
	if (iter == beautySourceIdData.end()) {
		return false;
	}

	BeautyConfigMap configMap = iter->second;
	for (auto configIter = configMap.begin(); configIter != configMap.end(); ++configIter) {
		auto &config = configIter->second;
		config.beautyModel.dynamicParam = config.beautyModel.defaultParam;
		config.smoothModel.dymamic_value = config.smoothModel.default_value;
		config.beautyStatus.beauty_enable = false;
		SetBeautyConfig(destItem, config.beautyModel.token.strID, config);
	}
	return true;
}

bool PLSBeautyDataMgr::CopyBeautyConfig(OBSSceneItem destItem)
{
	auto iter = beautySourceIdData.find(destItem);
	if (iter != beautySourceIdData.end()) {
		return true;
	}

	// copy from preset and custom beauty config.
	if (0 == beautyPresetConfig.size() && 0 == beautyCustomConfig.size())
		return false;

	// Copy the preset config.
	for (auto configIter = beautyPresetConfig.cbegin(); configIter != beautyPresetConfig.cend(); ++configIter) {
		auto config = configIter->second;
		config.beautyModel.dynamicParam = config.beautyModel.defaultParam;
		config.smoothModel.dymamic_value = config.smoothModel.default_value;
		config.beautyStatus.beauty_enable = false;
		SetBeautyConfig(destItem, config.beautyModel.token.strID, config);
	}

	// Copy the custom config.
	for (auto configIter = beautyCustomConfig.cbegin(); configIter != beautyCustomConfig.cend(); ++configIter) {
		auto config = configIter->second;
		config.beautyModel.dynamicParam = config.beautyModel.defaultParam;
		config.smoothModel.dymamic_value = config.smoothModel.default_value;
		config.beautyStatus.beauty_enable = false;
		SetBeautyConfig(destItem, config.beautyModel.token.strID, config);
	}
	return true;
}

QString PLSBeautyDataMgr::GetCheckedFilterName(OBSSceneItem item)
{
	auto iter = beautySourceIdData.find(item);
	if (iter == beautySourceIdData.end()) {
		return "";
	}

	BeautyConfigMap &configMap = iter->second;
	for (auto filterIter = configMap.begin(); filterIter != configMap.end(); ++filterIter) {
		BeautyConfig config = filterIter->second;
		if (config.isCurrent) {
			return config.beautyModel.token.strID;
		}
	}
	return "";
}

QString PLSBeautyDataMgr::GetCheckedFilterBaseName(OBSSceneItem item)
{
	auto iter = beautySourceIdData.find(item);
	if (iter == beautySourceIdData.end()) {
		return "";
	}

	BeautyConfigMap &configMap = iter->second;
	for (auto filterIter = configMap.begin(); filterIter != configMap.end(); ++filterIter) {
		BeautyConfig config = filterIter->second;
		if (config.isCurrent) {
			return config.beautyModel.token.category;
		}
	}
	return "";
}

QString PLSBeautyDataMgr::GetCloseUnusedFilterName(OBSSceneItem item, const QString &baseId)
{
	int index = 1;
	while (true) {
		QString name = baseId + "-" + QString::number(index);
		auto iter = beautySourceIdData.find(item);
		if (iter == beautySourceIdData.end()) {
			return name;
		}

		BeautyConfigMap configMap = iter->second;
		bool find = false;
		for (auto filterIter = configMap.begin(); filterIter != configMap.end(); ++filterIter) {
			if (filterIter->first == name) {
				find = true;
				break;
			}
		}
		if (!find) {
			return name;
		}
		index++;
	}
}

void PLSBeautyDataMgr::SetBeautyConfig(OBSSceneItem item, const QString &filterId, const BeautyConfig &config)
{
	if (!item || filterId.isEmpty()) {
		return;
	}

	auto iter = beautySourceIdData.find(item);
	if (iter == beautySourceIdData.end()) { // not found
		BeautyConfigMap beautyConfigData;
		beautyConfigData.emplace_back(BeautyConfigMap::value_type(filterId, config));
		beautySourceIdData[item] = beautyConfigData;
	} else {
		bool find = false;
		auto &beautyConfigData = iter->second;
		for (auto configIter = beautyConfigData.begin(); configIter != beautyConfigData.end(); ++configIter) {
			QString &filterId_ = configIter->first;
			if (filterId_ == filterId) {
				configIter->second = config;
				find = true;
			}
		}
		if (!find) {
			beautyConfigData.emplace_back(BeautyConfigMap::value_type(filterId, config));
		}
	}

	UpdateSameSource(item, filterId, config);
}

void PLSBeautyDataMgr::SetBeautyConfig(OBSSceneItem item, const QList<BeautyConfig> &configList)
{
	for (auto &config : configList) {
		SetBeautyConfig(item, config.beautyModel.token.strID, config);
	}
}

void PLSBeautyDataMgr::SetBeautyCustomConfig(const QList<BeautyConfig> &configList)
{
	for (auto &config : configList) {
		beautyCustomConfig.emplace_back(BeautyConfigMap::value_type(config.beautyModel.token.strID, config));
	}
}

void PLSBeautyDataMgr::AddBeautyCustomConfig(const QString &filterId, const BeautyConfig &config)
{
	if (filterId.isEmpty())
		return;
	BeautyConfig copyConfig = config;
	// Init default config.
	copyConfig.isCurrent = false;
	copyConfig.beautyStatus.beauty_enable = false;
	beautyCustomConfig.emplace_back(BeautyConfigMap::value_type(filterId, copyConfig));
}

void PLSBeautyDataMgr::SetBeautyPresetConfig(const QList<BeautyConfig> &configList)
{
	for (auto &config : configList) {
		beautyPresetConfig.emplace_back(BeautyConfigMap::value_type(config.beautyModel.token.strID, config));
	}
}

void PLSBeautyDataMgr::AddBeautyPresetConfig(const QString &filterId, const BeautyConfig &config)
{
	if (filterId.isEmpty())
		return;
	BeautyConfig copyConfig = config;
	// Init default config.
	copyConfig.isCurrent = false;
	copyConfig.beautyStatus.beauty_enable = false;
	beautyPresetConfig.emplace_back(BeautyConfigMap::value_type(filterId, copyConfig));
}

const BeautyConfigMap &PLSBeautyDataMgr::GetBeautyPresetConfig() const
{
	return beautyPresetConfig;
}

BeautyConfig PLSBeautyDataMgr::GetBeautyPresetConfigById(const QString &filterId) const
{
	for (auto configIter = beautyPresetConfig.begin(); configIter != beautyPresetConfig.end(); ++configIter) {
		if (configIter->first == filterId) {
			return configIter->second;
		}
	}
	PLS_ERROR(MAIN_BEAUTY_MODULE, "[PLSBeautyDataMgr]:%s filterId is not found in beautyPresetConfig,function returns default data", filterId.toStdString().c_str());
	return BeautyConfig();
}

bool PLSBeautyDataMgr::RenameCustomBeautyConfig(const QString &filterId, const QString &newFilterId)
{
	if (filterId.isEmpty() || newFilterId.isEmpty())
		return false;
	for (auto configIter = beautyCustomConfig.begin(); configIter != beautyCustomConfig.end(); ++configIter) {
		if (configIter->first == filterId) {
			auto config = configIter->second;
			config.beautyModel.token.strID = newFilterId;
			configIter = beautyCustomConfig.erase(configIter);
			beautyCustomConfig.insert(configIter, BeautyConfigMap::value_type(newFilterId, config));
			break;
		}
	}
	return true;
}

const BeautyConfigMap &PLSBeautyDataMgr::GetBeautyCustomConfig() const
{
	return beautyCustomConfig;
}

void PLSBeautyDataMgr::SetCustomBeautyConfig(OBSSceneItem item, const QString &filterId, const BeautyConfig &config)
{
	for (auto iter = beautySourceIdData.begin(); iter != beautySourceIdData.end(); ++iter) {
		BeautyConfigMap &configMap = iter->second;
		//do not modified the "isCurrent" value of other sources.
		if (iter->first != item) {
			BeautyConfig copyConfig = config;
			copyConfig.isCurrent = false;
			configMap.emplace_back(BeautyConfigMap::value_type(filterId, copyConfig));
			continue;
		}
		configMap.emplace_back(BeautyConfigMap::value_type(filterId, config));
	}
}

void PLSBeautyDataMgr::SetBeautyCurrentStateConfig(OBSSceneItem item, const QString &filterId, const bool &state)
{
	auto iter = beautySourceIdData.find(item);
	if (iter == beautySourceIdData.end()) {
		return;
	}
	BeautyConfigMap &configMap = iter->second;
	for (auto filterIter = configMap.begin(); filterIter != configMap.end(); ++filterIter) {
		auto &config = filterIter->second;
		if (filterIter->first == filterId) {
			config.isCurrent = state;
		} else {
			config.isCurrent = !state;
		}
		SetBeautyConfig(item, config.beautyModel.token.strID, config);
	}
}

void PLSBeautyDataMgr::SetBeautyCheckedStateConfig(OBSSceneItem item, const bool &state)
{
	auto iter = beautySourceIdData.find(item);
	if (iter == beautySourceIdData.end()) {
		return;
	}
	BeautyConfigMap &configMap = iter->second;
	for (auto filterIter = configMap.begin(); filterIter != configMap.end(); ++filterIter) {
		auto &config = filterIter->second;
		config.beautyStatus.beauty_enable = state;
		SetBeautyConfig(item, config.beautyModel.token.strID, config);
	}
}

PLSBeautyDataMgr::PLSBeautyDataMgr() {}

PLSBeautyDataMgr::~PLSBeautyDataMgr() {}

bool PLSBeautyDataMgr::UpdateSameSource(OBSSceneItem item, const QString &filterId, const BeautyConfig &config)
{
	OBSSource source = obs_sceneitem_get_source(item);
	for (auto iter = beautySourceIdData.begin(); iter != beautySourceIdData.end(); ++iter) {
		if (item == iter->first) {
			continue;
		}

		OBSSource source_ = obs_sceneitem_get_source(iter->first);
		if (source_ != source) {
			continue;
		}

		BeautyConfigMap &configMap = iter->second;
		for (auto configIter = configMap.begin(); configIter != configMap.end(); ++configIter) {
			if (0 == configIter->first.compare(filterId)) {
				configIter->second = config;
			}
		}
	}
	return true;
}

bool PLSBeautyDataMgr::FindFilterId(OBSSceneItem item, const QString &filterId, BeautyConfig &config)
{
	auto iter = beautySourceIdData.find(item);
	if (iter == beautySourceIdData.end()) {
		return false;
	}
	BeautyConfigMap &configMap = iter->second;
	for (auto filterIter = configMap.begin(); filterIter != configMap.end(); ++filterIter) {
		if (filterIter->first == filterId) {
			config = filterIter->second;
			return true;
		}
	}
	return false;
}

bool PLSBeautyDataMgr::FindFilterId(OBSSceneItem item, const QString &filterId)
{
	auto iter = beautySourceIdData.find(item);
	if (iter == beautySourceIdData.end()) {
		return false;
	}
	BeautyConfigMap &configMap = iter->second;
	for (auto filterIter = configMap.begin(); filterIter != configMap.end(); ++filterIter) {
		if (filterIter->first == filterId) {
			return true;
		}
	}
	return false;
}

bool PLSBeautyDataMgr::RenameFilterId(const QString &filterId, const QString &newId)
{
	if (0 == beautySourceIdData.size()) {
		return false;
	}
	for (auto iter = beautySourceIdData.begin(); iter != beautySourceIdData.end(); ++iter) {
		BeautyConfigMap &configMap = iter->second;
		for (auto filterIter = configMap.begin(); filterIter != configMap.end();) {
			if (filterIter->first == filterId) {
				BeautyConfig config = filterIter->second;
				config.beautyModel.token.strID = newId;
				filterIter = configMap.erase(filterIter);
				filterIter = configMap.insert(filterIter, BeautyConfigMap::value_type(newId, config));
				continue;
			}
			filterIter++;
		}
	}
	return true;
}

int PLSBeautyDataMgr::GetValidFilterIndex(OBSSceneItem item, const QString &baseId)
{
	QVector<int> filterIndexUsedCount{0, 0, 0};
	auto iter = beautySourceIdData.find(item);
	if (iter == beautySourceIdData.end()) {
		return -1;
	}
	BeautyConfigMap &configMap = iter->second;
	for (auto filterIter = configMap.begin(); filterIter != configMap.end(); ++filterIter) {
		BeautyConfig config = filterIter->second;
		//Judge filterIndex of config, make filterIndexUsed[0]++ if filterIndex is 1
		//make filterIndexUsed[1]++ if filterIndex is 2,filterIndexUsed[0]++ if filterIndex is 3.
		if (config.beautyModel.token.category == baseId) {
			int filterId = config.filterIndex;
			switch (filterId) {
			case 1:
				++filterIndexUsedCount[0];
				break;
			case 2:
				++filterIndexUsedCount[1];
				break;
			case 3:
				++filterIndexUsedCount[2];
				break;
			default:
				break;
			}
		}
	}
	//Find out index of min number in filterIndexUsedCount,++index is the result.
	int minCount = filterIndexUsedCount[0];
	int minIndex = 0;
	for (int i = 0; i < filterIndexUsedCount.size(); ++i) {
		int count = filterIndexUsedCount[i];
		if (i == 0 && count == 0) {
			break;
		}
		if (count < minCount) {
			minCount = count;
			minIndex = i;
		}
	}

	return ++minIndex;
}

bool PLSBeautyDataMgr::ParseJsonArrayToBeautyConfig(const QByteArray &array, BeautyConfig &beautyConfig)
{
	if (array.isEmpty()) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "[PLSBeautyDataMgr]:beauty initialization config data error");
		return false;
	}
	QJsonObject object = QJsonDocument::fromJson(array).object();
	if (object.isEmpty() || !object.contains("ID") || !object.contains("models")) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "[PLSBeautyDataMgr]:beauty initialization config data error");
		return false;
	}

	QString mainId = object["ID"].toString();
	QJsonArray modelArray = object["models"].toArray();

	SkinSmoothModel smoothModel;
	smoothModel.dymamic_value = smoothModel.default_value;

	BeautyModel beautyModel;
	beautyModel.token.strID = mainId;
	beautyModel.token.category = mainId;

	for (auto iter = modelArray.begin(); iter != modelArray.end(); ++iter) {
		QJsonObject obj = iter->toObject();
		QString id = obj["ID"].toString();
		double value = obj["initialStrength"].toDouble();

		if (0 == id.compare("chin")) {
			beautyModel.defaultParam.chin = value * 100;
		} else if (0 == id.compare("cheek")) {
			beautyModel.defaultParam.cheek = value * 100;
		} else if (0 == id.compare("cheekbone")) {
			beautyModel.defaultParam.cheekbone = value * 100;
		} else if (0 == id.compare("eyes")) {
			beautyModel.defaultParam.eyes = value * 100;
		} else if (0 == id.compare("nose")) {
			beautyModel.defaultParam.nose = value * 100;
		}
		beautyModel.dynamicParam = beautyModel.defaultParam;
	}

	beautyConfig.smoothModel = smoothModel;
	beautyConfig.beautyModel = beautyModel;

	return true;
}
