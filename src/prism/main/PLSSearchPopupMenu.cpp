#include "PLSSearchPopupMenu.h"
#include "ui_PLSSearchPopupMenu.h"
#include "log/module_names.h"
#include "liblog.h"
#include "action.h"
#include <QPushButton>
#include <QMouseEvent>
#include <QTimer>
#include <QAbstractListModel>
#include <pls-app.hpp>
#include <GiphyDefine.h>

PLSSearchPopupMenu::PLSSearchPopupMenu(QWidget *parent) : QFrame(parent), ui(new Ui::PLSSearchPopupMenu)
{
	ui->setupUi(this);
	this->setCursor(Qt::ArrowCursor);
	setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	connect(ui->recommendList, &RecommendSearchList::RecommendItemClicked, this, &PLSSearchPopupMenu::ItemClicked);
	connect(ui->historyList, &HistorySearchList::Clicked, this, &PLSSearchPopupMenu::ItemClicked);
	connect(ui->historyList, &HistorySearchList::ItemDelete, this, &PLSSearchPopupMenu::ItemDelete);
	connect(ui->historyList, &HistorySearchList::UpdateContentHeight, this, &PLSSearchPopupMenu::UpdateContentHeight);
	connect(ui->recommendList, &RecommendSearchList::UpdateContentHeight, this, &PLSSearchPopupMenu::UpdateContentHeight);
	QStringList list;
	list << "Reactions"
	     << "Thanks"
	     << "Hi"
	     << "Love"
	     << "Animals"
	     << "Holidays"
	     << "Emoji";
	ui->recommendList->AddListData(list);
}

PLSSearchPopupMenu::~PLSSearchPopupMenu()
{
	delete ui;
}

void PLSSearchPopupMenu::Show(const QPoint &offset, int contentWidth_)
{
	SetContentWidth(contentWidth_);
	move(offset.x(), offset.y());
	show();
}

void PLSSearchPopupMenu::SetContentWidth(int width)
{
	contentWidth = width;
	QTimer::singleShot(0, this, SLOT(resizeToContent()));
}

void PLSSearchPopupMenu::AddHistoryItem(const QString &keyword)
{
	ui->historyList->AddItem(keyword);
}

void PLSSearchPopupMenu::AddRecommendItems(const QStringList &listwords)
{
	ui->recommendList->AddListData(listwords);
}

void PLSSearchPopupMenu::DpiChanged(double dpi)
{
	ui->recommendList->DpiChanged(dpi);
}

void PLSSearchPopupMenu::UpdateContentHeight()
{
	int left, top, right, bottom;
	ui->verticalLayout->getContentsMargins(&left, &top, &right, &bottom);
	if (ui->historyList->ItemCount() == 0) {
		ui->label_space->hide();
		ui->historyList->hide();
		ui->verticalLayout->setContentsMargins(left, 17, right, bottom);
	} else {
		ui->label_space->show();
		ui->historyList->show();
		ui->verticalLayout->setContentsMargins(left, 11, right, bottom);
	}
	QTimer::singleShot(0, this, SLOT(resizeToContent()));
}

void PLSSearchPopupMenu::resizeToContent()
{
	bool has = this->hasHeightForWidth();
	if (has)
		this->resize(contentWidth, this->heightForWidth(contentWidth));
	else
		this->resize(contentWidth, minimumSizeHint().height());
}

RecommendSearchList::RecommendSearchList(QWidget *parent) : QFrame(parent)
{
	QVBoxLayout *layout_main = new QVBoxLayout(this);
	layout_main->setContentsMargins(0, 5, 0, 0);
	layout_main->setSpacing(8);
	QLabel *labelTitle = new QLabel(this);
	labelTitle->setObjectName("recommendListTitle");
	labelTitle->setAlignment(Qt::AlignLeft);
	labelTitle->setText(QTStr("main.giphy.recommendList.title"));
	flowLayout = new FlowLayout(0, 18, 9);
	layout_main->addWidget(labelTitle);
	layout_main->addItem(flowLayout);
}

RecommendSearchList::~RecommendSearchList() {}

void RecommendSearchList::AddListData(const QStringList &words)
{
	for (QString word : words) {
		QPushButton *item = new QPushButton(word, this);
		item->setFocusPolicy(Qt::NoFocus);
		connect(item, &QPushButton::clicked, [=]() {
			QString key;
			key.sprintf("Recommend search word '%s'", word);
			PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, key.toUtf8().constData(), ACTION_CLICK);
			emit RecommendItemClicked(item->text());
		});
		item->setObjectName("recommendWordLabel");
		flowLayout->addWidget(item);
		listItem << item;
	}
	flowLayout->showLayoutItemWidget();
	emit UpdateContentHeight();
}

void RecommendSearchList::ClearList()
{
	auto iter = listItem.begin();
	while (iter != listItem.end()) {
		flowLayout->removeWidget(*iter);
		delete *iter;
		*iter = nullptr;
		iter = listItem.erase(iter);
	}
	emit UpdateContentHeight();
}

void RecommendSearchList::DpiChanged(double dpi) {}

HistorySearchItem::HistorySearchItem(QWidget *parent) : QFrame(parent)
{
	QPushButton *btn_delete = new QPushButton(this);
	btn_delete->setFocusPolicy(Qt::NoFocus);
	connect(btn_delete, &QPushButton::clicked, [=]() { emit Deleted(this); });
	btn_delete->setObjectName("buttonDelete");

	contentLabel = new QPushButton(this);
	connect(contentLabel, &QPushButton::clicked, [=]() { emit Clicked(content); });
	contentLabel->setFocusPolicy(Qt::NoFocus);
	contentLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	contentLabel->setObjectName("labelContent");

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(6);

	layout->addWidget(contentLabel);
	layout->addWidget(btn_delete);
}

HistorySearchItem::~HistorySearchItem() {}

void HistorySearchItem::SetContent(const QString &text)
{
	content = text;
	OptimizeDisplay();
}

QString HistorySearchItem::GetContent() const
{
	return content;
}

void HistorySearchItem::OptimizeDisplay()
{
	if (contentLabel->fontMetrics().width(content) > contentLabel->width()) {
		QString omited = contentLabel->fontMetrics().elidedText(content, Qt::ElideRight, contentLabel->width());
		contentLabel->setText(omited);
		contentLabel->setToolTip(content);
	} else {
		contentLabel->setText(content);
		contentLabel->setToolTip("");
	}
}

void HistorySearchItem::resizeEvent(QResizeEvent *event)
{
	OptimizeDisplay();
	QFrame::resizeEvent(event);
}

HistorySearchList::HistorySearchList(QWidget *parent) : QFrame(parent)
{
	layout_main = new QVBoxLayout(this);
	layout_main->setAlignment(Qt::AlignTop);
	layout_main->setContentsMargins(0, 0, 0, 0);
	layout_main->setSpacing(3);
}

HistorySearchList::~HistorySearchList() {}

void HistorySearchList::AddItem(const QString &content)
{
	// Find in items to confirm if there is a item whose text is same as content.
	// If exist ,just move it to the front of layout. if not ,add a new one in front of layout.
	if (FindAndMoveToFront(content))
		return;

	HistorySearchItem *item = new HistorySearchItem(this);
	item->SetContent(content);
	connect(item, &HistorySearchItem::Clicked, [=](const QString &content) {
		QString key("history search word: %1");
		key.arg(content);
		PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, key.toUtf8().constData(), ACTION_CLICK);
		emit Clicked(content);
	});
	connect(item, &HistorySearchItem::Deleted, this, &HistorySearchList::OnItemDeleted);

	if (layout_main->count() >= MAX_HISTORY_LIST_COUNT) {
		HistorySearchItem *item_del = qobject_cast<HistorySearchItem *>(layout_main->itemAt(MAX_HISTORY_LIST_COUNT - 1)->widget());
		if (item_del) {
			layout_main->removeWidget(item_del);
			item_del->deleteLater();
		}
	}

	layout_main->insertWidget(0, item);
	emit UpdateContentHeight();
}

void HistorySearchList::AddItems(const QStringList &contents)
{
	for (QString content : contents)
		AddItem(content);
}

int HistorySearchList::ItemCount()
{
	return layout_main->count();
}

bool HistorySearchList::FindAndMoveToFront(const QString &content)
{
	for (int i = 0; i < layout_main->count(); ++i) {
		HistorySearchItem *item = qobject_cast<HistorySearchItem *>(layout_main->itemAt(i)->widget());
		if (item->GetContent() == content) {
			layout_main->removeWidget(item);
			layout_main->insertWidget(0, item);
			return true;
		}
	}
	return false;
}

void HistorySearchList::OnItemDeleted(HistorySearchItem *item)
{
	if (!item)
		return;
	PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, "history search delete button", ACTION_CLICK);
	layout_main->removeWidget(item);
	emit UpdateContentHeight();
	emit ItemDelete(item->GetContent());
	item->deleteLater();
}
