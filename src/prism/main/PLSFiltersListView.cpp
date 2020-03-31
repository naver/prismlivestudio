#include "PLSFiltersListView.h"
#include "ui_PLSFiltersListView.h"

#include "pls-common-define.hpp"
#include "qt-wrappers.hpp"
#include "pls-app.hpp"

#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QPen>

#include "PLSDpiHelper.h"

Q_DECLARE_METATYPE(OBSSource);

PLSFiltersListView::PLSFiltersListView(QWidget *parent) : FocusList(parent), ui(new Ui::PLSFiltersListView)
{
	ui->setupUi(this);
	this->setAcceptDrops(true);
}

PLSFiltersListView::~PLSFiltersListView()
{
	delete ui;
}

bool PLSFiltersListView::QueryRemove(QWidget *parent, obs_source_t *source)
{
	const char *name = obs_source_get_name(source);

	QString text = QTStr("ConfirmRemove.Text.title");
	if (0 == strcmp(App()->GetLocale(), "ko-KR")) {
		return PLSAlertView::Button::Ok == PLSMessageBox::question(parent, QTStr("ConfirmRemove.Title"), name, text, PLSAlertView::Button::Ok | PLSAlertView::Button::Cancel);

	} else {
		return PLSAlertView::Button::Ok == PLSMessageBox::question(parent, QTStr("ConfirmRemove.Title"), text, name, PLSAlertView::Button::Ok | PLSAlertView::Button::Cancel);
	}
}

void PLSFiltersListView::AddFilterItemView(OBSSource filter, bool reorder)
{
	QListWidgetItem *item = new QListWidgetItem();
	this->addItem(item);

	PLSFiltersItemView *filterView = new PLSFiltersItemView(filter);
	connect(filterView, &PLSFiltersItemView::FilterRemoveTriggered, this, &PLSFiltersListView::OnFilterRemoveTriggered);
	connect(filterView, &PLSFiltersItemView::FilterRenameTriggered, this, &PLSFiltersListView::OnFilterRenameTriggered);
	connect(filterView, &PLSFiltersItemView::FinishingEditName, this, &PLSFiltersListView::OnFinishingEditName);
	connect(filterView, &PLSFiltersItemView::CurrentItemChanged, this, &PLSFiltersListView::OnCurrentItemChanged);
	PLSDpiHelper::dpiDynamicUpdate(filterView);

	this->setItemWidget(item, filterView);
	item->setData(Qt::UserRole, QVariant::fromValue(filter));

	if (reorder) {
		emit RowChanged(this->count() - 1, 0);
	}
	show();
	PLSDpiHelper::dpiDynamicUpdate(filterView);
}

void PLSFiltersListView::RemoveFilterItemView(OBSSource filter)
{
	for (int i = 0; i < this->count(); i++) {
		QListWidgetItem *item = this->item(i);
		QVariant v = item->data(Qt::UserRole);
		OBSSource curFilter = v.value<OBSSource>();

		if (filter == curFilter) {
			QListWidgetItem *itemView = this->takeItem(i);
			PLSFiltersItemView *filterView = static_cast<PLSFiltersItemView *>(this->itemWidget(item));
			if (filterView) {
				delete filterView;
				filterView = nullptr;
			}

			if (itemView) {
				delete itemView;
				itemView = nullptr;
			}
			break;
		}
	}
}

void PLSFiltersListView::RenameCurrentFilter()
{
	for (int i = 0; i < this->count(); i++) {
		QListWidgetItem *itemView = this->item(i);
		if (!itemView) {
			continue;
		}
		PLSFiltersItemView *filter = static_cast<PLSFiltersItemView *>(this->itemWidget(itemView));
		if (filter && filter->GetCurrentState()) {
			filter->OnRenameActionTriggered();
			break;
		}
	}
}

int PLSFiltersListView::GetCurrentRow()
{
	for (int i = 0; i < this->count(); i++) {
		QListWidgetItem *itemView = this->item(i);
		if (!itemView) {
			continue;
		}
		PLSFiltersItemView *filter = static_cast<PLSFiltersItemView *>(this->itemWidget(itemView));
		if (filter && filter->GetCurrentState()) {
			return i;
		}
	}
	return -1;
}

OBSSource PLSFiltersListView::GetFilter(int row)
{
	if (row == -1)
		return OBSSource();

	QListWidgetItem *item = this->item(row);
	if (!item)
		return OBSSource();

	QVariant v = item->data(Qt::UserRole);
	return v.value<OBSSource>();
}

OBSSource PLSFiltersListView::GetStartDragFilter()
{
	return startDragSource;
}

void PLSFiltersListView::SetCurrentItem(PLSFiltersItemView *item)
{
	if (!item)
		return;
	for (int i = 0; i < this->count(); i++) {
		QListWidgetItem *itemView = this->item(i);
		if (!itemView) {
			continue;
		}
		PLSFiltersItemView *filter = static_cast<PLSFiltersItemView *>(this->itemWidget(itemView));
		if (filter) {
			if (filter == item) {
				filter->SetCurrentItemState(true);
				emit CurrentItemIndexChanged(i);
			} else {
				filter->SetCurrentItemState(false);
			}
		}
	}
}

void PLSFiltersListView::SetCurrentItem(const int &row)
{
	QListWidgetItem *itemView = this->item(row);
	if (!itemView) {
		return;
	}

	this->SetCurrentItem(static_cast<PLSFiltersItemView *>(this->itemWidget(itemView)));
}

void PLSFiltersListView::SetSource(OBSSource source)
{
	this->source = source;
}

void PLSFiltersListView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		startDragPoint = event->pos();
		startDragItem = this->itemAt(event->pos());
	}

	FocusList::mousePressEvent(event);
}

void PLSFiltersListView::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		QPoint distance = event->pos() - startDragPoint;
		if (distance.manhattanLength() > QApplication::startDragDistance()) {
			QDrag *drag = new QDrag(this);
			QMimeData *mimeData = new QMimeData();
			startDragIndex = this->row(startDragItem);

			mimeData->setData(FILTER_DRAG_MIME_TYPE, QByteArray(QString::number(startDragIndex).toStdString().c_str()));
			//QPixmap pixmap = grab();
			//drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
			//drag->setPixmap(pixmap);

			drag->setMimeData(mimeData);
			drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
		}
	}
	FocusList::mouseMoveEvent(event);
}

void PLSFiltersListView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(FILTER_DRAG_MIME_TYPE)) {
		event->setDropAction(Qt::MoveAction);
		event->accept();
		isDraging = true;
	} else {
		event->ignore();
	}
}

void PLSFiltersListView::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat(FILTER_DRAG_MIME_TYPE)) {

		QRect currentRect{};
		QListWidgetItem *itemRow = itemAt(event->pos());
		int rowCount = row(itemRow);
		if (rowCount < 0) {
			currentRect = visualRect(this->indexFromItem(this->item(this->count() - 1)));
		} else {
			currentRect = visualRect(this->indexAt(event->pos()));
		}

		QPoint topleft = currentRect.topLeft();
		if (event->pos().y() - topleft.y() > FILTERS_ITEM_VIEW_FIXED_HEIGHT / 2) {
			SetPaintLinePos(currentRect.bottomLeft().x(), currentRect.bottomLeft().y(), currentRect.bottomRight().x(), currentRect.bottomRight().y());
		} else {
			SetPaintLinePos(currentRect.topLeft().x(), currentRect.topLeft().y(), currentRect.topRight().x(), currentRect.topRight().y());
		}

		this->viewport()->update();
		event->setDropAction(Qt::MoveAction);
		event->accept();
	} else {
		event->ignore();
	}
}

void PLSFiltersListView::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasFormat(FILTER_DRAG_MIME_TYPE)) {
		event->setDropAction(Qt::MoveAction);
		event->accept();
		isDraging = false;
		auto currentItem = this->itemAt(event->pos());
		int currentIndex = this->row(currentItem);
		if (-1 == currentIndex || currentIndex >= this->count()) {
			currentIndex = this->count() - 1;
		}

		startDragSource = this->GetFilter(startDragIndex);
		emit RowChanged(startDragIndex, currentIndex);
	} else {
		event->ignore();
	}
}

void PLSFiltersListView::dragLeaveEvent(QDragLeaveEvent *event)
{
	isDraging = false;
	update();
}

void PLSFiltersListView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this->viewport());
	double dpi = PLSDpiHelper::getDpi(this);
	if (isDraging) {
		painter.setPen(QPen(QColor(SCENE_SCROLLCONTENT_LINE_COLOR), PLSDpiHelper::calculate(dpi, 1)));
	} else {
		painter.setPen(QPen(QColor(SCENE_SCROLLCONTENT_DEFAULT_COLOR), PLSDpiHelper::calculate(dpi, 1)));
	}

	QLine l;
	l.setP1(lineStart);
	l.setP2(lineEnd);
	painter.drawLine(l);

	FocusList::paintEvent(event);
}

void PLSFiltersListView::OnFilterRenameTriggered(PLSFiltersItemView *item)
{
	this->SetCurrentItem(item);
}

void PLSFiltersListView::OnFilterRemoveTriggered(PLSFiltersItemView *item)
{
	this->SetCurrentItem(item);

	OBSSource filter = GetFilter(GetCurrentRow());

	if (filter) {
		if (QueryRemove(this, filter))
			obs_source_filter_remove(source, filter);
	}
}

void PLSFiltersListView::OnFinishingEditName(const QString &text, PLSFiltersItemView *item)
{
	this->SetCurrentItem(item);

	OBSSource filter = item->GetFilter();
	if (!filter) {
		return;
	}

	std::string name = QT_TO_UTF8(text.simplified());

	const char *prevName = obs_source_get_name(filter);
	bool sameName = (name == prevName);
	obs_source_t *foundFilter = nullptr;

	if (!sameName)
		foundFilter = obs_source_get_filter_by_name(source, name.c_str());

	if (foundFilter || name.empty() || sameName) {
		item->SetText(QT_UTF8(prevName));

		if (foundFilter) {
			PLSMessageBox::information(window(), QTStr("NameExists.Title"), QTStr("NameExists.Text"));
			obs_source_release(foundFilter);

		} else if (name.empty()) {
			PLSMessageBox::information(window(), QTStr("NoNameEntered.Title"), QTStr("NoNameEntered.Text"));
		}
	} else {
		const char *sourceName = obs_source_get_name(source);

		blog(LOG_INFO, "User renamed filter '%s' on source '%s' to '%s'", prevName, sourceName, name.c_str());

		item->SetText(QT_UTF8(name.c_str()));
		obs_source_set_name(filter, name.c_str());
	}
}

void PLSFiltersListView::OnCurrentItemChanged(PLSFiltersItemView *item)
{
	SetCurrentItem(item);
}

void PLSFiltersListView::SetPaintLinePos(const int &startPosX, const int &startPosY, const int &endPosX, const int &endPosY)
{
	lineStart.setX(startPosX);
	lineStart.setY(startPosY);
	lineEnd.setX(endPosX);
	lineEnd.setY(endPosY);
}
