#include "PLSFacebookGameLineEdit.h"
#include "ui_PLSFacebookGameLineEdit.h"
#include "PLSLoadingComboxMenu.h"
#include <QFocusEvent>
#include <QDebug>

#define TAG_HEIGHT 40

PLSFacebookGameLineEdit::PLSFacebookGameLineEdit(QWidget *parent) : QLineEdit(parent), ui(new Ui::PLSFacebookGameLineEdit)
{
	ui->setupUi(this);

	//set the clear button focus policy
	bool empty = this->text().isEmpty();
	ui->pushButtonClear->setCursor(Qt::ArrowCursor);
	ui->pushButtonClear->setFocusPolicy(Qt::NoFocus);
	ui->searchButton->setFocusPolicy(Qt::NoFocus);
	ui->pushButtonClear->setHidden(empty);

	//set connect method
	connect(this, &QLineEdit::textChanged, this, [this](const QString &text) {
		if (text.isEmpty()) {
			ui->pushButtonClear->hide();
			this->clear();
			emit clearText();
		} else {
			ui->pushButtonClear->show();
			emit searchKeyword(text);
		}
	});
	connect(ui->pushButtonClear, &QPushButton::clicked, this, [this]() { this->setText(QString()); });
}

PLSFacebookGameLineEdit::~PLSFacebookGameLineEdit()
{
	delete ui;
}

void PLSFacebookGameLineEdit::focusInEvent(QFocusEvent *event)
{
	ui->searchButton->setStyleSheet("image: url(:/images/btn-search-on-normal.svg);");
	QLineEdit::focusInEvent(event);
}

void PLSFacebookGameLineEdit::focusOutEvent(QFocusEvent *event)
{
	ui->searchButton->setStyleSheet("image: url(:/images/btn-search-normal.svg);");
	QLineEdit::focusOutEvent(event);
}
