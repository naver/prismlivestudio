#include "DefaultPlatformsAddList.h"
#include <QLabel>
#include <QToolButton>
#include "ChannelCommonFunctions.h"
#include "ChannelConst.h"
#include "PLSChannelDataAPI.h"
#include "PLSChannelsVirualAPI.h"
#include "ui_DefaultPlatformsAddList.h"
using namespace ChannelData;

DefaultPlatformsAddList::DefaultPlatformsAddList(QWidget *parent) : QFrame(parent), ui(new Ui::DefaultPlatformsAddList)
{
	ui->setupUi(this);
	initUi();
}

DefaultPlatformsAddList::~DefaultPlatformsAddList()
{
	delete ui;
}

void DefaultPlatformsAddList::runBtnCMD()
{
	auto btn = dynamic_cast<QToolButton *>(sender());
	auto cmdStr = getInfoOfObject(btn, g_channelName.toStdString().c_str(), QString("add"));
	runCMD(cmdStr);
}

void DefaultPlatformsAddList::changeEvent(QEvent *e)
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

bool DefaultPlatformsAddList::eventFilter(QObject *watched, QEvent *event)
{
	auto srcBtn = dynamic_cast<QToolButton *>(watched);
	if (srcBtn == nullptr) {
		return false;
	}
	auto hoverBtn = srcBtn->findChild<QToolButton *>();
	if (hoverBtn == nullptr) {
		return false;
	}

	if (!hoverBtn->isEnabled()) {
		return false;
	}

	if (event->type() == QEvent::HoverEnter) {
		hoverBtn->show();
		return true;
	}

	if (event->type() == QEvent::HoverLeave) {
		hoverBtn->hide();
		return true;
	}
	return false;
}

void DefaultPlatformsAddList::initUi()
{
	auto platforms = getDefaultPlatforms();
	int size = platforms.size();
	for (int i = 0; i < size; ++i) {
		const QString &platformName = platforms[i];
		auto btn = new QToolButton;

		btn->setObjectName(platformName);
		btn->setToolButtonStyle(Qt::ToolButtonIconOnly);

		PLSDpiHelper dpiHelper;
		dpiHelper.notifyDpiChanged(btn, [=](double dpi) {
			extern QPixmap paintSvg(const QString &pixmapPath, const QSize &pixSize);

			QString iconPath;
			if (platformName.contains(CUSTOM_RTMP)) {
				iconPath = g_defaultRTMPAddButtonIcon;
			} else {
				iconPath = getPlatformImageFromName(platformName, "btn-mych.+", "\\.svg");
			}
			QIcon icon;
			icon.addPixmap(paintSvg(iconPath, PLSDpiHelper::calculate(dpi, QSize(115, 38))), QIcon::Normal);
			btn->setIcon(icon);
		});

		btn->installEventFilter(this);

		auto hoverBtn = new QToolButton(btn);
		hoverBtn->setObjectName("hoverBtn");

		hoverBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
		hoverBtn->setProperty(g_channelName.toStdString().c_str(), platformName);
		hoverBtn->hide();
		connect(hoverBtn, &QToolButton::clicked, this, &DefaultPlatformsAddList::runBtnCMD, Qt::QueuedConnection);
		if (i == size - 1) {
			auto spaceLabel = new QLabel(this);
			spaceLabel->setObjectName("spaceLabel");
			ui->DefaultPlatformsLayout->addWidget(spaceLabel);
		}
		ui->DefaultPlatformsLayout->addWidget(btn);
		if (i == size - 1) {
			auto spaceLabel = new QLabel(this);
			spaceLabel->setObjectName("spaceLabel");
			ui->DefaultPlatformsLayout->addWidget(spaceLabel);
		}
	}
}
