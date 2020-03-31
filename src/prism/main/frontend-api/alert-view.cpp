#include "alert-view.hpp"
#include "ui_PLSAlertView.h"

#include <QPushButton>
#include <QApplication>
#include <QHBoxLayout>

#include <log.h>

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif // Q_OS_WINDOWS

#include "PLSThemeManager.h"

#define APPEND_SPACE_WIDTH 10
#define MESSAGE_LABEL_FIX_WIDTH 360

static PLSAlertView::Buttons getButtons(const QMap<PLSAlertView::Button, QString> &buttonMap)
{
	PLSAlertView::Buttons buttons = PLSAlertView::Button::NoButton;
	for (auto iter = buttonMap.begin(); iter != buttonMap.end(); ++iter) {
		buttons |= iter.key();
	}
	return buttons;
}
static int getButtonCount(PLSAlertView::Buttons buttons)
{
	int count = 0;
	for (int i = 0; i < 32; ++i) {
		if (buttons.testFlag(static_cast<PLSAlertView::Button>(1 << i))) {
			++count;
		}
	}
	return count;
}
static void setDefaultButton(QDialogButtonBox *buttonBox, PLSAlertView::Button defaultButton)
{
	for (int i = 0; i < 32; ++i) {
		if (QPushButton *button = buttonBox->button(static_cast<PLSAlertView::Button>(1 << i)); button) {
			button->setAutoDefault(false);
			button->setDefault(false);
		}
	}

	if (QPushButton *button = buttonBox->button(defaultButton)) {
		button->setDefault(true);
	}
}
static void resetButtonWidth(QDialogButtonBox *buttonBox, int minWidth, int maxWidth)
{
	for (int i = 0; i < 32; ++i) {
		if (QPushButton *button = buttonBox->button(static_cast<PLSAlertView::Button>(1 << i)); button) {
			button->setMinimumWidth(minWidth);
			button->setMaximumWidth(maxWidth);
			button->resize(button->fontMetrics().size(Qt::TextShowMnemonic, button->text()));
		}
	}
}
static int calcButtonWidth(QDialogButtonBox *buttonBox, int minWidth, int maxWidth)
{
	int width = minWidth;
	for (int i = 0; i < 32; ++i) {
		if (QPushButton *button = buttonBox->button(static_cast<PLSAlertView::Button>(1 << i)); button) {
			width = qMax(width, qMin(button->width(), maxWidth));
		}
	}
	return width;
}
static int calcButtonWidth(PLSDialogButtonBox *buttonBox, double dpi)
{
	resetButtonWidth(buttonBox, 0, QWIDGETSIZE_MAX);
	switch (getButtonCount(buttonBox->standardButtons())) {
	case 1:
		return calcButtonWidth(buttonBox, PLSDpiHelper::calculate(dpi, 140), PLSDpiHelper::calculate(dpi, 180));
	case 2:
		return calcButtonWidth(buttonBox, PLSDpiHelper::calculate(dpi, 140), PLSDpiHelper::calculate(dpi, 180));
	case 3:
	default:
		return calcButtonWidth(buttonBox, PLSDpiHelper::calculate(dpi, 117), PLSDpiHelper::calculate(dpi, 117));
	}
}

// class PLSAlertView Implements
PLSAlertView::PLSAlertView(QWidget *parent, Icon icon_, const QString &title, const QString &message, const QString &checkbox, const Buttons &buttons, Button defaultButton, const QSize &sugsize_,
			   PLSDpiHelper dpiHelper)
	: PLSDialogView(parent, dpiHelper), ui(new Ui::PLSAlertView), icon(Icon::NoIcon), checkBox(nullptr), sugsize(sugsize_)
{
	dpiHelper.setCss(this, {PLSThemeManager::PLSAlertView});
	dpiHelper.setFixedWidth(this, 411);
	dpiHelper.setDynamicStyleSheet(this, [=](double dpi, bool) -> QString {
		QString css = "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Ok\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Save\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"SaveAll\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Open\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Yes\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"YesToAll\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"No\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"NoToAll\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Abort\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Retry\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Ignore\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Close\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Cancel\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Discard\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Help\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Apply\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"Reset\"],"
			      "PLSAlertView #widget > #buttonBox QPushButton[useFor=\"RestoreDefaults\"] {"
			      "	min-width: %1px;"
			      "	max-width: %1px;"
			      "	margin: 0;"
			      "	padding: 0;"
			      "}";
		int width = calcButtonWidth(ui->buttonBox, dpi);
		return css.arg(width);
	});
	dpiHelper.notifyDpiChanged(this, [=](double dpi, double, bool firstShow) {
		if (firstShow) {
			updateGeometry();
		}

		activateWindow();
	});

	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	setEscapeCloseEnabled(true);
	setResizeEnabled(false);

	QWidget *content = this->content();
	ui->setupUi(content);

	setWindowTitle(title);

	if (!title.isEmpty()) {
		setHasCaption(true);
	} else {
		setHasCaption(false);
		setHasHLine(false);
	}

	setIcon(icon_);

	ui->nameLabel->hide();
	ui->message->setText(message);
	ui->buttonBox->setStandardButtons(buttons);
	connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &PLSAlertView::onButtonClicked);
	ui->buttonBox->findChild<QHBoxLayout *>(QString(), Qt::FindDirectChildrenOnly)->setSpacing(10);

	if (!checkbox.isEmpty()) {
		checkBox = new QCheckBox(checkbox, this);
		ui->contentLayout->addSpacing(15);
		ui->contentLayout->addWidget(checkBox);
	} else {
		checkBox = nullptr;
	}

	setDefaultButton(ui->buttonBox, defaultButton);
	adjustSize();

#define TranslateButtonText(name, text)                                                        \
	do {                                                                                   \
		if (QPushButton *button = ui->buttonBox->button(PLSAlertView::Button::name)) { \
			button->setText(tr(#text));                                            \
		}                                                                              \
	} while (false)
	TranslateButtonText(Ok, OK);
	TranslateButtonText(Open, Open);
	TranslateButtonText(Save, Save);
	TranslateButtonText(Cancel, Cancel);
	TranslateButtonText(Close, Close);
	TranslateButtonText(Discard, Discard);
	TranslateButtonText(Apply, Apply);
	TranslateButtonText(Reset, Reset);
	TranslateButtonText(Yes, Yes);
	TranslateButtonText(No, No);
	TranslateButtonText(Abort, Abort);
	TranslateButtonText(Retry, Retry);
	TranslateButtonText(Ignore, Ignore);
	TranslateButtonText(RestoreDefaults, Defaults);
#undef TranslateButtonText
}

PLSAlertView::PLSAlertView(QWidget *parent, Icon icon, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons, Button defaultButton,
			   const QSize &sugsize, PLSDpiHelper dpiHelper)
	: PLSAlertView(parent, icon, title, message, checkbox, getButtons(buttons), defaultButton, sugsize, dpiHelper)
{
	for (auto iter = buttons.begin(); iter != buttons.end(); ++iter) {
		if (auto button = ui->buttonBox->button(iter.key())) {
			button->setText(iter.value());
		}
	}
}

PLSAlertView::PLSAlertView(Icon icon, const QString &title, const QString &messageTitle, const QString &messageContent, QWidget *parent, PLSAlertView::Buttons buttons, Button defaultButton,
			   const QSize &sugsize, PLSDpiHelper dpiHelper)
	: PLSAlertView(parent, icon, title, !messageTitle.isEmpty() ? messageTitle.left(1) : QString(), QString(), buttons, defaultButton, sugsize, dpiHelper)
{
	dpiHelper.notifyDpiChanged(this, [=](double dpi, double oldDpi, bool firstShow) {
		Q_UNUSED(oldDpi)
		Q_UNUSED(firstShow)

		ui->message->setFixedWidth(PLSDpiHelper::calculate(dpi, MESSAGE_LABEL_FIX_WIDTH));
		ui->message->setText(GetNameElideString(messageTitle, ui->message));
		ui->nameLabel->setText(GetNameElideString(messageContent, ui->nameLabel) + "?");
	});

	ui->nameLabel->setAlignment(Qt::AlignCenter);
	ui->nameLabel->show();
}

QString PLSAlertView::GetNameElideString(const QString &name, QWidget *widget)
{
	if (widget) {
		QFontMetrics fontWidth(widget->font());
		if (fontWidth.width(name) > widget->width() - APPEND_SPACE_WIDTH)
			return fontWidth.elidedText(name, Qt::ElideRight, widget->width() - APPEND_SPACE_WIDTH);
	}

	return name;
}

PLSAlertView::~PLSAlertView()
{
	delete ui;
}

PLSAlertView::Button PLSAlertView::open(QWidget *parent, const QString &message, const Buttons &buttons, Button defaultButton, const QSize &size)
{
	return open(parent, Icon::Information, QString(), message, buttons, defaultButton, size);
}

PLSAlertView::Button PLSAlertView::open(QWidget *parent, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton, const QSize &size)
{
	return open(parent, Icon::Information, QString(), message, buttons, defaultButton, size);
}

PLSAlertView::Button PLSAlertView::open(QWidget *parent, const QString &title, const QString &message, const Buttons &buttons, Button defaultButton, const QSize &size)
{
	return open(parent, Icon::Information, title, message, buttons, defaultButton, size);
}

PLSAlertView::Button PLSAlertView::open(QWidget *parent, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton, const QSize &size)
{
	return open(parent, Icon::Information, title, message, buttons, defaultButton, size);
}

PLSAlertView::Button PLSAlertView::open(QWidget *parent, Icon icon, const QString &title, const QString &message, const Buttons &buttons, Button defaultButton, const QSize &size)
{
	PLSAlertView alertView(parent, icon, title, message, QString(), buttons, defaultButton, size);
	return static_cast<Button>(alertView.exec());
}

PLSAlertView::Button PLSAlertView::open(QWidget *parent, Icon icon, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton, const QSize &size)
{
	PLSAlertView alertView(parent, icon, title, message, QString(), buttons, defaultButton, size);
	return static_cast<Button>(alertView.exec());
}

PLSAlertView::Result PLSAlertView::open(QWidget *parent, Icon icon, const QString &title, const QString &message, const QString &checkbox, const Buttons &buttons, Button defaultButton,
					const QSize &size)
{
	PLSAlertView alertView(parent, icon, title, message, checkbox, buttons, defaultButton, size);
	return {static_cast<Button>(alertView.exec()), alertView.isChecked()};
}

PLSAlertView::Result PLSAlertView::open(QWidget *parent, Icon icon, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons, Button defaultButton,
					const QSize &size)
{
	PLSAlertView alertView(parent, icon, title, message, checkbox, buttons, defaultButton, size);
	return {static_cast<Button>(alertView.exec()), alertView.isChecked()};
}

PLSAlertView::Button PLSAlertView::open(Icon icon, const QString &title, const QString &messageTitle, const QString &messageContent, QWidget *parent, Buttons buttons,
					Button defaultButton /*= Button::NoButton*/, const QSize &sugsize /*= QSize()*/)
{
	PLSAlertView alertView(icon, title, messageTitle, messageContent, parent, buttons, defaultButton, sugsize);
	return static_cast<Button>(alertView.exec());
}

PLSAlertView::Button PLSAlertView::information(QWidget *parent, const QString &title, const QString &message, Buttons buttons, Button defaultButton)
{
	return open(parent, Icon::Information, title, message, buttons, defaultButton, QSize());
}

PLSAlertView::Button PLSAlertView::information(QWidget *parent, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton)
{
	return open(parent, Icon::Information, title, message, buttons, defaultButton, QSize());
}

PLSAlertView::Result PLSAlertView::information(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, Buttons buttons, Button defaultButton)
{
	return open(parent, Icon::Information, title, message, checkbox, buttons, defaultButton, QSize());
}

PLSAlertView::Result PLSAlertView::information(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons, Button defaultButton)
{
	return open(parent, Icon::Information, title, message, checkbox, buttons, defaultButton, QSize());
}

PLSAlertView::Button PLSAlertView::question(QWidget *parent, const QString &title, const QString &message, Buttons buttons, Button defaultButton)
{
	return open(parent, Icon::Question, title, message, buttons, defaultButton, QSize());
}

PLSAlertView::Button PLSAlertView::question(QWidget *parent, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton)
{
	return open(parent, Icon::Question, title, message, buttons, defaultButton, QSize());
}

PLSAlertView::Result PLSAlertView::question(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, Buttons buttons, Button defaultButton)
{
	return open(parent, Icon::Question, title, message, checkbox, buttons, defaultButton, QSize());
}

PLSAlertView::Result PLSAlertView::question(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons, Button defaultButton)
{
	return open(parent, Icon::Question, title, message, checkbox, buttons, defaultButton, QSize());
}

PLSAlertView::Button PLSAlertView::question(const QString &title, const QString &messageTitle, const QString &messageContent, QWidget *parent, PLSAlertView::Buttons buttons,
					    Button defaultButton /*= Button::NoButton*/)
{
	return open(Icon::Question, title, messageTitle, messageContent, parent, buttons, defaultButton, QSize());
}

PLSAlertView::Button PLSAlertView::warning(QWidget *parent, const QString &title, const QString &message, Buttons buttons, Button defaultButton)
{
	return open(parent, Icon::Warning, title, message, buttons, defaultButton, QSize());
}

PLSAlertView::Button PLSAlertView::warning(QWidget *parent, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton)
{
	return open(parent, Icon::Warning, title, message, buttons, defaultButton, QSize());
}

PLSAlertView::Result PLSAlertView::warning(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, Buttons buttons, Button defaultButton)
{
	return open(parent, Icon::Warning, title, message, checkbox, buttons, defaultButton, QSize());
}

PLSAlertView::Result PLSAlertView::warning(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons, Button defaultButton)
{
	return open(parent, Icon::Warning, title, message, checkbox, buttons, defaultButton, QSize());
}

PLSAlertView::Button PLSAlertView::critical(QWidget *parent, const QString &title, const QString &message, Buttons buttons, Button defaultButton)
{
	return open(parent, Icon::Critical, title, message, buttons, defaultButton, QSize());
}

PLSAlertView::Button PLSAlertView::critical(QWidget *parent, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton)
{
	return open(parent, Icon::Critical, title, message, buttons, defaultButton, QSize());
}

PLSAlertView::Result PLSAlertView::critical(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, Buttons buttons, Button defaultButton)
{
	return open(parent, Icon::Critical, title, message, checkbox, buttons, defaultButton, QSize());
}

PLSAlertView::Result PLSAlertView::critical(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons, Button defaultButton)
{
	return open(parent, Icon::Critical, title, message, checkbox, buttons, defaultButton, QSize());
}

bool PLSAlertView::isChecked() const
{
	return checkBox ? checkBox->isChecked() : false;
}

Qt::TextFormat PLSAlertView::getTextFormat() const
{
	return ui->message->textFormat();
}

void PLSAlertView::setTextFormat(Qt::TextFormat format)
{
	ui->message->setTextFormat(format);
}

PLSAlertView::Icon PLSAlertView::getIcon() const
{
	return icon;
}

void PLSAlertView::setIcon(Icon icon)
{
	this->icon = icon;

	QString iconPath;
	switch (icon) {
	case Icon::Information:
		break;
	case Icon::Question:
		break;
	case Icon::Warning:
		break;
	case Icon::Critical:
		break;
	}

	if (!iconPath.isEmpty()) {
		QPixmap pixmap(iconPath);
		ui->icon->setPixmap(pixmap);
		ui->icon->setFixedSize(pixmap.width(), pixmap.height());
		ui->icon->show();
	} else {
		ui->icon->hide();
	}
}

void PLSAlertView::onButtonClicked(QAbstractButton *button)
{
	done(ui->buttonBox->standardButton(button));
}

void PLSAlertView::showEvent(QShowEvent *event)
{
	PLS_INFO("alert-view", "UI: [ALERT] %s%s", ui->message->text().toUtf8().constData(), ui->nameLabel->text().toUtf8().constData());
	PLSDialogView::showEvent(event);
}
