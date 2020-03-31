#include "custom-help-menu-item.hpp"
#include "ui_PLSCustomHelpMenuItem.h"

#define TITLE_DISABLE_COLOR "color:#666666"
#define TITLE_NORMAL_COLOR "color:#ffffff"

CustomHelpMenuItem::CustomHelpMenuItem(const QString &title, QWidget *parent) : QWidget(parent), ui(new Ui::CustomHelpMenuItem)
{
	ui->setupUi(this);
	setText(title);
	setBadgeVisible();
	ui->titleLabel->setStyleSheet(TITLE_NORMAL_COLOR);
	m_disabled = false;
}

CustomHelpMenuItem::~CustomHelpMenuItem()
{
	delete ui;
}

void CustomHelpMenuItem::setText(const QString &text)
{
	ui->titleLabel->setText(text);
}

void CustomHelpMenuItem::setBadgeVisible(bool visible)
{
	ui->badgeNew->setVisible(visible);
}

void CustomHelpMenuItem::setItemDisabled(bool disabled)
{
	m_disabled = disabled;
	if (m_disabled) {
		ui->titleLabel->setStyleSheet(TITLE_DISABLE_COLOR);
	} else {
		ui->titleLabel->setStyleSheet(TITLE_NORMAL_COLOR);
	}
}

void CustomHelpMenuItem::enterEvent(QEvent *event)
{
	if (m_disabled) {
		return;
	}
	ui->titleLabel->setStyleSheet(TITLE_NORMAL_COLOR);
	QWidget::enterEvent(event);
}

void CustomHelpMenuItem::leaveEvent(QEvent *event)
{
	if (m_disabled) {
		return;
	}
	ui->titleLabel->setStyleSheet(TITLE_NORMAL_COLOR);
	QWidget::leaveEvent(event);
}

void CustomHelpMenuItem::mousePressEvent(QMouseEvent *event)
{
	if (m_disabled) {
		return;
	}
	QWidget::mousePressEvent(event);
}

void CustomHelpMenuItem::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_disabled) {
		return;
	}
	QWidget::mouseReleaseEvent(event);
}
