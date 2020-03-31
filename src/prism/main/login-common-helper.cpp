#include "login-common-helper.hpp"
#include "IMACManager.h"
#include <QRegExp>
#include <QUrl>
#include "pls-common-define.hpp"
#include <qstyle.h>
#include <QDialog>

bool ::LoginCommonHelpers::isValidEmailByRegExp(const QString &email)
{
	QRegExp rep(EMAIL_REGEXP);
	return rep.exactMatch(email);
}

bool ::LoginCommonHelpers::isValidPasswordByRegExp(const QString &password)
{
	QRegExp rep(PASSWORD_REGEXP);
	return rep.exactMatch(password);
}

bool ::LoginCommonHelpers::isValidNickNameByRegExp(const QString &nickName)
{
	QRegExp rep(NICK_REGEXP);
	return rep.exactMatch(nickName);
}

void LoginCommonHelpers::setCurrentStackWidget(QStackedWidget *stackWidget, const QString &ojbName)
{
	if (stackWidget) {
		QWidget *widget = stackWidget->findChild<QWidget *>(ojbName);
		if (widget) {
			stackWidget->setCurrentWidget(widget);
		}
	}
}

QWidget *LoginCommonHelpers::getCurrentStackWidget(QStackedWidget *stackWidget, const QString &ojbName)
{
	if (stackWidget) {
		return stackWidget->findChild<QWidget *>(ojbName);
	}
	return nullptr;
}

void LoginCommonHelpers::refreshStyle(QWidget *widget)
{
	assert(widget != nullptr);
	widget->style()->unpolish(widget);
	widget->style()->polish(widget);
}

void LoginCommonHelpers::loginResultHandler(QStackedWidget *stackWidget, bool isSuccess)
{
	if (!isSuccess) {
		return;
	}

	QDialog *dialog = nullptr;
	for (QWidget *parent = stackWidget->parentWidget(); parent && !dialog;) {
		dialog = dynamic_cast<QDialog *>(parent);
		parent = parent->parentWidget();
	}

	if (dialog) {
		dialog->done(QDialog::Accepted);
	}
}
