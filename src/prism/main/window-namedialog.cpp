/******************************************************************************
    Copyright (C) 2013 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "window-namedialog.hpp"
#include "qt-wrappers.hpp"
#include "ui_NameDialog.h"
#include "pls-app.hpp"

#include <QPushButton>
#include <QStyle>

using namespace std;

void NameDialog::OnTextEditd(const QString &text)
{
	Q_UNUSED(text);
	ui->userText->setProperty("status", "editing");
	style()->unpolish(ui->userText);
	style()->polish(ui->userText);
}

void NameDialog::OnTextEditingFinished()
{
	ui->userText->setProperty("status", "editingFinish");
	style()->unpolish(ui->userText);
	style()->polish(ui->userText);
}

void NameDialog::showEvent(QShowEvent *event)
{
	PLSDialogView::showEvent(event);

	ui->userText->selectAll();
	ui->userText->setFocus();
	activateWindow();
}

bool NameDialog::eventFilter(QObject *watcher, QEvent *event)
{
	if (watcher == ui->userText && event->type() == QEvent::FocusIn) {
		OnTextEditd("");
		return false;
	}

	if (watcher == ui->userText && event->type() == QEvent::FocusOut) {
		OnTextEditingFinished();
		return false;
	}
	return PLSDialogView::eventFilter(watcher, event);
}

NameDialog::NameDialog(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper), ui(new Ui::NameDialog)
{
	ui->setupUi(this->content());
	dpiHelper.setCss(this, {PLSCssIndex::NameDialog});
	installEventFilter(CreateShortcutFilter());
	ui->userText->installEventFilter(this);
	connect(ui->userText, &QLineEdit::textEdited, this, &NameDialog::OnTextEditd);
	connect(ui->userText, &QLineEdit::editingFinished, this, &NameDialog::OnTextEditingFinished);

	ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
	ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");

	connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &NameDialog::accept);
	connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &NameDialog::reject);
}

static bool IsWhitespace(char ch)
{
	return ch == ' ' || ch == '\t';
}

bool NameDialog::AskForName(QWidget *parent, const QString &title, const QString &text, string &str, const QString &placeHolder, int maxSize)
{
	if (maxSize <= 0 || maxSize > 32767)
		maxSize = 170;

	NameDialog dialog(parent);
	dialog.setWindowTitle(title);
	dialog.ui->titleLabel->setText(text);
	dialog.ui->userText->setMaxLength(maxSize);
	dialog.ui->userText->setText(placeHolder);

	bool accepted = (dialog.exec() == DialogCode::Accepted);
	if (accepted) {
		str = QT_TO_UTF8(dialog.ui->userText->text());

		while (str.size() && IsWhitespace(str.back()))
			str.erase(str.end() - 1);
		while (str.size() && IsWhitespace(str.front()))
			str.erase(str.begin());
	}

	return accepted;
}
