#ifndef PLSBGMITEMVIEW_H
#define PLSBGMITEMVIEW_H

#include "PLSBgmDataManager.h"
#include "PLSBgmItemDelegate.h"
#include "loading-event.hpp"

#include <QFrame>
#include <QLabel>
#include <QTimer>
#include <QAbstractListModel>

namespace Ui {
class PLSBgmItemView;
}

class QSpacerItem;
class PLSLoadingEvent;
class QListWidgetItem;
class QLabel;
class PLSBgmDragView;

class PLSBgmItemViewModel : public QAbstractListModel {
	Q_OBJECT

	friend class PLSBgmDragView;
	friend class PLSBgmItemView;

	PLSBgmDragView *view{};
	QVector<PLSBgmItemData> datas{};
	bool needPaint{false};

	QVector<PLSBgmItemData> GetData() const;
	void SetData(const QVector<PLSBgmItemData> &data);
	void InsertData(const QVector<PLSBgmItemData> &data);
	void Insert(const PLSBgmItemData &data, const int &index);
	void Remove(const QString &url, const int &id);
	void Clear();
	int Count();
	PLSBgmItemData Get(int idx);
	PLSBgmItemData Get(const QModelIndex &idx);

public:
	explicit PLSBgmItemViewModel(PLSBgmDragView *view);
	~PLSBgmItemViewModel();

	virtual int rowCount(const QModelIndex &parent) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
	virtual Qt::DropActions supportedDropActions() const override;
};

#endif // PLSBGMITEMVIEW_H
