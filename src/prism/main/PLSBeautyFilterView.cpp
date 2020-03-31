#include "PLSBeautyFilterView.h"
#include "PLSBeautyDataMgr.h"
#include "ui_PLSBeautyFilterView.h"
#include "PLSBeautyFaceItemView.h"
#include "PLSSceneDataMgr.h"

#include <QDir>
#include <QDesktopWidget>

#include "frontend-api.h"
#include "pls-app.hpp"
#include "pls-common-define.hpp"
#include "json-data-handler.hpp"
#include "liblog.h"
#include "log/module_names.h"
#include "action.h"
#include "layout/flowlayout.h"
#include "qt-wrappers.hpp"
#include "pls-common-language.hpp"
#include "window-namedialog.hpp"
#include "main-view.hpp"
#include "PLSAction.h"

Q_DECLARE_METATYPE(OBSSceneItem);

const int BEAUTY_CUSTOM_FACE_MAX_NUM = 9;
const QString &paramRegEx = "^([1-9][0-9]{0,1}|100)$";
const int minFilterValue = 1;
const int maxFilterValue = 100;
const char *lastValidValueDefine = "lastValidValue";
QVector<QVector<QString>> PLSBeautyFilterView::iconFileNameVector;

void OnPrismAppQuit(enum obs_frontend_event event, void *context)
{
	if (event == OBS_FRONTEND_EVENT_EXIT) {
		PLSBeautyFilterView *view = static_cast<PLSBeautyFilterView *>(context);
		if (view) {
			if (!view->getMaxState())
				view->onSaveNormalGeometry();
			view->deleteLater();
		}
	}
}

PLSBeautyFilterView::PLSBeautyFilterView(const QString &sourceName_, QWidget *parent, PLSDpiHelper dpiHelper)
	: PLSDialogView(parent, dpiHelper), currentSourceName(sourceName_), ui(new Ui::PLSBeautyFilterView)
{
	dpiHelper.setCss(this, {PLSCssIndex::QComboBox, PLSCssIndex::Beauty});

	notifyFirstShow([=]() { this->InitGeometry(); });

	ui->setupUi(this->content());
	setHasMaxResButton(true);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	this->setWindowTitle(QTStr(MAIN_BEAUTY_TITLE));
	ui->verticalLayout->setAlignment(Qt::AlignTop);
	obs_frontend_add_event_callback(PLSFrontendEvent, this);
	obs_frontend_add_event_callback(OnPrismAppQuit, this);

	flowLayout = new FlowLayout(ui->thumbnailWidget, 0, 8, 8);
	connect(flowLayout, &FlowLayout::LayoutFinished, this, &PLSBeautyFilterView::OnLayoutFinished, Qt::DirectConnection);
	flowLayout->setContentsMargins(20, 17, 20, 17);
	flowLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	ui->verticalLayout_scroll->setAlignment(Qt::AlignTop);

	InitSlider(ui->skinSlider, minFilterValue, maxFilterValue, 1);
	InitSlider(ui->chinSlider, minFilterValue, maxFilterValue, 1);
	InitSlider(ui->cheekSlider, minFilterValue, maxFilterValue, 1);
	InitSlider(ui->cheekboneSlider, minFilterValue, maxFilterValue, 1);
	InitSlider(ui->eyeSlider, minFilterValue, maxFilterValue, 1);
	InitSlider(ui->noseSlider, minFilterValue, maxFilterValue, 1);

	InitLineEdit();
	InitConnections();

	auto notifyDipChangeEvent = [this](double dpi) {
		flowLayout->setHorizontalSpacing(PLSDpiHelper::calculate(dpi, 8));
		flowLayout->setverticalSpacing(PLSDpiHelper::calculate(dpi, 8));
	};
	dpiHelper.notifyDpiChanged(this, notifyDipChangeEvent);
}

PLSBeautyFilterView::~PLSBeautyFilterView()
{
	obs_frontend_remove_event_callback(PLSFrontendEvent, this);
	obs_frontend_remove_event_callback(OnPrismAppQuit, this);
	DeleteAllBeautyViewInCurrentSource();
	delete ui;
}

void PLSBeautyFilterView::AddSourceName(const QString &sourceName_, OBSSceneItem item)
{
	if (sourceName_.isEmpty() || !item) {
		return;
	}

	// Add new source,copy config from preset config and custom config.
	if (!PLSBeautyDataMgr::Instance()->CopyBeautyConfig(item)) {
		PLS_INFO(MAIN_BEAUTY_MODULE, "No source beauty filter Existed, Use default.");
		SetCurrentSourceName(sourceName_, item);
		DShowSourceVecType sourceList{DShowSourceVecType::value_type(sourceName_, item)};
		AddSourceComboboxList(sourceList);
		LoadBeautyFaceView(item);
		return;
	}

	ignoreChangeIndex = true;
	SetCurrentSourceName(sourceName_, item);
	ui->sourceListComboBox->insertItem(0, sourceName_, QVariant::fromValue(item));
	SetSourceVisible(sourceName_, item, IsDShowSourceAvailable(sourceName_, item));
	SetCurrentClickedFaceView();
	ignoreChangeIndex = false;
}

void PLSBeautyFilterView::RenameSourceName(OBSSceneItem item, const QString &newName, const QString &prevName)
{
	Q_UNUSED(item);
	for (int i = 0; i < ui->sourceListComboBox->count(); i++) {
		if (prevName == ui->sourceListComboBox->itemText(i)) {
			ui->sourceListComboBox->setItemText(i, newName);
		}
	}
	WriteBeautyConfigToLocal();
}

void PLSBeautyFilterView::RemoveSourceName(const QString &sourceName, OBSSceneItem item)
{
	RemoveItem(sourceName, item);

	for (auto iter = listItems.begin(); iter != listItems.end();) {
		PLSBeautyFaceItemView *item = *iter;
		if (!item) {
			++iter;
			continue;
		}

		if (!item->IsCustom()) {
			++iter;
			continue;
		}

		item->deleteLater();
		item = nullptr;
		iter = listItems.erase(iter);
	}

	WriteBeautyConfigToLocal();
}

void PLSBeautyFilterView::RemoveSourceNameList(bool isCurrentScene, const DShowSourceVecType &list)
{
	if (isCurrentScene) {
		currentSource = nullptr;
	}

	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		RemoveSourceName(iter->first, iter->second);
	}

	if (0 == GetSourceComboboxVisibleCount()) {
		ui->sourceListComboBox->addItem(QTStr("main.beauty.face.nosource"));
	}

	WriteBeautyConfigToLocal();
}

static int GetNextVisibleItemIndex(QWidget *widget)
{
	if (!widget) {
		return -1;
	}

	PLSComboBox *combobox = qobject_cast<PLSComboBox *>(widget);
	if (!combobox) {
		return -1;
	}

	QListView *view = qobject_cast<QListView *>(combobox->view());
	if (!view) {
		return -1;
	}

	for (int i = 0; i < combobox->count(); i++) {
		if (view->isRowHidden(i)) {
			continue;
		}
		return i;
	}

	return -1;
}

void PLSBeautyFilterView::SetSourceVisible(const QString &sourceName, OBSSceneItem item, bool visible)
{
	QListView *view = qobject_cast<QListView *>(ui->sourceListComboBox->view());
	if (!view) {
		return;
	}

	int index = FindText(sourceName, item);
	view->setRowHidden(index, !visible);

	if (!visible) {
		SetSourceInvisible(sourceName, item);
		return;
	}

	if (!IsDShowSourceValid(sourceName, item)) {
		view->setRowHidden(index, visible);
		return;
	}

	// If the visible source is the selected source,set it as current index in combobox.
	if (item == currentSource && currentSourceName == sourceName) {
		SetCurrentIndex(sourceName, item);
	}

	if (ui->sourceListComboBox->currentText() == QTStr("main.beauty.face.nosource")) {
		SetCurrentIndex(sourceName, item);
	}

	UpdateBeautyParamToRender(item, GetSourceCurrentFilterName(item));
}

void PLSBeautyFilterView::SetSourceSelect(const QString &sourceName, OBSSceneItem item, bool selected)
{
	QListView *view = qobject_cast<QListView *>(ui->sourceListComboBox->view());
	if (!view) {
		return;
	}

	if (!IsDShowSourceAvailable(sourceName, item)) {
		SetCurrentSourceName(sourceName, item);
		return;
	}

	if (sourceName == ui->sourceListComboBox->currentText() && item == ui->sourceListComboBox->currentData().value<OBSSceneItem>()) {
		SetCurrentSourceName(ui->sourceListComboBox->currentText(), ui->sourceListComboBox->currentData().value<OBSSceneItem>());
		return;
	}

	if (isSourceComboboxVisible(sourceName, item)) {
		if (SetCurrentIndex(sourceName, item)) {
			return;
		}
	}

	int index = GetNextVisibleItemIndex(ui->sourceListComboBox);
	SetCurrentIndex(ui->sourceListComboBox->itemText(index), ui->sourceListComboBox->itemData(index).value<OBSSceneItem>());
}

void PLSBeautyFilterView::UpdateSourceList(const QString &sourceName, OBSSceneItem item, const DShowSourceVecType &sourceList)
{
	ignoreChangeIndex = true;
	WriteBeautyConfigToLocal();
	AddSourceComboboxList(sourceList);

	if (isSourceComboboxVisible(sourceName, item) && !sourceName.isEmpty()) {
		if (SetCurrentIndex(sourceName, item)) {
			SetCurrentClickedFaceView();
			ignoreChangeIndex = false;
			return;
		}
	}

	if (isCurrentSourceExisted(currentSourceName) && isSourceComboboxVisible(currentSourceName, currentSource) && -1 != ui->sourceListComboBox->findText(this->currentSourceName)) {
		ui->sourceListComboBox->setCurrentText(currentSourceName);
		SetCurrentClickedFaceView();
		ignoreChangeIndex = false;
		return;
	}

	int index = GetNextVisibleItemIndex(ui->sourceListComboBox);
	SetCurrentIndex(ui->sourceListComboBox->itemText(index), ui->sourceListComboBox->itemData(index).value<OBSSceneItem>());
	SetCurrentClickedFaceView();
	ignoreChangeIndex = false;
}

void PLSBeautyFilterView::ReorderSourceList(const QString &sourceName, OBSSceneItem item, const DShowSourceVecType &sourceList)
{
	WriteBeautyConfigToLocal();
	ignoreChangeIndex = true;
	AddSourceComboboxList(sourceList, true);

	if (isSourceComboboxVisible(sourceName, item) && !sourceName.isEmpty()) {
		SetCurrentIndex(sourceName, item);
	}
	ignoreChangeIndex = false;
}

void PLSBeautyFilterView::InitGeometry()
{
	auto initGeometry = [this](double dpi, bool inConstructor) {
		extern void setGeometrySys(PLSWidgetDpiAdapter * adapter, const QRect &geometry);

		const char *geometry = config_get_string(App()->GlobalConfig(), BEAUTY_CONFIG, GEOMETRY_DATA);
		if (!geometry || !geometry[0]) {
			const int defaultWidth = 298;
			const int defaultHeight = 802;
			const int mainRightOffest = 5;
			PLSMainView *mainView = App()->getMainView();
			QPoint mainTopRight = App()->getMainView()->mapToGlobal(QPoint(mainView->frameGeometry().width(), 0));
			geometryOfNormal = QRect(mainTopRight.x() + PLSDpiHelper::calculate(dpi, mainRightOffest), mainTopRight.y(), PLSDpiHelper::calculate(dpi, defaultWidth),
						 PLSDpiHelper::calculate(dpi, defaultHeight));
			setGeometrySys(this, geometryOfNormal);
		} else if (inConstructor) {
			QByteArray byteArray = QByteArray::fromBase64(QByteArray(geometry));
			restoreGeometry(byteArray);
			if (config_get_bool(App()->GlobalConfig(), BEAUTY_CONFIG, MAXIMIZED_STATE)) {
				showMaximized();
			}
		}
	};

	PLSDpiHelper dpiHelper;
	dpiHelper.notifyDpiChanged(this, [=](double dpi, double, bool isFirstShow) {
		extern QRect normalShow(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);

		if (isFirstShow) {
			initGeometry(dpi, false);
			if (!isMaxState && !isFullScreenState) {
				normalShow(this, geometryOfNormal);
			}
		}
	});

	initGeometry(PLSDpiHelper::getDpi(this), true);
}

void PLSBeautyFilterView::SaveShowModeToConfig()
{
	config_set_bool(App()->GlobalConfig(), BEAUTY_CONFIG, "showMode", isVisible());
	config_save(App()->GlobalConfig());
}

OBSSceneItem PLSBeautyFilterView::GetCurrentItemData()
{
	return ui->sourceListComboBox->currentData().value<OBSSceneItem>();
}

void PLSBeautyFilterView::Clear()
{
	DeleteAllBeautyViewInCurrentSource();
	ClearSourceComboboxSceneData(QList<OBSSceneItem>(), true);
}

static bool enumItemToVector(obs_scene_t *, obs_sceneitem_t *item, void *ptr)
{
	QVector<OBSSceneItem> &items = *reinterpret_cast<QVector<OBSSceneItem> *>(ptr);

	if (obs_sceneitem_is_group(item)) {
		//need consider collapsed item
		obs_scene_t *scene = obs_sceneitem_group_get_scene(item);
		obs_scene_enum_items(scene, enumItemToVector, &items);
	}

	items.insert(0, item);
	return true;
}

void PLSBeautyFilterView::TurnOffBeauty(obs_source_t *source)
{
	if (!source) {
		return;
	}

	OBSSceneItem item = GetCurrentItemData();
	if (item && (obs_sceneitem_get_source(item) == source)) {
		ui->faceStatusCheckbox->setChecked(false);
	} else {
		SceneDisplayVector displayVector = PLSSceneDataMgr::Instance()->GetDisplayVector();
		auto iter = displayVector.begin();
		while (iter != displayVector.end()) {
			if (iter->second) {
				OBSScene scene = iter->second->GetData();
				QVector<OBSSceneItem> items;
				obs_scene_enum_items(scene, enumItemToVector, &items);
				for (auto iter = items.begin(); iter != items.end(); ++iter) {
					if (!*iter)
						continue;
					obs_source_t *source_ = obs_sceneitem_get_source(*iter);
					if (source_ == source) {
						PLSBeautyDataMgr::Instance()->SetBeautyCheckedStateConfig(*iter, false);
						UpdateBeautyParamToRender(*iter, GetSourceCurrentFilterName(*iter));
						WriteBeautyConfigToLocal();
						return;
					}
				}
			}
			iter++;
		}
	}
}

void PLSBeautyFilterView::InitIconFileName()
{
	QDir dir(pls_get_user_path(CONFIGS_BEATURY_DEFAULT_IMAGE_PATH));
	dir.setFilter(QDir::Dirs);
	dir.setSorting(QDir::Name);
	QFileInfoList fileInfoList = dir.entryInfoList();
	foreach(QFileInfo info, fileInfoList)
	{
		QString name = info.fileName();
		if (name == "." || name == "..")
			continue;
		if (info.isDir()) {
			QVector<QString> files;
			getFilesFromDir(info.absoluteFilePath(), files);
			iconFileNameVector.push_back(files);
		}
	}
}

QString PLSBeautyFilterView::getIconFileByIndex(int groupIndex, int index)
{
	if (groupIndex >= 0 && groupIndex < iconFileNameVector.size()) {
		const QVector<QString> &files = iconFileNameVector[groupIndex];
		if (index >= 0 && index < files.size()) {
			return files[index];
		}
	}
	return QString();
}

void PLSBeautyFilterView::UpdateItemIcon()
{
	for (PLSBeautyFaceItemView *item : listItems) {
		if (item)
			item->SetFilterIconPixmap();
	}
}

void PLSBeautyFilterView::showEvent(QShowEvent *event)
{
	PLSDialogView::showEvent(event);
	ignoreChangeIndex = false;
	SaveShowModeToConfig();
	emit beautyViewVisibleChanged(true);
}

void PLSBeautyFilterView::hideEvent(QHideEvent *event)
{
	resizeReason = FlowLayoutChange;

	WriteBeautyConfigToLocal();
	if (!getMaxState()) {
		onSaveNormalGeometry();
	}
	config_save(App()->GlobalConfig());
	emit beautyViewVisibleChanged(false);

	PLSDialogView::hideEvent(event);
	SaveShowModeToConfig();
}

bool PLSBeautyFilterView::eventFilter(QObject *watcher, QEvent *event)
{
	if (watcher->inherits("QLineEdit")) {
		if (LineEditChanged(event)) {
			QLineEdit *edit = qobject_cast<QLineEdit *>(watcher);
			if (edit && edit->text().isEmpty()) {
				edit->setText(QString::number(edit->property(lastValidValueDefine).toInt()));
			}
			if (event->type() == QEvent::KeyRelease) {
				QKeyEvent *keyEvent = reinterpret_cast<QKeyEvent *>(event);
				switch (keyEvent->key()) {
				case Qt::Key_Enter:
				case Qt::Key_Return:
					this->focusNextChild();
				}
			}
		}
	}
	return PLSDialogView::eventFilter(watcher, event);
}

void PLSBeautyFilterView::closeEvent(QCloseEvent *event)
{
	hide();
	event->ignore();
}

void PLSBeautyFilterView::onMaxFullScreenStateChanged()
{
	config_set_bool(App()->GlobalConfig(), BEAUTY_CONFIG, MAXIMIZED_STATE, getMaxState());
	config_save(App()->GlobalConfig());
}

void PLSBeautyFilterView::onSaveNormalGeometry()
{
	config_set_string(App()->GlobalConfig(), BEAUTY_CONFIG, GEOMETRY_DATA, saveGeometry().toBase64().constData());
	config_save(App()->GlobalConfig());
}

void PLSBeautyFilterView::PLSFrontendEvent(enum obs_frontend_event event, void *ptr)
{
	PLSBeautyFilterView *view = reinterpret_cast<PLSBeautyFilterView *>(ptr);

	switch ((int)event) {
	case OBS_FRONTEND_EVENT_SCENE_COLLECTION_CHANGED:
		QMetaObject::invokeMethod(view, "OnSceneCollectionChanged", Qt::QueuedConnection);
		break;
	case OBS_FRONTEND_EVENT_SCENE_LIST_CHANGED:
		QMetaObject::invokeMethod(view, "OnSceneChanged", Qt::QueuedConnection);
		break;
	case OBS_FRONTEND_EVENT_SCENE_COPY:
		QMetaObject::invokeMethod(view, "OnSceneCopy", Qt::QueuedConnection);
		break;
	}
}

void PLSBeautyFilterView::getFilesFromDir(const QString &filePath, QVector<QString> &files)
{
	QDir dir(filePath);
	dir.setSorting(QDir::Name | QDir::DirsLast);
	QFileInfoList fileInfoList = dir.entryInfoList();
	foreach(QFileInfo info, fileInfoList)
	{
		QString name = info.fileName();
		if (name == "." || name == "..")
			continue;

		if (info.isFile()) {
			if (name.contains("@3x"))
				files.push_back(info.absoluteFilePath());
		} else if (info.isDir()) {
			getFilesFromDir(info.absoluteFilePath(), files);
		}
	}
}

template<typename T> void PLSBeautyFilterView::InitSlider(QSlider *slider, const T &min, const T &max, const T &step, Qt::Orientation ori)
{
	if (!slider) {
		return;
	}
	slider->setMinimum(min);
	slider->setMaximum(max);
	slider->setPageStep(step);
	slider->setOrientation(ori);
}

void PLSBeautyFilterView::LoadBeautyFaceView(OBSSceneItem item)
{
	// load default beauty files which are downloaded from server when application startups
	LoadPresetBeautyConfig(item);

	LoadPrivateBeautySetting(item);

	//Load Custom
	UpdateBeautyFaceView(item);

	// set Beauty item checked if there is one that last time checked, otherwise select "Natural" as default
	SetCurrentClickedFaceView();

	OnBeautyEffectStateCheckBoxClicked(ui->faceStatusCheckbox->isChecked());
}

void PLSBeautyFilterView::InitBeautyFaceView(const BeautyConfig &beautyConfig)
{
	CreateBeautyFaceView(beautyConfig.beautyModel.token.strID, beautyConfig.filterType, beautyConfig.isCustom, beautyConfig.isCurrent, beautyConfig.beautyModel.token.category,
			     beautyConfig.filterIndex);
	PLSBeautyDataMgr::Instance()->SetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), beautyConfig.beautyModel.token.strID, beautyConfig);
}

void PLSBeautyFilterView::InitSliderAndLineEditConnections(QSlider *slider, QLineEdit *lineEdit, void (PLSBeautyFilterView::*func)(int))
{
	lineEdit->installEventFilter(this);
	connect(slider, &QSlider::valueChanged, this, [=](int value) {
		lineEdit->blockSignals(true);
		lineEdit->setText(QString::number(value));
		lineEdit->blockSignals(false);
		SaveLastValidValue(lineEdit, value);
		ApplySliderValue(func, value);
	});

	connect(lineEdit, &QLineEdit::textChanged, this, [=](const QString &text) {
		if (!text.isEmpty()) {
			int value = text.toInt();
			slider->blockSignals(true);
			slider->setValue(value);
			slider->blockSignals(false);
			SaveLastValidValue(lineEdit, value);
			ApplySliderValue(func, value);
		}
	});
}

void PLSBeautyFilterView::CreateBeautyFaceView(const QString &id, int filterType, bool isCustom, bool isCurrent, QString baseName, int filterIndex)
{
	PLSBeautyFaceItemView *view = isCustomFaceExisted(id);
	if (!view) {
		view = new PLSBeautyFaceItemView(id, filterType, isCustom, baseName, this);
		connect(view, &PLSBeautyFaceItemView::FaceItemClicked, [=](PLSBeautyFaceItemView *item) {
			OnFaceItemClicked(item);
			OnScrollToCurrentItem(item);
		});
		connect(view, &PLSBeautyFaceItemView::FaceItemIdEdited, this, &PLSBeautyFilterView::OnFaceItemEdited);
		flowLayout->addWidget(view);
		listItems << view;
	} else {
		view->SetBaseId(baseName);
		view->SetCustom(isCustom);
		if (isCurrent)
			OnScrollToCurrentItem(view);
	}

	if (isCustom)
		view->SetFilterIndex(filterIndex);

	SetCheckedState(view, isCurrent);
}

PLSBeautyFaceItemView *PLSBeautyFilterView::isCustomFaceExisted(const QString &filterId)
{
	for (auto face : listItems) {
		if (!face) {
			continue;
		}

		if (face->GetFilterId() == filterId) {
			return face;
		}
	}
	return nullptr;
}

void PLSBeautyFilterView::UpdateBeautyFaceView(OBSSceneItem item)
{
	if (!item) {
		return;
	}

	BeautyConfigMap configMap = PLSBeautyDataMgr::Instance()->GetBeautyConfigBySourceId(item);
	if (0 == configMap.size()) {
		QDir dir(pls_get_user_path(CONFIGS_BEATURY_USER_PATH));
		if (!dir.exists()) {
			return;
		}
		// Be carefull the infinite recursion.
		LoadBeautyFaceView(ui->sourceListComboBox->currentData().value<OBSSceneItem>());
		return;
	}

	// error data handle
	int offset = listItems.size() - configMap.size();
	if (offset > 0) {
		for (int i = 0; i < offset; ++i) {
			auto item = listItems.last();
			if (item) {
				item->deleteLater();
				item = nullptr;
				listItems.removeLast();
			}
		}
	}

	for (auto iter = configMap.begin(); iter != configMap.end(); ++iter) {
		InitBeautyFaceView(iter->second);
	}

	SetCurrentClickedFaceView();
	UpdateUI(selectFilterName);
	UpdateBeautyParamToRender(item, GetSourceCurrentFilterName(item));
}

void PLSBeautyFilterView::SetRecommendValue(const BeautyConfig &config)
{
	ui->skinSlider->SetRecomendValue(config.smoothModel.default_value);
	ui->chinSlider->SetRecomendValue(config.beautyModel.defaultParam.chin);
	ui->cheekSlider->SetRecomendValue(config.beautyModel.defaultParam.cheek);
	ui->cheekboneSlider->SetRecomendValue(config.beautyModel.defaultParam.cheekbone);
	ui->eyeSlider->SetRecomendValue(config.beautyModel.defaultParam.eyes);
	ui->noseSlider->SetRecomendValue(config.beautyModel.defaultParam.nose);
}

void PLSBeautyFilterView::OnGetByteArraySuccess(const QByteArray &array, BeautyConfig &beautyConfig)
{
	QJsonObject object = QJsonDocument::fromJson(array).object();
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
}

void PLSBeautyFilterView::UpdateUI(const QString &filterId)
{
	BeautyConfig config;
	if (!GetCurrentSourceItem())
		return;
	if (!PLSBeautyDataMgr::Instance()->GetBeautyConfig(GetCurrentSourceItem(), filterId, config)) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "Get beauty[%s] config failed.", filterId.toStdString().c_str());
		return;
	}

	SetRecommendValue(config);

	SaveLastValidValue(ui->skinLineEdit, config.smoothModel.dymamic_value);
	SaveLastValidValue(ui->chinLineEdit, config.beautyModel.dynamicParam.chin);
	SaveLastValidValue(ui->cheekLineEdit, config.beautyModel.dynamicParam.cheek);
	SaveLastValidValue(ui->cheekBoneLineEdit, config.beautyModel.dynamicParam.cheekbone);
	SaveLastValidValue(ui->eyeLineEdit, config.beautyModel.dynamicParam.eyes);
	SaveLastValidValue(ui->noseLineEdit, config.beautyModel.dynamicParam.nose);

	ui->faceStatusCheckbox->setChecked(config.beautyStatus.beauty_enable);
	ui->skinSlider->setValue(config.smoothModel.dymamic_value);
	ui->chinSlider->setValue(config.beautyModel.dynamicParam.chin);
	ui->cheekSlider->setValue(config.beautyModel.dynamicParam.cheek);
	ui->cheekboneSlider->setValue(config.beautyModel.dynamicParam.cheekbone);
	ui->eyeSlider->setValue(config.beautyModel.dynamicParam.eyes);
	ui->noseSlider->setValue(config.beautyModel.dynamicParam.nose);

	ui->skinLineEdit->setText(QString::number(config.smoothModel.dymamic_value));
	ui->chinLineEdit->setText(QString::number(config.beautyModel.dynamicParam.chin));
	ui->cheekLineEdit->setText(QString::number(config.beautyModel.dynamicParam.cheek));
	ui->cheekBoneLineEdit->setText(QString::number(config.beautyModel.dynamicParam.cheekbone));
	ui->eyeLineEdit->setText(QString::number(config.beautyModel.dynamicParam.eyes));
	ui->noseLineEdit->setText(QString::number(config.beautyModel.dynamicParam.nose));

	UpdateButtonState(config.isCustom, config.beautyModel.IsChanged() || config.smoothModel.IsChanged());
	OnBeautyEffectStateCheckBoxClicked(ui->faceStatusCheckbox->isChecked());
}

void PLSBeautyFilterView::UpdateButtonState(bool isCustom, bool isChanged)
{
	if (isCustom) {
		ui->saveBtn->hide();
		ui->deleteBtn->show();
	} else {
		ui->saveBtn->show();
		ui->saveBtn->setEnabled(isChanged &&
					PLSBeautyDataMgr::Instance()->GetCustomFaceSizeBySourceId(ui->sourceListComboBox->currentData().value<OBSSceneItem>()) < BEAUTY_CUSTOM_FACE_MAX_NUM);
		ui->deleteBtn->hide();
	}

	ui->resetBtn->setEnabled(isChanged);
}

void PLSBeautyFilterView::WriteBeautyConfigToLocal()
{
	BeautySourceIdMap sourceMap = PLSBeautyDataMgr::Instance()->GetData();
	for (auto iter = sourceMap.begin(); iter != sourceMap.end(); ++iter) {
		obs_data_array_t *sceneArray = obs_data_array_create();
		BeautyConfigMap &configMap = iter->second;
		for (auto configIter = configMap.begin(); configIter != configMap.end(); ++configIter) {
			BeautyConfig config = configIter->second;

			//common value
			obs_data_t *data = obs_data_create();
			obs_data_set_string(data, BASE_ID, config.beautyModel.token.category.toStdString().c_str());
			obs_data_set_bool(data, BEAUTY_ON, config.beautyStatus.beauty_enable);
			obs_data_set_string(data, FILTER_ID, config.beautyModel.token.strID.toStdString().c_str());
			obs_data_set_int(data, FILTER_INDEX, config.filterIndex);
			obs_data_set_int(data, FILTER_TYPE, config.filterType);
			obs_data_set_bool(data, IS_CURRENT, config.isCurrent);
			obs_data_set_bool(data, IS_CUSTOM, config.isCustom);
			obs_data_array_push_back(sceneArray, data);
			obs_data_release(data);

			// custom value
			obs_data_t *customData = obs_data_create();
			obs_data_set_int(customData, SKIN_VALUE, config.smoothModel.dymamic_value);
			obs_data_set_int(customData, CHIN_VALUE, config.beautyModel.dynamicParam.chin);
			obs_data_set_int(customData, CHEEK_VALUE, config.beautyModel.dynamicParam.cheek);
			obs_data_set_int(customData, CHEEKBONE_VALUE, config.beautyModel.dynamicParam.cheekbone);
			obs_data_set_int(customData, EYE_VALUE, config.beautyModel.dynamicParam.eyes);
			obs_data_set_int(customData, NOSE_VALUE, config.beautyModel.dynamicParam.nose);
			obs_data_set_obj(data, CUSTOM_NODE, customData);
			obs_data_release(customData);

			// default value
			obs_data_t *defaultData = obs_data_create();
			obs_data_set_int(defaultData, SKIN_VALUE, config.smoothModel.default_value);
			obs_data_set_int(defaultData, CHIN_VALUE, config.beautyModel.defaultParam.chin);
			obs_data_set_int(defaultData, CHEEK_VALUE, config.beautyModel.defaultParam.cheek);
			obs_data_set_int(defaultData, CHEEKBONE_VALUE, config.beautyModel.defaultParam.cheekbone);
			obs_data_set_int(defaultData, EYE_VALUE, config.beautyModel.defaultParam.eyes);
			obs_data_set_int(defaultData, NOSE_VALUE, config.beautyModel.defaultParam.nose);
			obs_data_set_obj(data, DEFAULT_NODE, defaultData);
			obs_data_release(defaultData);
		}

		bool isCurrentSource = (iter->first == ui->sourceListComboBox->currentData().value<OBSSceneItem>());
		obs_data_t *sceneitemPrivateData = obs_sceneitem_get_private_settings(iter->first);
		obs_data_set_bool(sceneitemPrivateData, IS_CURRENT_SOURCE, isCurrentSource);
		obs_data_release(sceneitemPrivateData);

		OBSSource source = obs_sceneitem_get_source(iter->first);
		obs_data_t *privateData = obs_source_get_private_settings(source);
		obs_data_set_array(privateData, BEAUTY_NODE, sceneArray);
		obs_data_array_release(sceneArray);
		obs_data_release(privateData);
	}
}

void PLSBeautyFilterView::SetCheckedState(PLSBeautyFaceItemView *view, bool state)
{
	if (!view) {
		return;
	}

	SetCheckedState(view->GetFilterId(), state);
}

void PLSBeautyFilterView::SetCheckedState(const QString &filterId, bool state)
{
	if (listItems.isEmpty()) {
		return;
	}

	for (auto face : listItems) {
		if (!face) {
			continue;
		}
		if (!state) {
			if (face->GetFilterId() == filterId) {
				face->SetChecked(state);
			}
			continue;
		}

		if (face->GetFilterId() == filterId) {
			face->SetChecked(state);
			selectFilterName = filterId;
			PLSBeautyDataMgr::Instance()->SetBeautyCurrentStateConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), filterId, state);
			continue;
		}
		face->SetChecked(!state);
	}
}

void PLSBeautyFilterView::SetCurrentClickedFaceView()
{
	if (listItems.empty()) {
		return;
	}

	bool findSelected = false;
	for (auto &item : listItems) {
		if (!item) {
			continue;
		}

		BeautyConfig config;
		PLSBeautyDataMgr::Instance()->GetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), item->GetFilterId(), config);
		if (config.isCurrent) {
			findSelected = true;
			SetFaceItem(item);
			break;
		}
	}

	//set a default selection
	if (listItems.at(0) && !findSelected) {
		SetFaceItem(listItems.at(0));
	}
}

void PLSBeautyFilterView::UpdateBeautyParamToRender(OBSSceneItem item, const QString &filterName)
{
	if (!item)
		return;

	if (filterName.isEmpty()) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "Beauty filter name is empty");
		return;
	}

	BeautyConfig config;
	if (!PLSBeautyDataMgr::Instance()->GetBeautyConfig(item, filterName, config)) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "Get face[%s] config failed.", filterName.toStdString().c_str());
		return;
	}

	obs_source_t *source = obs_sceneitem_get_source(item);
	if (!source) {
		return;
	}

	obs_data_t *beautyData = obs_data_create();

	obs_data_set_string(beautyData, "method", "beauty");
	obs_data_set_bool(beautyData, "enable", !!config.beautyStatus.beauty_enable);
	obs_data_set_string(beautyData, "id", filterName.toStdString().c_str());
	obs_data_set_string(beautyData, "category", config.beautyModel.token.category.toStdString().c_str());
	obs_data_set_double(beautyData, "chin", config.beautyModel.dynamicParam.chin / 100.0);
	obs_data_set_double(beautyData, "cheek", config.beautyModel.dynamicParam.cheek / 100.0);
	obs_data_set_double(beautyData, "cheekbone", config.beautyModel.dynamicParam.cheekbone / 100.0);
	obs_data_set_double(beautyData, "eyes", config.beautyModel.dynamicParam.eyes / 100.0);
	obs_data_set_double(beautyData, "nose", config.beautyModel.dynamicParam.nose / 100.0);
	obs_data_set_double(beautyData, "smooth", config.smoothModel.dymamic_value / 100.0);

	obs_source_set_private_data(source, beautyData);
	obs_data_release(beautyData);
}

void PLSBeautyFilterView::DeleteCustomFaceView(const QString &filterId)
{
	for (auto iter = listItems.begin(); iter != listItems.end(); ++iter) {
		PLSBeautyFaceItemView *face = *iter;
		if (!face) {
			continue;
		}

		if (!face->IsCustom()) {
			continue;
		}

		if (face->GetFilterId() == filterId) {
			face->deleteLater();
			face = nullptr;
			listItems.erase(iter);
			break;
		}
	}
}

void PLSBeautyFilterView::DeleteAllBeautyViewInCurrentSource()
{
	for (auto iter = listItems.begin(); iter != listItems.end();) {
		PLSBeautyFaceItemView *face = *iter;
		if (!face) {
			++iter;
			continue;
		}

		face->deleteLater();
		face = nullptr;
		iter = listItems.erase(iter);
	}
}

void PLSBeautyFilterView::CreateCustomFaceView(BeautyConfig config)
{
	BeautyConfig copyConfig = config;
	copyConfig.beautyModel.defaultParam = config.beautyModel.dynamicParam;
	copyConfig.smoothModel.default_value = config.smoothModel.dymamic_value;
	copyConfig.isCustom = true;
	copyConfig.isCurrent = true;

	QString holderPlaceText = PLSBeautyDataMgr::Instance()->GetCloseUnusedFilterName(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), copyConfig.beautyModel.token.category);
	int validFilterIndex = PLSBeautyDataMgr::Instance()->GetValidFilterIndex(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), copyConfig.beautyModel.token.category);

	std::string name;
	QString strName;
	while (true) {
		bool accepted = NameDialog::AskForName(this, QTStr("main.beauty.inputName.title"), QTStr("main.beauty.inputName.message"), name, holderPlaceText, maxFilerIdLength);
		if (!accepted) {
			return;
		}
		if (name.empty()) {
			PLSMessageBox::warning(this, QTStr("NoNameEntered.Title"), QTStr("NoNameEntered.Text"));
			continue;
		}

		strName = QString(name.c_str()).simplified();
		if (PLSBeautyDataMgr::Instance()->FindFilterId(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), strName)) {
			PLSMessageBox::warning(this, QTStr("NameExists.Title"), QTStr("NameExists.Text"));
			continue;
		}
		break;
	}

	copyConfig.beautyModel.token.strID = strName;
	copyConfig.filterIndex = validFilterIndex;
	copyConfig.filterType = config.filterType;
	if (copyConfig.isCustom) {
		PLSBeautyDataMgr::Instance()->AddBeautyCustomConfig(copyConfig.beautyModel.token.strID, copyConfig);
		PLSBeautyDataMgr::Instance()->SetCustomBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), copyConfig.beautyModel.token.strID, copyConfig);
	}
	InitBeautyFaceView(copyConfig);

	WriteBeautyConfigToLocal();
	SetCheckedState(copyConfig.beautyModel.token.strID, true);
	UpdateUI(selectFilterName);
}

OBSSceneItem PLSBeautyFilterView::GetCurrentSourceItem()
{
	return ui->sourceListComboBox->currentData().value<OBSSceneItem>();
}

QString PLSBeautyFilterView::GetSourceCurrentFilterName(OBSSceneItem item)
{
	QString checkedFilterName = PLSBeautyDataMgr::Instance()->GetCheckedFilterName(item);
	if (checkedFilterName.isEmpty()) {
		BeautyConfigMap configVector = PLSBeautyDataMgr::Instance()->GetBeautyConfigBySourceId(item);
		if (configVector.size() > 0) {
			checkedFilterName = configVector.at(0).second.beautyModel.token.strID;
		}
	}

	return checkedFilterName;
}

QString PLSBeautyFilterView::GetSourceCurrentFilterBaseName(OBSSceneItem item)
{
	QString checkedFilterBaseName = PLSBeautyDataMgr::Instance()->GetCheckedFilterBaseName(item);
	if (checkedFilterBaseName.isEmpty()) {
		BeautyConfigMap configVector = PLSBeautyDataMgr::Instance()->GetBeautyConfigBySourceId(item);
		if (configVector.size() > 0) {
			checkedFilterBaseName = configVector.at(0).second.beautyModel.token.category;
		}
	}

	return checkedFilterBaseName;
}

void PLSBeautyFilterView::LoadPresetBeautyConfig(OBSSceneItem item)
{
	BeautyConfigMap configMap = PLSBeautyDataMgr::Instance()->GetBeautyPresetConfig();
	if (0 == configMap.size()) {
		// If there is no preset data ,load it from local files.
		auto loadPresetConfigFromDir = [=](const QString &dirPath) {
			QDir dir(pls_get_user_path(dirPath));
			dir.setFilter(QDir::Files);
			QFileInfoList list = dir.entryInfoList();
			for (int i = 0; i < list.size(); i++) {
				QFileInfo fileInfo = list.at(i);
				QString name = fileInfo.fileName();
				if (fileInfo.fileName() == "." || fileInfo.fileName() == ".." || fileInfo.isDir() || !fileInfo.fileName().contains(".json")) {
					continue;
				}

				QByteArray byteArray;
				if (PLSJsonDataHandler::getJsonArrayFromFile(byteArray, fileInfo.filePath())) {
					BeautyConfig config;
					config.filterType = i;
					PLSBeautyDataMgr::Instance()->ParseJsonArrayToBeautyConfig(byteArray, config);
					PLSBeautyDataMgr::Instance()->AddBeautyPresetConfig(config.beautyModel.token.strID, config);
					continue;
				}
				PLS_ERROR(MAIN_BEAUTY_MODULE, "Load %s Failed.", fileInfo.filePath().toStdString().c_str());
			}
		};
		loadPresetConfigFromDir(CONFIGS_BEATURY_USER_PATH);
	}
	auto configIter = configMap.cbegin();
	while (configIter != configMap.cend()) {
		BeautyConfig config = configIter->second;
		PLSBeautyDataMgr::Instance()->SetBeautyConfig(item, config.beautyModel.token.strID, config);
		configIter++;
	}
}

void PLSBeautyFilterView::InitLineEdit()
{
	QList<QLineEdit *> listLineEdit;
	listLineEdit << ui->chinLineEdit << ui->cheekLineEdit << ui->cheekBoneLineEdit << ui->eyeLineEdit << ui->noseLineEdit << ui->skinLineEdit;
	foreach(QLineEdit * lineEdit, listLineEdit) { setLineEditRegularExpression(lineEdit); }
}

void PLSBeautyFilterView::setLineEditRegularExpression(QLineEdit *lineEdit)
{
	QRegExp regularExpression(paramRegEx);
	QValidator *pValidator = new QRegExpValidator(regularExpression, this);
	lineEdit->setValidator(pValidator);
}

bool PLSBeautyFilterView::IsDShowSourceAvailable(const QString &name, OBSSceneItem item)
{
	return IsDShowSourceValid(name, item) && IsDShowSourceVisible(name, item);
}

bool PLSBeautyFilterView::IsDShowSourceValid(const QString &name, OBSSceneItem item)
{
	obs_source_t *source = obs_get_source_by_name(name.toStdString().c_str());
	if (!source) {
		return false;
	}

	obs_source_error error;
	bool res = (obs_source_get_capture_valid(source, &error) && obs_source_get_image_status(source));
	obs_source_release(source);
	return res;
}

bool PLSBeautyFilterView::IsDShowSourceVisible(const QString &name, OBSSceneItem item)
{
	obs_source_t *source = obs_get_source_by_name(name.toStdString().c_str());
	if (!source) {
		return false;
	}

	if (!item) {
		obs_source_release(source);
		return false;
	}

	bool res = (obs_sceneitem_visible(item) && obs_source_get_image_status(source));
	obs_source_release(source);
	return res;
}

bool PLSBeautyFilterView::isSourceComboboxVisible(const QString &sourceName, OBSSceneItem item)
{
	QListView *view = qobject_cast<QListView *>(ui->sourceListComboBox->view());
	if (!view) {
		return false;
	}

	int index = FindText(sourceName, item);
	if (-1 == index) {
		return false;
	}

	return !view->isRowHidden(index);
}

bool PLSBeautyFilterView::isCurrentSourceExisted(const QString &currentSourceName)
{
	return (!currentSourceName.isEmpty()) && (0 != currentSourceName.compare(QTStr("main.beauty.face.nosource")));
}

void PLSBeautyFilterView::ClearSourceComboboxSceneData(const QList<OBSSceneItem> &vec, bool releaseCurrent)
{
	ui->sourceListComboBox->clear();
	for (auto &sceneitem : vec) {
		PLSBeautyDataMgr::Instance()->DeleteBeautySourceConfig(sceneitem);
	}
	if (releaseCurrent) {
		currentSource = nullptr;
	}
}

void PLSBeautyFilterView::AddSourceComboboxList(const DShowSourceVecType &list, bool releaseCurrent)
{
	ui->sourceListComboBox->clear();

	if (0 == list.size()) {
		QList<OBSSceneItem> sceneList;
		for (auto iter = list.begin(); iter != list.end(); ++iter) {
			sceneList << iter->second;
		}
		ClearSourceComboboxSceneData(sceneList, releaseCurrent);
		ui->sourceListComboBox->addItem(QTStr("main.beauty.face.nosource"));
	} else {
		for (auto iter = list.begin(); iter != list.end(); ++iter) {
			BeautyConfigMap configMap = PLSBeautyDataMgr::Instance()->GetBeautyConfigBySourceId(iter->second);
			if (0 == configMap.size()) {
				LoadPrivateBeautySetting(iter->second);
			}
			ui->sourceListComboBox->addItem(iter->first, QVariant::fromValue(iter->second));
			SetSourceVisible(iter->first, iter->second, IsDShowSourceAvailable(iter->first, iter->second));
		}
	}
}

int PLSBeautyFilterView::GetSourceComboboxVisibleCount()
{
	QListView *view = qobject_cast<QListView *>(ui->sourceListComboBox->view());
	if (!view) {
		return -1;
	}

	int count = 0;
	for (int i = 0; i < ui->sourceListComboBox->count(); i++) {
		if (view->isRowHidden(i)) {
			continue;
		}
		count++;
	}
	return count;
}

void PLSBeautyFilterView::InitConnections()
{
	connect(ui->sourceListComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PLSBeautyFilterView::OnSourceComboboxCurrentIndexChanged);
	connect(ui->faceStatusCheckbox, &QCheckBox::stateChanged, this, &PLSBeautyFilterView::OnBeautyEffectStateCheckBoxClicked);

	InitSliderAndLineEditConnections(ui->chinSlider, ui->chinLineEdit, &PLSBeautyFilterView::OnChinSliderValueChanged);
	InitSliderAndLineEditConnections(ui->cheekSlider, ui->cheekLineEdit, &PLSBeautyFilterView::OnCheekSliderValueChanged);
	InitSliderAndLineEditConnections(ui->cheekboneSlider, ui->cheekBoneLineEdit, &PLSBeautyFilterView::OnCheekBoneSliderValueChanged);
	InitSliderAndLineEditConnections(ui->eyeSlider, ui->eyeLineEdit, &PLSBeautyFilterView::OnEyeSliderValueChanged);
	InitSliderAndLineEditConnections(ui->noseSlider, ui->noseLineEdit, &PLSBeautyFilterView::OnNoseSliderValueChanged);
	InitSliderAndLineEditConnections(ui->skinSlider, ui->skinLineEdit, &PLSBeautyFilterView::OnSkinSliderValueChanged);
	connect(ui->skinSlider, &SliderIgnoreScroll::mouseReleaseSignal, this, &PLSBeautyFilterView::OnSkinSliderMouseRelease);

	connect(ui->saveBtn, &QPushButton::clicked, this, &PLSBeautyFilterView::OnSaveCustomButtonClicked);
	connect(ui->deleteBtn, &QPushButton::clicked, this, &PLSBeautyFilterView::OnDeleteButtonClicked);
	connect(ui->resetBtn, &QPushButton::clicked, this, &PLSBeautyFilterView::OnResetButtonClicked);
}

void PLSBeautyFilterView::ApplySliderValue(void (PLSBeautyFilterView::*func)(int), int value)
{
	if (func) {
		(this->*func)(value);
	}
}

void PLSBeautyFilterView::CheckeAndSetValidValue(int *value)
{
	if (*value < minFilterValue)
		*value = minFilterValue;
	if (*value > maxFilterValue)
		*value = maxFilterValue;
}

void PLSBeautyFilterView::SetSourceUnSelect(const QString &sourceName, OBSSceneItem item)
{
	Q_UNUSED(sourceName);
	Q_UNUSED(item);
}

void PLSBeautyFilterView::SetSourceInvisible(const QString &sourceName, OBSSceneItem item)
{
	if (!isCurrentSourceExisted(currentSourceName) || FindText(ui->sourceListComboBox->currentText(), ui->sourceListComboBox->currentData().value<OBSSceneItem>()) == FindText(sourceName, item)) {
		int index = GetNextVisibleItemIndex(ui->sourceListComboBox);
		SetCurrentIndex(ui->sourceListComboBox->itemText(index), ui->sourceListComboBox->itemData(index).value<OBSSceneItem>());
	}

	if (0 == GetSourceComboboxVisibleCount()) {
		ui->sourceListComboBox->addItem(QTStr("main.beauty.face.nosource"));
		ui->sourceListComboBox->setCurrentText(QTStr("main.beauty.face.nosource"));
	}
}

void PLSBeautyFilterView::SetCurrentSourceName(const QString &sourceName, OBSSceneItem item)
{
	if (isCurrentSourceExisted(sourceName)) {
		currentSourceName = sourceName;
		currentSource = item;
	}
}

int PLSBeautyFilterView::FindText(const QString &sourceName, OBSSceneItem item)
{
	for (int i = 0; i < ui->sourceListComboBox->count(); i++) {
		OBSSceneItem itemData = ui->sourceListComboBox->itemData(i).value<OBSSceneItem>();
		if (itemData == item && 0 == sourceName.compare(ui->sourceListComboBox->itemText(i))) {
			return i;
		}
	}
	return -1;
}

void PLSBeautyFilterView::RemoveItem(const QString &sourceName, OBSSceneItem item)
{
	int index = FindText(sourceName, item);
	if (-1 == index) {
		return;
	}

	if (currentSource == item) {
		currentSource = nullptr;
	}
	ui->sourceListComboBox->blockSignals(true);
	ui->sourceListComboBox->removeItem(index);
	ui->sourceListComboBox->blockSignals(false);
}

bool PLSBeautyFilterView::SetCurrentIndex(const QString &sourceName, OBSSceneItem item)
{
	int index = FindText(sourceName, item);
	if (-1 == index) {
		return false;
	}
	ignoreChangeIndex = true;
	//modified by xiewei issue#2582 calling SetCurrentSourceName function here is not necessary.
	ui->sourceListComboBox->setCurrentIndex(index);
	WriteBeautyConfigToLocal();
	ignoreChangeIndex = false;
	return true;
}

void PLSBeautyFilterView::SaveLastValidValue(QLineEdit *object, int value)
{
	if (object) {
		object->setProperty(lastValidValueDefine, value);
	}
}

void PLSBeautyFilterView::LoadPrivateBeautySetting(OBSSceneItem item)
{
	obs_data_t *sceneitemPrivData = obs_sceneitem_get_private_settings(item);

	OBSSource source = obs_sceneitem_get_source(item);
	obs_data_t *privData = obs_source_get_private_settings(source);
	obs_data_array_t *beautyArray = obs_data_get_array(privData, BEAUTY_NODE);
	if (!beautyArray) {
		obs_data_release(privData);
		return;
	}
	QList<BeautyConfig> configList;
	size_t num = obs_data_array_count(beautyArray);
	bool emptyCustomConfig = (0 == PLSBeautyDataMgr::Instance()->GetBeautyCustomConfig().size());
	for (size_t i = 0; i < num; i++) {
		BeautyConfig config;
		obs_data_t *data = obs_data_array_item(beautyArray, i);

		config.beautyModel.token.category = obs_data_get_string(data, BASE_ID);
		config.beautyModel.token.strID = obs_data_get_string(data, FILTER_ID);
		config.beautyStatus.beauty_enable = obs_data_get_bool(data, BEAUTY_ON);
		config.filterIndex = obs_data_get_int(data, FILTER_INDEX);
		config.isCurrent = obs_data_get_bool(data, IS_CURRENT);
		config.isCustom = obs_data_get_bool(data, IS_CUSTOM);

		// error handle. If there is no custom beauty config in "beauty_config" field,
		// custom config in "private settings" should not be loaded.
		// https://oss.navercorp.com/cd-live-platform/PRISMLiveStudio/issues/2950
		if (emptyCustomConfig && config.isCustom) {
			obs_data_release(data);
			continue;
		}

		if (config.isCustom)
			config.filterType = obs_data_get_int(data, FILTER_TYPE);
		else
			config.filterType = i;

		obs_data_t *customData = obs_data_get_obj(data, CUSTOM_NODE);
		if (!customData) {
			obs_data_release(data);
			continue;
		}
		config.beautyModel.dynamicParam.cheek = obs_data_get_int(customData, CHEEK_VALUE);
		config.beautyModel.dynamicParam.cheekbone = obs_data_get_int(customData, CHEEKBONE_VALUE);
		config.beautyModel.dynamicParam.chin = obs_data_get_int(customData, CHIN_VALUE);
		config.beautyModel.dynamicParam.eyes = obs_data_get_int(customData, EYE_VALUE);
		config.beautyModel.dynamicParam.nose = obs_data_get_int(customData, NOSE_VALUE);
		config.smoothModel.dymamic_value = obs_data_get_int(customData, SKIN_VALUE);
		obs_data_release(customData);

		obs_data_t *defaultData = obs_data_get_obj(data, DEFAULT_NODE);
		if (!defaultData) {
			obs_data_release(data);
			continue;
		}

		if (config.isCustom) {
			config.beautyModel.defaultParam.cheek = obs_data_get_int(defaultData, CHEEK_VALUE);
			config.beautyModel.defaultParam.cheekbone = obs_data_get_int(defaultData, CHEEKBONE_VALUE);
			config.beautyModel.defaultParam.chin = obs_data_get_int(defaultData, CHIN_VALUE);
			config.beautyModel.defaultParam.eyes = obs_data_get_int(defaultData, EYE_VALUE);
			config.beautyModel.defaultParam.nose = obs_data_get_int(defaultData, NOSE_VALUE);
			config.smoothModel.default_value = obs_data_get_int(defaultData, SKIN_VALUE);
		} else {
			// Everytime we should update the default value of preset beauty config which is download from gpop.
			BeautyConfig configPreset = PLSBeautyDataMgr::Instance()->GetBeautyPresetConfigById(config.beautyModel.token.strID);
			config.beautyModel.defaultParam.cheek = configPreset.beautyModel.defaultParam.cheek;
			config.beautyModel.defaultParam.cheekbone = configPreset.beautyModel.defaultParam.cheekbone;
			config.beautyModel.defaultParam.chin = configPreset.beautyModel.defaultParam.chin;
			config.beautyModel.defaultParam.eyes = configPreset.beautyModel.defaultParam.eyes;
			config.beautyModel.defaultParam.nose = configPreset.beautyModel.defaultParam.nose;
			config.smoothModel.default_value = configPreset.smoothModel.default_value;
		}

		obs_data_release(defaultData);
		obs_data_release(data);

		configList << config;
	}
	obs_data_array_release(beautyArray);
	obs_data_release(privData);
	PLSBeautyDataMgr::Instance()->SetBeautyConfig(item, configList);
}

void PLSBeautyFilterView::SetFaceItem(PLSBeautyFaceItemView *item)
{
	selectFilterName = item->GetFilterId();
	//modified by xiewei issue#2582 call SetCurrentSourceName function here is not necessary
	SetCheckedState(item, true);
	UpdateUI(item->GetFilterId());

	WriteBeautyConfigToLocal();
}

void PLSBeautyFilterView::OnLayoutFinished()
{
	if (0 == listItems.size()) {
		return;
	}

	if (resizeReason == FlowLayoutChange) {
		OnScrollToCurrentItem(selectFilterName);
		flowLayout->showLayoutItemWidget();
	} else if (resizeReason == ItemDelete) {
		OnScrollToCurrentItem(listItems.at(0));
		flowLayout->showLayoutItemWidget();
	} else if (resizeReason == ItemAdded) {
		OnScrollToCurrentItem(listItems.at(listItems.size() - 1));
		flowLayout->showLayoutItemWidget();
	}
}

void PLSBeautyFilterView::OnChinSliderValueChanged(int value)
{
	CheckeAndSetValidValue(&value);
	BeautyConfig config;
	if (PLSBeautyDataMgr::Instance()->GetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config)) {
		config.beautyModel.dynamicParam.chin = value;
		PLSBeautyDataMgr::Instance()->SetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config);

		UpdateBeautyParamToRender(GetCurrentSourceItem(), selectFilterName);
		UpdateButtonState(config.isCustom, config.beautyModel.IsChanged() || config.smoothModel.IsChanged());
		WriteBeautyConfigToLocal();
	}
}

void PLSBeautyFilterView::OnCheekSliderValueChanged(int value)
{
	CheckeAndSetValidValue(&value);
	BeautyConfig config;
	if (PLSBeautyDataMgr::Instance()->GetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config)) {
		config.beautyModel.dynamicParam.cheek = value;
		PLSBeautyDataMgr::Instance()->SetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config);

		UpdateBeautyParamToRender(GetCurrentSourceItem(), selectFilterName);
		UpdateButtonState(config.isCustom, config.beautyModel.IsChanged() || config.smoothModel.IsChanged());
		WriteBeautyConfigToLocal();
	}
}

void PLSBeautyFilterView::OnCheekBoneSliderValueChanged(int value)
{
	CheckeAndSetValidValue(&value);
	BeautyConfig config;
	if (PLSBeautyDataMgr::Instance()->GetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config)) {
		config.beautyModel.dynamicParam.cheekbone = value;
		PLSBeautyDataMgr::Instance()->SetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config);

		UpdateBeautyParamToRender(GetCurrentSourceItem(), selectFilterName);
		UpdateButtonState(config.isCustom, config.beautyModel.IsChanged() || config.smoothModel.IsChanged());
		WriteBeautyConfigToLocal();
	}
}

void PLSBeautyFilterView::OnEyeSliderValueChanged(int value)
{
	CheckeAndSetValidValue(&value);
	BeautyConfig config;
	if (PLSBeautyDataMgr::Instance()->GetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config)) {
		config.beautyModel.dynamicParam.eyes = value;
		PLSBeautyDataMgr::Instance()->SetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config);

		UpdateBeautyParamToRender(GetCurrentSourceItem(), selectFilterName);
		UpdateButtonState(config.isCustom, config.beautyModel.IsChanged() || config.smoothModel.IsChanged());
		WriteBeautyConfigToLocal();
	}
}

void PLSBeautyFilterView::OnNoseSliderValueChanged(int value)
{
	CheckeAndSetValidValue(&value);
	BeautyConfig config;
	if (PLSBeautyDataMgr::Instance()->GetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config)) {
		config.beautyModel.dynamicParam.nose = value;
		PLSBeautyDataMgr::Instance()->SetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config);

		UpdateBeautyParamToRender(GetCurrentSourceItem(), selectFilterName);
		UpdateButtonState(config.isCustom, config.beautyModel.IsChanged() || config.smoothModel.IsChanged());
		WriteBeautyConfigToLocal();
	}
}

void PLSBeautyFilterView::OnSkinSliderValueChanged(int value)
{
	CheckeAndSetValidValue(&value);
	BeautyConfig config;
	if (PLSBeautyDataMgr::Instance()->GetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config)) {
		config.smoothModel.dymamic_value = value;
		PLSBeautyDataMgr::Instance()->SetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config);

		UpdateBeautyParamToRender(GetCurrentSourceItem(), selectFilterName);
		UpdateButtonState(config.isCustom, config.beautyModel.IsChanged() || config.smoothModel.IsChanged());
		WriteBeautyConfigToLocal();
	}
}

void PLSBeautyFilterView::OnSourceComboboxCurrentIndexChanged(int index)
{
	bool noSource = (0 == ui->sourceListComboBox->itemText(index).compare(QTStr("main.beauty.face.nosource")));
	if (0 == GetSourceComboboxVisibleCount() || noSource || -1 == index) {
		ui->stackedWidget->setCurrentWidget(ui->page_noSource);
	} else {
		ui->sourceListComboBox->removeItem(ui->sourceListComboBox->findText(QTStr("main.beauty.face.nosource")));
		ui->stackedWidget->setCurrentWidget(ui->page_normal);

		if (!ignoreChangeIndex) {
			SetCurrentSourceName(ui->sourceListComboBox->currentText(), ui->sourceListComboBox->currentData().value<OBSSceneItem>());
			emit currentSourceChanged(ui->sourceListComboBox->currentText(), ui->sourceListComboBox->currentData().value<OBSSceneItem>());
		}

		UpdateBeautyFaceView(ui->sourceListComboBox->itemData(index).value<OBSSceneItem>());
		if (ui->faceStatusCheckbox->isChecked()) {
			QString filterName = GetSourceCurrentFilterBaseName(ui->sourceListComboBox->currentData().value<OBSSceneItem>());
			if (!filterName.isEmpty()) {
				sendActionLog = false;
			}
		} else {
			sendActionLog = true;
		}
	}
	bool panelEnable = (index == -1 || noSource ? false : true);
	ui->sourceLabel->setEnabled(panelEnable);
	ui->sourceListComboBox->setEnabled(panelEnable);
}

void PLSBeautyFilterView::OnSaveCustomButtonClicked()
{
	PLS_UI_STEP(MAIN_BEAUTY_MODULE, "Save as Custom Button", ACTION_CLICK);
	if (selectFilterName.isEmpty()) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "No Select Face View.");
		return;
	}
	BeautyConfig config;
	if (!PLSBeautyDataMgr::Instance()->GetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName, config)) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "Get face[%s] config failed.", selectFilterName.toStdString().c_str());
		return;
	}

	resizeReason = ItemAdded;
	WriteBeautyConfigToLocal();
	CreateCustomFaceView(config);
}

void PLSBeautyFilterView::OnResetButtonClicked()
{
	PLS_UI_STEP(MAIN_BEAUTY_MODULE, "Reset Button", ACTION_CLICK);
	if (!PLSBeautyDataMgr::Instance()->ResetBeautyConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), selectFilterName)) {
		return;
	}

	UpdateUI(selectFilterName);
	WriteBeautyConfigToLocal();
}

void PLSBeautyFilterView::OnBeautyEffectStateCheckBoxClicked(int state)
{
	switch (state) {
	case Qt::Checked:
		ui->filterWidget->setEnabled(true);
		ui->scrollArea->setEnabled(true);
		ui->skinWidget->setEnabled(true);
		ui->buttonWidget->setEnabled(true);
		if (sendActionLog) {
			QString filterName = GetSourceCurrentFilterBaseName(ui->sourceListComboBox->currentData().value<OBSSceneItem>());
			if (!filterName.isEmpty()) {
				sendActionLog = false;
			}
		}
		PLS_UI_STEP(MAIN_BEAUTY_MODULE, "Beauty Effects switch Open", ACTION_CLICK);
		break;
	case Qt::Unchecked:
		ui->filterWidget->setEnabled(false);
		ui->scrollArea->setEnabled(false);
		ui->skinWidget->setEnabled(false);
		ui->buttonWidget->setEnabled(false);
		PLS_UI_STEP(MAIN_BEAUTY_MODULE, "Beauty Effects switch Close", ACTION_CLICK);
		break;
	default:
		break;
	}

	OnBeautyFaceCheckboxStateChanged(state);
}

void PLSBeautyFilterView::OnBeautyFaceCheckboxStateChanged(int state)
{
	PLSBeautyDataMgr::Instance()->SetBeautyCheckedStateConfig(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), state);
	UpdateBeautyParamToRender(GetCurrentSourceItem(), selectFilterName);
	WriteBeautyConfigToLocal();
}

void PLSBeautyFilterView::OnSceneCollectionChanged()
{
	OnSceneChanged();
	setVisible(config_get_bool(App()->GlobalConfig(), BEAUTY_CONFIG, SHOW_MADE));
}

void PLSBeautyFilterView::OnSceneChanged()
{
	QString name{};
	OBSSceneItem item;
	DShowSourceVecType sourceList;
	PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
	if (!main) {
		return;
	}
	main->GetSelectDshowSourceAndList(name, item, sourceList);

	currentSource = item;
	UpdateSourceList(name, item, sourceList);
}

void PLSBeautyFilterView::OnSceneCopy()
{
	ui->sourceListComboBox->clear();
}

void PLSBeautyFilterView::OnDeleteButtonClicked()
{
	PLS_UI_STEP(MAIN_BEAUTY_MODULE, "Delete Button", ACTION_CLICK);

	if (PLSAlertView::Button::Ok !=
	    PLSMessageBox::question(this, QTStr("ConfirmRemove.Title"), QTStr(MAIN_BEAUTY_DELETEBEAUTY_MESSAGE).arg(selectFilterName), PLSAlertView::Button::Ok | PLSAlertView::Button::Cancel)) {
		return;
	}

	if (!PLSBeautyDataMgr::Instance()->DeleteBeautyFilterConfig(selectFilterName)) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "Delete beauty filter[%s] config failed.", selectFilterName.toStdString().c_str());
		return;
	}

	// delete custom beauty config at the same time.
	if (!PLSBeautyDataMgr::Instance()->DeleteBeautyCustomConfig(selectFilterName)) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "Delete custom beauty filter[%s] config failed.", selectFilterName.toStdString().c_str());
		return;
	}

	resizeReason = ItemDelete;
	DeleteCustomFaceView(selectFilterName);
	SetCurrentClickedFaceView();
}

void PLSBeautyFilterView::OnFaceItemClicked(PLSBeautyFaceItemView *item)
{
	SetFaceItem(item);
	if (ui->faceStatusCheckbox->isChecked()) {
	}
	PLS_UI_STEP(MAIN_BEAUTY_MODULE, item->GetFilterId().toStdString().c_str(), ACTION_CLICK);
}

void PLSBeautyFilterView::OnFaceItemEdited(const QString &newId, PLSBeautyFaceItemView *item)
{
	if (!item) {
		return;
	}
	if (newId.isEmpty()) {
		PLSMessageBox::warning(this, QTStr("NoNameEntered.Title"), QTStr("NoNameEntered.Text"));
		return;
	}

	if (PLSBeautyDataMgr::Instance()->FindFilterId(ui->sourceListComboBox->currentData().value<OBSSceneItem>(), newId.toStdString().c_str())) {
		PLSMessageBox::warning(this, QTStr("NameExists.Title"), QTStr("NameExists.Text"));
		return;
	}

	if (!PLSBeautyDataMgr::Instance()->RenameFilterId(item->GetFilterId(), newId.toStdString().c_str())) {

		return;
	}

	if (!PLSBeautyDataMgr::Instance()->RenameCustomBeautyConfig(item->GetFilterId(), newId)) {
		return;
	}

	if (selectFilterName == item->GetFilterId()) {
		selectFilterName = newId;
	}

	item->SetFilterId(newId);
}

void PLSBeautyFilterView::OnScrollToCurrentItem(PLSBeautyFaceItemView *item)
{
	if (!item) {
		return;
	}
	int itemTop = item->mapToParent(QPoint(0, 0)).y();
	int itemBottom = itemTop + item->height();
	int value = ui->scrollArea->verticalScrollBar()->value();

	int scrollAreaTopY = value, scrollAreaBottomY = ui->scrollArea->height() + value;
	int offset = itemTop - scrollAreaTopY;
	int marginTop, marginBottom;
	flowLayout->getContentsMargins(nullptr, &marginTop, nullptr, &marginBottom);
	if (offset < 0) {
		ui->scrollArea->verticalScrollBar()->setValue(value + offset - marginTop);
	}
	offset = itemBottom - scrollAreaBottomY;
	if (offset > 0) {
		int maxValue = ui->scrollArea->verticalScrollBar()->maximum();
		int newValue = value + offset + marginBottom;
		if (maxValue < newValue)
			newValue = maxValue;
		ui->scrollArea->verticalScrollBar()->setValue(newValue);
	}
}

void PLSBeautyFilterView::OnScrollToCurrentItem(const QString &filterId)
{
	if (listItems.isEmpty()) {
		return;
	}

	for (auto face : listItems) {
		if (face->GetFilterId() == filterId) {
			OnScrollToCurrentItem(face);
			break;
		}
	}
}

void PLSBeautyFilterView::OnSkinSliderMouseRelease()
{
}
