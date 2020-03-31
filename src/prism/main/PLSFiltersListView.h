#ifndef PLSFILTERSLISTVIEW_H
#define PLSFILTERSLISTVIEW_H

#include "focus-list.hpp"
#include <PLSFiltersItemView.h>

#include <QListWidget>

namespace Ui {
class PLSFiltersListView;
}

class PLSFiltersListView : public FocusList {
	Q_OBJECT

public:
	explicit PLSFiltersListView(QWidget *parent = nullptr);
	~PLSFiltersListView();

	bool QueryRemove(QWidget *parent, obs_source_t *source);
	void AddFilterItemView(OBSSource filter, bool reorder = false);
	void RemoveFilterItemView(OBSSource filter);
	void RenameCurrentFilter();
	int GetCurrentRow();
	OBSSource GetFilter(int row);
	OBSSource GetStartDragFilter();
	void SetCurrentItem(PLSFiltersItemView *item);
	void SetCurrentItem(const int &row);
	void SetSource(OBSSource source);

protected:
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dragMoveEvent(QDragMoveEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;
	virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
	virtual void paintEvent(QPaintEvent *event) override;

private slots:
	void OnFilterRenameTriggered(PLSFiltersItemView *item);
	void OnFilterRemoveTriggered(PLSFiltersItemView *item);
	void OnFinishingEditName(const QString &text, PLSFiltersItemView *item);
	void OnCurrentItemChanged(PLSFiltersItemView *item);

private:
	void SetPaintLinePos(const int &startPosX, const int &startPosY, const int &endPosX, const int &endPosY);

signals:
	void RowChanged(const int &pre, const int &next);
	void CurrentItemIndexChanged(const int &row);

private:
	Ui::PLSFiltersListView *ui;
	QPoint startDragPoint{};
	QListWidgetItem *startDragItem{};
	int startDragIndex{-1};
	bool isDraging{false};
	QPoint lineStart{};
	QPoint lineEnd{};
	OBSSource startDragSource;
	OBSSource source;
};

#endif // PLSFILTERSLISTVIEW_H
