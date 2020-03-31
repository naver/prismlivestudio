#include "PLSSceneDataMgr.h"

#include "window-basic-main.hpp"

#include <QDebug>

PLSSceneDataMgr *PLSSceneDataMgr::Instance()
{
	static PLSSceneDataMgr mgr;
	return &mgr;
}

PLSSceneDataMgr::PLSSceneDataMgr() {}

PLSSceneDataMgr::~PLSSceneDataMgr() {}

void PLSSceneDataMgr::AddSceneData(const QString &name, PLSSceneItemView *view)
{
	auto iter = sceneDisplay.find(GetCurrentSceneCollectionName());
	if (iter != sceneDisplay.end()) {
		SceneDisplayVector &vec = iter->second;
		for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
			if (iter->first == name) {
				iter->second = view;
				return;
			}
		}
		vec.emplace_back(SceneDisplayVector::value_type(name, view));
	} else {
		SceneDisplayVector vec;
		vec.emplace_back(SceneDisplayVector::value_type(name, view));
		sceneDisplay[GetCurrentSceneCollectionName()] = vec;
	}
}

void PLSSceneDataMgr::RenameSceneData(const QString &preName, const QString &nextName)
{
	auto iterMap = sceneDisplay.find(GetCurrentSceneCollectionName());
	if (iterMap != sceneDisplay.end()) {
		SceneDisplayVector &vec = iterMap->second;
		auto iter = vec.begin();
		for (; iter != vec.end(); ++iter) {
			if (0 == strcmp(iter->first.toStdString().c_str(), preName.toStdString().c_str())) {
				iter->first = nextName;
				break;
			}
		}
	}
}

void PLSSceneDataMgr::CopySrcToDest(const QString &srcName, const QString &destName)
{
	auto iterMap = sceneDisplay.find(srcName);
	if (iterMap != sceneDisplay.end()) {
		SceneDisplayVector vec = iterMap->second;
		sceneDisplay[destName] = vec;
	}
}

void PLSSceneDataMgr::MoveSrcToDest(const QString &srcName, const QString &destName)
{
	auto iterMap = sceneDisplay.find(srcName);
	if (iterMap != sceneDisplay.end()) {
		SceneDisplayVector vec = iterMap->second;
		sceneDisplay.erase(iterMap);
		sceneDisplay[destName] = vec;
	}
}

PLSSceneItemView *PLSSceneDataMgr::FindSceneData(const QString &name)
{
	auto iterMap = sceneDisplay.find(GetCurrentSceneCollectionName());
	if (iterMap != sceneDisplay.end()) {
		SceneDisplayVector &vec = iterMap->second;
		auto iter = vec.begin();
		for (; iter != vec.end(); ++iter) {
			if (0 == strcmp(iter->first.toStdString().c_str(), name.toStdString().c_str())) {
				return iter->second;
			}
		}
	}

	return nullptr;
}

PLSSceneItemView *PLSSceneDataMgr::DeleteSceneData(const QString &name)
{
	auto iterMap = sceneDisplay.find(GetCurrentSceneCollectionName());
	if (iterMap != sceneDisplay.end()) {
		SceneDisplayVector &vec = iterMap->second;
		for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
			if (0 == strcmp(iter->first.toStdString().c_str(), name.toStdString().c_str())) {
				PLSSceneItemView *view = iter->second;
				if (view) {
					view->SetRenderFlag(false);
					view->close();
				}
				iter = vec.erase(iter);
				if (iter != vec.end()) {
					return iter->second;
				} else if (iter == vec.end()) {
					return (--iter)->second;
				}
				break;
			}
		}
	}
	return nullptr;
}

void PLSSceneDataMgr::DeleteAllData()
{
	for (auto iter = sceneDisplay.begin(); iter != sceneDisplay.end(); ++iter) {
		SceneDisplayVector &vec = iter->second;
		for (auto iter = vec.begin(); iter != vec.end();) {
			PLSSceneItemView *view = iter->second;
			if (view) {
				view->SetRenderFlag(false);
				view->close();
				iter = vec.erase(iter);
			} else {
				++iter;
			}
		}
		vec.clear();
	}
	sceneDisplay.clear();
}

void PLSSceneDataMgr::SetDisplayVector(const SceneDisplayVector &dVec, QString file)
{
	if (file.isEmpty()) {
		file = GetCurrentSceneCollectionName();
	}
	auto iter = sceneDisplay.find(file);
	if (iter != sceneDisplay.end()) {
		iter->second = dVec;
	} else {
		sceneDisplay[GetCurrentSceneCollectionName()] = dVec;
	}
}

void PLSSceneDataMgr::SwapData(const int &romoveRow, const int &removeCol, const int &appendRow, const int &appendCol, const int &columnCount)
{
	if (removeCol == appendCol && romoveRow == appendRow) {
		return;
	}

	auto iterMap = sceneDisplay.find(GetCurrentSceneCollectionName());
	if (iterMap == sceneDisplay.end()) {
		return;
	}

	SceneDisplayVector &vec = iterMap->second;
	auto iter = vec.begin() + romoveRow * columnCount + removeCol;
	if (iter == vec.end()) {
		return;
	}

	PLSSceneItemView *view = iter->second;
	vec.erase(iter);
	if (!view) {
		return;
	}
	SwapDataToVec(romoveRow, removeCol, appendRow, appendCol, columnCount, view, vec);
}

void PLSSceneDataMgr::SwapToUp(const QString &name)
{
	PLSSceneItemView *view = nullptr;
	auto iterMap = sceneDisplay.find(GetCurrentSceneCollectionName());
	if (iterMap != sceneDisplay.end()) {
		SceneDisplayVector &vec = iterMap->second;
		auto iter = vec.begin();
		for (; iter != vec.end(); ++iter) {
			if (iter->first == name) {
				if (iter == vec.begin()) { // already in top
					return;
				}
				view = iter->second;
				iter = vec.erase(iter);
				vec.emplace(--iter, SceneDisplayVector::value_type(name, view));
				break;
			}
		}
	}
}

void PLSSceneDataMgr::SwapToDown(const QString &name)
{
	PLSSceneItemView *view = nullptr;
	auto iterMap = sceneDisplay.find(GetCurrentSceneCollectionName());
	if (iterMap != sceneDisplay.end()) {
		SceneDisplayVector &vec = iterMap->second;
		auto iter = vec.begin();
		for (; iter != vec.end(); ++iter) {
			if (iter->first == name) {
				if (iter == vec.end() - 1) { // already in bottom
					return;
				}
				view = iter->second;
				iter = vec.erase(iter);
				vec.emplace(++iter, SceneDisplayVector::value_type(name, view));
				break;
			}
		}
	}
}

void PLSSceneDataMgr::SwapToBottom(const QString &name)
{
	PLSSceneItemView *view = nullptr;
	auto iterMap = sceneDisplay.find(GetCurrentSceneCollectionName());
	if (iterMap != sceneDisplay.end()) {
		SceneDisplayVector &vec = iterMap->second;
		auto iter = vec.begin();
		for (; iter != vec.end(); ++iter) {
			if (iter->first == name) {
				view = iter->second;
				iter = vec.erase(iter);
				break;
			}
		}
		if (iter != vec.end())
			vec.emplace_back(SceneDisplayVector::value_type(name, view));
	}
}

void PLSSceneDataMgr::SwapToTop(const QString &name)
{
	PLSSceneItemView *view = nullptr;
	auto iterMap = sceneDisplay.find(GetCurrentSceneCollectionName());
	if (iterMap != sceneDisplay.end()) {
		SceneDisplayVector &vec = iterMap->second;
		auto iter = vec.begin();
		for (; iter != vec.end(); ++iter) {
			if (iter->first == name) {
				view = iter->second;
				vec.erase(iter);
				vec.emplace(vec.begin(), SceneDisplayVector::value_type(name, view));
				break;
			}
		}
	}
}

int PLSSceneDataMgr::GetSceneSize(QString file)
{
	if (file.isEmpty()) {
		file = GetCurrentSceneCollectionName();
	}
	auto iterMap = sceneDisplay.find(file);
	if (iterMap != sceneDisplay.end()) {
		SceneDisplayVector &vec = iterMap->second;
		return static_cast<int>(vec.size());
	}

	return 0;
}

SceneDisplayVector PLSSceneDataMgr::GetDisplayVector(QString file)
{
	if (file.isEmpty()) {
		file = GetCurrentSceneCollectionName();
	}

	auto iterMap = sceneDisplay.find(file);
	if (iterMap != sceneDisplay.end()) {
		return iterMap->second;
	}

	return SceneDisplayVector();
}

SceneDisplayMap PLSSceneDataMgr::GetAllData()
{
	return sceneDisplay;
}

QStringList PLSSceneDataMgr::GetAllSceneName()
{
	QStringList list{};
	SceneDisplayVector vec = GetDisplayVector();
	for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
		list << iter->first;
	}
	return list;
}

QString PLSSceneDataMgr::GetCurrentSceneCollectionName()
{
	return QString(config_get_string(App()->GlobalConfig(), "Basic", "SceneCollectionFile"));
}

QString PLSSceneDataMgr::GetCurrentSceneCollectionAbsName()
{
	char path[512]{};
	if (GetConfigPath(path, sizeof(path), "PRISMLiveStudio/basic/scenes") <= 0)
		return "";

	return QString(path).append("/").append(GetCurrentSceneCollectionName());
}

void PLSSceneDataMgr::SwapDataToVec(const int &romoveRow, const int &removeCol, const int &appendRow, const int &appendCol, const int &columnCount, PLSSceneItemView *view, SceneDisplayVector &vec)
{
	if (!view) {
		return;
	}

	if (appendRow * columnCount + appendCol >= vec.size() + 1) { // append last
		vec.emplace_back(SceneDisplayVector::value_type(view->GetName(), view));
		return;
	}
	if (appendRow * columnCount + appendCol == 0) { // append first
		vec.emplace(vec.begin(), SceneDisplayVector::value_type(view->GetName(), view));
		return;
	}

	if (removeCol + romoveRow > appendCol + appendRow) { // append middle
		if (romoveRow < appendRow) {
			vec.emplace(vec.begin() + appendRow * columnCount + appendCol - 1, SceneDisplayVector::value_type(view->GetName(), view));
		} else {
			vec.emplace(vec.begin() + appendRow * columnCount + appendCol, SceneDisplayVector::value_type(view->GetName(), view));
		}
	} else {
		if (romoveRow > appendRow) {
			vec.emplace(vec.begin() + appendRow * columnCount + appendCol, SceneDisplayVector::value_type(view->GetName(), view));
		} else {
			vec.emplace(vec.begin() + appendRow * columnCount + appendCol - 1, SceneDisplayVector::value_type(view->GetName(), view));
		}
	}
}
