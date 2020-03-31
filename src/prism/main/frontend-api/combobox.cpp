#include "combobox.hpp"

#include <QWheelEvent>
#include <QApplication>
#include <QLineEdit>
#include <QStylePainter>
#include <QDebug>
#include <QLabel>

#include "frontend-api.h"
#include "PLSDpiHelper.h"

class PLSComboBoxListViewLabel : public QLabel {
	Q_OBJECT
	Q_PROPERTY(int itemPosition READ itemPosition)
public:
	PLSComboBoxListViewLabel(PLSComboBoxListView *listView_, const QString &text) : QLabel(text), listView(listView_), listWindowWidth(), originalText(text)
	{
		setMouseTracking(true);
		setObjectName("itemLabel");
		setProperty("isCurrent", false);
		setProperty("isHover", false);
	}

public:
	int itemPosition() const
	{
		QPoint center = mapToGlobal(rect().center());
		QRect firstRect(listView->mapToGlobal(QPoint(0, 0)), size());
		QRect lastRect(listView->mapToGlobal(QPoint(0, listView->height() - height())), size());
		if (firstRect.contains(center) && lastRect.contains(center)) {
			return 2;
		} else if (firstRect.contains(center)) {
			return 0;
		} else if (lastRect.contains(center)) {
			return 1;
		}
		return 3;
	}
	void setText(const QString &text)
	{
		originalText = text;
		if (listWindowWidth > 0) {
			updateText(listWindowWidth);
		} else {
			QLabel::setText(text);
		}
	}
	void updateText(int width)
	{
		listWindowWidth = width;
		width -= listView->scrollBarShow() ? listView->verticalScrollBar()->width() : 0;
		QString text = fontMetrics().elidedText(originalText, Qt::ElideRight, width - PLSDpiHelper::calculate(this, 13) * 2);
		QLabel::setText(text);
	}

protected:
	virtual void leaveEvent(QEvent *event)
	{
		QLabel::leaveEvent(event);
		setProperty("isHover", false);
		pls_flush_style(this);
	}
	virtual void enterEvent(QEvent *event)
	{
		QLabel::enterEvent(event);
		setProperty("isHover", true);
		pls_flush_style(this);
	}

private:
	PLSComboBoxListView *listView;
	int listWindowWidth;
	QString originalText;
};

class PLSComboBoxListViewResize : public QObject {
public:
	PLSComboBoxListViewResize(PLSComboBoxListView *listView_, QObject *parent) : QObject(parent), listView(listView_) {}

protected:
	virtual bool eventFilter(QObject *watched, QEvent *event) override
	{
		if (event->type() == QEvent::Show) {
			QWidget *widget = static_cast<QWidget *>(watched);
			QAbstractItemModel *model = listView->model();
			for (int row = 0, rowCount = model->rowCount(); row < rowCount; ++row) {
				auto index = model->index(row, 0);
				PLSComboBoxListViewLabel *label = dynamic_cast<PLSComboBoxListViewLabel *>(listView->indexWidget(index));
				if (label) {
					label->updateText(widget->width());
				}
			}
		}

		return QObject::eventFilter(watched, event);
	}

private:
	PLSComboBoxListView *listView;
};

PLSComboBoxListView::PLSComboBoxListView(QWidget *parent) : QListView(parent)
{
	this->installEventFilter(this);
}
PLSComboBoxListView::~PLSComboBoxListView() {}

bool PLSComboBoxListView::scrollBarShow() const
{
	QScrollBar *bar = verticalScrollBar();
	return bar ? bar->isVisible() : false;
}

QItemSelectionModel::SelectionFlags PLSComboBoxListView::selectionCommand(const QModelIndex &index, const QEvent *event) const
{
	Q_UNUSED(index)
	Q_UNUSED(event)

	return QItemSelectionModel::SelectionFlag::NoUpdate;
}

bool PLSComboBoxListView::eventFilter(QObject *i_Object, QEvent *i_Event)
{
	if (i_Object == this && i_Event->type() == QEvent::KeyPress) {
		if (i_Object == this && i_Event->type() == QEvent::KeyPress) {
			QKeyEvent *key = static_cast<QKeyEvent *>(i_Event);
			if ((key->key() != Qt::Key_Enter) && (key->key() != Qt::Key_Return)) {
				//disable up down and other key.
				return true;
			}
		}
	}
	return QWidget::eventFilter(i_Object, i_Event);
}

void PLSComboBoxListView::verticalScrollbarValueChanged(int value)
{
	QListView::verticalScrollbarValueChanged(value);
	auto model = this->model();
	for (int row = 0, rowCount = model->rowCount(); row < rowCount; ++row) {
		QWidget *widget = indexWidget(model->index(row, 0));
		if (widget) {
			pls_flush_style(widget);
		}
	}
}

void PLSComboBoxListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	QListView::selectionChanged(selected, deselected);

	if (!selected.indexes().isEmpty()) {
		QWidget *currentWidget = indexWidget(selected.indexes().first());
		if (currentWidget) {
			currentWidget->setProperty("isCurrent", true);
			pls_flush_style(currentWidget);
		}
	}

	if (!deselected.indexes().isEmpty()) {
		QWidget *previousWidget = indexWidget(deselected.indexes().first());
		if (previousWidget) {
			previousWidget->setProperty("isCurrent", false);
			pls_flush_style(previousWidget);
		}
	}
}

void PLSComboBoxListView::rowsInserted(const QModelIndex &parent, int start, int end)
{
	QListView::rowsInserted(parent, start, end);

	for (int index = start; index <= end; ++index) {
		QModelIndex child = model()->index(index, 0, rootIndex());
		if (!indexWidget(child)) {
			setIndexWidget(child, new PLSComboBoxListViewLabel(this, child.data().toString()));
		}
	}
}

void PLSComboBoxListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
	QListView::dataChanged(topLeft, bottomRight, roles);

	int row = topLeft.row();
	QModelIndex index = topLeft;
	do {
		PLSComboBoxListViewLabel *currentWidget = dynamic_cast<PLSComboBoxListViewLabel *>(indexWidget(index));
		if (currentWidget) {
			currentWidget->setText(index.data().toString());
			if (index.flags() == Qt::NoItemFlags) {
				currentWidget->setProperty("enable", false);
			}
		}
		index = topLeft.siblingAtRow(++row);
	} while (row <= bottomRight.row());
}

PLSComboBox::PLSComboBox(QWidget *parent) : QComboBox(parent)
{
	isAnimateCombo = qApp->isEffectEnabled(Qt::UI_AnimateCombo);

	auto listView = new PLSComboBoxListView();
	PLSComboBoxListViewResize *resizeEvent = new PLSComboBoxListViewResize(listView, this);

	setView(listView);
	view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	view()->window()->setAttribute(Qt::WA_TranslucentBackground);
	view()->window()->installEventFilter(resizeEvent);
	setMaxVisibleItems(5);
}
PLSComboBox ::~PLSComboBox() {}

void PLSComboBox::showPopup()
{
	auto model = this->model();
	for (int row = 0, rowCount = model->rowCount(); row < rowCount; ++row) {
		QWidget *widget = view()->indexWidget(model->index(row, 0));
		if (widget && widget->property("isHover").toBool()) {
			widget->setProperty("isHover", false);
			pls_flush_style(widget);
		}
	}

	qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);
	QComboBox::showPopup();
	emit popupShown(true);

	for (int row = 0, rowCount = model->rowCount(); row < rowCount; ++row) {
		QWidget *widget = view()->indexWidget(model->index(row, 0));
		if (widget) {
			pls_flush_style(widget);
		}
	}
}

void PLSComboBox::hidePopup()
{
	QComboBox::hidePopup();
	qApp->setEffectEnabled(Qt::UI_AnimateCombo, isAnimateCombo);
	emit popupShown(false);
}

void PLSComboBox::wheelEvent(QWheelEvent *event)
{
	event->ignore();
}

void PLSComboBox::paintEvent(QPaintEvent *event)
{
	QStylePainter painter(this);
	painter.setPen(palette().color(QPalette::Text));

	// draw the combobox frame, focusrect and selected etc.
	QStyleOptionComboBox opt;
	initStyleOption(&opt);
	painter.drawComplexControl(QStyle::CC_ComboBox, opt);

	// check if it needs to elid text.
	if (!opt.editable) {
		QRect textRect = style()->subControlRect(QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxEditField, this);
		int textSpace = textRect.width();
		if (opt.fontMetrics.width(opt.currentText) > textSpace) {
			opt.currentText = opt.fontMetrics.elidedText(opt.currentText, Qt::ElideRight, textSpace);
		}
	}
	// draw the icon and text
	painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}

PLSEditableComboBox::PLSEditableComboBox(QWidget *parent) : PLSComboBox(parent) {}

PLSEditableComboBox::~PLSEditableComboBox() {}

void PLSEditableComboBox::showPopup()
{
	if (QLineEdit *lineEdit = this->lineEdit(); isEditable() && lineEdit) {
		QString lineEditText = lineEdit->text();
		QSignalBlocker comboxSignalBlocker(this);
		QSignalBlocker lineEditSignalBlocker(lineEdit);
		if (int index = findText(lineEditText); index >= 0) {
			setCurrentIndex(index);
		} else {
			setCurrentIndex(-1);
			lineEdit->setText(lineEditText);
		}
	}

	PLSComboBox::showPopup();
}

#include "combobox.moc"
