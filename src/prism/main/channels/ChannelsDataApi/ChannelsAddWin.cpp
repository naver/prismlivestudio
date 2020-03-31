#include "ChannelsAddWin.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QToolButton>
#include "ChannelCommonFunctions.h"
#include "ChannelConst.h"
#include "LogPredefine.h"
#include "PLSChannelDataAPI.h"
#include "PLSChannelsVirualAPI.h"
#include "PLSDpiHelper.h"
#include "ui_ChannelsAddWin.h"

using namespace ChannelData;

ChannelsAddWin::ChannelsAddWin(QWidget *parent) : WidgetDpiAdapter(parent), ui(new Ui::ChannelsAddWin)
{
	ui->setupUi(this);
	PLSDpiHelper dpiHelper;
	dpiHelper.setCss(this, {PLSCssIndex::ChannelsAddWin});
	initDefault();
	updateUi();
}

ChannelsAddWin::~ChannelsAddWin()
{
	delete ui;
}

void ChannelsAddWin::updateUi()
{
	for (int i = 0; i < ui->ItemGridLayout->count(); ++i) {
		updateItem(i);
	}
}

void ChannelsAddWin::updateItem(int index)
{
	auto item = ui->ItemGridLayout->itemAt(index);
	auto widget = dynamic_cast<QToolButton *>(item->widget());
	if (widget) {
		QString platForm = getInfoOfObject(widget, g_channelName.toStdString().c_str(), QString());
		auto &info = PLSCHANNELS_API->getChanelInfoRefByPlatformName(platForm, ChannelType);

		if (info.isEmpty()) {
			widget->setEnabled(true);
			widget->setText(platForm);
			return;
		}
		widget->setEnabled(false);
		widget->setText(CHANNELS_TR(Linked));
	}
}

void ChannelsAddWin::changeEvent(QEvent *e)
{
	QFrame::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

bool ChannelsAddWin::eventFilter(QObject *watched, QEvent *event)
{
	auto srcBtn = dynamic_cast<QToolButton *>(watched);
	if (srcBtn == nullptr) {
		return false;
	}
	auto hoverBtn = srcBtn->findChild<QToolButton *>();
	if (hoverBtn == nullptr) {
		return false;
	}
	if (event->type() == QEvent::HoverEnter && srcBtn->isEnabled()) {
		hoverBtn->show();
		return true;
	}

	if (event->type() == QEvent::HoverLeave) {
		hoverBtn->hide();
		return true;
	}
	return false;
}

void ChannelsAddWin::appendItem(const QString &text)
{
	QToolButton *btn = new QToolButton(ui->scrollAreaWidgetContents);
	btn->setProperty(g_channelName.toStdString().c_str(), text);
	btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	btn->setObjectName(text);
	btn->setText(text);
	btn->installEventFilter(this);

	if (text.contains(CUSTOM_RTMP)) {
		btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
	}

	PLSDpiHelper dpiHelper;
	dpiHelper.notifyDpiChanged(btn, [=](double dpi) {
		extern QPixmap paintSvg(const QString &pixmapPath, const QSize &pixSize);

		QString iconPath, disablePath;
		if (text.contains(CUSTOM_RTMP)) {
			iconPath = g_defaultRTMPAddButtonIcon;
			disablePath = iconPath;
		} else {
			iconPath = getPlatformImageFromName(text, "btn.+", "\\.svg");
			disablePath = getPlatformImageFromName(text, "btn.+", "\\-on.svg");
		}

		QIcon Icon;
		Icon.addPixmap(paintSvg(iconPath, PLSDpiHelper::calculate(dpi, QSize(115, 40))), QIcon::Normal);
		Icon.addPixmap(paintSvg(disablePath, PLSDpiHelper::calculate(dpi, QSize(115, 40))), QIcon::Disabled);
		btn->setIcon(Icon);
	});

	auto hoverBtn = new QToolButton(btn);
	hoverBtn->setObjectName("hoverBtn");
	hoverBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hoverBtn->setProperty(g_channelName.toStdString().c_str(), text);
	hoverBtn->hide();
	connect(hoverBtn, &QToolButton::clicked, this, &ChannelsAddWin::runBtnCMD, Qt::QueuedConnection);

	static int countOfRow = 4;
	auto layout = ui->ItemGridLayout;
	int row = layout->count() / countOfRow;
	int column = layout->count() % countOfRow;
	layout->addWidget(btn, row, column);
}

void ChannelsAddWin::on_ClosePtn_clicked()
{
	this->close();
}

void ChannelsAddWin::initDefault()
{
	for (const QString &platform : getDefaultPlatforms()) {
		appendItem(platform);
	}
	ui->ChannelsListWid->adjustSize();
}

void ChannelsAddWin::runBtnCMD()
{
	PRE_LOG_UI(try add channel, ChannelsAddWin);
	auto btn = dynamic_cast<QToolButton *>(sender());
	auto cmdStr = getInfoOfObject(btn, g_channelName.toStdString().c_str(), QString("add"));
	runCMD(cmdStr);
}
