#include "PLSSceneListView.h"
#include "ui_PLSSceneListView.h"

#include "PLSSceneDataMgr.h"
#include "PLSSceneTransitionsView.h"
#include "pls-app.hpp"
#include "qt-wrappers.hpp"
#include "window-namedialog.hpp"
#include "window-basic-main.hpp"
#include "pls-common-language.hpp"
#include "pls-common-define.hpp"
#include "liblog.h"
#include "log/module_names.h"
#include "action.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QPainter>
#include <QDebug>
#include <QThread>
#include <QDirIterator>
#include <QDir>

PLSSceneListView::PLSSceneListView(QWidget *parent) : QFrame(parent), ui(new Ui::PLSSceneListView)
{
	ui->setupUi(this);
	PLSDpiHelper dpiHelper;
	dpiHelper.setCss(this, {PLSCssIndex::PLSScene});
	this->setWindowFlags(windowFlags() ^ Qt::FramelessWindowHint);

	connect(ui->scrollAreaWidgetContents, &PLSScrollAreaContent::DragFinished, this, &PLSSceneListView::OnDragFinished);
	connect(ui->scrollAreaWidgetContents, &PLSScrollAreaContent::resizeEventChanged, this, &PLSSceneListView::RefreshScene);

	CreateSceneTransitionsView();
}

PLSSceneListView::~PLSSceneListView()
{
	if (transitionsView) {
		delete transitionsView;
		transitionsView = nullptr;
	}

	delete ui;
}

void PLSSceneListView::AddScene(const QString &name, OBSScene scene, SignalContainer<OBSScene> handler, bool loadingScene)
{
	PLSSceneItemView *view = PLSSceneDataMgr::Instance()->FindSceneData(name);
	if (!view) {
		view = new PLSSceneItemView(name, scene, ui->scrollAreaWidgetContents);
		view->SetSignalHandler(handler);
		view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		connect(view, &PLSSceneItemView::MouseButtonClicked, this, &PLSSceneListView::OnMouseButtonClicked);
		connect(view, &PLSSceneItemView::ModifyButtonClicked, this, &PLSSceneListView::OnModifySceneButtonClicked);
		connect(view, &PLSSceneItemView::DeleteButtonClicked, this, &PLSSceneListView::OnDeleteSceneButtonClicked);
		connect(view, &PLSSceneItemView::FinishingEditName, this, &PLSSceneListView::OnFinishingEditName);
	}

	PLSSceneDataMgr::Instance()->AddSceneData(name, view);
	if (!loadingScene) {
		SetCurrentItem(view);
	}
}

void PLSSceneListView::DeleteScene(const QString &name)
{
	PLSSceneItemView *view = PLSSceneDataMgr::Instance()->DeleteSceneData(name);

	PLSBasic *main = PLSBasic::Get();
	if (nullptr == main->GetCurrentSceneItem()) {
		if (view) {
			obs_source_t *source = obs_scene_get_source(view->GetData());
			main->SetCurrentScene(source);
		}
	}

	this->RefreshScene();
	this->setFocus();
	main->SaveProjectDeferred();
}

PLSSceneItemView *PLSSceneListView::GetCurrentItem()
{
	SceneDisplayVector data = PLSSceneDataMgr::Instance()->GetDisplayVector();
	for (auto iter = data.begin(); iter != data.end(); ++iter) {
		PLSSceneItemView *item = iter->second;
		if (item && item->GetCurrentFlag()) {
			return item;
		}
	}
	return nullptr;
}

void PLSSceneListView::SetCurrentItem(PLSSceneItemView *item)
{
	if (!item) {
		return;
	}
	SceneDisplayVector data = PLSSceneDataMgr::Instance()->GetDisplayVector();
	for (auto iter = data.begin(); iter != data.end(); ++iter) {
		if (0 == strcmp(item->GetName().toStdString().c_str(), iter->first.toStdString().c_str())) {
			iter->second->SetCurrentFlag(true);
		} else {
			iter->second->SetCurrentFlag(false);
		}
	}
}

void PLSSceneListView::SetCurrentItem(const QString &name)
{
	SceneDisplayVector data = PLSSceneDataMgr::Instance()->GetDisplayVector();
	for (auto iter = data.begin(); iter != data.end(); ++iter) {
		if (0 == strcmp(iter->first.toStdString().c_str(), name.toStdString().c_str())) {
			SetCurrentItem(iter->second);
			break;
		}
	}
}

QList<PLSSceneItemView *> PLSSceneListView::FindItems(const QString &name)
{
	QList<PLSSceneItemView *> items;
	SceneDisplayVector data = PLSSceneDataMgr::Instance()->GetDisplayVector();
	for (auto iter = data.begin(); iter != data.end(); ++iter) {
		if (0 == strcmp(iter->first.toStdString().c_str(), name.toStdString().c_str())) {
			items.push_back(iter->second);
		}
	}
	return items;
}

void PLSSceneListView::SetLoadTransitionsData(obs_data_array_t *transitions, obs_source_t *fadeTransition, const int &transitionDuration_, const char *currentTransition_)
{
	loadTransitions = transitions;
	loadTransitionDuration = transitionDuration_;
	loadCurrentTransition = currentTransition_;

	if (transitionsView) {
		transitionsView->InitLoadTransition(transitions, fadeTransition, transitionDuration_, currentTransition_);
	}
}

void PLSSceneListView::ClearTransition()
{
	if (transitionsView) {
		transitionsView->ClearTransition();
	}
}

void PLSSceneListView::InitTransition(obs_source_t *transition)
{
	if (transitionsView) {
		transitionsView->InitTransition(transition);
	}
}

void PLSSceneListView::SetTransition(obs_source_t *transition)
{
	if (transitionsView) {
		return transitionsView->SetTransition(transition);
	}
}

void PLSSceneListView::AddTransitionsItem(std::vector<OBSSource> &transitions)
{
	if (transitionsView) {
		return transitionsView->AddTransitionsItem(transitions);
	}
}

OBSSource PLSSceneListView::GetCurrentTransition()
{
	if (transitionsView) {
		return transitionsView->GetCurrentTransition();
	}
	return nullptr;
}

obs_source_t *PLSSceneListView::FindTransition(const char *name)
{
	if (transitionsView) {
		return transitionsView->FindTransition(name);
	}
	return nullptr;
}

int PLSSceneListView::GetTransitionDurationValue()
{
	if (transitionsView) {
		return transitionsView->GetTransitionDurationValue();
	}
	return SCENE_TRANSITION_DEFAULT_DURATION_VALUE;
}

void PLSSceneListView::SetTransitionDurationValue(const int &value)
{
	if (transitionsView) {
		return transitionsView->SetTransitionDurationValue(value);
	}
}

OBSSource PLSSceneListView::GetTransitionComboItem(int idx)
{
	if (transitionsView) {
		return transitionsView->GetTransitionByIndex(idx);
	} else {
		return OBSSource();
	}
}

int PLSSceneListView::GetTransitionComboBoxCount()
{
	if (transitionsView) {
		return transitionsView->GetTransitionComboBoxCount();
	} else {
		return 0;
	}
}

obs_data_array_t *PLSSceneListView::SaveTransitions()
{
	if (transitionsView) {
		return transitionsView->SaveTransitions();
	}
	return nullptr;
}

QSpinBox *PLSSceneListView::GetTransitionDurationSpinBox()
{
	if (transitionsView) {
		return transitionsView->GetTransitionDuration();
	}
	return nullptr;
}

QComboBox *PLSSceneListView::GetTransitionCombobox()
{
	if (transitionsView) {
		return transitionsView->GetTransitionCombobox();
	}
	return nullptr;
}

void PLSSceneListView::RefreshScene(bool scrollToCurrent)
{
	int y = ui->scrollAreaWidgetContents->Refresh();
	if (scrollToCurrent) {
		ui->scrollArea->verticalScrollBar()->setValue(y);
	}
}

void PLSSceneListView::MoveSceneToUp()
{
	PLSSceneItemView *item = this->GetCurrentItem();
	if (item) {
		PLSSceneDataMgr::Instance()->SwapToUp(item->GetName());
		RefreshScene();
	}
}

void PLSSceneListView::MoveSceneToDown()
{
	PLSSceneItemView *item = this->GetCurrentItem();
	if (item) {
		PLSSceneDataMgr::Instance()->SwapToDown(item->GetName());
		RefreshScene();
	}
}

void PLSSceneListView::MoveSceneToTop()
{
	PLSSceneItemView *item = this->GetCurrentItem();
	if (item) {
		PLSSceneDataMgr::Instance()->SwapToTop(item->GetName());
		RefreshScene();
	}
}

void PLSSceneListView::MoveSceneToBottom()
{
	PLSSceneItemView *item = this->GetCurrentItem();
	if (item) {
		PLSSceneDataMgr::Instance()->SwapToBottom(item->GetName());
		RefreshScene();
	}
}

void PLSSceneListView::OnAddSceneButtonClicked()
{
	std::string name;
	QString format{QTStr("Basic.Main.DefaultSceneName.Text")};
	PLS_UI_STEP(MAINSCENE_MODULE, "Add Scene", ACTION_CLICK);

	int i = 2;
	QString placeHolderText = format.arg(i);
	obs_source_t *source = nullptr;
	while ((source = obs_get_source_by_name(QT_TO_UTF8(placeHolderText)))) {
		obs_source_release(source);
		placeHolderText = format.arg(++i);
	}

	bool accepted = NameDialog::AskForName(this, QTStr("Basic.Main.AddSceneDlg.Title"), QTStr("Basic.Main.AddSceneDlg.Text"), name, placeHolderText);
	if (accepted) {

		name = QString(name.c_str()).simplified().toStdString();
		if (name.empty()) {
			PLSMessageBox::warning(this, QTStr("NoNameEntered.Title"), QTStr("NoNameEntered.Text"));
			OnAddSceneButtonClicked();
			return;
		}

		obs_source_t *source = obs_get_source_by_name(name.c_str());
		if (source) {
			PLSMessageBox::warning(this, QTStr("NameExists.Title"), QTStr("NameExists.Text"));

			obs_source_release(source);
			OnAddSceneButtonClicked();
			return;
		}

		obs_scene_t *scene = obs_scene_create(name.c_str());
		source = obs_scene_get_source(scene);

		PLSBasic *main = PLSBasic::Get();
		if (main) {
			main->SetCurrentScene(source);
		}
		obs_scene_release(scene);
	}
}

void PLSSceneListView::OnSceneSwitchEffectBtnClicked()
{
	PLS_UI_STEP(MAINSCENE_MODULE, "scene transition", ACTION_CLICK);
	if (!transitionsView) {
		CreateSceneTransitionsView();
	}
	transitionsView->show();
	transitionsView->raise();
}

void PLSSceneListView::OnDragFinished()
{
	this->RefreshScene();

	PLSBasic *main = PLSBasic::Get();
	if (main) {
		main->SaveProjectDeferred();
	}
	PLSProjector::UpdateMultiviewProjectors();
}

void PLSSceneListView::resizeEvent(QResizeEvent *event)
{
	QFrame::resizeEvent(event);
}

void PLSSceneListView::showEvent(QShowEvent *event)
{
	QFrame::showEvent(event);
	ui->scrollAreaWidgetContents->resize(ui->scrollArea->width(), ui->scrollArea->height());
}

void PLSSceneListView::OnMouseButtonClicked(PLSSceneItemView *item)
{
	PLSBasic *main = PLSBasic::Get();
	if (main && item) {
		OBSScene scene = item->GetData();
		if (scene == main->GetCurrentScene()) {
			return;
		}
		obs_source_t *source = obs_scene_get_source(scene);
		main->SetCurrentScene(source);
		main->OnScenesCurrentItemChanged();
	}
}

struct DeleteSceneSourceHelper {
	const char *delete_scene_name;        // input param
	std::vector<obs_sceneitem_t *> items; // output list
};

static bool EnumItemForDelScene(obs_scene_t *, obs_sceneitem_t *item, void *ptr)
{
	DeleteSceneSourceHelper *helper = (DeleteSceneSourceHelper *)ptr;

	if (obs_sceneitem_is_group(item)) {
		obs_sceneitem_group_enum_items(item, EnumItemForDelScene, ptr);
	} else {
		obs_source_t *source = obs_sceneitem_get_source(item);
		if (source) {
			const char *id = obs_source_get_id(source);
			if (id && 0 == strcmp(id, SCENE_SOURCE_ID)) {
				const char *name = obs_source_get_name(source);
				if (name && 0 == strcmp(name, helper->delete_scene_name)) {
					helper->items.push_back(item);
				}
			}
		}
	}

	return true;
}

void DeleteRelatedSceneSource(const char *scene_name)
{
	if (!scene_name) {
		return;
	}

	auto cb = [](void *helper, obs_source_t *src) {
		obs_scene_t *scene = obs_scene_from_source(src);
		if (scene) {
			obs_scene_enum_items(scene, EnumItemForDelScene, (void *)helper);
		}
		return true;
	};

	DeleteSceneSourceHelper helper;
	helper.delete_scene_name = scene_name;
	obs_enum_scenes(cb, &helper);

	size_t count = helper.items.size();
	for (size_t i = 0; i < count; i++) {
		obs_sceneitem_remove(helper.items[i]);
	}
}

static bool EnumItemForInteraction(obs_scene_t *, obs_sceneitem_t *item, void *ptr)
{
	if (obs_sceneitem_is_group(item)) {
		obs_sceneitem_group_enum_items(item, EnumItemForInteraction, ptr);
	} else {
		PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
		bool successed = main->CheckHideInteraction(OBSSceneItem(item));
		if (successed) {
			return false;
		}
	}

	return true;
}

void PLSSceneListView::OnDeleteSceneButtonClicked(PLSSceneItemView *item)
{
	if (!item) {
		return;
	}
	OBSScene scene = item->GetData();
	obs_source_t *source = obs_scene_get_source(scene);

	PLSBasic *main = PLSBasic::Get();
	if (source && main && main->QueryRemoveSource(source)) {
		obs_scene_enum_items(scene, EnumItemForInteraction, NULL);
		DeleteRelatedSceneSource(obs_source_get_name(source));
		obs_source_remove(source);
	}
}

void PLSSceneListView::OnModifySceneButtonClicked(PLSSceneItemView *item)
{
	Q_UNUSED(item);
}

void PLSSceneListView::OnFinishingEditName(const QString &text, PLSSceneItemView *item)
{
	if (!item) {
		return;
	}
	OBSScene scene = item->GetData();
	obs_source_t *source = obs_scene_get_source(scene);
	RenameSceneItem(item, source, text);
}

void PLSSceneListView::contextMenuEvent(QContextMenuEvent *event)
{
	PLSBasic *main = PLSBasic::Get();
	if (main) {
		PLSSceneItemView *item = main->GetCurrentSceneItem();
		if (item) {
			obs_source_t *source = obs_scene_get_source(item->GetData());
			main->SetCurrentScene(source);
			main->OnScenesCustomContextMenuRequested(item);
		}
	}
	QFrame::contextMenuEvent(event);
}

void PLSSceneListView::RenameSceneItem(PLSSceneItemView *item, obs_source_t *source, const QString &name)
{
	const char *prevName = obs_source_get_name(source);
	if (name == prevName || !item)
		return;

	QString trimmedText = QT_TO_UTF8(name.simplified());

	obs_source_t *foundSource = obs_get_source_by_name(QT_TO_UTF8(trimmedText));
	if (foundSource || trimmedText.isEmpty()) {
		item->SetName(prevName);
		PLSBasic *main = PLSBasic::Get();
		if (foundSource) {
			PLSMessageBox::warning(main, QTStr("NameExists.Title"), QTStr("NameExists.Text"));
		} else if (trimmedText.isEmpty()) {
			PLSMessageBox::warning(main, QTStr("NoNameEntered.Title"), QTStr("NoNameEntered.Text"));
		}

		obs_source_release(foundSource);
	} else {
		item->SetName(trimmedText);
		obs_source_set_name(source, trimmedText.toStdString().c_str());
		PLSSceneDataMgr::Instance()->RenameSceneData(prevName, trimmedText);
		emit SceneRenameFinished();
	}
}

void PLSSceneListView::CreateSceneTransitionsView()
{
	PLSBasic *main = PLSBasic::Get();
	transitionsView = new PLSSceneTransitionsView(main);
	transitionsView->hide();
}
