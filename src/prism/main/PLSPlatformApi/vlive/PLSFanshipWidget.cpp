#include "PLSFanshipWidget.h"
#include <qdebug.h>
#include <QEvent>
#include <QHBoxLayout>
#include "ChannelCommonFunctions.h"
#include "PLSDpiHelper.h"
#include "ui_PLSFanshipWidget.h"

const static int itemHeight = 40;

PLSFanshipWidget::PLSFanshipWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PLSFanshipWidget)
{
	ui->setupUi(this);
	PLSDpiHelper dpiHelper;
	dpiHelper.setCss(this, {PLSCssIndex::PLSFanshipWidget});
}

PLSFanshipWidget::~PLSFanshipWidget()
{
	delete ui;
}

void PLSFanshipWidget::setupDatas(const vector<PLSVLiveFanshipData> &datas, bool isSchedule)
{
	m_isSchedule = isSchedule;
	m_vecDatas.clear();
	m_vecDatas = datas;

	bool isFirstSelect = false;
	for (int i = 0; i < m_vecDatas.size(); i++) {
		PLSVLiveFanshipData &data = m_vecDatas[i];
		if (i == 0 && data.isChecked) {
			isFirstSelect = true;
		}

		if (i != 0 && isFirstSelect) {
			data.isChecked = true;
			data.uiEnabled = false;
		}
		if (m_vecViews.size() > i) {
			PLSFanshipWidgetItem *itemView = dynamic_cast<PLSFanshipWidgetItem *>(m_vecViews[i]);
			if (itemView != nullptr) {
				itemView->updateData(data);
			}
		} else {
			int myWidth = 547;
			PLSFanshipWidgetItem *itemView = new PLSFanshipWidgetItem(data, myWidth, this);
			this->layout()->addWidget(itemView);
			itemView->setProperty("index", i);
			m_vecViews.push_back(itemView);
			connect(itemView, &PLSFanshipWidgetItem::checkBoxClick, this, &PLSFanshipWidget::updateSubViewBySelectIndex);

			connect(itemView, &PLSFanshipWidgetItem::checkBoxClick, this, [=](int index) { emit shipBoxClick(index); });
		}
	}

	this->layout()->removeWidget(ui->detailLabel);
	this->layout()->addWidget(ui->detailLabel);
	ui->detailLabel->setHidden(!isSchedule);

	this->layout()->removeItem(ui->verticalSpacer);
	this->layout()->addItem(ui->verticalSpacer);

	this->setHidden(datas.size() == 1);
}

void PLSFanshipWidget::getChecked(vector<bool> &checks)
{
	for (auto data : m_vecDatas) {
		checks.push_back(data.isChecked);
	}
}

void PLSFanshipWidget::updateSubViewBySelectIndex(int index)
{
	for (int i = 0; i < m_vecDatas.size(); i++) {
		auto &data = m_vecDatas[i];

		PLSFanshipWidgetItem *itemView = dynamic_cast<PLSFanshipWidgetItem *>(m_vecViews[i]);
		if (itemView == nullptr) {
			break;
		}

		if (i == index) {
			data.isChecked = !data.isChecked;
			itemView->updateData(data);
		}
		if (i != 0 && index == 0) {
			data.uiEnabled = !m_vecDatas[0].isChecked;

			if (m_vecDatas[0].isChecked == true) {
				data.isChecked = true;
			}
			itemView->updateData(data);
		}
	}
}

PLSFanshipWidgetItem::PLSFanshipWidgetItem(const PLSVLiveFanshipData data, int superWidth, QWidget *parent) : QWidget(parent), _data(data), m_superWidth(superWidth)
{ //QString text = data.channelName;

	QHBoxLayout *HLayout = new QHBoxLayout(this);
	HLayout->setSpacing(8);
	HLayout->setContentsMargins(0, 0, 0, 0);

	titleBox = new QCheckBox();
	titleBox->setObjectName("titleBox");

	badgeLabel = new QLabel();
	badgeLabel->setObjectName("badgeLabel");

	HLayout->addWidget(titleBox);
	HLayout->addWidget(badgeLabel);

	QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	HLayout->addItem(horizontalSpacer);
	updateData(data);

	connect(titleBox, &QCheckBox::clicked, this, [=]() {
		int selectIndex = this->property("index").toInt();
		emit checkBoxClick(selectIndex);
	});
}

PLSFanshipWidgetItem::~PLSFanshipWidgetItem() {}
void PLSFanshipWidgetItem::updateData(const PLSVLiveFanshipData &data)
{
	const static int boxWidth = 25;
	const static int labelSpace = 8;
	const static int badgePadding = 10;

	int realTotalWidth = m_superWidth - boxWidth;
	badgeLabel->adjustSize();
	QFontMetrics fontMetrics(badgeLabel->font());
	int badgeWidth = fontMetrics.width(data.badgeName);
	badgeWidth = badgeWidth == 0 ? 0 : (badgeWidth + badgePadding);

	titleBox->adjustSize();
	QFontMetrics titleFontMetrics(titleBox->font());
	int titleWidth = titleFontMetrics.width(data.channelName);

	bool isReachRight = false;
	if (badgeWidth == 0) {
		titleWidth = realTotalWidth;
	} else {
		int tempAllWidth = titleWidth + labelSpace + badgeWidth;
		if (tempAllWidth > realTotalWidth) {
			isReachRight = true;
			//both than mid
			if (badgeWidth > (realTotalWidth - labelSpace) * 0.5 && titleWidth > (realTotalWidth - labelSpace) * 0.5) {
				badgeWidth = (realTotalWidth - labelSpace) * 0.5;
				titleWidth = badgeWidth;
			} else if (badgeWidth > titleWidth) {
				badgeWidth = realTotalWidth - titleWidth - labelSpace;
			} else {
				titleWidth = realTotalWidth - badgeWidth - labelSpace;
			}
		}
	}

	QFontMetrics badgeFont(badgeLabel->font());
	QString elidedTextBadge = badgeFont.elidedText(data.badgeName, Qt::ElideRight, badgeWidth - badgePadding);
	badgeLabel->setText(elidedTextBadge);
	badgeLabel->setHidden(badgeLabel->text().isEmpty());

	if (isReachRight) {
		badgeWidth = badgeFont.width(badgeLabel->text()) + badgePadding;
		titleWidth = realTotalWidth - labelSpace - badgeWidth;
	}

	QFontMetrics titleFont(titleBox->font());
	QString elidedText = titleFont.elidedText(data.channelName, Qt::ElideRight, titleWidth);
	titleBox->setText(elidedText);

	titleBox->setChecked(data.isChecked);
	titleBox->setToolTip(data.channelName);

	badgeLabel->setToolTip(data.badgeName);

	setItemEnable(data.uiEnabled);
}

void PLSFanshipWidgetItem::enterEvent(QEvent *event)
{
	this->setProperty("status", "hover");
	QWidget::enterEvent(event);
}

void PLSFanshipWidgetItem::setItemEnable(bool enable)
{
	titleBox->setEnabled(enable);
	badgeLabel->setEnabled(enable);
}

void PLSFanshipWidgetItem::leaveEvent(QEvent *event)
{
	this->setProperty("status", "normal");
	QWidget::leaveEvent(event);
}
