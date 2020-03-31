#include "PLSBgmDataManager.h"
#include "PLSBgmItemCoverView.h"
#include "PLSBgmItemView.h"

#include <QDir>

#include "frontend-api.h"
#include "json-data-handler.hpp"
#include "log.h"
#include "log/module_names.h"
#include "pls-common-define.hpp"

PLSBgmDataViewManager *PLSBgmDataViewManager::Instance()
{
	static PLSBgmDataViewManager instance;
	return &instance;
}

// mp3 *.aac *.ogg *.wav
bool PLSBgmDataViewManager::IsSupportFormat(const QString &url)
{
	if (!url.toLower().right(3).compare("mp3") || !url.toLower().right(3).compare("aac") || !url.toLower().right(3).compare("ogg") || !url.toLower().right(3).compare("wav")) {
		return true;
	}
	return false;
}

bool PLSBgmDataViewManager::IsSupportFormat(const QList<QUrl> urls)
{
	bool existedSupportFormat = false;
	for (auto &url : urls) {
		if (IsSupportFormat(url.path())) {
			existedSupportFormat = true;
			break;
		}
	}
	return existedSupportFormat;
}

QString PLSBgmDataViewManager::ConvertIntToTimeString(const int &seconds)
{
	QTime currentTime((seconds / 3600) % 60, (seconds / 60) % 60, seconds % 60, (seconds * 1000) % 1000);

	QString format = "mm:ss";
	if (seconds >= 3600) {
		format = "hh:mm:ss";
	}
	return currentTime.toString(format);
}

QPushButton *PLSBgmDataViewManager::GetGroupButton(const QString &group)
{
	for (auto iter = groupButtonList.begin(); iter != groupButtonList.end(); ++iter) {
		if (0 == iter->first.compare(group)) {
			return iter->second;
		}
	}

	return nullptr;
}

void PLSBgmDataViewManager::AddGroupButton(const QString group, QPushButton *button)
{
	if (group.isEmpty() || !button) {
		return;
	}

	groupButtonList.emplace(group, button);
}

void PLSBgmDataViewManager::DeleteGroupButton()
{
	if (groupButtonList.empty()) {
		return;
	}

	for (auto iter = groupButtonList.begin(); iter != groupButtonList.end();) {
		QPushButton *item = iter->second;
		iter = groupButtonList.erase(iter);
		if (item) {
			item->deleteLater();
			item = nullptr;
		}
	}
}

PushButtonMapType PLSBgmDataViewManager::GetGroupButtonList()
{
	return groupButtonList;
}

void PLSBgmDataViewManager::AddGroupList(const QString &group, const BgmLibraryData &bgm)
{
	groupMusicList.emplace_back(group, bgm);
}

void PLSBgmDataViewManager::InsertFrontGroupList(const QString &group, const BgmLibraryData &bgm)
{
	groupMusicList.insert(groupMusicList.begin(), {group, bgm});
}

const BgmItemGroupVectorType &PLSBgmDataViewManager::GetGroupList() const
{
	return groupMusicList;
}

BgmLibraryData PLSBgmDataViewManager::GetGroupList(const QString &group)
{
	if (group.isEmpty()) {
		return BgmLibraryData();
	}

	bool findGroup = false;
	auto iterGroup = groupMusicList.cbegin();
	for (; iterGroup != groupMusicList.cend(); ++iterGroup) {
		if (0 == group.compare(iterGroup->first)) {
			findGroup = true;
			break;
		}
	}

	if (!findGroup) {
		return BgmLibraryData();
	}

	return iterGroup->second;
}

QString PLSBgmDataViewManager::GetFirstGroupName()
{
	auto groupList = PLSBgmDataViewManager::Instance()->GetGroupList();
	auto iter = groupList.begin();
	if (iter == groupList.end()) {
		return QString();
	}

	return iter->first;
}

void PLSBgmDataViewManager::AddCachePlayList(const PLSBgmItemData &data)
{
	bool find = CachePlayListExisted(data);
	if (find) {
		return;
	}
	cachePlayList.push_front(data);
}

void PLSBgmDataViewManager::ClearCachePlayList()
{
	cachePlayList.clear();
}

void PLSBgmDataViewManager::DeleteCachePlayList(const PLSBgmItemData &data)
{
	for (auto iter = cachePlayList.begin(); iter != cachePlayList.end(); ++iter) {
		PLSBgmItemData data_ = *iter;
		if (data_.title == data.title && data_.id == data.id) {
			cachePlayList.erase(iter);
			break;
		}
	}
}

bool PLSBgmDataViewManager::CachePlayListExisted(const PLSBgmItemData &data)
{
	bool find = false;
	for (auto iter = cachePlayList.begin(); iter != cachePlayList.end(); ++iter) {
		PLSBgmItemData data_ = *iter;
		if (data_.title == data.title && data_.id == data.id) {
			find = true;
			break;
		}
	}
	return find;
}

size_t PLSBgmDataViewManager::GetCachePlayListSize() const
{
	return cachePlayList.size();
}

BgmItemCacheType PLSBgmDataViewManager::GetCachePlayList()
{
	return cachePlayList;
}

bool PLSBgmDataViewManager::LoadDataFormLocalFile(QByteArray &array)
{
	QString paths = pls_get_user_path(QString(CONFIGS_MUSIC_USER_PATH).append(MUSIC_JSON_FILE));
	if (PLSJsonDataHandler::getJsonArrayFromFile(array, paths)) {
		return true;
	}

	PLS_ERROR("Load %s Failed", paths.toStdString().c_str());
	return false;
}

void PLSBgmItemData::SetUrl(const QString &url, const int &id)
{
	if (!url.isEmpty())
		urls.insert({url, id});
	DurationType type = static_cast<DurationType>(id);
	switch (type) {
	case DurationType::FifteenSeconds:
		fifteenUrl = url;
		break;
	case DurationType::ThirtySeconds:
		thirtyUrl = url;
		break;
	case DurationType::SixtySeconds:
		sixtyUrl = url;
		break;
	default:
		fullUrl = url;
		break;
	}
}

QString PLSBgmItemData::GetUrl(const int &id) const
{
	DurationType type = static_cast<DurationType>(id);
	if (type == DurationType::FifteenSeconds) {
		return fifteenUrl;
	} else if (type == DurationType::ThirtySeconds) {
		return thirtyUrl;
	} else if (type == DurationType::SixtySeconds) {
		return sixtyUrl;
	} else {
		return fullUrl;
	}
}

int PLSBgmItemData::GetDuration(const int &id) const
{
	DurationType type = static_cast<DurationType>(id);
	if (type == DurationType::FifteenSeconds) {
		return fifteenDuration;
	} else if (type == DurationType::ThirtySeconds) {
		return thirtyDuration;
	} else if (type == DurationType::SixtySeconds) {
		return sixtyDuration;
	} else {
		return fullDuration;
	}
}

void PLSBgmItemData::SetDuration(const int &id, const int &duration)
{
	DurationType type = static_cast<DurationType>(id);
	switch (type) {
	case DurationType::FifteenSeconds:
		fifteenDuration = duration;
		break;
	case DurationType::ThirtySeconds:
		thirtyDuration = duration;
		break;
	case DurationType::SixtySeconds:
		sixtyDuration = duration;
		break;
	default:
		fullDuration = duration;
		break;
	}
}

QString PLSBgmItemData::GetKey()
{
	return title + QString::number(id);
}

QString PLSBgmItemData::ConvertIntToTimeString(const int &seconds)
{
	QTime currentTime((seconds / 3600) % 60, (seconds / 60) % 60, seconds % 60, (seconds * 1000) % 1000);

	QString format = "mm:ss";
	if (seconds >= 3600) {
		format = "hh:mm:ss";
	}
	return currentTime.toString(format);
}
