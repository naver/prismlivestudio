#include "window-basic-main.hpp"
#include "pls-app.hpp"
#include "source-tree.hpp"
#include "qt-wrappers.hpp"
#include "visibility-checkbox.hpp"
#include "locked-checkbox.hpp"
#include "expand-checkbox.hpp"
#include "platform.hpp"
#include "pls-common-define.hpp"
#include "PLSAction.h"
#include "action.h"
#include "liblog.h"
#include "log/module_names.h"
#include "ChannelCommonFunctions.h"
#include "PLSDpiHelper.h"

#include <frontend-api.h>
#include <obs.h>
#include <string>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>

#include <QStylePainter>
#include <QStyleOptionFocusRect>

static inline OBSScene GetCurrentScene()
{
	PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
	return main->GetCurrentScene();
}

#define SOURCEITEM_MARGIN_UNNORMAL_LONG 30 // while scroll is hiden and mouse status is unnormal
#define SOURCEITEM_MARGIN_UNNORMAL_SHORT 20 // while scroll is shown and mouse status is unnormal

#define SOURCEITEM_MARGIN_NORMAL_LONG 20 // while scroll is hiden and mouse status is normal
#define SOURCEITEM_MARGIN_NORMAL_SHORT 10 // while scroll is shown and mouse status is normal

#define SOURCEITEM_SPACE_BEFORE_VIS 5
#define SOURCEITEM_SPACE_BEFORE_LOCK 10

/* ========================================================================= */
void SourceLabel::resizeEvent(QResizeEvent *event)
{
	//__super::setText(SnapSourceName());
	update();
	__super::resizeEvent(event);
}

void SourceLabel::paintEvent(QPaintEvent *event)
{
	QPainter dc(this);
	int padding = PLSDpiHelper::calculate(this, 5);
	dc.setFont(font());

	QStyleOption opt;
	opt.init(this);
	auto textColor = opt.palette.color(QPalette::Text);

	dc.setPen(textColor);
	dc.drawText(QRect(padding, 0, width() - padding, height()), SnapSourceName(), Qt::AlignLeft | Qt::AlignVCenter);
	QLabel::paintEvent(event);
}

QString SourceLabel::SnapSourceName()
{
	if (currentText.isEmpty())
		return currentText;

	QFontMetrics fontWidth(font());
	if (fontWidth.width(currentText) > width() - PLSDpiHelper::calculate(this, 5)) // padding-left : 5px;
		return fontWidth.elidedText(currentText, Qt::ElideRight, width() - PLSDpiHelper::calculate(this, 5));
	else
		return currentText;
}

void SourceLabel::setText(const QString &text)
{
	currentText = text;
	//__super::setText(SnapSourceName());
	update();
}

void SourceLabel::setText(const char *text)
{
	currentText = text ? text : "";
	//__super::setText(SnapSourceName());
	update();
}

QString SourceLabel::GetText()
{
	return currentText;
}

void SourceTreeItem::OnSourceCaptureState(void *data, calldata_t *calldata)
{
	// This callback maybe invoked from kinds of threads, such as libobsGraphicThread, source plugin.
	// So it is not safe to pass obs_source_t by shared pointer named OBSSource.
	QMetaObject::invokeMethod((SourceTree *)data, "OnSourceStateChanged", Q_ARG(unsigned long long, (unsigned long long)calldata_ptr(calldata, "source")));
}

void SourceTreeItem::BeautySourceStatusChanged(void *data, calldata_t *params)
{
	SourceTree *window = static_cast<SourceTree *>(data);

	obs_source_t *source = (obs_source_t *)calldata_ptr(params, "source");
	if (!source) {
		return;
	}

	QString name = obs_source_get_name(source);
	bool status = calldata_bool(params, "image_status");
	QMetaObject::invokeMethod(window, "OnBeautySourceStatusChanged", Qt::QueuedConnection, Q_ARG(const QString &, name), Q_ARG(bool, status));
}

SourceTreeItem::SourceTreeItem(SourceTree *tree_, OBSSceneItem sceneitem_) : tree(tree_), sceneitem(sceneitem_), selected(false), isItemNormal(true), editing(false)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setMouseTracking(true);

	isScrollShowed = tree_->scrollBar->isVisible();
	connect(tree_->scrollBar, &QSourceScrollBar::SourceScrollShow, this, &SourceTreeItem::OnSourceScrollShow);

	obs_source_t *source = obs_sceneitem_get_source(sceneitem);
	const char *name = obs_source_get_name(source);

	signal_handler_connect_ref(obs_source_get_signal_handler(source), "capture_state", OnSourceCaptureState, tree_);
	signal_handler_connect_ref(obs_source_get_signal_handler(source), "source_image_status", BeautySourceStatusChanged, tree_);

	iconLabel = new QLabel();
	iconLabel->setObjectName("sourceIconLabel");

	vis = new VisibilityCheckBox();
	vis->setObjectName("sourceIconViewCheckbox");
	vis->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	vis->setChecked(obs_sceneitem_visible(sceneitem));

	lock = new LockedCheckBox();
	lock->setObjectName("sourceLockCheckbox");
	lock->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	lock->setChecked(obs_sceneitem_locked(sceneitem));

	label = new SourceLabel(this);
	label->setText(name);
	label->setToolTip(name);
	label->setObjectName("sourceNameLabel");
	label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	label->setAttribute(Qt::WA_TranslucentBackground);

	double dpi = PLSDpiHelper::getDpi(tree_);

	aboveIndicator = new QLabel(this);
	belowIndicator = new QLabel(this);

	aboveIndicator->setFixedHeight(PLSDpiHelper::calculate(dpi, 1));
	belowIndicator->setFixedHeight(PLSDpiHelper::calculate(dpi, 1));
	UpdateIndicator(IndicatorNormal);

#ifdef __APPLE__
	vis->setAttribute(Qt::WA_LayoutUsesWidgetRect);
	lock->setAttribute(Qt::WA_LayoutUsesWidgetRect);
#endif

	spaceBeforeVis = new QSpacerItem(PLSDpiHelper::calculate(dpi, SOURCEITEM_SPACE_BEFORE_VIS), 1);
	spaceBeforeLock = new QSpacerItem(PLSDpiHelper::calculate(dpi, SOURCEITEM_SPACE_BEFORE_LOCK), 1);
	rightMargin = new QSpacerItem(PLSDpiHelper::calculate(dpi, SOURCEITEM_MARGIN_NORMAL_LONG), 1);

	boxLayout = new QHBoxLayout();
	PLSDpiHelper::setDynamicSpacerItem(boxLayout, spaceBeforeVis, true);
	PLSDpiHelper::setDynamicSpacerItem(boxLayout, spaceBeforeLock, true);
	PLSDpiHelper::setDynamicSpacerItem(boxLayout, rightMargin, true);
	boxLayout->setContentsMargins(18, 0, 0, 0);
	boxLayout->addWidget(iconLabel);
	boxLayout->addWidget(label);
	boxLayout->addItem(spaceBeforeVis);
	boxLayout->addWidget(vis);
	boxLayout->addItem(spaceBeforeLock);
	boxLayout->addWidget(lock);
	boxLayout->addItem(rightMargin);
#ifdef __APPLE__
	/* Hack: Fixes a bug where scrollbars would be above the lock icon */
	boxLayout->addSpacing(16);
#endif

	QVBoxLayout *vLayout = new QVBoxLayout(this);
	vLayout->setContentsMargins(0, 0, 0, 0);
	vLayout->setSpacing(0);
	vLayout->addWidget(aboveIndicator);
	vLayout->addLayout(boxLayout);
	vLayout->addWidget(belowIndicator);

	obs_data_t *privData = obs_sceneitem_get_private_settings(sceneitem);
	int preset = obs_data_get_int(privData, "color-preset");
	if (preset == 1) {
		const char *color = obs_data_get_string(privData, "color");
		SetBgColor(BgCustom, (void *)color); // format of color is like "#5555aa7f"
	} else if (preset > 1) {
		int presetIndex = (preset - 2); // the index should start with 0
		SetBgColor(BgPreset, (void *)presetIndex);
	} else {
		SetBgColor(BgDefault, NULL);
	}
	obs_data_release(privData);

	UpdateIcon();
	Update(false);

	/* --------------------------------------------------------- */
	auto setItemLocked = [this](bool checked) {
		QString checkedString = checked ? "checked" : "unchecked";
		PLS_UI_STEP(SOURCE_MODULE, QString("source[%1] lock %2").arg(label->GetText()).arg(checkedString).toStdString().c_str(), ACTION_CLICK);
		SignalBlocker sourcesSignalBlocker(this);
		obs_sceneitem_set_locked(sceneitem, checked);
	};

	connect(vis, &QAbstractButton::clicked, this, &SourceTreeItem::OnVisibleClicked);
	connect(lock, &QAbstractButton::clicked, setItemLocked);

	OnSelectChanged(obs_sceneitem_selected(sceneitem_));
	OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_NORMAL);

	PLSDpiHelper dpiHelper;
	dpiHelper.notifyDpiChanged(this, [=](double dpi) {
		UpdateRightMargin();
		aboveIndicator->setFixedHeight(PLSDpiHelper::calculate(dpi, 1));
		belowIndicator->setFixedHeight(PLSDpiHelper::calculate(dpi, 1));
	});
}

SourceTreeItem::~SourceTreeItem()
{
	if (sceneitem) {
		obs_source_t *source = obs_sceneitem_get_source(sceneitem);
		if (source) {
			signal_handler_disconnect(obs_source_get_signal_handler(source), "capture_state", OnSourceCaptureState, tree);
			signal_handler_disconnect(obs_source_get_signal_handler(source), "source_image_status", BeautySourceStatusChanged, tree);
		}
	}
}

void SourceTreeItem::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

	QWidget::paintEvent(event);
}

void SourceTreeItem::DisconnectSignals()
{
	sceneRemoveSignal.Disconnect();
	itemRemoveSignal.Disconnect();
	deselectSignal.Disconnect();
	visibleSignal.Disconnect();
	lockedSignal.Disconnect();
	renameSignal.Disconnect();
	removeSignal.Disconnect();
}

void SourceTreeItem::Clear()
{
	DisconnectSignals();
	sceneitem = nullptr;
}

void SourceTreeItem::ReconnectSignals()
{
	if (!sceneitem)
		return;

	DisconnectSignals();

	/* --------------------------------------------------------- */

	auto removeItem = [](void *data, calldata_t *cd) {
		SourceTreeItem *this_ = reinterpret_cast<SourceTreeItem *>(data);
		obs_sceneitem_t *curItem = (obs_sceneitem_t *)calldata_ptr(cd, "item");

		if (curItem == this_->sceneitem) {
			QMetaObject::invokeMethod(this_->tree, "Remove", Qt::QueuedConnection, Q_ARG(OBSSceneItem, curItem));
			curItem = nullptr;
		}
		if (!curItem)
			QMetaObject::invokeMethod(this_, "Clear", Qt::QueuedConnection);
	};

	auto itemVisible = [](void *data, calldata_t *cd) {
		SourceTreeItem *this_ = reinterpret_cast<SourceTreeItem *>(data);
		obs_sceneitem_t *curItem = (obs_sceneitem_t *)calldata_ptr(cd, "item");
		bool visible = calldata_bool(cd, "visible");

		if (curItem == this_->sceneitem)
			QMetaObject::invokeMethod(this_, "VisibilityChanged", Qt::QueuedConnection, Q_ARG(bool, visible));
	};

	auto itemLocked = [](void *data, calldata_t *cd) {
		SourceTreeItem *this_ = reinterpret_cast<SourceTreeItem *>(data);
		obs_sceneitem_t *curItem = (obs_sceneitem_t *)calldata_ptr(cd, "item");
		bool locked = calldata_bool(cd, "locked");

		if (curItem == this_->sceneitem)
			QMetaObject::invokeMethod(this_, "LockedChanged", Qt::QueuedConnection, Q_ARG(bool, locked));
	};

	auto itemDeselect = [](void *data, calldata_t *cd) {
		SourceTreeItem *this_ = reinterpret_cast<SourceTreeItem *>(data);
		obs_sceneitem_t *curItem = (obs_sceneitem_t *)calldata_ptr(cd, "item");

		if (curItem == this_->sceneitem)
			QMetaObject::invokeMethod(this_, "Deselect", Qt::QueuedConnection);
	};

	auto reorderGroup = [](void *data, calldata_t *) {
		SourceTreeItem *this_ = reinterpret_cast<SourceTreeItem *>(data);
		QMetaObject::invokeMethod(this_->tree, "ReorderItems", Qt::QueuedConnection);
	};

	obs_scene_t *scene = obs_sceneitem_get_scene(sceneitem);
	obs_source_t *sceneSource = obs_scene_get_source(scene);
	signal_handler_t *signal = obs_source_get_signal_handler(sceneSource);

	sceneRemoveSignal.Connect(signal, "remove", removeItem, this);
	itemRemoveSignal.Connect(signal, "item_remove", removeItem, this);
	visibleSignal.Connect(signal, "item_visible", itemVisible, this);
	lockedSignal.Connect(signal, "item_locked", itemLocked, this);

	if (obs_sceneitem_is_group(sceneitem)) {
		obs_source_t *source = obs_sceneitem_get_source(sceneitem);
		signal = obs_source_get_signal_handler(source);

		groupReorderSignal.Connect(signal, "reorder", reorderGroup, this);
	}

	if (scene != GetCurrentScene())
		deselectSignal.Connect(signal, "item_deselect", itemDeselect, this);

	/* --------------------------------------------------------- */

	auto renamed = [](void *data, calldata_t *cd) {
		SourceTreeItem *this_ = reinterpret_cast<SourceTreeItem *>(data);
		const char *name = calldata_string(cd, "new_name");
		QMetaObject::invokeMethod(this_, "Renamed", Q_ARG(QString, QT_UTF8(name)));
	};

	auto removeSource = [](void *data, calldata_t *) {
		SourceTreeItem *this_ = reinterpret_cast<SourceTreeItem *>(data);
		QMetaObject::invokeMethod(this_->tree, "OnSourceItemRemove", Q_ARG(unsigned long long, (unsigned long long)this_->sceneitem.get()));
		this_->DisconnectSignals();
		this_->sceneitem = nullptr;
	};

	obs_source_t *source = obs_sceneitem_get_source(sceneitem);
	signal = obs_source_get_signal_handler(source);
	renameSignal.Connect(signal, "rename", renamed, this);
	removeSignal.Connect(signal, "remove", removeSource, this);
}

void SourceTreeItem::mouseDoubleClickEvent(QMouseEvent *event)
{
	QWidget::mouseDoubleClickEvent(event);

	if (expand) {
		PLS_UI_STEP(SOURCE_MODULE, QString("group[%1] item").arg(label->GetText()).toStdString().c_str(), ACTION_DBCLICK);
		expand->setChecked(!expand->isChecked());
	} else {
		PLS_UI_STEP(SOURCE_MODULE, QString("source[%1] item").arg(label->GetText()).toStdString().c_str(), ACTION_DBCLICK);
		obs_source_t *source = obs_sceneitem_get_source(sceneitem);
		PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
		if (source && main) {
			main->CreatePropertiesWindow(source, OPERATION_NONE, main);
		}
	}
}

void SourceTreeItem::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
	PLS_UI_STEP(SOURCE_MODULE, QString("source[%1] item").arg(label->GetText()).toStdString().c_str(), ACTION_CLICK);

	if (!editing)
		OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_PRESSED);
}

void SourceTreeItem::mouseReleaseEvent(QMouseEvent *event)
{
	QWidget::mouseReleaseEvent(event);

	if (!editing)
		OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_HOVER);
}

void SourceTreeItem::enterEvent(QEvent *event)
{
	QWidget::enterEvent(event);

	if (!editing)
		OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_HOVER);

	PLSBasicPreview *preview = PLSBasicPreview::Get();
	std::lock_guard<std::mutex> lock(preview->selectMutex);
	preview->hoveredPreviewItems.clear();
	preview->hoveredPreviewItems.push_back(sceneitem);
}

void SourceTreeItem::leaveEvent(QEvent *event)
{
	QWidget::leaveEvent(event);

	OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_NORMAL);

	PLSBasicPreview *preview = PLSBasicPreview::Get();
	std::lock_guard<std::mutex> lock(preview->selectMutex);
	preview->hoveredPreviewItems.clear();
}

void SourceTreeItem::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
}

bool SourceTreeItem::IsEditing()
{
	return editor != nullptr;
}

OBSSceneItem SourceTreeItem::SceneItem()
{
	return sceneitem;
}

QString GenerateSourceItemBg(const char *property, const char *color)
{
	QString temp;
	return temp.sprintf("SourceTreeItem,SourceTreeItem[status=\"%s\"]{background-color: %s;}", property, color);
}

void SourceTreeItem::SetBgColor(SourceItemBgType type, void *param)
{
	QString qss = "";

	do {
		if (type == BgCustom) {
			if (param) {
				const char *color = (const char *)param;
				qss += GenerateSourceItemBg(PROPERTY_VALUE_MOUSE_STATUS_NORMAL, color);
				qss += GenerateSourceItemBg(PROPERTY_VALUE_MOUSE_STATUS_HOVER, color);
				qss += GenerateSourceItemBg(PROPERTY_VALUE_MOUSE_STATUS_PRESSED, color);
			}
		} else if (type == BgPreset) {
			int index = (int)param;
			if (index < 0 || index >= presetColorList.size())
				break;
			QString color = presetColorList[index];
			qss += GenerateSourceItemBg(PROPERTY_VALUE_MOUSE_STATUS_NORMAL, color.toUtf8());
			qss += GenerateSourceItemBg(PROPERTY_VALUE_MOUSE_STATUS_HOVER, color.toUtf8());
			qss += GenerateSourceItemBg(PROPERTY_VALUE_MOUSE_STATUS_PRESSED, color.toUtf8());
		}

	} while (0);

	if (qss.isEmpty()) {
		// default style
		qss += GenerateSourceItemBg(PROPERTY_VALUE_MOUSE_STATUS_NORMAL, "#272727");
		qss += GenerateSourceItemBg(PROPERTY_VALUE_MOUSE_STATUS_HOVER, "#444444");
		qss += GenerateSourceItemBg(PROPERTY_VALUE_MOUSE_STATUS_PRESSED, "#2d2d2d");
	}

	this->setStyleSheet(qss);

	OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_NORMAL);
}

void SourceTreeItem::EnterEditMode()
{
	editing = true;
	OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_NORMAL);

	PLS_UI_STEP(SOURCE_MODULE, "enter rename source", ACTION_CLICK);

	obs_source_t *source = obs_sceneitem_get_source(sceneitem);
	const char *name = obs_source_get_name(source);

	setFocusPolicy(Qt::StrongFocus);
	boxLayout->removeWidget(label);
	editor = new QLineEdit(name ? name : "");
	editor->selectAll();
	editor->installEventFilter(this);
	editor->setObjectName(OBJECT_NAME_SOURCE_RENAME_EDIT);
	boxLayout->insertWidget(2, editor);
	setFocusProxy(editor);
}

void SourceTreeItem::ExitEditMode(bool save)
{
	editing = false;

	if (!editor)
		return;

	PLS_UI_STEP(SOURCE_MODULE, "exit rename source", ACTION_CLICK);

	PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
	OBSScene scene = main->GetCurrentScene();

	std::string newName = QT_TO_UTF8(editor->text().simplified());

	setFocusProxy(nullptr);
	boxLayout->removeWidget(editor);
	delete editor;
	editor = nullptr;
	setFocusPolicy(Qt::NoFocus);
	boxLayout->insertWidget(2, label);

	/* ----------------------------------------- */
	/* check for empty string                    */

	if (!save)
		return;

	if (newName.empty()) {
		PLSMessageBox::information(main, QTStr("NoNameEntered.Title"), QTStr("NoNameEntered.Text"));
		return;
	}

	/* ----------------------------------------- */
	/* Check for same name                       */

	obs_source_t *source = obs_sceneitem_get_source(sceneitem);
	if (!source)
		return;

	const char *tempName = obs_source_get_name(source);
	if (!tempName)
		return;

	if (strcmp(newName.c_str(), tempName) == 0)
		return;

	/* ----------------------------------------- */
	/* check for existing source                 */

	obs_source_t *existingSource = obs_get_source_by_name(newName.c_str());
	obs_source_release(existingSource);
	bool exists = !!existingSource;

	if (exists) {
		PLSMessageBox::information(main, QTStr("NameExists.Title"), QTStr("NameExists.Text"));
		return;
	}

	/* ----------------------------------------- */
	/* rename                                    */

	SignalBlocker sourcesSignalBlocker(this);
	obs_source_set_name(source, newName.c_str());
	label->setText(QT_UTF8(newName.c_str()));
	label->setToolTip(QT_UTF8(newName.c_str()));
}

bool SourceTreeItem::eventFilter(QObject *object, QEvent *event)
{
	if (editor != object)
		return false;

	if (LineEditCanceled(event)) {
		QMetaObject::invokeMethod(this, "ExitEditMode", Qt::QueuedConnection, Q_ARG(bool, false));
		return true;
	}
	if (LineEditChanged(event)) {
		QMetaObject::invokeMethod(this, "ExitEditMode", Qt::QueuedConnection, Q_ARG(bool, true));
		return true;
	}

	return false;
}

void SourceTreeItem::VisibilityChanged(bool visible)
{
	//PRISM/ZengQin/20200818/#4026/for all sources
	UpdateIcon();
	UpdateNameColor(selected, visible);

	//PRISM/ZengQin/20200811/#4026/for media controller
	tree->OnVisibleItemChanged(sceneitem, visible);

	vis->setChecked(visible);
}

void SourceTreeItem::LockedChanged(bool locked)
{
	lock->setChecked(locked);
}

void SourceTreeItem::Renamed(const QString &name)
{
	label->setText(name);
	label->setToolTip(name);
}

void SourceTreeItem::Update(bool force)
{
	OBSScene scene = GetCurrentScene();
	obs_scene_t *itemScene = obs_sceneitem_get_scene(sceneitem);

	Type newType;

	/* ------------------------------------------------- */
	/* if it's a group item, insert group checkbox       */

	if (obs_sceneitem_is_group(sceneitem)) {
		newType = Type::Group;

		/* ------------------------------------------------- */
		/* if it's a group sub-item                          */

	} else if (itemScene != scene) {
		newType = Type::SubItem;

		/* ------------------------------------------------- */
		/* if it's a regular item                            */

	} else {
		newType = Type::Item;
	}

	/* ------------------------------------------------- */

	if (!force && newType == type) {
		return;
	}

	/* ------------------------------------------------- */

	ReconnectSignals();

	if (spacer) {
		PLSDpiHelper::dynamicDeleteSpacerItem(boxLayout, spacer);
		spacer = nullptr;
	}

	if (type == Type::Group) {
		boxLayout->removeWidget(expand);
		expand->deleteLater();
		expand = nullptr;
	}

	type = newType;

	if (type == Type::SubItem) {
		spacer = new QSpacerItem(16, 1);
		boxLayout->insertItem(0, spacer);

	} else if (type == Type::Group) {
		expand = new SourceTreeSubItemCheckBox();
		expand->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
#ifdef __APPLE__
		expand->setAttribute(Qt::WA_LayoutUsesWidgetRect);
#endif
		boxLayout->insertWidget(0, expand);
		boxLayout->setContentsMargins(7, 0, 0, 0);

		obs_data_t *data = obs_sceneitem_get_private_settings(sceneitem);
		expand->blockSignals(true);
		expand->setChecked(obs_data_get_bool(data, "collapsed"));
		expand->blockSignals(false);
		obs_data_release(data);

		connect(expand, &QPushButton::toggled, this, &SourceTreeItem::ExpandClicked);

	} else {
		spacer = new QSpacerItem(3, 1);
		boxLayout->insertItem(0, spacer);
	}
}

void SourceTreeItem::OnIconTypeChanged(QString value)
{
	iconLabel->setProperty(PROPERTY_NAME_ICON_TYPE, value);
	iconLabel->style()->unpolish(iconLabel);
	iconLabel->style()->polish(iconLabel);
}

void SourceTreeItem::OnMouseStatusChanged(const char *s)
{
	isItemNormal = (s && 0 == strcmp(s, PROPERTY_VALUE_MOUSE_STATUS_NORMAL));
	if (isItemNormal) {
		vis->hide();
		lock->hide();
	} else {
		vis->show();
		lock->show();
	}
	UpdateRightMargin();

	this->setProperty(PROPERTY_NAME_MOUSE_STATUS, s);
	this->style()->unpolish(this);
	this->style()->polish(this);
}

void SourceTreeItem::UpdateIndicator(IndicatorType type)
{
	QString normalBk = "background: transparent;";
	QString highlightBk = "background:" + QString(SCENE_SCROLLCONTENT_LINE_COLOR) + QString(";");

	switch (type) {
	case SourceTreeItem::IndicatorAbove:
		belowIndicator->setStyleSheet(normalBk);
		aboveIndicator->setStyleSheet(highlightBk);
		break;
	case SourceTreeItem::IndicatorBelow:
		aboveIndicator->setStyleSheet(normalBk);
		belowIndicator->setStyleSheet(highlightBk);
		break;

	case SourceTreeItem::IndicatorNormal:
	default:
		aboveIndicator->setStyleSheet(normalBk);
		belowIndicator->setStyleSheet(normalBk);
		break;
	}
}

void SourceTreeItem::OnSelectChanged(bool isSelected)
{
	selected = isSelected;

	UpdateIcon();
	UpdateNameColor(isSelected, obs_sceneitem_visible(sceneitem));
	emit SelectItemChanged(sceneitem, isSelected);
}

void SourceTreeItem::UpdateRightMargin()
{
	double dpi = PLSDpiHelper::getDpi(this);
	if (isItemNormal) {
		spaceBeforeVis->changeSize(0, 0);
		spaceBeforeLock->changeSize(0, 0);
		if (isScrollShowed)
			rightMargin->changeSize(PLSDpiHelper::calculate(dpi, SOURCEITEM_MARGIN_NORMAL_SHORT), 1);
		else
			rightMargin->changeSize(PLSDpiHelper::calculate(dpi, SOURCEITEM_MARGIN_NORMAL_LONG), 1);
	} else {
		spaceBeforeVis->changeSize(PLSDpiHelper::calculate(dpi, SOURCEITEM_SPACE_BEFORE_VIS), 1);
		spaceBeforeLock->changeSize(PLSDpiHelper::calculate(dpi, SOURCEITEM_SPACE_BEFORE_LOCK), 1);
		if (isScrollShowed)
			rightMargin->changeSize(PLSDpiHelper::calculate(dpi, SOURCEITEM_MARGIN_UNNORMAL_SHORT), 1);
		else
			rightMargin->changeSize(PLSDpiHelper::calculate(dpi, SOURCEITEM_MARGIN_UNNORMAL_LONG), 1);
	}

	boxLayout->invalidate();
}

QString SourceTreeItem::GetErrorTips(const char *id, enum obs_source_error error)
{
	switch (obs_source_get_icon_type(id)) {
	case OBS_ICON_TYPE_SLIDESHOW:
		return QTStr("Source.ErrorTips.SlidShow.Error");

	case OBS_ICON_TYPE_IMAGE: {
		switch (error) {
		case OBS_SOURCE_ERROR_NOT_FOUND:
			return QTStr("Source.ErrorTips.Image.NotFound");
		default:
			return QTStr("Source.ErrorTips.Image.Error");
		}
	}

	case OBS_ICON_TYPE_MEDIA: {
		switch (error) {
		case OBS_SOURCE_ERROR_NOT_FOUND:
			return QTStr("Source.ErrorTips.Media.NotFound");
		default:
			return QTStr("Source.ErrorTips.Media.Error");
		}
	}

	case OBS_ICON_TYPE_AUDIO_INPUT:
	case OBS_ICON_TYPE_AUDIO_OUTPUT: {
		switch (error) {
		case OBS_SOURCE_ERROR_NOT_FOUND:
			return QTStr("Source.ErrorTips.Audio.NotFound");
		default:
			return QTStr("Source.ErrorTips.Audio.Error");
		}
	}

	case OBS_ICON_TYPE_CAMERA: {
		switch (error) {
		case OBS_SOURCE_ERROR_BE_USING:
			return QTStr("Source.ErrorTips.Camera.BeUsing");
		case OBS_SOURCE_ERROR_NOT_FOUND:
			return QTStr("Source.ErrorTips.Camera.NotFound");
		default:
			return QTStr("Source.ErrorTips.Camera.Error");
		}
	}

	case OBS_ICON_TYPE_WINDOW_CAPTURE:
		return QTStr("Source.ErrorTips.Window.NotFound");

	case OBS_ICON_TYPE_GAME_CAPTURE: {
		switch (error) {
		case OBS_SOURCE_ERROR_NOT_FOUND:
			return QTStr("Source.ErrorTips.Game.NotFound");
		default:
			return QTStr("Source.ErrorTips.Game.Error");
		}
	}
	case OBS_ICON_TYPE_BGM: {
		switch (error) {
		case OBS_SOURCE_ERROR_UNKNOWN:
			return QTStr("Source.ErrorTips.Bgm.Error");
		default:
			return QTStr("Source.ErrorTips.Bgm.Error");
		}
	}

	case OBS_ICON_TYPE_GIPHY: {
		switch (error) {
		case OBS_SOURCE_ERROR_NOT_FOUND:
			return QTStr("Source.ErrorTips.Giphy.NotFound");
		default:
			return QTStr("Source.ErrorTips.Giphy.Error");
		}
	}

	case OBS_ICON_TYPE_REGION: {
		switch (error) {
		case OBS_SOURCE_ERROR_NOT_FOUND:
			return QTStr("Source.ErrorTips.Region.NotFound");
		default:
			assert(false && "lost string for invalid state");
			return "";
		}
	}

	case OBS_ICON_TYPE_CUSTOM:
	default: {
		assert(false && "plugin is invalid but there is no string");
		return "";
	}
	}
}

void SourceTreeItem::ExpandClicked(bool checked)
{
	PLS_UI_STEP(SOURCE_MODULE, QString("group[%1] item expand").arg(label->GetText()).toStdString().c_str(), ACTION_CLICK);

	OBSData data = obs_sceneitem_get_private_settings(sceneitem);
	obs_data_release(data);

	obs_data_set_bool(data, "collapsed", checked);

	if (!checked)
		tree->GetStm()->ExpandGroup(sceneitem);
	else
		tree->GetStm()->CollapseGroup(sceneitem);
}

void SourceTreeItem::Deselect()
{
	tree->SelectItem(sceneitem, false);
}

// string should match with qss
#define SOURCE_ICON_VISIBLE "visible"
#define SOURCE_ICON_INVIS "invisible"
#define SOURCE_ICON_SELECT "select"
#define SOURCE_ICON_UNSELECT "unselect"
#define SOURCE_ICON_VALID "valid"
#define SOURCE_ICON_INVALID "invalid"
#define SOURCE_ICON_SCENE "scene"
#define SOURCE_ICON_GROUP "group"
#define SOURCE_ICON_IMAGE "image"
#define SOURCE_ICON_COLOR "color"
#define SOURCE_ICON_SLIDESHOW "slideshow"
#define SOURCE_ICON_AUDIOINPUT "audioinput"
#define SOURCE_ICON_AUDIOOUTPUT "audiooutput"
#define SOURCE_ICON_MONITOR "monitor"
#define SOURCE_ICON_WINDOW "window"
#define SOURCE_ICON_GAME "game"
#define SOURCE_ICON_CAMERA "camera"
#define SOURCE_ICON_TEXT "text"
#define SOURCE_ICON_MEDIA "media"
#define SOURCE_ICON_BROWSER "browser"
#define SOURCE_ICON_GIPHY "giphy"
#define SOURCE_ICON_DEFAULT "default"
#define SOURCE_ICON_BGM "bgm"
#define SOURCE_ICON_NDI "ndi"
#define SOURCE_ICON_TEXT_MOTION "textmotion"
#define SOURCE_ICON_CHAT "chat"

QString GetIconKey(obs_icon_type type)
{
	switch (type) {
	case OBS_ICON_TYPE_IMAGE:
		return SOURCE_ICON_IMAGE;
	case OBS_ICON_TYPE_COLOR:
		return SOURCE_ICON_COLOR;
	case OBS_ICON_TYPE_SLIDESHOW:
		return SOURCE_ICON_SLIDESHOW;
	case OBS_ICON_TYPE_AUDIO_INPUT:
		return SOURCE_ICON_AUDIOINPUT;
	case OBS_ICON_TYPE_AUDIO_OUTPUT:
		return SOURCE_ICON_AUDIOOUTPUT;
	case OBS_ICON_TYPE_DESKTOP_CAPTURE:
	case OBS_ICON_TYPE_REGION:
		return SOURCE_ICON_MONITOR;
	case OBS_ICON_TYPE_WINDOW_CAPTURE:
		return SOURCE_ICON_WINDOW;
	case OBS_ICON_TYPE_GAME_CAPTURE:
		return SOURCE_ICON_GAME;
	case OBS_ICON_TYPE_CAMERA:
		return SOURCE_ICON_CAMERA;
	case OBS_ICON_TYPE_TEXT:
		return SOURCE_ICON_TEXT;
	case OBS_ICON_TYPE_MEDIA:
		return SOURCE_ICON_MEDIA;
	case OBS_ICON_TYPE_BROWSER:
		return SOURCE_ICON_BROWSER;
	case OBS_ICON_TYPE_BGM:
		return SOURCE_ICON_BGM;
	case OBS_ICON_TYPE_GIPHY:
		return SOURCE_ICON_GIPHY;
	case OBS_ICON_TYPE_NDI:
		return SOURCE_ICON_NDI;
	case OBS_ICON_TYPE_TEXT_MOTION:
		return SOURCE_ICON_TEXT_MOTION;
	case OBS_ICON_TYPE_CHAT:
		return SOURCE_ICON_CHAT;
	default:
		return SOURCE_ICON_DEFAULT;
	}
}

void SourceTreeItem::UpdateNameColor(bool selected, bool visible)
{
	QString visibleStr = visible ? SOURCE_ICON_VISIBLE : SOURCE_ICON_INVIS;
	QString selectStr = selected ? SOURCE_ICON_SELECT : SOURCE_ICON_UNSELECT;

	QString value = visibleStr + QString(".") + selectStr;
	label->setProperty(PROPERTY_NAME_STATUS, value);

	label->style()->unpolish(label);
	label->style()->polish(label);
}

void SourceTreeItem::UpdateIcon()
{
	obs_source_t *source = obs_sceneitem_get_source(sceneitem);
	if (!source)
		return;

	const char *id = obs_source_get_id(source);
	if (!id)
		return;

	QString visibleStr = obs_sceneitem_visible(sceneitem) ? SOURCE_ICON_VISIBLE : SOURCE_ICON_INVIS;
	QString selectStr = selected ? SOURCE_ICON_SELECT : SOURCE_ICON_UNSELECT;

	if (strcmp(id, "scene") == 0) {
		OnIconTypeChanged(visibleStr + QString(".") + selectStr + QString(".") + QString(SOURCE_ICON_SCENE));
	} else if (strcmp(id, "group") == 0) {
		OnIconTypeChanged(visibleStr + QString(".") + selectStr + QString(".") + QString(SOURCE_ICON_GROUP));
	} else {
		enum obs_source_error error;
		if (obs_source_get_capture_valid(source, &error)) {
			QString value = visibleStr + QString(".") + QString(SOURCE_ICON_VALID) + QString(".") + selectStr + QString(".") + GetIconKey(obs_source_get_icon_type(id));
			OnIconTypeChanged(value);
			iconLabel->setToolTip("");

		} else {
			QString value = visibleStr + QString(".") + QString(SOURCE_ICON_INVALID) + QString(".") + selectStr + QString(".") + GetIconKey(obs_source_get_icon_type(id));
			OnIconTypeChanged(value);
			iconLabel->setToolTip(GetErrorTips(id, error));
		}
	}
}

bool SourceTreeItem::isDshowSourceChangedState(obs_source_t *source)
{
	if (!source) {
		return false;
	}
	const char *id = obs_source_get_id(source);
	if (!id)
		return false;

	return 0 == strcmp(id, DSHOW_SOURCE_ID);
}

void SourceTreeItem::OnVisibleClicked(bool visible)
{
	auto setItemVisible = [this](bool checked) {
		QString visibleString = checked ? "visible" : "invisible";
		PLS_UI_STEP(SOURCE_MODULE, QString("source[%1] %2").arg(label->GetText()).arg(visibleString).toStdString().c_str(), ACTION_CLICK);
		SignalBlocker sourcesSignalBlocker(this);
		obs_sceneitem_set_visible(sceneitem, checked);
		//PRISM/ZengQin/20200818/#4026/for all sources
		//UpdateIcon();
		//UpdateNameColor(selected, checked);
	};

	setItemVisible(visible);
	//PRISM/ZengQin/20200811/#4026/for media controller
	//emit VisibleItemChanged(sceneitem, visible);
}

void SourceTreeItem::OnSourceScrollShow(bool isShow)
{
	isScrollShowed = isShow;
	UpdateRightMargin();
}

/* ========================================================================= */

void SourceTreeModel::PLSFrontendEvent(enum obs_frontend_event event, void *ptr)
{
	SourceTreeModel *stm = reinterpret_cast<SourceTreeModel *>(ptr);

	switch ((int)event) {
	case OBS_FRONTEND_EVENT_PREVIEW_SCENE_CHANGED:
		stm->SceneChanged();
		break;
	case OBS_FRONTEND_EVENT_EXIT:
	case OBS_FRONTEND_EVENT_SCENE_COLLECTION_CLEANUP:
		stm->Clear();
		break;
	}
}

void SourceTreeModel::Clear()
{
	beginResetModel();
	items.clear();
	endResetModel();

	hasGroups = false;
}

static bool enumItem(obs_scene_t *, obs_sceneitem_t *item, void *ptr)
{
	QVector<OBSSceneItem> &items = *reinterpret_cast<QVector<OBSSceneItem> *>(ptr);

	if (obs_sceneitem_is_group(item)) {
		obs_data_t *data = obs_sceneitem_get_private_settings(item);

		bool collapse = obs_data_get_bool(data, "collapsed");
		if (!collapse) {
			obs_scene_t *scene = obs_sceneitem_group_get_scene(item);

			obs_scene_enum_items(scene, enumItem, &items);
		}

		obs_data_release(data);
	}

	items.insert(0, item);
	return true;
}

struct ChildAndParent {
	std::map<obs_sceneitem_t *, std::pair<obs_sceneitem_t *, bool>> childAndParent;
	std::pair<obs_sceneitem_t *, bool> parent;
};
static bool enumItemForParent(obs_scene_t *, obs_sceneitem_t *item, void *ptr)
{
	ChildAndParent &sourceParents = *reinterpret_cast<ChildAndParent *>(ptr);
	sourceParents.childAndParent.insert(std::make_pair(item, sourceParents.parent));
	if (obs_sceneitem_is_group(item)) {

		std::pair<obs_sceneitem_t *, bool> parentTemp = sourceParents.parent;
		sourceParents.parent = std::make_pair(item, obs_sceneitem_visible(item));
		obs_sceneitem_group_enum_items(item, enumItemForParent, &sourceParents);
		sourceParents.parent = parentTemp;
	}
	return true;
}

void SourceTreeModel::SceneChanged()
{
	OBSScene scene = GetCurrentScene();

	beginResetModel();
	items.clear();
	obs_scene_enum_items(scene, enumItem, &items);
	endResetModel();

	UpdateGroupState(false);
	st->ResetWidgets();

	for (int i = 0; i < items.count(); i++) {
		bool select = obs_sceneitem_selected(items[i]);
		QModelIndex index = createIndex(i, 0);
		st->selectionModel()->select(index, select ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
	}
}

/* moves a scene item index (blame linux distros for using older Qt builds) */
static inline void MoveItem(QVector<OBSSceneItem> &items, int oldIdx, int newIdx)
{
	OBSSceneItem item = items[oldIdx];
	items.remove(oldIdx);
	items.insert(newIdx, item);
}

/* reorders list optimally with model reorder funcs */
void SourceTreeModel::ReorderItems()
{
	OBSScene scene = GetCurrentScene();

	QVector<OBSSceneItem> newitems;
	obs_scene_enum_items(scene, enumItem, &newitems);

	/* if item list has changed size, do full reset */
	if (newitems.count() != items.count()) {
		SceneChanged();
		return;
	}

	for (;;) {
		int idx1Old = 0;
		int idx1New = 0;
		int count;
		int i;

		/* find first starting changed item index */
		for (i = 0; i < newitems.count(); i++) {
			obs_sceneitem_t *oldItem = items[i];
			obs_sceneitem_t *newItem = newitems[i];
			if (oldItem != newItem) {
				idx1Old = i;
				break;
			}
		}

		/* if everything is the same, break */
		if (i == newitems.count()) {
			break;
		}

		/* find new starting index */
		for (i = idx1Old + 1; i < newitems.count(); i++) {
			obs_sceneitem_t *oldItem = items[idx1Old];
			obs_sceneitem_t *newItem = newitems[i];

			if (oldItem == newItem) {
				idx1New = i;
				break;
			}
		}

		/* if item could not be found, do full reset */
		if (i == newitems.count()) {
			SceneChanged();
			return;
		}

		/* get move count */
		for (count = 1; (idx1New + count) < newitems.count(); count++) {
			int oldIdx = idx1Old + count;
			int newIdx = idx1New + count;

			obs_sceneitem_t *oldItem = items[oldIdx];
			obs_sceneitem_t *newItem = newitems[newIdx];

			if (oldItem != newItem) {
				break;
			}
		}

		/* move items */
		beginMoveRows(QModelIndex(), idx1Old, idx1Old + count - 1, QModelIndex(), idx1New + count);
		for (i = 0; i < count; i++) {
			int to = idx1New + count;
			if (to > idx1Old)
				to--;
			MoveItem(items, idx1Old, to);
		}
		endMoveRows();
		emit itemReorder();
	}
}

void SourceTreeModel::Add(obs_sceneitem_t *item)
{
	if (obs_sceneitem_is_group(item)) {
		SceneChanged();
	} else {
		beginInsertRows(QModelIndex(), 0, 0);
		items.insert(0, item);
		endInsertRows();

		st->UpdateWidget(createIndex(0, 0, nullptr), item);
	}
}

void SourceTreeModel::Remove(void *itemPointer)
{
	int idx = -1;
	OBSSceneItem item;
	for (int i = 0; i < items.count(); i++) {
		if (items[i].get() == itemPointer) {
			item = items[i];
			idx = i;
			break;
		}
	}

	if (idx == -1)
		return;

	int startIdx = idx;
	int endIdx = idx;

	bool is_group = obs_sceneitem_is_group(item);
	if (is_group) {
		obs_scene_t *scene = obs_sceneitem_group_get_scene(item);

		for (int i = endIdx + 1; i < items.count(); i++) {
			obs_sceneitem_t *subitem = items[i];
			obs_scene_t *subscene = obs_sceneitem_get_scene(subitem);

			if (subscene == scene)
				endIdx = i;
			else
				break;
		}
	}

	QVector<OBSSceneItem> tmpitems;
	for (int i = startIdx; i <= endIdx; i++) {
		tmpitems.push_back(items[i]);
	}
	emit itemRemoves(tmpitems);

	beginRemoveRows(QModelIndex(), startIdx, endIdx);
	items.remove(idx, endIdx - startIdx + 1);
	endRemoveRows();
	if (is_group)
		UpdateGroupState(true);
}

OBSSceneItem SourceTreeModel::Get(int idx)
{
	if (idx == -1 || idx >= items.count())
		return OBSSceneItem();
	return items[idx];
}

SourceTreeModel::SourceTreeModel(SourceTree *st_) : QAbstractListModel(st_), st(st_)
{
	obs_frontend_add_event_callback(PLSFrontendEvent, this);
}

SourceTreeModel::~SourceTreeModel()
{
	obs_frontend_remove_event_callback(PLSFrontendEvent, this);
}

int SourceTreeModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : items.count();
}

QVariant SourceTreeModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::AccessibleTextRole) {
		OBSSceneItem item = items[index.row()];
		obs_source_t *source = obs_sceneitem_get_source(item);
		return QVariant(QT_UTF8(obs_source_get_name(source)));
	}

	return QVariant();
}

Qt::ItemFlags SourceTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return QAbstractListModel::flags(index) | Qt::ItemIsDropEnabled;

	obs_sceneitem_t *item = items[index.row()];
	bool is_group = obs_sceneitem_is_group(item);

	return QAbstractListModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | (is_group ? Qt::ItemIsDropEnabled : Qt::NoItemFlags);
}

Qt::DropActions SourceTreeModel::supportedDropActions() const
{
	return QAbstractItemModel::supportedDropActions() | Qt::MoveAction;
}

QString SourceTreeModel::GetNewGroupName()
{
	OBSScene scene = GetCurrentScene();
	QString name = QTStr("Group");

	int i = 2;
	for (;;) {
		obs_source_t *group = obs_get_source_by_name(QT_TO_UTF8(name));
		obs_source_release(group);
		if (!group)
			break;
		name = QTStr("Basic.Main.Group").arg(QString::number(i++));
	}

	return name;
}

void SourceTreeModel::AddGroup()
{
	QString name = GetNewGroupName();
	obs_sceneitem_t *group = obs_scene_add_group(GetCurrentScene(), QT_TO_UTF8(name));
	if (!group)
		return;

	beginInsertRows(QModelIndex(), 0, 0);
	items.insert(0, group);
	endInsertRows();

	st->UpdateWidget(createIndex(0, 0, nullptr), group);
	UpdateGroupState(true);

	QMetaObject::invokeMethod(st, "Edit", Qt::QueuedConnection, Q_ARG(int, 0));
}

void SourceTreeModel::GroupSelectedItems(QModelIndexList &indices)
{
	if (indices.count() == 0)
		return;

	PLS_UI_STEP(MAINFRAME_MODULE, QT_TO_UTF8(QTStr("Basic.Main.GroupItems")), ACTION_CLICK);
	OBSScene scene = GetCurrentScene();
	QString name = GetNewGroupName();

	QVector<obs_sceneitem_t *> item_order;

	for (int i = indices.count() - 1; i >= 0; i--) {
		obs_sceneitem_t *item = items[indices[i].row()];
		item_order << item;
	}

	obs_sceneitem_t *item = obs_scene_insert_group(scene, QT_TO_UTF8(name), item_order.data(), item_order.size());
	if (!item) {
		return;
	}

	for (obs_sceneitem_t *item : item_order)
		obs_sceneitem_select(item, true);

	int newIdx = indices[0].row();

	beginInsertRows(QModelIndex(), newIdx, newIdx);
	items.insert(newIdx, item);
	endInsertRows();

	for (int i = 0; i < indices.size(); i++) {
		int fromIdx = indices[i].row() + 1;
		int toIdx = newIdx + i + 1;
		if (fromIdx != toIdx) {
			beginMoveRows(QModelIndex(), fromIdx, fromIdx, QModelIndex(), toIdx);
			MoveItem(items, fromIdx, toIdx);
			endMoveRows();
		}
	}

	hasGroups = true;
	st->UpdateWidgets(true);

	obs_sceneitem_select(item, true);

	QMetaObject::invokeMethod(st, "Edit", Qt::QueuedConnection, Q_ARG(int, newIdx));
}

static bool enumDshowItem(obs_scene_t *, obs_sceneitem_t *item, void *param)
{
	QVector<OBSSceneItem> &items = *reinterpret_cast<QVector<OBSSceneItem> *>(param);
	obs_source_t *source = obs_sceneitem_get_source(item);
	if (!source) {
		return true;
	}
	const char *id = obs_source_get_id(source);
	if (!id) {
		return true;
	}

	enum obs_source_error error;
	if (0 != strcmp(id, DSHOW_SOURCE_ID) && 0 != strcmp(id, BGM_SOURCE_ID)) {
		return true;
	}

	const char *name = obs_source_get_name(source);
	items.push_back(item);

	return true;
}

void SourceTreeModel::UngroupSelectedGroups(QModelIndexList &indices)
{
	if (indices.count() == 0)
		return;

	PLS_UI_STEP(MAINFRAME_MODULE, QT_TO_UTF8(QTStr("Basic.Main.Ungroup")), ACTION_CLICK);

	for (int i = indices.count() - 1; i >= 0; i--) {
		obs_sceneitem_t *item = items[indices[i].row()];

		// scene item was released when ungroup
		QVector<OBSSceneItem> itemsList;
		if (obs_sceneitem_is_group(item)) {
			obs_sceneitem_group_enum_items(item, enumDshowItem, &itemsList);
			if (0 != itemsList.size()) {
				emit itemRemoves(itemsList);
			}
		}

		obs_sceneitem_group_ungroup(item);
	}

	SceneChanged();
}

void SourceTreeModel::ExpandGroup(obs_sceneitem_t *item)
{
	int itemIdx = items.indexOf(item);
	if (itemIdx == -1)
		return;

	itemIdx++;

	obs_scene_t *scene = obs_sceneitem_group_get_scene(item);

	QVector<OBSSceneItem> subItems;
	obs_scene_enum_items(scene, enumItem, &subItems);

	if (!subItems.size())
		return;

	beginInsertRows(QModelIndex(), itemIdx, itemIdx + subItems.size() - 1);
	for (int i = 0; i < subItems.size(); i++)
		items.insert(i + itemIdx, subItems[i]);
	endInsertRows();

	st->UpdateWidgets();
}

void SourceTreeModel::CollapseGroup(obs_sceneitem_t *item)
{
	int startIdx = -1;
	int endIdx = -1;

	obs_scene_t *scene = obs_sceneitem_group_get_scene(item);

	for (int i = 0; i < items.size(); i++) {
		obs_scene_t *itemScene = obs_sceneitem_get_scene(items[i]);

		if (itemScene == scene) {
			if (startIdx == -1)
				startIdx = i;
			endIdx = i;
		}
	}

	if (startIdx == -1)
		return;

	beginRemoveRows(QModelIndex(), startIdx, endIdx);
	items.remove(startIdx, endIdx - startIdx + 1);
	endRemoveRows();
}

void SourceTreeModel::UpdateGroupState(bool update)
{
	bool nowHasGroups = false;
	for (auto &item : items) {
		if (obs_sceneitem_is_group(item)) {
			nowHasGroups = true;
			break;
		}
	}

	if (nowHasGroups != hasGroups) {
		hasGroups = nowHasGroups;
		if (update) {
			st->UpdateWidgets(true);
		}
	}
}

int SourceTreeModel::Count()
{
	return items.count();
}

QVector<OBSSceneItem> SourceTreeModel::GetItems()
{
	return items;
}

/* ========================================================================= */

SourceTree::SourceTree(QWidget *parent_, PLSDpiHelper dpiHelper) : QListView(parent_)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSSource, PLSCssIndex::VisibilityCheckBox});

	SourceTreeModel *stm_ = new SourceTreeModel(this);
	connect(stm_, &SourceTreeModel::itemRemoves, this, [=](QVector<OBSSceneItem> items) { emit itemsRemove(items); });
	connect(stm_, &SourceTreeModel::itemReorder, this, [=] { emit itemsReorder(); });

	setModel(stm_);

	scrollBar = new QSourceScrollBar(this);
	setVerticalScrollBar(scrollBar);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	setMouseTracking(true);

	noSourceTips = new QLabel(this);
	noSourceTips->setObjectName(NO_SOURCE_TEXT_LABEL);
	noSourceTips->hide();

	connect(App(), &PLSApp::StyleChanged, this, &SourceTree::UpdateIcons);
}

void SourceTree::UpdateIcons()
{
	SourceTreeModel *stm = GetStm();
	stm->SceneChanged();
}

int SourceTree::Count()
{
	return GetStm()->Count();
}

QVector<OBSSceneItem> SourceTree::GetItems()
{
	return GetStm()->GetItems();
}

void SourceTree::ResetDragOver()
{
	if (!preDragOver)
		return;

	int cnt = GetStm()->Count();
	for (int i = 0; i < cnt; ++i) {
		SourceTreeItem *item = GetItemWidget(i);
		if (item == preDragOver) {
			preDragOver->UpdateIndicator(SourceTreeItem::IndicatorNormal);
			break;
		}
	}

	preDragOver = NULL;
}

bool SourceTree::GetDestGroupItem(QPoint pos, obs_sceneitem_t *&item_output)
{
	OBSScene scene = GetCurrentScene();
	SourceTreeModel *stm = GetStm();
	auto &items = stm->items;
	QModelIndexList indices = selectedIndexes();

	DropIndicatorPosition indicator = dropIndicatorPosition();
	int row = indexAt(pos).row();
	bool emptyDrop = row == -1;

	if (emptyDrop) {
		if (!items.size()) {
			return false;
		}

		row = items.size() - 1;
		indicator = QAbstractItemView::BelowItem;
	}

	/* --------------------------------------- */
	/* store destination group if moving to a  */
	/* group                                   */

	obs_sceneitem_t *dropItem = items[row]; /* item being dropped on */
	bool itemIsGroup = obs_sceneitem_is_group(dropItem);

	obs_sceneitem_t *dropGroup = itemIsGroup ? dropItem : obs_sceneitem_get_group(scene, dropItem);

	/* not a group if moving above the group */
	if (indicator == QAbstractItemView::AboveItem && itemIsGroup) {
		dropGroup = nullptr;
	}
	if (emptyDrop)
		dropGroup = nullptr;

	/* --------------------------------------- */
	/* remember to remove list items if        */
	/* dropping on collapsed group             */

	bool dropOnCollapsed = false;
	if (dropGroup) {
		obs_data_t *data = obs_sceneitem_get_private_settings(dropGroup);
		dropOnCollapsed = obs_data_get_bool(data, "collapsed");
		obs_data_release(data);
	}

	if (indicator == QAbstractItemView::BelowItem || indicator == QAbstractItemView::OnItem || indicator == QAbstractItemView::OnViewport)
		row++;

	if (row < 0 || row > stm->items.count()) {
		return false;
	}

	/* --------------------------------------- */
	/* determine if any base group is selected */

	bool hasGroups = false;
	for (int i = 0; i < indices.size(); i++) {
		obs_sceneitem_t *item = items[indices[i].row()];
		if (obs_sceneitem_is_group(item)) {
			hasGroups = true;
			break;
		}
	}

	/* --------------------------------------- */
	/* if dropping a group, detect if it's     */
	/* below another group                     */

	obs_sceneitem_t *itemBelow;
	if (row == stm->items.count())
		itemBelow = nullptr;
	else
		itemBelow = stm->items[row];

	if (hasGroups) {
		if (!itemBelow || obs_sceneitem_get_group(scene, itemBelow) != dropGroup) {
			indicator = QAbstractItemView::BelowItem;
			dropGroup = nullptr;
			dropOnCollapsed = false;
		}
	}

	item_output = dropGroup;
	return true;
}

struct CheckChildHelper {
	obs_source_t *destChild;
	bool includeChild;
};

static bool enumChildInclude(obs_scene_t *, obs_sceneitem_t *item, void *ptr)
{
	CheckChildHelper *helper = (CheckChildHelper *)ptr;

	obs_source_t *source = obs_sceneitem_get_source(item);
	if (source) {
		if (source == helper->destChild) {
			helper->includeChild = true;
			return false;
		}

		if (obs_sceneitem_is_group(item)) {
			obs_sceneitem_group_enum_items(item, enumChildInclude, ptr);
		} else {
			const char *id = obs_source_get_id(source);
			if (id && 0 == strcmp(id, SCENE_SOURCE_ID)) {
				obs_scene_t *sceneTemp = obs_scene_from_source(source);
				obs_scene_enum_items(sceneTemp, enumChildInclude, ptr);
			}
		}
	}

	return true;
}

// return false : invalid drag
bool SourceTree::CheckDragSceneToGroup(obs_sceneitem_t *dragItem, obs_sceneitem_t *destGroupItem)
{
	assert(destGroupItem);
	obs_source_t *groupSource = obs_sceneitem_get_source(destGroupItem);
	if (!groupSource) {
		return true;
	}

	if (!dragItem) {
		return false;
	}

	obs_source_t *dragSource = obs_sceneitem_get_source(dragItem);
	if (!dragSource) {
		return false;
	}

	const char *id = obs_source_get_id(dragSource);
	if (!id) {
		return false;
	}

	if (0 != strcmp(id, SCENE_SOURCE_ID)) {
		return true;
	}

	CheckChildHelper helper;
	helper.destChild = groupSource;
	helper.includeChild = false;

	obs_scene_t *dragScene = obs_scene_from_source(dragSource);
	obs_scene_enum_items(dragScene, enumChildInclude, &helper);

	if (helper.includeChild) {
		return false;
	} else {
		return true;
	}
}

bool SourceTree::IsValidDrag(obs_sceneitem_t *destGroupItem, QVector<OBSSceneItem> items)
{
	std::map<obs_sceneitem_t *, std::pair<obs_sceneitem_t *, bool>> selectedSource;

	ChildAndParent childParents;
	childParents.parent = std::make_pair(nullptr, true);
	obs_scene_enum_items(GetCurrentScene(), enumItemForParent, &childParents);

	QModelIndexList indexs = selectedIndexes();
	for (int i = 0; i < indexs.size(); i++) {
		obs_sceneitem_t *item = items[indexs[i].row()];
		selectedSource.insert(std::make_pair(item, childParents.childAndParent[item]));
	}

	bool isDragValid = true;
	bool isDestGroup = obs_sceneitem_is_group(destGroupItem);
	bool isDestVisible = obs_sceneitem_visible(destGroupItem);

	auto iter = selectedSource.begin();
	while (iter != selectedSource.end()) {
		if (iter->second.first != destGroupItem) {
			if (iter->second.first != nullptr && obs_sceneitem_is_group(iter->second.first) && !iter->second.second) {
				isDragValid = false; // cann't drag out of invisible group
				break;
			}

			if (destGroupItem && isDestGroup) {
				if (!isDestVisible) {
					isDragValid = false; // cann't drag into invisible group
					break;
				}

				if (!CheckDragSceneToGroup(iter->first, destGroupItem)) {
					isDragValid = false; // cann't drag scene into group which is included in scene
					break;
				}
			}
		}
		iter++;
	}

	return isDragValid;
}

void SourceTree::ResetWidgets()
{
	OBSScene scene = GetCurrentScene();

	SourceTreeModel *stm = GetStm();
	stm->UpdateGroupState(false);

	for (int i = 0; i < stm->items.count(); i++) {
		QModelIndex index = stm->createIndex(i, 0, nullptr);
		SourceTreeItem *uiItem = new SourceTreeItem(this, stm->items[i]);
		connect(uiItem, &SourceTreeItem::SelectItemChanged, this, &SourceTree::OnSelectItemChanged, Qt::QueuedConnection);
		connect(uiItem, &SourceTreeItem::VisibleItemChanged, this, &SourceTree::OnVisibleItemChanged, Qt::QueuedConnection);
		setIndexWidget(index, uiItem);
	}
	PLSDpiHelper::dpiDynamicUpdate(this);
}

void SourceTree::UpdateWidget(const QModelIndex &idx, obs_sceneitem_t *item)
{
	SourceTreeItem *uiItem = new SourceTreeItem(this, item);
	connect(uiItem, &SourceTreeItem::SelectItemChanged, this, &SourceTree::OnSelectItemChanged, Qt::QueuedConnection);
	connect(uiItem, &SourceTreeItem::VisibleItemChanged, this, &SourceTree::OnVisibleItemChanged, Qt::QueuedConnection);
	setIndexWidget(idx, uiItem);
	PLSDpiHelper::dpiDynamicUpdate(this);
}

void SourceTree::UpdateWidgets(bool force)
{
	SourceTreeModel *stm = GetStm();

	for (int i = 0; i < stm->items.size(); i++) {
		obs_sceneitem_t *item = stm->items[i];
		SourceTreeItem *widget = GetItemWidget(i);

		if (!widget) {
			UpdateWidget(stm->createIndex(i, 0), item);
		} else {
			widget->Update(force);
		}
	}
	PLSDpiHelper::dpiDynamicUpdate(this);
}

void SourceTree::NotifyItemSelect(obs_sceneitem_t *sceneitem, bool select)
{
	int cnt = GetStm()->Count();
	for (int i = 0; i < cnt; ++i) {
		SourceTreeItem *item = GetItemWidget(i);
		if (item && item->SceneItem() == sceneitem) {
			item->OnSelectChanged(select);
			break;
		}
	}
}

void SourceTree::SelectItem(obs_sceneitem_t *sceneitem, bool select)
{
	SourceTreeModel *stm = GetStm();
	int i = 0;

	for (; i < stm->items.count(); i++) {
		if (stm->items[i] == sceneitem)
			break;
	}

	if (i == stm->items.count())
		return;

	NotifyItemSelect(sceneitem, select);

	QModelIndex index = stm->createIndex(i, 0);
	if (index.isValid())
		selectionModel()->select(index, select ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

Q_DECLARE_METATYPE(OBSSceneItem);

void SourceTree::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		QListView::mouseDoubleClickEvent(event);
}

bool TravelGroupChilds(obs_scene_t *, obs_sceneitem_t *item, void *val)
{
	return true;
}

void SourceTree::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->source() == this)
		ResetDragOver();

	QListView::dragEnterEvent(event);
}

void SourceTree::dragLeaveEvent(QDragLeaveEvent *event)
{
	ResetDragOver();
	QListView::dragLeaveEvent(event);
}

void SourceTree::dragMoveEvent(QDragMoveEvent *event)
{
	do {
		if (event->source() != this)
			break;

		auto &items = GetStm()->items;
		if (!items.size())
			break;

		obs_sceneitem_t *destGroupItem = nullptr;
		if (!GetDestGroupItem(event->pos(), destGroupItem)) {
			break;
		}

		if (!IsValidDrag(destGroupItem, items)) {
			ResetDragOver();
			QListView::dragMoveEvent(event);
			event->ignore();
			return;
		}

		DropIndicatorPosition indicator = dropIndicatorPosition();
		int row = indexAt(event->pos()).row();
		if (row == -1) {
			row = items.size() - 1;
			indicator = QAbstractItemView::BelowItem;
		} else {
			SourceTreeItem *item = GetItemWidget(row);
			int itemY = item->mapTo(this, QPoint(0, 0)).y();
			int itemH = item->size().height();
			int cursorY = event->pos().y();
			// Sometimes indicator can't match current position of cursor, which will cause UI refresh
			if (indicator == QAbstractItemView::AboveItem) {
				if (cursorY > itemY + itemH / 2)
					break;
			} else if (indicator == QAbstractItemView::BelowItem) {
				if (cursorY < itemY + itemH / 2)
					break;
			}
		}

		SourceTreeItem *item = GetItemWidget(row);
		if (item != preDragOver) {
			ResetDragOver();
			preDragOver = item;
		}

		switch (indicator) {
		case QAbstractItemView::AboveItem:
			preDragOver->UpdateIndicator(SourceTreeItem::IndicatorAbove);
			break;

		case QAbstractItemView::OnItem:
		case QAbstractItemView::BelowItem:
			preDragOver->UpdateIndicator(SourceTreeItem::IndicatorBelow);
			break;

		case QAbstractItemView::OnViewport:
		default:
			break;
		}
	} while (0);

	QListView::dragMoveEvent(event);
}

void SourceTree::dropEvent(QDropEvent *event)
{
	if (event->source() != this) {
		QListView::dropEvent(event);
		return;
	}

	PLS_UI_STEP(SOURCE_MODULE, "source tree", ACTION_DROP);

	ResetDragOver();

	OBSScene scene = GetCurrentScene();
	SourceTreeModel *stm = GetStm();
	auto &items = stm->items;
	QModelIndexList indices = selectedIndexes();

	DropIndicatorPosition indicator = dropIndicatorPosition();
	int row = indexAt(event->pos()).row();
	bool emptyDrop = row == -1;

	if (emptyDrop) {
		if (!items.size()) {
			QListView::dropEvent(event);
			return;
		}

		row = items.size() - 1;
		indicator = QAbstractItemView::BelowItem;
	}

	/* --------------------------------------- */
	/* store destination group if moving to a  */
	/* group                                   */

	obs_sceneitem_t *dropItem = items[row]; /* item being dropped on */
	bool itemIsGroup = obs_sceneitem_is_group(dropItem);

	obs_sceneitem_t *dropGroup = itemIsGroup ? dropItem : obs_sceneitem_get_group(scene, dropItem);

	std::vector<QString> srcChilds;
	std::vector<QString> destChilds;

	if (dropGroup && obs_sceneitem_is_group(dropGroup))
		obs_sceneitem_group_enum_items(dropGroup, TravelGroupChilds, (void *)&srcChilds);

	/* not a group if moving above the group */
	if (indicator == QAbstractItemView::AboveItem && itemIsGroup)
		dropGroup = nullptr;
	if (emptyDrop)
		dropGroup = nullptr;

	/* --------------------------------------- */
	/* remember to remove list items if        */
	/* dropping on collapsed group             */

	bool dropOnCollapsed = false;
	if (dropGroup) {
		obs_data_t *data = obs_sceneitem_get_private_settings(dropGroup);
		dropOnCollapsed = obs_data_get_bool(data, "collapsed");
		obs_data_release(data);
	}

	if (indicator == QAbstractItemView::BelowItem || indicator == QAbstractItemView::OnItem || indicator == QAbstractItemView::OnViewport)
		row++;

	if (row < 0 || row > stm->items.count()) {
		QListView::dropEvent(event);
		return;
	}

	/* --------------------------------------- */
	/* determine if any base group is selected */

	bool hasGroups = false;
	for (int i = 0; i < indices.size(); i++) {
		obs_sceneitem_t *item = items[indices[i].row()];
		if (obs_sceneitem_is_group(item)) {
			hasGroups = true;
			break;
		}
	}

	/* --------------------------------------- */
	/* if dropping a group, detect if it's     */
	/* below another group                     */

	obs_sceneitem_t *itemBelow;
	if (row == stm->items.count())
		itemBelow = nullptr;
	else
		itemBelow = stm->items[row];

	if (hasGroups) {
		if (!itemBelow || obs_sceneitem_get_group(scene, itemBelow) != dropGroup) {
			indicator = QAbstractItemView::BelowItem;
			dropGroup = nullptr;
			dropOnCollapsed = false;
		}
	}

	if (!IsValidDrag(dropGroup, items)) {
		blog(LOG_INFO, "There is an invalid drag in dropEvent");
		QListView::dropEvent(event);
		return;
	}

	/* --------------------------------------- */
	/* if dropping groups on other groups,     */
	/* disregard as invalid drag/drop          */

	if (dropGroup && hasGroups) {
		QListView::dropEvent(event);
		return;
	}

	/* --------------------------------------- */
	/* if selection includes base group items, */
	/* include all group sub-items and treat   */
	/* them all as one                         */

	if (hasGroups) {
		/* remove sub-items if selected */
		for (int i = indices.size() - 1; i >= 0; i--) {
			obs_sceneitem_t *item = items[indices[i].row()];
			obs_scene_t *itemScene = obs_sceneitem_get_scene(item);

			if (itemScene != scene) {
				indices.removeAt(i);
			}
		}

		/* add all sub-items of selected groups */
		for (int i = indices.size() - 1; i >= 0; i--) {
			obs_sceneitem_t *item = items[indices[i].row()];

			if (obs_sceneitem_is_group(item)) {
				for (int j = items.size() - 1; j >= 0; j--) {
					obs_sceneitem_t *subitem = items[j];
					obs_sceneitem_t *subitemGroup = obs_sceneitem_get_group(scene, subitem);

					if (subitemGroup == item) {
						QModelIndex idx = stm->createIndex(j, 0);
						indices.insert(i + 1, idx);
					}
				}
			}
		}
	}

	/* --------------------------------------- */
	/* build persistent indices                */

	QList<QPersistentModelIndex> persistentIndices;
	persistentIndices.reserve(indices.count());
	for (QModelIndex &index : indices)
		persistentIndices.append(index);
	std::sort(persistentIndices.begin(), persistentIndices.end());

	/* --------------------------------------- */
	/* move all items to destination index     */

	int r = row;
	for (auto &persistentIdx : persistentIndices) {
		int from = persistentIdx.row();
		int to = r;
		int itemTo = to;

		if (itemTo > from)
			itemTo--;

		if (itemTo != from) {
			stm->beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
			MoveItem(items, from, itemTo);
			stm->endMoveRows();
		}

		r = persistentIdx.row() + 1;
	}

	std::sort(persistentIndices.begin(), persistentIndices.end());
	int firstIdx = persistentIndices.front().row();
	int lastIdx = persistentIndices.back().row();

	/* --------------------------------------- */
	/* reorder scene items in back-end         */

	QVector<struct obs_sceneitem_order_info> orderList;
	obs_sceneitem_t *lastGroup = nullptr;
	int insertCollapsedIdx = 0;

	auto insertCollapsed = [&](obs_sceneitem_t *item) {
		struct obs_sceneitem_order_info info;
		info.group = lastGroup;
		info.item = item;

		orderList.insert(insertCollapsedIdx++, info);
	};

	using insertCollapsed_t = decltype(insertCollapsed);

	auto preInsertCollapsed = [](obs_scene_t *, obs_sceneitem_t *item, void *param) {
		(*reinterpret_cast<insertCollapsed_t *>(param))(item);
		return true;
	};

	auto insertLastGroup = [&]() {
		obs_data_t *data = obs_sceneitem_get_private_settings(lastGroup);
		bool collapsed = obs_data_get_bool(data, "collapsed");
		obs_data_release(data);

		if (collapsed) {
			insertCollapsedIdx = 0;
			obs_sceneitem_group_enum_items(lastGroup, preInsertCollapsed, &insertCollapsed);
		}

		struct obs_sceneitem_order_info info;
		info.group = nullptr;
		info.item = lastGroup;
		orderList.insert(0, info);
	};

	auto updateScene = [&]() {
		struct obs_sceneitem_order_info info;

		for (int i = 0; i < items.size(); i++) {
			obs_sceneitem_t *item = items[i];
			obs_sceneitem_t *group;

			if (obs_sceneitem_is_group(item)) {
				if (lastGroup) {
					insertLastGroup();
				}
				lastGroup = item;
				continue;
			}

			if (!hasGroups && i >= firstIdx && i <= lastIdx)
				group = dropGroup;
			else
				group = obs_sceneitem_get_group(scene, item);

			if (lastGroup && lastGroup != group) {
				insertLastGroup();
			}

			lastGroup = group;

			info.group = group;
			info.item = item;
			orderList.insert(0, info);
		}

		if (lastGroup) {
			insertLastGroup();
		}

		obs_scene_reorder_items2(scene, orderList.data(), orderList.size());
	};

	using updateScene_t = decltype(updateScene);

	auto preUpdateScene = [](void *data, obs_scene_t *) { (*reinterpret_cast<updateScene_t *>(data))(); };

	ignoreReorder = true;
	obs_scene_atomic_update(scene, preUpdateScene, &updateScene);
	ignoreReorder = false;

	/* --------------------------------------- */
	/* remove items if dropped in to collapsed */
	/* group                                   */

	if (dropOnCollapsed) {
		stm->beginRemoveRows(QModelIndex(), firstIdx, lastIdx);
		items.remove(firstIdx, lastIdx - firstIdx + 1);
		stm->endRemoveRows();
	}

	/* --------------------------------------- */
	/* update widgets and accept event         */

	UpdateWidgets(true);

	event->accept();
	event->setDropAction(Qt::CopyAction);

	emit itemsReorder();
	QListView::dropEvent(event);
}

void SourceTree::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();
	SourceTreeItem *item = qobject_cast<SourceTreeItem *>(childAt(pos));

	PLSBasicPreview *preview = PLSBasicPreview::Get();

	QListView::mouseMoveEvent(event);

	std::lock_guard<std::mutex> lock(preview->selectMutex);
	preview->hoveredPreviewItems.clear();
	if (item)
		preview->hoveredPreviewItems.push_back(item->sceneitem);
}

void SourceTree::leaveEvent(QEvent *event)
{
	PLSBasicPreview *preview = PLSBasicPreview::Get();

	QListView::leaveEvent(event);

	std::lock_guard<std::mutex> lock(preview->selectMutex);
	preview->hoveredPreviewItems.clear();
}

void SourceTree::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	{
		SignalBlocker sourcesSignalBlocker(this);
		SourceTreeModel *stm = GetStm();

		QModelIndexList selectedIdxs = selected.indexes();
		QModelIndexList deselectedIdxs = deselected.indexes();

		for (int i = 0; i < selectedIdxs.count(); i++) {
			int idx = selectedIdxs[i].row();
			OBSSceneItem item = stm->items[idx];
			obs_sceneitem_select(item, true);
			NotifyItemSelect(item, true);
		}

		for (int i = 0; i < deselectedIdxs.count(); i++) {
			int idx = deselectedIdxs[i].row();
			OBSSceneItem item = stm->items[idx];
			obs_sceneitem_select(item, false);
			NotifyItemSelect(item, false);
		}
	}
	QListView::selectionChanged(selected, deselected);
}

void SourceTree::Edit(int row)
{
	SourceTreeModel *stm = GetStm();
	if (row < 0 || row >= stm->items.count())
		return;

	QModelIndex index = stm->createIndex(row, 0);
	QWidget *widget = indexWidget(index);
	SourceTreeItem *itemWidget = reinterpret_cast<SourceTreeItem *>(widget);
	if (itemWidget->IsEditing())
		return;

	itemWidget->EnterEditMode();
	edit(index);
}

void SourceTree::OnSourceStateChanged(unsigned long long srcPtr)
{
	int cnt = GetStm()->Count();
	for (int i = 0; i < cnt; ++i) {
		SourceTreeItem *item = GetItemWidget(i);
		if (!item)
			continue;

		OBSSceneItem sceneItem = item->SceneItem();
		if (!sceneItem)
			continue;

		obs_source_t *source = obs_sceneitem_get_source(sceneItem);
		if ((unsigned long long)source == srcPtr) {
			item->UpdateIcon();
			// Even we success to find item, we should not return or break.
			// Because maybe more than one scene item are holding same obs_source
			obs_source_t *source = obs_sceneitem_get_source(sceneItem);
			if (!source)
				continue;

			const char *id = obs_source_get_id(source);
			if (!id)
				continue;

			enum obs_source_error error;
			if (0 == strcmp(id, DSHOW_SOURCE_ID)) {
				emit DshowSourceStatusChanged(obs_source_get_name(source), sceneItem, obs_source_get_capture_valid(source, &error));
			}
		}
	}
}

void SourceTree::OnBeautySourceStatusChanged(const QString &sourceName, bool status)
{
	emit beautyStatusChanged(sourceName, status);
}

void SourceTree::OnSourceItemRemove(unsigned long long sceneItemPtr)
{
	GetStm()->Remove((void *)sceneItemPtr);
}

void SourceTree::OnSelectItemChanged(OBSSceneItem item, bool selected)
{
	emit SelectItemChanged(item, selected);
}

void SourceTree::OnVisibleItemChanged(OBSSceneItem item, bool visible)
{
	emit VisibleItemChanged(item, visible);
}

bool SourceTree::MultipleBaseSelected() const
{
	SourceTreeModel *stm = GetStm();
	QModelIndexList selectedIndices = selectedIndexes();

	OBSScene scene = GetCurrentScene();

	if (selectedIndices.size() < 1) {
		return false;
	}

	for (auto &idx : selectedIndices) {
		obs_sceneitem_t *item = stm->items[idx.row()];
		if (obs_sceneitem_is_group(item)) {
			return false;
		}

		obs_scene *itemScene = obs_sceneitem_get_scene(item);
		if (itemScene != scene) {
			return false;
		}
	}

	return true;
}

bool SourceTree::GroupsSelected() const
{
	SourceTreeModel *stm = GetStm();
	QModelIndexList selectedIndices = selectedIndexes();

	OBSScene scene = GetCurrentScene();

	if (selectedIndices.size() < 1) {
		return false;
	}

	for (auto &idx : selectedIndices) {
		obs_sceneitem_t *item = stm->items[idx.row()];
		if (!obs_sceneitem_is_group(item)) {
			return false;
		}
	}

	return true;
}

bool SourceTree::GroupedItemsSelected() const
{
	SourceTreeModel *stm = GetStm();
	QModelIndexList selectedIndices = selectedIndexes();
	OBSScene scene = GetCurrentScene();

	if (!selectedIndices.size()) {
		return false;
	}

	for (auto &idx : selectedIndices) {
		obs_sceneitem_t *item = stm->items[idx.row()];
		obs_scene *itemScene = obs_sceneitem_get_scene(item);

		if (itemScene != scene) {
			return true;
		}
	}

	return false;
}

void SourceTree::Remove(OBSSceneItem item)
{
	PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
	GetStm()->Remove(item);
	main->SaveProject();
	if (!main->SavingDisabled()) {
		obs_scene_t *scene = obs_sceneitem_get_scene(item);
		obs_source_t *sceneSource = obs_scene_get_source(scene);
		obs_source_t *itemSource = obs_sceneitem_get_source(item);
		main->DeletePropertiesWindow(itemSource);
		main->DeleteFiltersWindow(itemSource);
		blog(LOG_INFO, "User Removed source '%s' (%s) from scene '%s'", obs_source_get_name(itemSource), obs_source_get_id(itemSource), obs_source_get_name(sceneSource));
	}
}

void SourceTree::GroupSelectedItems()
{
	QModelIndexList indices = selectedIndexes();
	std::sort(indices.begin(), indices.end());
	GetStm()->GroupSelectedItems(indices);
}

void SourceTree::UngroupSelectedGroups()
{
	QModelIndexList indices = selectedIndexes();
	GetStm()->UngroupSelectedGroups(indices);
}

void SourceTree::AddGroup()
{
	GetStm()->AddGroup();
}

#define NO_SOURCE_TIPS_MARGIN 45

void SourceTree::paintEvent(QPaintEvent *event)
{
	SourceTreeModel *stm = GetStm();
	if (stm && !stm->items.count()) {
		QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
		option.setWrapMode(QTextOption::WordWrap);

		double dpi = PLSDpiHelper::getDpi(this);
		QWidget *dockTitleWidget = PLSBasic::Get()->ui->sourcesDock->titleBarWidget();
		int dockCaptionHeight = dockTitleWidget ? dockTitleWidget->size().height() : 0;
		int height = size().height() - dockCaptionHeight;
		int left = PLSDpiHelper::calculate(dpi, NO_SOURCE_TIPS_MARGIN);
		int width = size().width() - 2 * PLSDpiHelper::calculate(dpi, NO_SOURCE_TIPS_MARGIN);

		QPainter p(viewport());
		p.setPen(QColor(102, 102, 102));
		p.setFont(noSourceTips->font());
		p.drawText(QRect(left, 0, width, height), QTStr("NoSources.Label"), option);
	} else {
		QListView::paintEvent(event);
	}
}
