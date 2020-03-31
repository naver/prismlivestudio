#ifndef PLSBGMDRAGVIEW_H
#define PLSBGMDRAGVIEW_H

#include "focus-list.hpp"
#include "PLSBgmDataManager.h"
#include "PLSBgmItemDelegate.h"
#include <QListView>

class PLSBgmItemView;
class PLSBgmItemData;
class PLSBgmItemViewModel;
class PLSBgmDragView : public QListView {
	Q_OBJECT
public:
	explicit PLSBgmDragView(QWidget *parent = nullptr);
	~PLSBgmDragView();

	void SetDpi(double dpi);
	void SetMediaStatus(const int &row, const MediaStatus &status);
	int GetCurrentRow();
	int GetRow(const PLSBgmItemData &data);
	PLSBgmItemData GetData(const QModelIndex &idx);

	void UpdateWidget();
	void UpdateWidget(const QVector<PLSBgmItemData> &datas);
	void InsertWidget(const QVector<PLSBgmItemData> &datas);

	void UpdateWidget(const QModelIndex &idx, const PLSBgmItemData &data);
	void SetCurrentRow(const PLSBgmItemData &data);
	void UpdataData(const int &index, const PLSBgmItemData &data);
	void UpdataData(const int &index, QVariant value, CustomDataRole role);

	QVector<PLSBgmItemData> GetData() const;
	QModelIndex GetModelIndex(const int &row);
	void Remove(const PLSBgmItemData &data);
	bool Existed(const QString &url);
	void Clear();
	int Count();
	int GetId(const QString url);
	PLSBgmItemData Get(int idx);
	PLSBgmItemData Get(const QString &url, const int &id);
	PLSBgmItemData GetCurrent();

protected:
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dragMoveEvent(QDragMoveEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;
	virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void showEvent(QShowEvent *event) override;

private:
	void SetPaintLinePos(const int &startPosX, const int &startPosY, const int &endPosX, const int &endPosY);
	PLSBgmItemViewModel *GetStm() const;
	bool ExistedId(const QString url, const int &id);
	void RemoveFile(const PLSBgmItemData &data);
	void CreateItemDelegate();

signals:
	void DelButtonClickedSignal(const PLSBgmItemData &data, bool);
	void LoadingFailed(const QString &name);
	void MousePressedSignal(PLSBgmItemView *item);
	void MouseDoublePressedSignal(const QModelIndex &index);
	void RowChanged(const int &srcIndex, const int &destIndex);
	void AudioFileDraggedIn(const QStringList &paths);

private:
	QPoint startDragPoint{};
	QPoint lineStart{};
	QPoint lineEnd{};
	QModelIndex startDragModelIdx{};
	QModelIndex dragOverModelIndex;
	int startDragIndex{-1};
	bool isDraging{false};
	bool dragFile{false};
};

#endif // PLSBGMDRAGVIEW_H
