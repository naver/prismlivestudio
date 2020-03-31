#ifndef PLSSCENELISTVIEW_H
#define PLSSCENELISTVIEW_H

#include "PLSSceneItemView.h"
#include "PLSSceneTransitionsView.h"
#include "PLSDpiHelper.h"
#include <QFrame>

namespace Ui {
class PLSSceneListView;
}

class PLSSceneListView : public QFrame {
	Q_OBJECT

public:
	explicit PLSSceneListView(QWidget *parent = nullptr);
	~PLSSceneListView();

	void AddScene(const QString &name, OBSScene scene, SignalContainer<OBSScene> handler, bool loadingScene = false);
	void DeleteScene(const QString &name);
	void RefreshScene(bool scrollToCurrent = true);
	void MoveSceneToUp();
	void MoveSceneToDown();
	void MoveSceneToTop();
	void MoveSceneToBottom();

	QList<PLSSceneItemView *> FindItems(const QString &name);

	void SetLoadTransitionsData(obs_data_array_t *transitions, obs_source_t *fadeTransition, const int &transitionDuration, const char *currentTransition);
	void ClearTransition();
	void InitTransition(obs_source_t *transition);
	void SetTransition(obs_source_t *transition);
	void AddTransitionsItem(std::vector<OBSSource> &transitions);
	OBSSource GetCurrentTransition();
	obs_source_t *FindTransition(const char *name);
	int GetTransitionDurationValue();
	void SetTransitionDurationValue(const int &value);
	OBSSource GetTransitionComboItem(int idx);
	int GetTransitionComboBoxCount();
	obs_data_array_t *SaveTransitions();
	QSpinBox *GetTransitionDurationSpinBox();
	QComboBox *GetTransitionCombobox();

public slots:
	PLSSceneItemView *GetCurrentItem();

	void SetCurrentItem(PLSSceneItemView *item);
	void SetCurrentItem(const QString &name);
	void OnAddSceneButtonClicked();
	void OnSceneSwitchEffectBtnClicked();
	void OnDragFinished();
	void OnDeleteSceneButtonClicked(PLSSceneItemView *item);

protected:
	void resizeEvent(QResizeEvent *event) override;
	void showEvent(QShowEvent *event) override;

private slots:
	void OnMouseButtonClicked(PLSSceneItemView *item);
	void OnModifySceneButtonClicked(PLSSceneItemView *item);
	void OnFinishingEditName(const QString &text, PLSSceneItemView *item);
	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	void RenameSceneItem(PLSSceneItemView *item, obs_source_t *source, const QString &name);
	void CreateSceneTransitionsView();
signals:
	void SceneRenameFinished();

private:
	Ui::PLSSceneListView *ui;
	PLSSceneTransitionsView *transitionsView{};
	obs_data_array_t *loadTransitions{};
	int loadTransitionDuration;
	QString loadCurrentTransition;
};

#endif // PLSSCENELISTVIEW_H
