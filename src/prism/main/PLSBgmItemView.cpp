#include "PLSBgmItemView.h"
#include "PLSBgmDragView.h"
#include "ui_PLSBgmItemView.h"
#include "PLSBgmItemDelegate.h"

#include "frontend-api.h"
#include "liblog.h"
#include "log/module_names.h"
#include "action.h"
#include "pls-common-define.hpp"
#include "PLSDpiHelper.h"
#include "pls-app.hpp"

#include <QTime>
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include <QMovie>
#include <QMetaEnum>
#include <QSpacerItem>

QVector<PLSBgmItemData> PLSBgmItemViewModel::GetData() const
{
	return datas;
}

void PLSBgmItemViewModel::SetData(const QVector<PLSBgmItemData> &data)
{
	datas = data;
}

void PLSBgmItemViewModel::InsertData(const QVector<PLSBgmItemData> &datas_)
{
	int index = 0;
	for (int i = datas_.size() - 1; i >= 0; i--) {
		Insert(datas_[i], i);
	}
}

void PLSBgmItemViewModel::Insert(const PLSBgmItemData &data, const int &index)
{
	beginInsertRows(QModelIndex(), 0, 0);
	datas.insert(0, data);
	endInsertRows();
	view->UpdateWidget(createIndex(0, 0, nullptr), data);
}

void PLSBgmItemViewModel::Remove(const QString &url, const int &id)
{
	int idx = -1;
	PLSBgmItemData item;
	for (int i = 0; i < datas.count(); i++) {
		PLSBgmItemData data = datas[i];
		if (datas[i].GetUrl(data.id) == url && datas[i].id == id) {
			item = data;
			idx = i;
			break;
		}
	}

	if (idx == -1)
		return;

	int startIdx = idx;
	int endIdx = idx;

	beginRemoveRows(QModelIndex(), startIdx, endIdx);
	datas.remove(idx, endIdx - startIdx + 1);
	endRemoveRows();
}

void PLSBgmItemViewModel::Clear()
{
	beginResetModel();
	datas.clear();
	endResetModel();
}

int PLSBgmItemViewModel::Count()
{
	return datas.count();
}

PLSBgmItemData PLSBgmItemViewModel::Get(int idx)
{
	if (idx == -1 || idx >= datas.count())
		return PLSBgmItemData();
	return datas[idx];
}

PLSBgmItemData PLSBgmItemViewModel::Get(const QModelIndex &idx)
{
	return Get(idx.row());
}

PLSBgmItemViewModel::PLSBgmItemViewModel(PLSBgmDragView *view_) : QAbstractListModel(view_), view(view_) {}

PLSBgmItemViewModel::~PLSBgmItemViewModel() {}

int PLSBgmItemViewModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : datas.count();
}

QVariant PLSBgmItemViewModel::data(const QModelIndex &index, int role) const
{
	if (CustomDataRole::DataRole == role) {
		int row = index.row();
		if (row < 0 || row >= datas.count()) {
			return QVariant();
		}
		PLSBgmItemData data = datas[row];
		return QVariant::fromValue(data);
	} else if (CustomDataRole::NeedPaintRole == role) {
		return true;
	} else if (CustomDataRole::MediaStatusRole == role) {
		int row = index.row();
		if (row > -1 && row < datas.count()) {
			return QVariant::fromValue(datas[row].mediaStatus);
		}
		return QVariant();
	} else if (CustomDataRole::DropIndicatorRole == role) {
		int row = index.row();
		if (row > -1 && row < datas.count()) {
			return QVariant::fromValue(datas[row].dropIndicator);
		}
		return false;
	} else if (CustomDataRole::RowStatusRole == role) {
		int row = index.row();
		if (row > -1 && row < datas.count()) {
			return QVariant::fromValue(datas[row].rowStatus);
		}
		return false;
	}
	return QVariant();
}

bool PLSBgmItemViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (CustomDataRole::NeedPaintRole == role) {
		needPaint = value.toBool();
	} else if (CustomDataRole::MediaStatusRole == role) {
		auto mediaState = value.value<MediaStatus>();
		int row = index.row();
		if (row > -1 && row < datas.count()) {
			datas[row].mediaStatus = mediaState;
			if (mediaState == MediaStatus::stateInvalid) {
				datas[row].isDisable = true;
			} else if (mediaState == MediaStatus::stateNormal) {
				datas[row].isDisable = false;
			}
		}
	} else if (CustomDataRole::DataRole == role) {
		int row = index.row();
		if (row < 0) {
			return false;
		} else if (row >= datas.count()) {
			datas.push_back(value.value<PLSBgmItemData>());
		} else {
			datas[row] = value.value<PLSBgmItemData>();
		}
	} else if (CustomDataRole::DropIndicatorRole == role) {
		auto indicator = value.value<DropIndicator>();
		int row = index.row();
		if (row > -1 && row < datas.count()) {
			datas[row].dropIndicator = indicator;
		}
	} else if (CustomDataRole::CoverPathRole == role) {
		auto coverPath = value.value<QString>();
		int row = index.row();
		if (row > -1 && row < datas.count()) {
			datas[row].coverPath = coverPath;
		}
	} else if (CustomDataRole::RowStatusRole == role) {
		RowStatus rowStatus = value.value<RowStatus>();
		int row = index.row();
		if (row > -1 && row < datas.count()) {
			datas[row].rowStatus = rowStatus;
		}
	} else {
		return QAbstractListModel::setData(index, value, role);
	}
}

Qt::ItemFlags PLSBgmItemViewModel::flags(const QModelIndex &index) const
{
	return Qt::NoItemFlags;
}

Qt::DropActions PLSBgmItemViewModel::supportedDropActions() const
{
	return QAbstractItemModel::supportedDropActions() | Qt::MoveAction;
}
