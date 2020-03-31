#include "PLSBgmDragView.h"
#include "PLSBgmItemView.h"
#include "liblog.h"
#include "log/module_names.h"
#include "action.h"
#include "PLSDpiHelper.h"
#include "PLSBgmDataManager.h"
#include "PLSBgmItemDelegate.h"

#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QPen>
#include <QListWidget>

static const QString BGM_DRAG_MIME_TYPE = "bgmItem";
static const int FIX_BGM_ITEM_HEIGHT = 70;
PLSBgmDragView::PLSBgmDragView(QWidget *parent) : QListView(parent)
{
	PLSBgmItemViewModel *stm = new PLSBgmItemViewModel(this);
	setModel(stm);
	setMouseTracking(true);
	this->setAcceptDrops(true);
	CreateItemDelegate();
}

PLSBgmDragView::~PLSBgmDragView() {}

void PLSBgmDragView::SetDpi(double dpi)
{
	PLSBgmItemDelegate::setDpi(dpi);
}

void PLSBgmDragView::SetMediaStatus(const int &index, const MediaStatus &status)
{
	PLSBgmItemViewModel *stm = GetStm();
	QModelIndex modelIndex = stm->createIndex(index, 0, nullptr);
	stm->setData(modelIndex, QVariant::fromValue(status), CustomDataRole::MediaStatusRole);
	update(modelIndex);
}

int PLSBgmDragView::GetCurrentRow()
{
	QVector<PLSBgmItemData> datas = GetStm()->GetData();
	for (auto i = 0; i < datas.size(); i++) {
		PLSBgmItemData data_ = datas[i];
		if (data_.isCurrent) {
			return i;
		}
	}
	return -1;
}

int PLSBgmDragView::GetRow(const PLSBgmItemData &data)
{
	QVector<PLSBgmItemData> datas = GetStm()->GetData();
	for (auto i = 0; i < datas.size(); i++) {
		PLSBgmItemData &data_ = datas[i];
		if (data_.GetUrl(data_.id) == data.GetUrl(data.id) && data_.id == data.id) {
			return i;
		}
	}
	return -1;
}

PLSBgmItemData PLSBgmDragView::GetData(const QModelIndex &idx)
{
	PLSBgmItemViewModel *stm = GetStm();
	return stm->Get(idx);
}

void PLSBgmDragView::UpdateWidget()
{
	CreateItemDelegate();
	PLSBgmItemViewModel *stm = GetStm();
	for (int i = 0; i < stm->datas.count(); i++) {
		QModelIndex index = stm->createIndex(i, 0, nullptr);
		UpdateWidget(index, stm->datas[i]);
	}
}

void PLSBgmDragView::UpdateWidget(const QVector<PLSBgmItemData> &datas)
{
	PLSBgmItemViewModel *stm = GetStm();
	stm->SetData(datas);

	UpdateWidget();
}

void PLSBgmDragView::InsertWidget(const BgmItemCacheType &datas)
{
	PLSBgmItemViewModel *stm = GetStm();
	stm->InsertData(datas);
}

void PLSBgmDragView::UpdateWidget(const QModelIndex &idx, const PLSBgmItemData &data)
{
	GetStm()->setData(idx, QVariant::fromValue(data), CustomDataRole::DataRole);
}

void PLSBgmDragView::SetCurrentRow(const PLSBgmItemData &data)
{
	QVector<PLSBgmItemData> datas = GetStm()->GetData();
	for (auto i = 0; i < datas.size(); i++) {
		PLSBgmItemData &data_ = datas[i];
		if (data_.GetUrl(data_.id) == data.GetUrl(data.id) && data_.id == data.id) {
			data_ = data;
			data_.isCurrent = true;
			if (this->isVisible()) {
				this->scrollTo(GetModelIndex(i));
			}
		} else {
			data_.isCurrent = false;
		}
		UpdataData(i, data_);
	}
}

void PLSBgmDragView::UpdataData(const int &index, const PLSBgmItemData &data)
{
	QModelIndex modelIndex = GetStm()->createIndex(index, 0, nullptr);
	GetStm()->setData(modelIndex, QVariant::fromValue(data), CustomDataRole::DataRole);
}

void PLSBgmDragView::UpdataData(const int &index, QVariant value, CustomDataRole role)
{
	QModelIndex modelIndex = GetStm()->createIndex(index, 0, nullptr);
	GetStm()->setData(modelIndex, value, role);
}

QVector<PLSBgmItemData> PLSBgmDragView::GetData() const
{
	return GetStm()->GetData();
}

QModelIndex PLSBgmDragView::GetModelIndex(const int &row)
{
	return GetStm()->index(row);
}

void PLSBgmDragView::Remove(const PLSBgmItemData &data)
{
	GetStm()->Remove(data.GetUrl(data.id), data.id);
	RemoveFile(data);
}

bool PLSBgmDragView::Existed(const QString &url)
{
	QVector<PLSBgmItemData> datas = GetStm()->GetData();
	for (auto i = 0; i < datas.size(); i++) {
		PLSBgmItemData &data_ = datas[i];
		if (data_.GetUrl(data_.id) == url) {
			return true;
		}
	}
	return false;
}

void PLSBgmDragView::Clear()
{
	QVector<PLSBgmItemData> datas = GetStm()->GetData();
	for (auto i = 0; i < datas.size(); i++) {
		PLSBgmItemData &data_ = datas[i];
		RemoveFile(data_);
	}
	GetStm()->Clear();
}

int PLSBgmDragView::Count()
{
	return GetStm()->Count();
}

int PLSBgmDragView::GetId(const QString url)
{
	int id = 0;
	while (true) {
		if (ExistedId(url, id)) {
			id++;
			continue;
		}
		break;
	}
	return id;
}

PLSBgmItemData PLSBgmDragView::Get(int idx)
{
	return GetStm()->Get(idx);
}

PLSBgmItemData PLSBgmDragView::Get(const QString &url, const int &id)
{
	QVector<PLSBgmItemData> datas = GetStm()->GetData();
	for (auto i = 0; i < datas.size(); i++) {
		PLSBgmItemData data_ = datas[i];
		if (data_.GetUrl(data_.id) == url && data_.id == id) {
			return data_;
		}
	}
	return PLSBgmItemData();
}

PLSBgmItemData PLSBgmDragView::GetCurrent()
{
	QVector<PLSBgmItemData> datas = GetStm()->GetData();
	for (auto i = 0; i < datas.size(); i++) {
		PLSBgmItemData data_ = datas[i];
		if (data_.isCurrent) {
			return data_;
		}
	}
	return PLSBgmItemData();
}

void PLSBgmDragView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		startDragPoint = event->pos();
		startDragModelIdx = this->indexAt(event->pos());
	}

	QListView::mousePressEvent(event);
}

void PLSBgmDragView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		startDragModelIdx = this->indexAt(event->pos());
		emit MouseDoublePressedSignal(startDragModelIdx);
	}

	QListView::mouseDoubleClickEvent(event);
}

void PLSBgmDragView::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		QPoint distance = event->pos() - startDragPoint;
		if (distance.manhattanLength() > QApplication::startDragDistance()) {

			QDrag *drag = new QDrag(this);
			QMimeData *mimeData = new QMimeData();
			startDragIndex = startDragModelIdx.row();
			mimeData->setData(BGM_DRAG_MIME_TYPE, QByteArray(QString::number(startDragIndex).toStdString().c_str()));

			QRect currentRect = visualRect(this->indexAt(event->pos()));
			QPixmap pixmap = viewport()->grab(currentRect);
			drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
			drag->setPixmap(pixmap);

			drag->setMimeData(mimeData);
			drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
		}
	}
	QListView::mouseMoveEvent(event);
}

void PLSBgmDragView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls()) {
		if (PLSBgmDataViewManager::Instance()->IsSupportFormat(event->mimeData()->urls())) {
			event->acceptProposedAction();
			return;
		}
		event->ignore();
		return;
	}

	if (event->mimeData()->hasFormat(BGM_DRAG_MIME_TYPE)) {
		event->setDropAction(Qt::MoveAction);
		event->accept();
		isDraging = true;
	} else {
		event->ignore();
	}
}

void PLSBgmDragView::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat(BGM_DRAG_MIME_TYPE)) {
		isDraging = true;

		GetStm()->setData(dragOverModelIndex, QVariant::fromValue(DropIndicator::None), CustomDataRole::DropIndicatorRole);
		update(dragOverModelIndex);

		QModelIndex dragModelIdx = this->indexAt(event->pos());
		int rowCount = dragModelIdx.row();
		if (-1 == rowCount || rowCount > this->Count()) {
			rowCount = this->Count() - 1;
			dragModelIdx = model()->index(rowCount, 0);
		}
		dragOverModelIndex = dragModelIdx;
		QRect currentRect = visualRect(GetModelIndex(rowCount));
		QPoint topleft = currentRect.topLeft();
		if (event->pos().y() - topleft.y() > FIX_BGM_ITEM_HEIGHT / 2) {
			GetStm()->setData(dragOverModelIndex, QVariant::fromValue(DropIndicator::Bottom), CustomDataRole::DropIndicatorRole);
			update(dragOverModelIndex);
		} else {
			GetStm()->setData(dragOverModelIndex, QVariant::fromValue(DropIndicator::Top), CustomDataRole::DropIndicatorRole);
			update(dragOverModelIndex);
		}

		this->viewport()->update();
		event->setDropAction(Qt::MoveAction);
		QListView::dragMoveEvent(event);
		event->accept();
	} else if (event->mimeData()->hasUrls()) {
		event->setDropAction(Qt::MoveAction);
		QListView::dragMoveEvent(event);
		event->accept();
	} else {
		QListView::dragMoveEvent(event);
		event->ignore();
	}
}

void PLSBgmDragView::dropEvent(QDropEvent *event)
{
	QListView::dropEvent(event);

	if (event->mimeData()->hasUrls()) {
		QStringList paths;
		QList<QUrl> urls = event->mimeData()->urls();
		for (int i = 0; i < urls.size(); i++) {
			QString file = urls.at(i).toLocalFile();
			if (!PLSBgmDataViewManager::Instance()->IsSupportFormat(file)) {
				continue;
			}
			paths << file;
		}
		PLS_UI_STEP(MAIN_BGM_MODULE, QString("%1 Songs").arg(paths.size()).toStdString().c_str(), ACTION_DRAG);
		emit AudioFileDraggedIn(paths);
		return;
	}

	if (event->mimeData()->hasFormat(BGM_DRAG_MIME_TYPE)) {
		event->setDropAction(Qt::MoveAction);
		event->accept();
		isDraging = false;
		QModelIndex dropDragModelIdx = this->indexAt(event->pos());
		int currentIndex = dropDragModelIdx.row();
		if (-1 == currentIndex || currentIndex > this->Count()) {
			currentIndex = this->Count() - 1;
		}
		QRect currentRect = visualRect(GetModelIndex(currentIndex));
		QPoint topleft = currentRect.topLeft();
		if (event->pos().y() - topleft.y() < FIX_BGM_ITEM_HEIGHT / 2) {
			if (startDragIndex < currentIndex) {
				currentIndex <= 0 ? 0 : currentIndex -= 1;
			}
		} else {
			if (startDragIndex > currentIndex) {
				currentIndex >= Count() - 1 ? Count() - 1 : currentIndex += 1;
			}
		}

		QListView::dropEvent(event);
		emit RowChanged(startDragIndex, currentIndex);
		PLS_UI_STEP(MAIN_BGM_MODULE, "Music Playlist", ACTION_DROP);
		GetStm()->setData(dragOverModelIndex, QVariant::fromValue(DropIndicator::None), CustomDataRole::DropIndicatorRole);
		update(dragOverModelIndex);
	} else {
		QListView::dropEvent(event);
		event->ignore();
	}
}

void PLSBgmDragView::dragLeaveEvent(QDragLeaveEvent *event)
{
	isDraging = false;
	update();
	GetStm()->setData(dragOverModelIndex, QVariant::fromValue(DropIndicator::None), CustomDataRole::DropIndicatorRole);
	update(dragOverModelIndex);
	QListView::dragLeaveEvent(event);
}

void PLSBgmDragView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this->viewport());
	if (isDraging) {
		painter.setPen(QPen(QColor("#effc35"), 1));
	} else {
		painter.setPen(QPen(QColor("#1e1e1f"), 1));
	}

	QLine l;
	l.setP1(lineStart);
	l.setP2(lineEnd);
	painter.drawLine(l);

	QListView::paintEvent(event);
}

void PLSBgmDragView::showEvent(QShowEvent *event)
{
	QVector<PLSBgmItemData> datas = GetStm()->GetData();
	for (auto i = 0; i < datas.size(); i++) {
		PLSBgmItemData data_ = datas[i];
		if (data_.isCurrent) {
			QTimer::singleShot(0, this, [=]() { scrollTo(GetModelIndex(i)); });
			break;
		}
	}
	QListView::showEvent(event);
}

void PLSBgmDragView::SetPaintLinePos(const int &startPosX, const int &startPosY, const int &endPosX, const int &endPosY)
{
	lineStart.setX(startPosX);
	lineStart.setY(startPosY);
	lineEnd.setX(endPosX);
	lineEnd.setY(endPosY);
}

PLSBgmItemViewModel *PLSBgmDragView::GetStm() const
{
	return reinterpret_cast<PLSBgmItemViewModel *>(model());
}

bool PLSBgmDragView::ExistedId(const QString url, const int &id)
{
	QVector<PLSBgmItemData> datas = GetStm()->GetData();
	for (int i = 0; i < datas.size(); i++) {
		PLSBgmItemData data = datas[i];
		if (data.GetUrl(data.id) == url && id == data.id) {
			return true;
		}
		continue;
	}
	return false;
}

void PLSBgmDragView::RemoveFile(const PLSBgmItemData &data)
{
	if (!data.coverPath.isEmpty() && data.isLocalFile) {
		QFile::remove(data.coverPath);
	}
}

void PLSBgmDragView::CreateItemDelegate()
{
	PLSBgmItemDelegate *delegate = new PLSBgmItemDelegate(this, font(), this);
	connect(delegate, &PLSBgmItemDelegate::delBtnClicked, this, [=](const QModelIndex &index) { emit DelButtonClickedSignal(GetData(index), false); });

	setItemDelegate(delegate);
}
