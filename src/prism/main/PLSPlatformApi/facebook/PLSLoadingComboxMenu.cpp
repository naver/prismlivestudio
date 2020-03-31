#include "PLSLoadingComboxMenu.h"
#include "ui_PLSLoadingComboxMenu.h"
#include <QWidgetAction>
#include "PLSLoadingTitleItem.h"
#include "PLSLoadingMenuItem.h"
#include "../../frontend-api/frontend-api.h"
#include <QMouseEvent>

#define TAG_HEIGHT 40

PLSLoadingComboxMenu::PLSLoadingComboxMenu(QWidget *parent, PLSDpiHelper dpiHelper) : QMenu(parent), ui(new Ui::PLSLoadingComboxMenu)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSLoadingComboxMenu});
	ui->setupUi(this);
	m_listWidget = new QListWidget(this);
	m_listWidget->setObjectName("loadingListWidget");
	connect(m_listWidget, &QListWidget::itemClicked, this, [=](QListWidgetItem *item) { emit itemDidSelect(item); });
	QWidgetAction *action = new QWidgetAction(this);
	action->setDefaultWidget(m_listWidget);
	this->addAction(action);
	this->installEventFilter(this);
	setWindowFlags(windowFlags() | Qt::NoDropShadowWindowHint);
}

PLSLoadingComboxMenu::~PLSLoadingComboxMenu()
{
	delete ui;
}

void PLSLoadingComboxMenu::showLoading(QWidget *parent)
{
	double dpi = PLSDpiHelper::getDpi(this);
	int gameItemHeight = PLSDpiHelper::calculate(dpi, TAG_HEIGHT);
	m_parent = parent;
	m_listWidget->clear();
	PLSLoadingTitleItem *loadingItem = new PLSLoadingTitleItem;
	QListWidgetItem *item = new QListWidgetItem;
	PLSLoadingComboxItemData itemData = PLSLoadingComboxItemData();
	itemData.type = PLSLoadingComboxItemType::Fa_Loading;
	item->setData(Qt::UserRole, QVariant::fromValue<PLSLoadingComboxItemData>(itemData));
	QSize size = item->sizeHint();
	size.setHeight(gameItemHeight);
	item->setSizeHint(size);
	m_listWidget->addItem(item);
	m_listWidget->setItemWidget(item, loadingItem);
	int borderHeight = PLSDpiHelper::calculate(dpi, 1);
	int paddingHeight = PLSDpiHelper::calculate(dpi, 1);
	m_listWidget->setFixedSize(parent->width() - borderHeight * 2, gameItemHeight + paddingHeight * 2);
	this->setFixedSize(parent->width(), gameItemHeight + paddingHeight * 2 + borderHeight * 2);
}

void PLSLoadingComboxMenu::showTitleListView(QWidget *parent, int showIndex, QStringList list)
{
	showTitleItemsView(parent, showIndex, PLSLoadingComboxItemType::Fa_NormalTitle, list);
}

void PLSLoadingComboxMenu::showGuideTipView(QWidget *parent, QString guide)
{
	QStringList list;
	list.append(guide);
	showTitleItemsView(parent, MENU_DONT_SELECTED_INDEX, PLSLoadingComboxItemType::Fa_Guide, list);
}

void PLSLoadingComboxMenu::setSelectedTitleItemIndex(int index)
{
	if (m_items.size() <= index) {
		return;
	}
	for (auto item : m_items) {
		item->setSelected(false);
	}
	PLSLoadingMenuItem *titleItem = m_items.at(index);
	titleItem->setSelected(true);
}

bool PLSLoadingComboxMenu::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress) {
		QRect parentRect(0, 0, m_parent->width(), m_parent->height());
		QMouseEvent *mouseEvent = reinterpret_cast<QMouseEvent *>(event);
		QPoint clickPoint = m_parent->mapFromGlobal(mouseEvent->globalPos());
		if (parentRect.contains(clickPoint)) {
			QTimer::singleShot(1, [=] { this->setHidden(true); });
			return true;
		}
	}
	return QMenu::eventFilter(watched, event);
}

void PLSLoadingComboxMenu::setupCornerRadius()
{
	double dpi = PLSDpiHelper::getDpi(this);
	const int radius = PLSDpiHelper::calculate(dpi, 3);
	QPainterPath path;
	path.addRoundedRect(this->rect(), radius, radius);
	QRegion mask = QRegion(path.toFillPolygon().toPolygon());
	this->setMask(mask);
}

void PLSLoadingComboxMenu::showTitleItemsView(QWidget *parent, int showIndex, PLSLoadingComboxItemType type, QStringList list)
{
	m_parent = parent;
	m_listWidget->clear();
	m_items.clear();
	double dpi = PLSDpiHelper::getDpi(this);
	int gameItemHeight = PLSDpiHelper::calculate(dpi, TAG_HEIGHT);
	for (int i = 0; i < list.size(); i++) {
		QString title = list.at(i);
		PLSLoadingMenuItem *titleItem = new PLSLoadingMenuItem;
		titleItem->setTitle(title);
		QListWidgetItem *item = new QListWidgetItem;
		PLSLoadingComboxItemData itemData = PLSLoadingComboxItemData();
		itemData.showIndex = i;
		itemData.title = title;
		itemData.type = type;
		item->setData(Qt::UserRole, QVariant::fromValue<PLSLoadingComboxItemData>(itemData));
		QSize size = item->sizeHint();
		size.setHeight(gameItemHeight);
		item->setSizeHint(size);
		m_listWidget->addItem(item);
		m_listWidget->setItemWidget(item, titleItem);
		m_items.insert(i, titleItem);
	}
	if (showIndex != MENU_DONT_SELECTED_INDEX) {
		m_listWidget->setCurrentRow(showIndex);
		setSelectedTitleItemIndex(showIndex);
	}
	int count = list.size();
	if (count > 5) {
		count = 5;
	}
	int borderHeight = PLSDpiHelper::calculate(dpi, 1);
	int paddingHeight = PLSDpiHelper::calculate(dpi, 1);
	m_listWidget->setFixedSize(parent->width() - borderHeight * 2, gameItemHeight * count + paddingHeight * 2);
	this->setFixedSize(parent->width(), gameItemHeight * count + paddingHeight * 2 + borderHeight * 2);
	setupCornerRadius();
}
