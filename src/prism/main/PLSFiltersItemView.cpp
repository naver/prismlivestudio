#include "PLSFiltersItemView.h"
#include "ui_PLSFiltersItemView.h"

#include "qt-wrappers.hpp"
#include "pls-common-define.hpp"
#include "pls-app.hpp"
#include "liblog.h"
#include "action.h"
#include "log/module_names.h"
#include "PLSMenu.hpp"
#include "ChannelCommonFunctions.h"

#include <QStyle>
#include <QMenu>

PLSFiltersItemView::PLSFiltersItemView(obs_source_t *source_, QWidget *parent)
	: source(source_),
	  QFrame(parent),
	  ui(new Ui::PLSFiltersItemView),
	  enabledSignal(obs_source_get_signal_handler(source), "enable", OBSSourceEnabled, this),
	  renamedSignal(obs_source_get_signal_handler(source), "rename", OBSSourceRenamed, this)
{
	PLSDpiHelper dpiHelper;
	dpiHelper.setCss(this, {PLSCssIndex::VisibilityCheckBox});
	ui->setupUi(this);
	ui->visibleButton->hide();
	ui->advButton->hide();
	ui->nameLineEdit->hide();
	ui->nameLineEdit->installEventFilter(this);

	name = obs_source_get_name(source);
	ui->nameLabel->setText(GetNameElideString());
	ui->nameLabel->setToolTip(name);
	ui->nameLabel->installEventFilter(this);
	bool enabled = obs_source_enabled(source);
	ui->visibleButton->setChecked(enabled);
	UpdateNameStyle();

	connect(ui->visibleButton, &QPushButton::clicked, this, &PLSFiltersItemView::OnVisibilityButtonClicked);
	connect(ui->advButton, &QPushButton::clicked, this, &PLSFiltersItemView::OnAdvButtonClicked);
}

PLSFiltersItemView::~PLSFiltersItemView()
{
	delete ui;
}

void PLSFiltersItemView::SetText(const QString &text)
{
	name = text;
	ui->nameLabel->setText(GetNameElideString());
}

OBSSource PLSFiltersItemView::GetFilter()
{
	return source;
}

bool PLSFiltersItemView::GetCurrentState()
{
	return current;
}

void PLSFiltersItemView::SetCurrentItemState(bool state)
{
	if (this->current != state) {
		this->current = state;
		UpdateNameStyle();
		PLS_UI_STEP(MAINFILTER_MODULE, name.toStdString().c_str(), ACTION_LBUTTON_CLICK);
	}
}

void PLSFiltersItemView::mousePressEvent(QMouseEvent *event)
{
	PLS_UI_STEP(MAINFILTER_MODULE, QT_TO_UTF8(name), ACTION_CLICK);
	OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_PRESSED);

	emit CurrentItemChanged(this);
	QFrame::mousePressEvent(event);
}

void PLSFiltersItemView::mouseDoubleClickEvent(QMouseEvent *event)
{
	PLS_UI_STEP(MAINFILTER_MODULE, QT_TO_UTF8(name), ACTION_DBCLICK);
	emit CurrentItemChanged(this);
	QFrame::mouseDoubleClickEvent(event);
}

void PLSFiltersItemView::mouseReleaseEvent(QMouseEvent *event)
{
	OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_HOVER);

	QFrame::mouseReleaseEvent(event);
}

void PLSFiltersItemView::enterEvent(QEvent *event)
{
	if (isFinishEditing) {
		ui->visibleButton->show();
		ui->advButton->show();
		OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_HOVER);
	}

	QFrame::enterEvent(event);
}

void PLSFiltersItemView::leaveEvent(QEvent *event)
{
	OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_NORMAL);

	ui->visibleButton->hide();
	ui->advButton->hide();

	QFrame::leaveEvent(event);
}

bool PLSFiltersItemView::eventFilter(QObject *object, QEvent *event)
{
	if (ui->nameLineEdit == object && LineEditCanceled(event)) {
		OnFinishingEditName();
		return true;
	}
	if (ui->nameLineEdit == object && LineEditChanged(event) && !isFinishEditing) {
		isFinishEditing = true;
		OnFinishingEditName();
		return true;
	}

	if (object == ui->nameLabel && event->type() == QEvent::Resize) {
		QMetaObject::invokeMethod(
			this, [=]() { ui->nameLabel->setText(GetNameElideString()); }, Qt::QueuedConnection);
		return true;
	}
	return QFrame::eventFilter(object, event);
}

void PLSFiltersItemView::contextMenuEvent(QContextMenuEvent *event)
{
	CreatePopupMenu();
	QFrame::contextMenuEvent(event);
}

void PLSFiltersItemView::OBSSourceEnabled(void *param, calldata_t *data)
{
	PLSFiltersItemView *window = reinterpret_cast<PLSFiltersItemView *>(param);
	bool enabled = calldata_bool(data, "enabled");

	QMetaObject::invokeMethod(window, "SourceEnabled", Q_ARG(bool, enabled));
}

void PLSFiltersItemView::OBSSourceRenamed(void *param, calldata_t *data)
{
	PLSFiltersItemView *window = reinterpret_cast<PLSFiltersItemView *>(param);
	const char *name = calldata_string(data, "new_name");

	QMetaObject::invokeMethod(window, "SourceRenamed", Q_ARG(QString, QT_UTF8(name)));
}

QString PLSFiltersItemView::GetNameElideString()
{
	QFontMetrics fontWidth(ui->nameLabel->font());
	if (fontWidth.width(name) > ui->nameLabel->width())
		return fontWidth.elidedText(name, Qt::ElideRight, ui->nameLabel->width());

	return name;
}

void PLSFiltersItemView::OnMouseStatusChanged(const QString &status)
{
	SetProperty(this, PROPERTY_NAME_MOUSE_STATUS, status);
}

void PLSFiltersItemView::OnFinishingEditName()
{
	ui->nameLineEdit->hide();
	ui->nameLabel->show();
	ui->nameLabel->setText(GetNameElideString());
	emit FinishingEditName(ui->nameLineEdit->text(), this);
}

void PLSFiltersItemView::OnVisibilityButtonClicked(bool visible)
{
	QString log = name + " visibility button";
	PLS_UI_STEP(MAINFILTER_MODULE, QT_TO_UTF8(log), ACTION_CLICK);
	obs_source_set_enabled(source, visible);
}

void PLSFiltersItemView::CreatePopupMenu()
{
	QString log = name + " advance button";
	PLS_UI_STEP(MAINFILTER_MODULE, QT_TO_UTF8(log), ACTION_CLICK);

	PLSPopupMenu popup(this);
	popup.setObjectName(OBJECT_NAME_FILTER_ITEM_MENU);

	QAction *renameAction = new QAction(QTStr("Rename"));
	QAction *removeAction = new QAction(QTStr("Delete"));
	connect(renameAction, &QAction::triggered, this, &PLSFiltersItemView::OnRenameActionTriggered);
	connect(removeAction, &QAction::triggered, this, &PLSFiltersItemView::OnRemoveActionTriggered);

	popup.addAction(renameAction);
	popup.addAction(removeAction);

	popup.exec(QCursor::pos());
}

void PLSFiltersItemView::UpdateNameStyle()
{
	QString visibleStr = obs_source_enabled(source) ? STATUS_VISIBLE : STATUS_INVISIBLE;
	QString selectStr = current ? STATUS_SELECTED : STATUS_UNSELECTED;

	QString value = visibleStr + QString(".") + selectStr;
	SetProperty(ui->nameLabel, STATUS, value);
}

void PLSFiltersItemView::SetProperty(QWidget *widget, const char *property, const QVariant &value)
{
	if (widget && property) {
		widget->setProperty(property, value);
		pls_flush_style(widget);
	}
}

void PLSFiltersItemView::OnAdvButtonClicked()
{
	CreatePopupMenu();
}

void PLSFiltersItemView::OnRenameActionTriggered()
{
	isFinishEditing = false;

	QString log = name + " rename button";
	PLS_UI_STEP(MAINFILTER_MODULE, QT_TO_UTF8(log), ACTION_CLICK);

	ui->nameLineEdit->setContentsMargins(0, 0, 0, 0);
	ui->nameLineEdit->setAlignment(Qt::AlignLeft);
	ui->nameLineEdit->setText(name);
	ui->nameLineEdit->setFocus();
	ui->nameLineEdit->selectAll();
	ui->nameLineEdit->show();
	ui->nameLabel->hide();
	ui->visibleButton->hide();
	ui->advButton->hide();
	OnMouseStatusChanged(PROPERTY_VALUE_MOUSE_STATUS_NORMAL);
}

void PLSFiltersItemView::OnRemoveActionTriggered()
{
	QString log = name + " delete button";
	PLS_UI_STEP(MAINFILTER_MODULE, QT_TO_UTF8(log), ACTION_CLICK);
	emit FilterRemoveTriggered(this);
}

void PLSFiltersItemView::SourceEnabled(bool enabled)
{
	ui->visibleButton->setChecked(enabled);
	UpdateNameStyle();
}

void PLSFiltersItemView::SourceRenamed(QString name)
{
	this->name = name;
	ui->nameLabel->setText(GetNameElideString());
	ui->nameLabel->setToolTip(name);
}
