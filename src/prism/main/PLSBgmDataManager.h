#ifndef PLSBACKGROUNDMUSICDATAMGR_H
#define PLSBACKGROUNDMUSICDATAMGR_H

#include <QByteArray>
#include <QPointer>
#include <QPushButton>
#include <QString>
#include <map>
#include <set>
#include <vector>

#include "obs.hpp"

enum class DurationType { FifteenSeconds, ThirtySeconds, SixtySeconds, FullSeconds, Unknown };

enum CustomDataRole { DataRole = Qt::UserRole, NeedPaintRole, MediaStatusRole, DropIndicatorRole, CoverPathRole, RowStatusRole };
Q_DECLARE_METATYPE(CustomDataRole)

enum class MediaStatus { stateNormal, stateLoading, statePlaying, statePause, stateInvalid };
Q_DECLARE_METATYPE(MediaStatus)

enum class RowStatus { stateNormal, stateHover, stateSelected };
Q_DECLARE_METATYPE(RowStatus)

enum class ButtonState { Normal, Hover, Pressed, Disabled };

enum class DropIndicator { None, Top, Bottom };
Q_DECLARE_METATYPE(DropIndicator)

class PLSBgmItemView;
class PLSBgmItemCoverView;
class PLSBgmItemData;

using PushButtonMapType = std::map<QString, QPointer<QPushButton>>; //key: group
using BgmItemMapType = std::map<QString, QPointer<PLSBgmItemView>>; //key: title
//using BgmItemPairType = std::vector<std::pair<QString, QPointer<PLSBgmItemView>>>;       //key: title + duration type
//using BgmItemSourcePairType = std::vector<std::pair<quint64, BgmItemPairType>>;          //key: sceneitem ptr
using BgmItemCoverType = std::vector<std::pair<quint64, QPointer<PLSBgmItemCoverView>>>; //key: sceneitem ptr
using BgmItemVectorType = std::vector<QPointer<PLSBgmItemView>>;
using BgmItemCacheType = QVector<PLSBgmItemData>;

// music library data struct
using BgmLibraryData = std::map<QString, PLSBgmItemData>;
using BgmItemGroupVectorType = std::vector<std::pair<QString, BgmLibraryData>>; //key: group

class PLSBgmItemData {
public:
	PLSBgmItemData() {}
	~PLSBgmItemData() {}

	void SetUrl(const QString &url, const int &id);
	QString GetUrl(const int &id) const;
	int GetDuration(const int &id) const;
	void SetDuration(const int &id, const int &duration);
	QString GetKey();
	QString ConvertIntToTimeString(const int &seconds);

public:
	QString title{};
	QString producer{};
	QString group{};

	QString fullUrl{};
	QString fifteenUrl{};
	QString thirtyUrl{};
	QString sixtyUrl{};
	int fullDuration{0};
	int fifteenDuration{0};
	int thirtyDuration{0};
	int sixtyDuration{0};
	int id{-1};
	bool isLocalFile{false};
	bool isChecked{false};
	bool isCurrent{false};
	bool isDisable{false};
	bool haveCover{false};
	std::map<QString, int> urls;
	QString coverPath;
	MediaStatus mediaStatus{MediaStatus::stateNormal};
	DropIndicator dropIndicator{DropIndicator::None};
	RowStatus rowStatus{RowStatus::stateNormal};
};
Q_DECLARE_METATYPE(PLSBgmItemData)

class PLSBgmDataViewManager {
public:
	static PLSBgmDataViewManager *Instance();
	bool LoadDataFormLocalFile(QByteArray &array);
	bool IsSupportFormat(const QString &url);
	bool IsSupportFormat(const QList<QUrl> urls);
	QString ConvertIntToTimeString(const int &seconds);

	//group button
	QPushButton *GetGroupButton(const QString &group);
	void AddGroupButton(const QString group, QPushButton *button);
	void DeleteGroupButton();
	PushButtonMapType GetGroupButtonList();

	// group list
	void AddGroupList(const QString &group, const BgmLibraryData &bgm);
	void InsertFrontGroupList(const QString &group, const BgmLibraryData &bgm);

	const BgmItemGroupVectorType &GetGroupList() const;
	BgmLibraryData GetGroupList(const QString &group);
	QString GetFirstGroupName();

	// cache play list
	void AddCachePlayList(const PLSBgmItemData &data);
	void ClearCachePlayList();
	void DeleteCachePlayList(const PLSBgmItemData &data);
	bool CachePlayListExisted(const PLSBgmItemData &data);
	size_t GetCachePlayListSize() const;
	BgmItemCacheType GetCachePlayList();

private:
	PLSBgmDataViewManager() {}
	~PLSBgmDataViewManager() {}

private:
	PushButtonMapType groupButtonList{};
	BgmItemGroupVectorType groupMusicList{};
	BgmItemCacheType cachePlayList{}; //cache playlist when user clicked add button
					  //BgmItemCoverType coverList{};
};

#endif // PLSBACKGROUNDMUSICDATAMGR_H
