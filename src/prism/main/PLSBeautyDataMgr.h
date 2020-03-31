#ifndef PLSBEAUTYDATAMGR_H
#define PLSBEAUTYDATAMGR_H

#include "PLSBeautyDefine.h"

#include <map>
#include <vector>
#include <QString>
#include "obs.hpp"

using BeautyConfigMap = std::vector<std::pair<QString, BeautyConfig>>; // key: beauty filter id
using BeautySourceIdMap = std::map<OBSSceneItem, BeautyConfigMap>;     // key: scene item

class PLSBeautyDataMgr {
public:
	static PLSBeautyDataMgr *Instance();
	void SetBeautyConfig(OBSSceneItem item, const QList<BeautyConfig> &configList);
	void SetBeautyConfig(OBSSceneItem item, const QString &filterId, const BeautyConfig &config);

	// Apis for custom config.
	void SetBeautyCustomConfig(const QList<BeautyConfig> &configList);
	void AddBeautyCustomConfig(const QString &filterId, const BeautyConfig &config);
	const BeautyConfigMap &GetBeautyCustomConfig() const;

	// APIs for preset config.
	void SetBeautyPresetConfig(const QList<BeautyConfig> &configList);
	void AddBeautyPresetConfig(const QString &filterId, const BeautyConfig &config);
	const BeautyConfigMap &GetBeautyPresetConfig() const;
	BeautyConfig GetBeautyPresetConfigById(const QString &filterId) const;

	bool RenameCustomBeautyConfig(const QString &filterId, const QString &newFilterId);

	void SetCustomBeautyConfig(OBSSceneItem item, const QString &filterId, const BeautyConfig &config);
	void SetBeautyCurrentStateConfig(OBSSceneItem item, const QString &filterId, const bool &state);
	void SetBeautyCheckedStateConfig(OBSSceneItem item, const bool &state);
	int GetBeautyFaceSizeBySourceId(OBSSceneItem item);
	int GetCustomFaceSizeBySourceId(OBSSceneItem item);
	BeautySourceIdMap GetData();
	BeautyConfigMap GetBeautyConfigBySourceId(OBSSceneItem item);
	bool GetBeautyConfig(OBSSceneItem item, const QString &filterId, BeautyConfig &config);
	bool ResetBeautyConfig(OBSSceneItem item, const QString &filterId);
	bool DeleteBeautyFilterConfig(const QString &filterId);
	bool DeleteBeautyCustomConfig(const QString &filterId);
	bool DeleteBeautySourceConfig(OBSSceneItem item);
	void ClearBeautyConfig();
	bool CopyBeautyConfig(OBSSceneItem srcItem, OBSSceneItem destItem);
	bool CopyBeautyConfig(OBSSceneItem destItem);
	QString GetCheckedFilterName(OBSSceneItem item);
	QString GetCheckedFilterBaseName(OBSSceneItem item);
	QString GetCloseUnusedFilterName(OBSSceneItem item, const QString &baseId);
	bool FindFilterId(OBSSceneItem item, const QString &filterId, BeautyConfig &config);
	bool FindFilterId(OBSSceneItem item, const QString &filterId);
	bool RenameFilterId(const QString &filterId, const QString &newId);
	int GetValidFilterIndex(OBSSceneItem item, const QString &baseId);
	bool ParseJsonArrayToBeautyConfig(const QByteArray &byteArray, BeautyConfig &config);

private:
	PLSBeautyDataMgr();
	~PLSBeautyDataMgr();
	bool UpdateSameSource(OBSSceneItem item, const QString &filterId, const BeautyConfig &config);

private:
	BeautySourceIdMap beautySourceIdData;
	BeautyConfigMap beautyCustomConfig; //For saving custom beauty default config.
	BeautyConfigMap beautyPresetConfig; //For saving preset beauty default config.
};

#endif // PLSBEAUTYDATAMGR_H
