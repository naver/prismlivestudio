#include "PLSCompleter.hpp"

#include <Windows.h>
#include <QVBoxLayout>
#include <QEvent>
#include <QApplication>

#include "frontend-api.h"

const int ItemHeight = 40;

PLSCompleterPopupList::PLSCompleterPopupList(QWidget *toplevel, QLineEdit *lineEdit, const QStringList &completions, PLSDpiHelper dpiHelper)
	: WidgetDpiAdapter(toplevel, Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint)
{
	dpiHelper.setCss(this, {PLSCssIndex::QScrollBar, PLSCssIndex::PLSCompleterPopupList});

	setMouseTracking(true);

	this->lineEdit = lineEdit;

	QVBoxLayout *layout1 = new QVBoxLayout(this);
	layout1->setSpacing(0);
	layout1->setMargin(0);
	this->scrollArea = new QScrollArea(this);
	this->scrollArea->setWidgetResizable(true);
	this->scrollArea->setFrameShape(QFrame::NoFrame);
	layout1->addWidget(scrollArea);

	QWidget *widget = new QWidget();
	widget->setObjectName("scrollAreaWidget");
	this->scrollArea->setWidget(widget);

	QVBoxLayout *layout2 = new QVBoxLayout(widget);
	layout2->setSpacing(0);
	layout2->setMargin(0);

	for (auto &completion : completions) {
		QLabel *label = new QLabel(completion, widget);
		label->setMouseTracking(true);
		label->installEventFilter(this);
		layout2->addWidget(label);
		labels.append(label);
	}

	connect(qApp, &QApplication::applicationStateChanged, this, &PLSCompleterPopupList::hide);
	connect(lineEdit, &QLineEdit::textEdited, this, &PLSCompleterPopupList::showPopup);
}

PLSCompleterPopupList::~PLSCompleterPopupList() {}

bool PLSCompleterPopupList::isInCompleter(const QPoint &globalPos)
{
	if (this->geometry().contains(globalPos)) {
		return true;
	}
	if (QRect(lineEdit->mapToGlobal(QPoint(0, 0)), lineEdit->size()).contains(globalPos)) {
		return true;
	}
	return false;
}

void PLSCompleterPopupList::showPopup(const QString &text)
{
	int showCount = 0;
	for (auto &label : labels) {
		if (label->text().contains(text)) {
			label->show();
			pls_flush_style(label);
			++showCount;
		} else {
			label->hide();
		}
	}

	if (showCount <= 0) {
		hide();
		return;
	}

	PLSDpiHelper::Screen *screen = nullptr;
	double dpi = PLSDpiHelper::getDpi(this->lineEdit, screen);
	setFixedSize(this->lineEdit->width(), (showCount > 5 ? 5 : showCount) * PLSDpiHelper::calculate(dpi, ItemHeight) + PLSDpiHelper::calculate(dpi, 5));
	move(this->lineEdit->mapToGlobal(QPoint(0, int(this->lineEdit->height()))));
	PLSWidgetDpiAdapter *adapter = this;
	PLSDpiHelper::checkStatusChanged(adapter, screen);
	show();
}

bool PLSCompleterPopupList::eventFilter(QObject *watched, QEvent *event)
{
	switch (event->type()) {
	case QEvent::Enter:
		if (QLabel *label = dynamic_cast<QLabel *>(watched); label) {
			pls_flush_style(label, "state", "hover");
		}
		break;
	case QEvent::Leave:
		if (QLabel *label = dynamic_cast<QLabel *>(watched); label) {
			pls_flush_style(label, "state", "");
		}
		break;
	case QEvent::MouseButtonRelease:
		if (QLabel *label = dynamic_cast<QLabel *>(watched); label) {
			QSignalBlocker blocker(lineEdit);
			emit activated(label->text());
			hide();
		}
		break;
	}
	return WidgetDpiAdapter::eventFilter(watched, event);
}

PLSCompleter::PLSCompleter(QWidget *toplevel, QLineEdit *lineEdit_, const QStringList &completions) : QObject(lineEdit_), lineEdit(lineEdit_)
{
	popupList = new PLSCompleterPopupList(toplevel, lineEdit, completions);
	connect(popupList, &PLSCompleterPopupList::activated, this, [=](const QString &text) {
		if (lineEdit->text() != text) {
			lineEdit->setText(text);
			emit activated(text);
		}
	});

	auto focusPolicy = lineEdit->focusPolicy();
	popupList->setFocusPolicy(Qt::NoFocus);
	popupList->setFocusProxy(lineEdit);
	lineEdit->setFocusPolicy(focusPolicy);

	lineEdit->installEventFilter(this);
	popupList->installEventFilter(this);
}

PLSCompleter::~PLSCompleter()
{
	delete popupList;
}

#define QLINEEDIT_ATTACHED_KEY "__QLineEdit_attached_PLSCompleter"

PLSCompleter *PLSCompleter::attachLineEdit(QWidget *toplevel, QLineEdit *lineEdit, const QStringList &completions)
{
	PLSCompleter *completer = (PLSCompleter *)lineEdit->property(QLINEEDIT_ATTACHED_KEY).value<void *>();
	if (completer) {
		return nullptr;
	}

	completer = new PLSCompleter(toplevel, lineEdit, completions);
	lineEdit->setProperty(QLINEEDIT_ATTACHED_KEY, QVariant::fromValue<void *>(completer));
	return completer;
}

void PLSCompleter::detachLineEdit(QLineEdit *lineEdit)
{
	if (PLSCompleter *completer = (PLSCompleter *)lineEdit->property(QLINEEDIT_ATTACHED_KEY).value<void *>(); completer) {
		delete completer;
	}
}

bool PLSCompleter::eventFilter(QObject *watched, QEvent *event)
{
	if (eatFocusOut && watched == lineEdit && event->type() == QEvent::FocusOut && popupList->isVisible()) {
		return true;
	}
	if (watched == lineEdit && ((event->type() == QEvent::FocusIn) || (event->type() == QEvent::MouseButtonPress)) && !popupList->isVisible()) {
		popupList->showPopup(lineEdit->text());
	}

	if (watched == lineEdit && ((event->type() == QEvent::FocusOut) || (event->type() == QEvent::FocusIn))) {
		lineEdit->setProperty("focusState", event->type() == QEvent::FocusIn ? "FocusIn" : "FocusOut");
		pls_flush_style(lineEdit);
	}

	if (watched != popupList) {
		return QObject::eventFilter(watched, event);
	}

	switch (event->type()) {
	case QEvent::KeyPress: {
		eatFocusOut = false;
		(static_cast<QObject *>(lineEdit))->event(event);
		eatFocusOut = true;
		return true;
	}
	case QEvent::KeyRelease: {
		eatFocusOut = false;
		static_cast<QObject *>(lineEdit)->event(event);
		eatFocusOut = true;
		return true;
	}
	case QEvent::InputMethod:
	case QEvent::ShortcutOverride:
		QApplication::sendEvent(lineEdit, event);
		break;
	default:
		return false;
	}
	return false;
}
