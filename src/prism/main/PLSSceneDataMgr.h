#ifndef PLSSCENEDISPLAYMGR_H
#define PLSSCENEDISPLAYMGR_H

#include "PLSSceneItemView.h"
#include "window-projector.hpp"

using SceneDisplayVector = std::vector<std::pair<QString, PLSSceneItemView *>>; // key:scene name , value: scene item
using SceneDisplayMap = std::map<QString, SceneDisplayVector>;                  // key:scene collection name

class PLSSceneDataMgr {
public:
	static PLSSceneDataMgr *Instance();
	explicit PLSSceneDataMgr();
	~PLSSceneDataMgr();

	void AddSceneData(const QString &name, PLSSceneItemView *view);
	void RenameSceneData(const QString &preName, const QString &nextName);
	void CopySrcToDest(const QString &srcName, const QString &destName);
	void MoveSrcToDest(const QString &srcName, const QString &destName);

	PLSSceneItemView *FindSceneData(const QString &name);
	PLSSceneItemView *DeleteSceneData(const QString &name);
	void DeleteAllData();
	void SetDisplayVector(const SceneDisplayVector &dVec, QString file = "");
	void SwapData(const int &romoveRow, const int &removeCol, const int &appendRow, const int &appendCol, const int &columnCount);
	void SwapToUp(const QString &name);
	void SwapToDown(const QString &name);
	void SwapToBottom(const QString &name);
	void SwapToTop(const QString &name);
	int GetSceneSize(QString file = "");
	SceneDisplayVector GetDisplayVector(QString file = "");
	SceneDisplayMap GetAllData();
	QStringList GetAllSceneName();

private:
	QString GetCurrentSceneCollectionName();
	QString GetCurrentSceneCollectionAbsName();
	void SwapDataToVec(const int &romoveRow, const int &removeCol, const int &appendRow, const int &appendCol, const int &columnCount, PLSSceneItemView *view, SceneDisplayVector &vec);

private:
	SceneDisplayMap sceneDisplay;
};

#endif // PLSSCENEDISPLAYMGR_H
