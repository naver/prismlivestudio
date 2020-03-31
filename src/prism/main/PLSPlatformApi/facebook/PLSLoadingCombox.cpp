#include "PLSLoadingCombox.h"
#include <QMouseEvent>
#include <QTimer>
#include <QVBoxLayout>
#include "ui_PLSLoadingCombox.h"
#include "../../frontend-api/frontend-api.h"

PLSLoadingCombox::PLSLoadingCombox(QWidget *parent, PLSDpiHelper dpiHelper) : QPushButton(parent), ui(new Ui::PLSLoadingCombox)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSLoadingCombox});
	setDefault(false);
	setAutoDefault(false);
	ui->setupUi(this);
	m_menu = new PLSLoadingComboxMenu(this);
	connect(m_menu, &PLSLoadingComboxMenu::aboutToHide, [=]() { showComboListView(false); });
	connect(m_menu, &PLSLoadingComboxMenu::aboutToShow, [=]() { showComboListView(true); });
	connect(m_menu, &PLSLoadingComboxMenu::itemDidSelect, this, &PLSLoadingCombox::itemDidSelect);
	this->installEventFilter(this);
	pls_flush_style(ui->titleLabel);
}

PLSLoadingCombox::~PLSLoadingCombox()
{
	delete ui;
}

void PLSLoadingCombox::showLoadingView()
{
	m_menu->showLoading(this);
	showMenuView();
}

void PLSLoadingCombox::showTitlesView(QStringList list)
{
	updateTitlesView(list);
	showMenuView();
}

void PLSLoadingCombox::showTitleIdsView(QStringList list, QStringList titleIds)
{
	updateTitleIdsView(list, titleIds);
	showMenuView();
}

void PLSLoadingCombox::updateTitlesView(QStringList titles)
{
	QString title = getComboBoxTitle();
	int index = MENU_DONT_SELECTED_INDEX;
	if (titles.contains(title)) {
		index = titles.indexOf(title);
	}
	m_menu->showTitleListView(this, index, titles);
}

void PLSLoadingCombox::updateTitleIdsView(QStringList titles, QStringList titleIds)
{
	if (titles.size() != titleIds.size()) {
		return;
	}
	int index = 0;
	if (titleIds.contains(m_id)) {
		index = titleIds.indexOf(m_id);
	} else {
		index = MENU_DONT_SELECTED_INDEX;
	}
	m_menu->showTitleListView(this, index, titles);
}

void PLSLoadingCombox::refreshGuideView(QString guide)
{
	m_menu->showGuideTipView(this, guide);
}

void PLSLoadingCombox::hidenMenuView()
{
	m_menu->hide();
}

void PLSLoadingCombox::setComboBoxTitleData(const QString title, const QString id)
{
	m_title = title;
	m_id = id;
	QFontMetrics fontMetric(ui->titleLabel->font());
	QString str = fontMetric.elidedText(m_title, Qt::ElideRight, ui->titleLabel->width());
	ui->titleLabel->setText(str);
}

void PLSLoadingCombox::showMenuView()
{
	QPoint p = this->mapToGlobal(QPoint(0, this->height()));
	m_menu->exec(p);
}

QString &PLSLoadingCombox::getComboBoxTitle()
{
	return m_title;
}

QString &PLSLoadingCombox::getComboBoxId()
{
	return m_id;
}

bool PLSLoadingCombox::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type() == QEvent::LayoutRequest) {
		QFontMetrics fontMetric(ui->titleLabel->font());
		QString str = fontMetric.elidedText(m_title, Qt::ElideRight, ui->titleLabel->width());
		ui->titleLabel->setText(str);
	} else if (event->type() == QEvent::EnabledChange) {
		if (isEnabled()) {
			ui->titleLabel->setStyleSheet("color:white;");
		} else {
			ui->titleLabel->setStyleSheet("color:#666666;");
		}
	}
	return QPushButton::eventFilter(watched, event);
}

void PLSLoadingCombox::showComboListView(bool show)
{
	this->setChecked(show);
	if (isEnabled()) {
		ui->titleLabel->setStyleSheet("color:white;");
	} else {
		ui->titleLabel->setStyleSheet("color:#666666;");
	}
	pls_flush_style(this);
}

void PLSLoadingCombox::itemDidSelect(QListWidgetItem *item)
{
	PLSLoadingComboxItemData data = item->data(Qt::UserRole).value<PLSLoadingComboxItemData>();
	if (data.type != PLSLoadingComboxItemType::Fa_NormalTitle) {
		return;
	}
	m_menu->hide();
	emit clickItemIndex(data.showIndex);
}
