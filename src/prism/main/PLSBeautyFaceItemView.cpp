#include "PLSBeautyFaceItemView.h"
#include "ui_PLSBeautyFaceItemView.h"
#include "frontend-api.h"
#include "pls-common-define.hpp"
#include "qt-wrappers.hpp"
#include <QPainter>
#include <QDir>
#include "PLSBeautyDefine.h"
#include "log/module_names.h"
#include "liblog.h"
#include "PLSBeautyFilterView.h"

const int maxIdInputLength = 20;
const char *filterIndexProperty = "filterIndex";

PLSBeautyFaceItemView::PLSBeautyFaceItemView(const QString &id_, int filterType_, bool isCustom_, QString baseName_, QWidget *parent_)
	: QFrame(parent_), id(id_), filterType(filterType_), isCustom(isCustom_), baseName(baseName_), ui(new Ui::PLSBeautyFaceItemView)
{
	ui->setupUi(this);
	this->InitUi();
}

void PLSBeautyFaceItemView::InitUi()
{
	ui->modifyBtn->installEventFilter(this);
	ui->nameLineEdit->installEventFilter(this);
	ui->filterId->installEventFilter(this);
	ui->faceIconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
	ui->modifyBtn->hide();
	ui->nameLineEdit->hide();
	ui->filterId->setTextFormat(Qt::PlainText);
	ui->nameLineEdit->setMaxLength(maxFilerIdLength);
	if (isCustom && nullptr == customFlagIcon) {
		customFlagIcon = new QLabel(this);
		customFlagIcon->setObjectName("customFlagIcon");
		customFlagIcon->hide();
		pls_flush_style(customFlagIcon);
	}
	ui->faceIconLabel->setProperty("baseName", baseName);
	ui->faceIconLabel->setProperty("isCustom", isCustom);
	pls_flush_style(ui->faceIconLabel);
	SetFilterId(id);
	SetChecked(false);
	SetFilterIconPixmap();
}

QString PLSBeautyFaceItemView::GetNameElideString(int maxWith)
{
	QFontMetrics fontWidth(ui->filterId->font());
	if (fontWidth.width(id) > maxWith)
		return fontWidth.elidedText(id, Qt::ElideRight, maxWith);
	return id;
}

void PLSBeautyFaceItemView::SetFilterIconPixmap()
{
	ui->faceIconLabel->SetPixmap(PLSBeautyFilterView::getIconFileByIndex(filterType, filterIndex));
}

PLSBeautyFaceItemView::~PLSBeautyFaceItemView()
{
	delete ui;
}

QString PLSBeautyFaceItemView::GetFilterId() const
{
	return this->id;
}

void PLSBeautyFaceItemView::SetFilterId(const QString &filterId)
{
	this->id = filterId;
	int spaceWidth = width() - (ui->modifyBtn->isVisible() ? ui->modifyBtn->width() : 0) - ui->horizontalLayout->spacing();
	ui->filterId->setText(GetNameElideString(spaceWidth));
	ui->filterId->update();
	ui->filterId->setToolTip(id);
}

void PLSBeautyFaceItemView::SetChecked(bool isChecked_)
{
	this->isChecked = isChecked_;
	ui->filterId->setProperty(STATUS_CLICKED, isChecked_);
	ui->faceIconLabel->setProperty(STATUS_CLICKED, isChecked_);
	pls_flush_style(ui->filterId);
	pls_flush_style(ui->faceIconLabel);
}

bool PLSBeautyFaceItemView::IsChecked() const
{
	return this->isChecked;
}

void PLSBeautyFaceItemView::SetEnabled(bool enable_)
{
	QWidget::setEnabled(enable_);
}

bool PLSBeautyFaceItemView::IsEnabled() const
{
	return QWidget::isEnabled();
}

bool PLSBeautyFaceItemView::IsCustom() const
{
	return this->isCustom;
}

void PLSBeautyFaceItemView::SetCustom(bool isCustom_)
{
	isCustom = isCustom_;
}

void PLSBeautyFaceItemView::SetBaseId(const QString &baseId)
{
	baseName = baseId;
}

QString PLSBeautyFaceItemView::GetBaseId() const
{
	return baseName;
}

void PLSBeautyFaceItemView::SetFilterIndex(int index)
{
	this->filterIndex = index;
	SetFilterIconPixmap();
}

void PLSBeautyFaceItemView::SetFilterType(int type)
{
	this->filterType = type;
	SetFilterIconPixmap();
}

void PLSBeautyFaceItemView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && !isChecked) {
		emit FaceItemClicked(this);
	}
	QFrame::mousePressEvent(event);
}

void PLSBeautyFaceItemView::leaveEvent(QEvent *event)
{
	isEntered = false;
	ui->modifyBtn->hide();
	QFrame::leaveEvent(event);
}

void PLSBeautyFaceItemView::enterEvent(QEvent *event)
{
	isEntered = true;
	if (isCustom && QFrame::isEnabled() && !isEditting) {
		ui->modifyBtn->show();
	}
	QFrame::enterEvent(event);
}

void PLSBeautyFaceItemView::resizeEvent(QResizeEvent *event)
{
	if (nullptr != customFlagIcon)
		customFlagIcon->move(ui->iconFace->width() - customFlagIcon->width() - 4, ui->iconFace->height() - customFlagIcon->height() - 4);
	SetFilterId(id);
}

bool PLSBeautyFaceItemView::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == ui->modifyBtn) {
		if (event->type() == QEvent::Show || event->type() == QEvent::Hide) {
			SetFilterId(id);
			return true;
		}
	}

	if (watched == ui->nameLineEdit && LineEditCanceled(event)) {
		isEditting = false;
		EditFinishOperation(true);
		return true;
	}

	if (watched == ui->nameLineEdit && LineEditChanged(event) && isEditting) {
		isEditting = false;
		EditFinishOperation(false);
		return true;
	}

	if (watched == ui->filterId && event->type() == QEvent::Resize) {
		QMetaObject::invokeMethod(
			this, [=]() { ui->filterId->setText(GetNameElideString(ui->filterId->width())); }, Qt::QueuedConnection);
		return true;
	}

	return QFrame::eventFilter(watched, event);
}

void PLSBeautyFaceItemView::on_modifyBtn_clicked()
{
	isEditting = true;
	ui->filterId->hide();
	ui->modifyBtn->hide();
	ui->nameLineEdit->show();
	ui->nameLineEdit->setText(id);
	ui->nameLineEdit->setFocus();
	ui->nameLineEdit->selectAll();
}

void PLSBeautyFaceItemView::EditFinishOperation(bool cancel)
{
	QString oldId = id;
	if (oldId != ui->nameLineEdit->text() && !cancel) {
		emit FaceItemIdEdited(ui->nameLineEdit->text().simplified(), this);
	}
	ui->nameLineEdit->hide();
	ui->filterId->show();
	ui->modifyBtn->setVisible(isEntered);
}

FilterItemIcon::FilterItemIcon(QWidget *parent /*= nullptr*/) : QPushButton(parent) {}

FilterItemIcon::~FilterItemIcon() {}

void FilterItemIcon::SetPixmap(const QPixmap &pixmap)
{
	if (pixmap.isNull()) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "Load pixmap failed");
	}
	this->iconPixmap = pixmap;
	this->update();
}

void FilterItemIcon::SetPixmap(const QString &pixpath)
{
	if (!this->iconPixmap.load(pixpath)) {
		PLS_ERROR(MAIN_BEAUTY_MODULE, "Load pixmap [%s] failed", pixpath.toStdString().c_str());
	}
	this->update();
}

const QColor FilterItemIcon::DisableLayerColor() const
{
	return layerColor;
}

void FilterItemIcon::SetDisableLayerColor(const QColor &color)
{
	this->layerColor = color;
	this->update();
}

bool FilterItemIcon::isChecked()
{
	return this->property(STATUS_CLICKED).toBool();
}

void FilterItemIcon::paintEvent(QPaintEvent *event)
{
	do {
		if (iconPixmap.isNull())
			break;
		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing, true);
		painter.save();
		double dpi = PLSDpiHelper::getDpi(this);
		QPainterPath painterPath;
		painterPath.addRoundedRect(this->rect(), PLSDpiHelper::calculate(dpi, 3.0), PLSDpiHelper::calculate(dpi, 3.0));
		painter.setClipPath(painterPath);
		painter.drawPixmap(painterPath.boundingRect().toRect(), iconPixmap.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter.setPen(Qt::NoPen);
		painter.drawPath(painterPath);

		if (!isEnabled()) {
			painter.save();
			painter.setBrush(layerColor);
			painter.setPen(Qt::NoPen);
			painter.drawRect(painterPath.boundingRect().toRect());
			painter.restore();
			painter.restore();
			break;
		}
		painter.restore();
	} while (0);

	QPushButton::paintEvent(event);
}
