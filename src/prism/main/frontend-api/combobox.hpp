#pragma once

#include <QListView>
#include <QComboBox>
#include <QScrollBar>

#include "frontend-api-global.h"

class FRONTEND_API PLSComboBoxListView : public QListView {
	Q_OBJECT
	Q_PROPERTY(bool scrollBarShow READ scrollBarShow)

public:
	explicit PLSComboBoxListView(QWidget *parent = nullptr);
	~PLSComboBoxListView();

public:
	bool scrollBarShow() const;

protected:
	bool eventFilter(QObject *i_Object, QEvent *i_Event);

private:
	virtual void verticalScrollbarValueChanged(int value) override;
	virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
	void rowsInserted(const QModelIndex &parent, int start, int end) override;
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;

private:
	QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex &index, const QEvent *event = nullptr) const;
};

class FRONTEND_API PLSComboBox : public QComboBox {
	Q_OBJECT

public:
	explicit PLSComboBox(QWidget *parent = nullptr);
	~PLSComboBox();

signals:

	void popupShown(bool);

public:
	virtual void showPopup();
	virtual void hidePopup();

protected:
	void wheelEvent(QWheelEvent *event);
	virtual void paintEvent(QPaintEvent *event) override;

private:
	bool isAnimateCombo;
};

class FRONTEND_API PLSEditableComboBox : public PLSComboBox {
	Q_OBJECT

public:
	explicit PLSEditableComboBox(QWidget *parent = nullptr);
	~PLSEditableComboBox();

public:
	virtual void showPopup();
};
