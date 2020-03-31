#include "PLSScheduleComboxMenu.h"
#include <qdebug.h>
#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QListWidget>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWidgetAction>
#include "./common/PLSDateFormate.h"
#include "PLSDpiHelper.h"

PLSScheduleComboxMenu::PLSScheduleComboxMenu(QWidget *parent) : QMenu(parent)
{
	m_listWidget = new QListWidget(this);

	connect(m_listWidget, &QListWidget::itemClicked, this, &PLSScheduleComboxMenu::itemDidSelect);

	m_listWidget->setObjectName("scheduleListWidget");

	QHBoxLayout *layout = new QHBoxLayout(m_listWidget);
	layout->setContentsMargins(0, 0, 0, 0);
	m_listWidget->setLayout(layout);
	QWidgetAction *action = new QWidgetAction(this);
	action->setDefaultWidget(m_listWidget);
	this->addAction(action);
	this->installEventFilter(this);
	this->setWindowFlag(Qt::NoDropShadowWindowHint);
	setupTimer();
}

void PLSScheduleComboxMenu::setupDatas(const vector<PLSScheComboxItemData> &datas, int btnWidth)
{
	m_pLeftTimer->stop();
	m_listWidget->clear();
	m_vecItems.clear();

	int ignoreCount = 0;
	bool isNeedStartTimer = false;

	for (auto &itemData : datas) {
		bool isItemIgnore = false;
		QString showStr = PLSScheduleComboxMenu::getDetailTime(itemData, isItemIgnore);
		if (isItemIgnore || itemData.type == PLSScheComboxItemType::Ty_Placehoder) {
			ignoreCount++;
			continue;
		}
		if (itemData.needShowTimeLeftTimer) {
			isNeedStartTimer = true;
		}
		addAItem(showStr, itemData, btnWidth);
	}

	const static int maxShowCount = 5;
	int realShowCount = datas.size() - ignoreCount;
	if (realShowCount == 0) {
		for (auto &itemData : datas) {
			if (itemData.type != PLSScheComboxItemType::Ty_Placehoder) {
				continue;
			}
			addAItem(itemData.time, itemData, btnWidth);
		}
		realShowCount = 1;
	}

	double dpi = PLSDpiHelper::getDpi(this);
	int currentCount = realShowCount > maxShowCount ? maxShowCount : realShowCount;
	m_listWidget->setFixedSize(btnWidth, currentCount * PLSDpiHelper::calculate(dpi, 70) + PLSDpiHelper::calculate(dpi, 4));
	this->setFixedSize(btnWidth, currentCount * PLSDpiHelper::calculate(dpi, 70) + PLSDpiHelper::calculate(dpi, 4));

	if (isNeedStartTimer && m_pLeftTimer && !m_pLeftTimer->isActive()) {
		m_pLeftTimer->start();
	}
}

void PLSScheduleComboxMenu::addAItem(const QString &showStr, const PLSScheComboxItemData &itemData, int superWidth)
{
	QListWidgetItem *item = new QListWidgetItem();
	item->setData(Qt::UserRole, QVariant::fromValue<PLSScheComboxItemData>(itemData));
	PLSScheduleComboxItem *widget = new PLSScheduleComboxItem(itemData, superWidth);
	widget->setDetailLabelStr(showStr);
	m_listWidget->addItem(item);
	m_listWidget->setItemWidget(item, widget);
	m_vecItems.push_back(widget);
}

void PLSScheduleComboxMenu::updateDetailLabel()
{
	vector<QString> ids;
	for (PLSScheduleComboxItem *menuItem : m_vecItems) {
		PLSScheComboxItemData data = menuItem->getData();
		if (!data.needShowTimeLeftTimer) {
			continue;
		}
		bool willDelete = false;
		QString showStr = PLSScheduleComboxMenu::getDetailTime(data, willDelete);
		menuItem->setDetailLabelStr(showStr);
		if (willDelete && !data._id.isEmpty()) {
			ids.push_back(data._id);
		}
	}
	if (ids.size() > 0) {
		emit scheduleItemExpired(ids);
	}
}

void PLSScheduleComboxMenu::setupTimer()
{
	m_pLeftTimer = new QTimer();
	m_pLeftTimer->setInterval(1000);
	connect(m_pLeftTimer, SIGNAL(timeout()), this, SLOT(updateDetailLabel()));
}

void PLSScheduleComboxMenu::itemDidSelect(QListWidgetItem *item)
{
	PLSScheComboxItemData data = item->data(Qt::UserRole).value<PLSScheComboxItemData>();
	emit scheduleItemClicked(data._id);
	this->setHidden(true);
}

bool PLSScheduleComboxMenu::eventFilter(QObject *i_Object, QEvent *i_Event)
{
	if (i_Object == this && i_Event->type() == QEvent::KeyPress) {
		QKeyEvent *key = static_cast<QKeyEvent *>(i_Event);

		if ((key->key() != Qt::Key_Enter) && (key->key() != Qt::Key_Return)) {
			return true;
		}

		const QPoint localPoint = m_listWidget->viewport()->mapFromGlobal(QCursor::pos());
		QListWidgetItem *itemLocal = m_listWidget->itemAt(localPoint);
		if (itemLocal != nullptr) {
			itemDidSelect(itemLocal);
		} else {
			this->setHidden(true);
		}
		return true;
	} else if (i_Object == this && (i_Event->type() == QEvent::MouseButtonPress || i_Event->type() == QEvent::MouseButtonDblClick)) {
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(i_Event);
		QPoint superPoint = parentWidget()->mapToGlobal(QPoint(0, 0));
		QRect superRect(superPoint.x(), superPoint.y(), parentWidget()->width(), parentWidget()->height());
		if (superRect.contains(mouseEvent->globalPos())) {
			QTimer::singleShot(1, [this] { this->setHidden(true); });
			return true;
		} else {
			return QWidget::eventFilter(i_Object, i_Event);
		}
	} else {
		return QWidget::eventFilter(i_Object, i_Event);
	}
}

QString PLSScheduleComboxMenu::getDetailTime(const PLSScheComboxItemData &data, bool &willDelete)
{
	willDelete = false;
	if (data.needShowTimeLeftTimer == false) {
		return data.time;
	}

	QString timeString;
	long startTime = data.isVliveUpcoming ? data.timeStamp : data.endTimeStamp;
	long nowTime = PLSDateFormate::getNowTimeStamp();

	if (data.isVliveUpcoming) {
		//not have broadcasted
		if (startTime - nowTime > 0 && startTime - nowTime < 10 * 60) {
			//Within 10 minutes， start timer.
			long leftTimeStamp = startTime - nowTime;
			QString leftTime = formateTheLeftTime(leftTimeStamp);
			timeString = leftTime.append(" left");
		} else {
			//the normal time
			timeString = data.time;
		}
	} else {
		//already broadcasted, the startTime is the last broadcast stoped time
		if (nowTime - startTime < 10 * 60) {
			//when broadcast stoped, start 10min stoped.
			long leftTimeStamp = 10 * 60 - (nowTime - startTime);
			QString leftTime = formateTheLeftTime(leftTimeStamp);
			timeString = tr("LiveInfo.vlive.schedule.Recently.streamed").arg(leftTime);
		} else {
			//when 10 min later, delete it.
			willDelete = true;
			QString leftTime = formateTheLeftTime(0);
			timeString = tr("LiveInfo.vlive.schedule.Recently.streamed").arg(leftTime);
		}
	}
	return timeString;
}

QString PLSScheduleComboxMenu::formateTheLeftTime(long leftTimeStamp)
{
	char szText[56] = {0};
	_snprintf(szText, sizeof(szText), "%02ld:%02ld", leftTimeStamp / 60, leftTimeStamp % 60);
	return QString(szText);
}

static const QString kTitleLabelObjectName = "titleLabel";

PLSScheduleComboxMenu::~PLSScheduleComboxMenu() {}

PLSScheduleComboxItem::PLSScheduleComboxItem(const PLSScheComboxItemData data, int superWidth, QWidget *parent) : QWidget(parent), _data(data), detailLabel(new QLabel()), m_superWidth(superWidth)
{
	if (data.type == PLSScheComboxItemType::Ty_Loading) {
		QHBoxLayout *hbl = new QHBoxLayout(this);
		titleLabel = new QLabel(_data.title);
		titleLabel->setObjectName("loadingTitleLabel");
		setupTimer();
		titleLabel->setText("");
		m_pTimer->start();
		hbl->addWidget(titleLabel);

		loadingDetailLabel = new QLabel(_data.time);
		loadingDetailLabel->setObjectName("loadingDetailLabel");
		hbl->addWidget(loadingDetailLabel);
		hbl->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
		this->setLayout(hbl);
	} else {
		QVBoxLayout *vbl = new QVBoxLayout(this);
		vbl->setSpacing(4);
		vbl->setContentsMargins(12, 0, 0, 0);
		titleLabel = new QLabel();
		titleLabel->setObjectName(kTitleLabelObjectName);
		titleLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeading | Qt::AlignLeft);
		titleLabel->setText(GetNameElideString());
		vbl->addWidget(titleLabel);

		detailLabel = new QLabel(_data.time);
		detailLabel->setObjectName("detailLabel");
		this->detailLabel->setStyleSheet("color:#777777");
		detailLabel->setAlignment(Qt::AlignTop | Qt::AlignLeading | Qt::AlignLeft);
		vbl->addWidget(detailLabel);
		detailLabel->setHidden(data.type == PLSScheComboxItemType::Ty_NormalLive || detailLabel->text().isEmpty());
		this->setLayout(vbl);

		if (detailLabel->isHidden()) {
			titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeading | Qt::AlignLeft);
		}
	}
}

PLSScheduleComboxItem::~PLSScheduleComboxItem() {}

void PLSScheduleComboxItem::setDetailLabelStr(const QString &str)
{
	if (detailLabel != nullptr && !detailLabel->isHidden()) {
		detailLabel->setText(str);
	}
}

void PLSScheduleComboxItem::enterEvent(QEvent *event)
{
	QFontMetrics fontWidth(titleLabel->font());
	QWidget::enterEvent(event);
}

void PLSScheduleComboxItem::leaveEvent(QEvent *event)
{
	QWidget::leaveEvent(event);
}

void PLSScheduleComboxItem::setupTimer()
{
	m_pTimer = new QTimer();
	m_pTimer->setInterval(300);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(updateProgress()));
}

void PLSScheduleComboxItem::updateProgress()
{
	static int showIndex = 0;
	showIndex = showIndex > 7 ? 1 : ++showIndex;
	titleLabel->setStyleSheet(QString("image: url(\":/images/loading-%1.svg\")").arg(showIndex));
	QApplication::processEvents();
}

void PLSScheduleComboxItem::resizeEvent(QResizeEvent *event)
{
	if (titleLabel == nullptr || titleLabel->objectName() != kTitleLabelObjectName) {
		__super::resizeEvent(event);
		return;
	}
	titleLabel->setText(GetNameElideString());
	__super::resizeEvent(event);
}

QString PLSScheduleComboxItem::GetNameElideString()
{
	double dpi = PLSDpiHelper::getDpi(this);
	QFontMetrics fontWidth(titleLabel->font());
	QString elidedText = fontWidth.elidedText(_data.title, Qt::ElideRight, m_superWidth - PLSDpiHelper::calculate(dpi, 50));
	return elidedText;
}
