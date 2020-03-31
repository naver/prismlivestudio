#include "dialogbuttonbox.hpp"

#include <QPushButton>
#include <QVariant>
#include <QStyle>
#include <QDynamicPropertyChangeEvent>

#include <log.h>
#include <action.h>

#include "dialog-view.hpp"
#include "frontend-api.h"

static PLSDialogView *getDialogView(QWidget *widget)
{
	PLSDialogView *dialogView = nullptr;
	for (QWidget *parent = widget; !dialogView && parent; parent = parent->parentWidget()) {
		dialogView = dynamic_cast<PLSDialogView *>(parent);
	}
	return dialogView;
}

PLSDialogButtonBox::PLSDialogButtonBox(QWidget *parent) : QDialogButtonBox(parent)
{
	connect(this, &PLSDialogButtonBox::clicked, this, &PLSDialogButtonBox::onButtonClicked);
}

PLSDialogButtonBox::~PLSDialogButtonBox() {}

void PLSDialogButtonBox::updateStandardButtonsStyle(QDialogButtonBox *buttonBox)
{
#define BUTTON_USE_FOR(Name)                                                         \
	do {                                                                         \
		if (QPushButton *button = buttonBox->button(StandardButton::Name)) { \
			button->setProperty("useFor", #Name);                        \
			pls_flush_style(button);                                     \
		}                                                                    \
	} while (false)

	BUTTON_USE_FOR(Ok);
	BUTTON_USE_FOR(Save);
	BUTTON_USE_FOR(SaveAll);
	BUTTON_USE_FOR(Open);
	BUTTON_USE_FOR(Yes);
	BUTTON_USE_FOR(YesToAll);
	BUTTON_USE_FOR(No);
	BUTTON_USE_FOR(NoToAll);
	BUTTON_USE_FOR(Abort);
	BUTTON_USE_FOR(Retry);
	BUTTON_USE_FOR(Ignore);
	BUTTON_USE_FOR(Close);
	BUTTON_USE_FOR(Cancel);
	BUTTON_USE_FOR(Discard);
	BUTTON_USE_FOR(Help);
	BUTTON_USE_FOR(Apply);
	BUTTON_USE_FOR(Reset);
	BUTTON_USE_FOR(RestoreDefaults);

#undef BUTTON_USE_FOR
}

void PLSDialogButtonBox::setStandardButtons(StandardButtons buttons)
{
	QDialogButtonBox::setStandardButtons(buttons);
	updateStandardButtonsStyle(this);
}

void PLSDialogButtonBox::onButtonClicked(QAbstractButton *button)
{
	PLSDialogView *dialog = nullptr;
	for (QWidget *parent = parentWidget(); !dialog && parent; parent = parent->parentWidget()) {
		dialog = dynamic_cast<PLSDialogView *>(parent);
	}

	if (dialog) {
		QString btnName = button->property("useFor").toString();
		if (btnName.isEmpty()) {
			btnName = ("\"" + button->text() + "\"");
		}
		PLS_UI_STEP(dialog->getModuleName(), (dialog->getViewName() + "'s " + btnName.toStdString() + " Button").c_str(), ACTION_CLICK);
	}
}

bool PLSDialogButtonBox::event(QEvent *event)
{
	if (event->type() == QEvent::DynamicPropertyChange) {
		QDynamicPropertyChangeEvent *dpce = dynamic_cast<QDynamicPropertyChangeEvent *>(event);
		if ((dpce->propertyName() == "autoClose") && property(dpce->propertyName().constData()).toBool()) {
			if (PLSDialogView *dialogView = getDialogView(this)) {
				connect(this, &QDialogButtonBox::accepted, dialogView, &PLSDialogView::accept);
				connect(this, &QDialogButtonBox::rejected, dialogView, &PLSDialogView::reject);
			}
		}
	}
	return QDialogButtonBox::event(event);
}
