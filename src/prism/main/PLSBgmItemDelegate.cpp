#include "PLSBgmItemDelegate.h"
#include "PLSBgmDragView.h"
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QApplication>
#include <QDateTime>
#include <QMouseEvent>
#include <QToolTip>

float PLSBgmItemDelegate::dpi = 1.0f;
int PLSBgmItemDelegate::iconIndex = 1;
int PLSBgmItemDelegate::frameCount = 8;

QFont fontName;
QFont fontProducer;
const static int nameFontSize = 14;
const static int producerFontSize = 12;
const static QColor rowColorNormal = QColor("#1e1e1e");
const static QColor rowColorOver = QColor("#272727");
const static QColor nameClickColor = QColor("#effc35");
const static QColor producerClickColor = QColor("#7a7f34");
const static QColor nameColor = QColor("#bababa");
const static QColor producerColor = QColor("#666666");
const static QColor nameColorInvalid = QColor("#666666");
const static QColor producerColorInvalid = QColor("#666666");
const static QColor dropIndicatorColor = QColor("#effc35");

PLSBgmItemDelegate::PLSBgmItemDelegate(QAbstractItemView *view_, QFont font, QObject *parent)
	: QItemDelegate(parent),
	  svgRendererLoading(new QSvgRenderer(this)),
	  svgRendererDelBtn(new QSvgRenderer(this)),
	  svgRendererFlag(new QSvgRenderer(this)),
	  svgRendererDot(new QSvgRenderer(this)),
	  view(view_)
{
	iconIndex = 1;
	fontName = font;
	fontProducer = font;
	svgRendererFlag->load(QString(":/images/bgm/img-free.svg"));
	svgRendererDot->load(QString(":/images/bgm/img-music-dot.svg"));
}

PLSBgmItemDelegate::~PLSBgmItemDelegate() {}

void PLSBgmItemDelegate::setDpi(float dpi_)
{
	dpi = dpi_;
}

void PLSBgmItemDelegate::nextLoadFrame()
{
	iconIndex++;
	if (iconIndex > frameCount)
		iconIndex = 1;
}

void PLSBgmItemDelegate::totalFrame(int frameCount_)
{
	frameCount = frameCount_;
}

void PLSBgmItemDelegate::setCurrentFrame(int frameIndex)
{
	if (frameIndex > frameCount)
		return;
	iconIndex = frameCount;
}

QWidget *PLSBgmItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(parent)
	Q_UNUSED(option)
	Q_UNUSED(index)
	return nullptr;
}

void PLSBgmItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QItemDelegate::paint(painter, option, index);

	auto data = index.model()->data(index, CustomDataRole::DataRole).value<PLSBgmItemData>();
	bool needPaint = index.model()->data(index, CustomDataRole::NeedPaintRole).toBool();
	drawBackground(data, painter, option, index);
	if (needPaint) {
		drawName(data, painter, option, index);
		drawProducer(data, painter, option, index);
		drawStateIcon(data, painter, option, index);
		drawDropIndicator(data, painter, option, index);
	}
}

QSize PLSBgmItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	auto size = QItemDelegate::sizeHint(option, index);
	size.setHeight(qRound(dpi * 70.0f));
	return size;
}

bool PLSBgmItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	switch (event->type()) {
	case QEvent::MouseMove: {
		auto moveEvent = static_cast<QMouseEvent *>(event);
		do {
			QRect name(option.rect.left() + qRound(52 * dpi), option.rect.top() + qRound(15 * dpi), option.rect.width() - qRound(19 * dpi) - qRound(20 * dpi) - qRound(52 * dpi),
				   qRound(18 * dpi));
			if (name.contains(moveEvent->pos())) {
				auto data = model->data(index, CustomDataRole::DataRole).value<PLSBgmItemData>();
				QToolTip::showText(QCursor::pos(), data.title, view);
				break;
			}
			QRect rectProducer(option.rect.left() + qRound(52 * dpi), option.rect.top() + qRound(37 * dpi), option.rect.width() - qRound(19 * dpi) - qRound(20 * dpi) - qRound(52 * dpi),
					   qRound(18 * dpi));
			if (rectProducer.contains(moveEvent->pos())) {
				auto data = model->data(index, CustomDataRole::DataRole).value<PLSBgmItemData>();
				QToolTip::showText(QCursor::pos(), data.producer, view);
				break;
			}
			QToolTip::hideText();
		} while (false);

		QRect rectDelBtn(option.rect.right() - qRound(19 * dpi) - qRound(20 * dpi), option.rect.top() + (option.rect.height() - qRound(19 * dpi)) / 2, qRound(19 * dpi), qRound(19 * dpi));
		if (rectDelBtn.contains(moveEvent->pos())) {
			deleteBtnState = ButtonState::Hover;
			entered = true;
			UpdateIndex(index);
		} else {
			entered = false;
			deleteBtnState = ButtonState::Normal;
			UpdateIndex(index);
		}
		break;
	}
	case QEvent::MouseButtonPress: {
		auto mouseEvent = static_cast<QMouseEvent *>(event);
		QToolTip::hideText();
		QRect rectDelBtn(option.rect.right() - qRound(19 * dpi) - qRound(20 * dpi), option.rect.top() + (option.rect.height() - qRound(19 * dpi)) / 2, qRound(19 * dpi), qRound(19 * dpi));
		if (mouseEvent->button() == Qt::LeftButton && rectDelBtn.contains(mouseEvent->pos())) {
			deleteBtnState = ButtonState::Pressed;
			UpdateIndex(index);
		}
		break;
	}
	case QEvent::MouseButtonRelease: {
		auto mouseEvent = static_cast<QMouseEvent *>(event);
		QRect rectDelBtn(option.rect.right() - qRound(19 * dpi) - qRound(20 * dpi), option.rect.top() + (option.rect.height() - qRound(19 * dpi)) / 2, qRound(19 * dpi), qRound(19 * dpi));
		if (mouseEvent->button() == Qt::LeftButton) {
			deleteBtnState = entered ? ButtonState::Hover : ButtonState::Normal;
			UpdateIndex(index);
			if (rectDelBtn.contains(mouseEvent->pos())) {
				emit delBtnClicked(index);
			}
		}
		break;
	}
	default:
		break;
	}
	return QItemDelegate::editorEvent(event, model, option, index);
}

void PLSBgmItemDelegate::drawBackground(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();
	QStyleOptionViewItem view_option(option);

	RowStatus status = index.model()->data(index, CustomDataRole::RowStatusRole).value<RowStatus>();
	if (((view_option.state & QStyle::State_MouseOver) && DropIndicator::None == data.dropIndicator) || status == RowStatus::stateHover) {
		view_option.state = view_option.state ^ QStyle::State_MouseOver;
		painter->fillRect(view_option.rect, rowColorOver);
		drawDeleteIcon(data, painter, option, index);
		PLSBgmDragView *dView = dynamic_cast<PLSBgmDragView *>(view);
		if (dView) {
			dView->UpdataData(index.row(), QVariant::fromValue(RowStatus::stateNormal), CustomDataRole::RowStatusRole);
		}
	} else {
		painter->fillRect(view_option.rect, rowColorNormal);
	}

	painter->restore();
}

void PLSBgmItemDelegate::drawProducer(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	MediaStatus state = index.model()->data(index, CustomDataRole::MediaStatusRole).value<MediaStatus>();
	switch (state) {
	case MediaStatus::stateNormal: {
		painter->setPen(producerColor);
		fontProducer.setPixelSize(qRound(dpi * producerFontSize));
		fontProducer.setBold(false);
		break;
	}
	case MediaStatus::stateLoading:
	case MediaStatus::statePause:
	case MediaStatus::statePlaying: {
		painter->setPen(producerClickColor);
		fontProducer.setPixelSize(qRound(dpi * producerFontSize));
		fontProducer.setBold(true);
		break;
	}
	case MediaStatus::stateInvalid: {
		painter->setPen(producerColorInvalid);
		fontProducer.setPixelSize(qRound(dpi * producerFontSize));
		fontProducer.setBold(false);
		break;
	}
	default: {
		painter->setPen(producerColor);
		fontProducer.setPixelSize(qRound(dpi * producerFontSize));
		fontProducer.setBold(false);
		break;
	}
	}
	painter->setFont(fontProducer);
	QTextOption textOption;
	QString name = data.producer;
	textOption.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	QString durration = ConvertIntToTimeString(data.GetDuration(data.id));
	int originalWidth = option.rect.width();
	if (option.state & QStyle::State_MouseOver) {
		originalWidth = originalWidth - qRound(18 * dpi) - qRound(19 * dpi);
	}

	int maxTextSpacing = originalWidth - qRound(dpi * 52) - qRound(20 * dpi) - painter->fontMetrics().width(durration) - qRound(10 * dpi) - qRound(3 * dpi);
	int offset = painter->fontMetrics().width(name) - maxTextSpacing;
	if (offset >= 0) {
		name = painter->fontMetrics().elidedText(name, Qt::ElideRight, maxTextSpacing);
	}
	QRect rectText(option.rect.left() + qRound(52 * dpi), option.rect.top() + qRound(37 * dpi), painter->fontMetrics().width(name), qRound(18 * dpi));
	//draw producer
	painter->drawText(rectText, name, textOption);
	//draw dot
	if (svgRendererDot)
		svgRendererDot->render(painter, QRect(rectText.right() + qRound(5 * dpi), option.rect.top() + qRound(45 * dpi), qRound(3 * dpi), qRound(3 * dpi)));
	//draw duration
	rectText.setRect(rectText.right() + qRound(10 * dpi) + qRound(3 * dpi), option.rect.top() + qRound(37 * dpi), painter->fontMetrics().width(durration), qRound(18 * dpi));
	painter->drawText(rectText, durration, textOption);
	painter->restore();
}

void PLSBgmItemDelegate::drawName(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	MediaStatus state = index.model()->data(index, CustomDataRole::MediaStatusRole).value<MediaStatus>();
	switch (state) {
	case MediaStatus::stateNormal: {
		painter->setPen(nameColor);
		fontName.setPixelSize(qRound(dpi * nameFontSize));
		fontName.setBold(false);
		break;
	}
	case MediaStatus::stateLoading:
	case MediaStatus::statePause:
	case MediaStatus::statePlaying: {
		painter->setPen(nameClickColor);
		fontName.setPixelSize(qRound(dpi * nameFontSize));
		fontName.setBold(true);
		break;
	}
	case MediaStatus::stateInvalid: {
		painter->setPen(nameColorInvalid);
		fontName.setPixelSize(qRound(dpi * nameFontSize));
		fontName.setBold(false);
		break;
	}
	default: {
		painter->setPen(nameColor);
		fontName.setPixelSize(qRound(dpi * nameFontSize));
		fontName.setBold(false);
		break;
	}
	}
	painter->setFont(fontName);
	QTextOption textOption;
	QString name = data.title;
	textOption.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

	int originalWidth = option.rect.width();
	if (option.state & QStyle::State_MouseOver) {
		originalWidth = originalWidth - qRound(18 * dpi) - qRound(19 * dpi);
	}
	int maxTextSpacing = 0;
	data.isLocalFile ? maxTextSpacing = originalWidth - qRound(dpi * 52) - qRound(20 * dpi)
			 : maxTextSpacing = originalWidth - qRound(dpi * 52) - qRound(20 * dpi) - qRound(27 * dpi) - qRound(8 * dpi);
	int offset = painter->fontMetrics().width(name) - maxTextSpacing;
	if (offset >= 0) {
		name = painter->fontMetrics().elidedText(name, Qt::ElideRight, maxTextSpacing);
	}

	QRect nameRect(option.rect.left() + qRound(52 * dpi), option.rect.top() + qRound(15 * dpi), painter->fontMetrics().width(name), painter->fontMetrics().boundingRect(name).height());
	painter->drawText(nameRect, name, textOption);
	if (!data.isLocalFile && svgRendererFlag) {
		svgRendererFlag->render(painter, QRect(nameRect.right() + qRound(8 * dpi), option.rect.top() + qRound(18 * dpi), qRound(27 * dpi), qRound(14 * dpi)));
	}
	painter->restore();
}

void PLSBgmItemDelegate::drawDeleteIcon(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (!svgRendererDelBtn)
		return;
	painter->save();
	QString fileName;
	switch (deleteBtnState) {
	case ButtonState::Normal: {
		fileName.append(":/images/bgm/btn-playlist-delete-normal.svg");
		break;
	}
	case ButtonState::Hover: {
		fileName.append(":/images/bgm/btn-playlist-delete-over.svg");
		break;
	}
	case ButtonState::Pressed: {
		fileName.append(":/images/bgm/btn-playlist-delete-click.svg");
		break;
	}
	default:
		fileName.append(":/images/bgm/btn-playlist-delete-normal.svg");
		break;
	}
	svgRendererDelBtn->load(fileName);
	QRect buttonRect(option.rect.right() - qRound(19 * dpi) - qRound(20 * dpi), option.rect.top() + (option.rect.height() - qRound(19 * dpi)) / 2, qRound(19 * dpi), qRound(19 * dpi));
	svgRendererDelBtn->render(painter, buttonRect);
	painter->restore();
}

void PLSBgmItemDelegate::drawStateIcon(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	MediaStatus state = index.model()->data(index, CustomDataRole::MediaStatusRole).value<MediaStatus>();
	painter->save();
	QRect loadingRect;
	switch (state) {
	case MediaStatus::statePause:
	case MediaStatus::stateInvalid:
	case MediaStatus::stateNormal: {
		if (!svgRendererLoading) {
			painter->restore();
			return;
		}
		loadingRect.setRect(option.rect.left() + qRound(20 * dpi), option.rect.top() + (option.rect.height() - qRound(18 * dpi)) / 2, qRound(18 * dpi), qRound(18 * dpi));
		svgRendererLoading->load(QString(":/images/bgm/btn-template-musiclist-play.svg"));
		svgRendererLoading->render(painter, loadingRect);
		break;
	}
	case MediaStatus::stateLoading: {
		if (!svgRendererLoading) {
			painter->restore();
			return;
		}
		loadingRect.setRect(option.rect.left() + qRound(20 * dpi), option.rect.top() + (option.rect.height() - qRound(18 * dpi)) / 2, qRound(18 * dpi), qRound(18 * dpi));
		svgRendererLoading->load(QString::asprintf(":/images/loading-%d.svg", iconIndex));
		svgRendererLoading->render(painter, loadingRect);
		break;
	}
	case MediaStatus::statePlaying: {
		loadingRect.setRect(option.rect.left() + qRound(21 * dpi), option.rect.top() + (option.rect.height() - qRound(16 * dpi)) / 2, qRound(16 * dpi), qRound(16 * dpi));
		painter->drawPixmap(loadingRect, QPixmap(QString::asprintf(":/images/bgm/BGM_equalizer-%d.png", iconIndex)).scaled(loadingRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		break;
	}
	}
	painter->restore();
}

void PLSBgmItemDelegate::drawDropIndicator(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	DropIndicator state = index.model()->data(index, CustomDataRole::DropIndicatorRole).value<DropIndicator>();
	painter->save();
	switch (state) {
	case DropIndicator::None:
		break;
	case DropIndicator::Top: {
		QPen pen;
		pen.setWidth(qRound(1 * dpi));
		pen.setColor(dropIndicatorColor);
		painter->setPen(pen);
		painter->drawLine(option.rect.topLeft(), option.rect.topRight());
		break;
	}
	case DropIndicator::Bottom: {
		QPen pen;
		pen.setWidth(qRound(1 * dpi));
		pen.setColor(dropIndicatorColor);
		painter->setPen(pen);
		painter->drawLine(option.rect.bottomLeft().x(), option.rect.bottomLeft().y() - qRound(1 * dpi), option.rect.bottomRight().x(), option.rect.bottomRight().y() - qRound(1 * dpi));
		break;
	}
	default:
		break;
	}
	painter->restore();
}

void PLSBgmItemDelegate::UpdateIndex(const QModelIndex &index)
{
	if (view && index.isValid())
		view->update(index);
}

QString PLSBgmItemDelegate::ConvertIntToTimeString(int seconds) const
{
	QTime currentTime((seconds / 3600) % 60, (seconds / 60) % 60, seconds % 60, (seconds * 1000) % 1000);

	QString format = "mm:ss";
	if (seconds >= 3600) {
		format = "hh:mm:ss";
	}
	return currentTime.toString(format);
}
