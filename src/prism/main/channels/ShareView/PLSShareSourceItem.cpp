#include "PLSShareSourceItem.h"

#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QEvent>
#include <QPainter>
#include <QUrl>

#include "ChannelCommonFunctions.h"
#include "ChannelConst.h"
#include "LogPredefine.h"

#include "PLSShareViewConst.h"

using namespace shareView;
using namespace ChannelData;

constexpr int maxUrlLenth = 259;

static inline QPoint operator+(const QPoint &pt, const QSize &sz)
{
	return QPoint(pt.x() + sz.width(), pt.y() + sz.height());
}

PLSShareSourceItem::PLSShareSourceItem(QWidget *parent) : WidgetDpiAdapter(parent), ui(new Ui::ShareSourceItem)
{
	ui->setupUi(this);
	PLSDpiHelper dpiHelper;
	dpiHelper.setCss(this, {PLSCssIndex::PLSShareSourceItem});

	connect(ui->CopyBtn, &QPushButton::toggled, ui->UrlComplexFrame, &CheckedFrame::setChecked);
	connect(ui->CopyBtn, &QPushButton::clicked, this, &PLSShareSourceItem::onCopyPressed);
	connect(ui->GotoBtn, SIGNAL(clicked()), this, SLOT(openUrl()));
	connect(ui->OpenUrlTwitter, SIGNAL(clicked()), this, SLOT(openUrl()));
	connect(ui->OpenUrlFaceBook, SIGNAL(clicked()), this, SLOT(openUrl()));

	ui->UserIconLabel->installEventFilter(this);

	auto centerShow = [=]() { move(parent->pos() + (parent->size() - size()) / 2); };
	notifyFirstShow(centerShow);
	dpiHelper.notifyDpiChanged(this, [=](double, double, bool firstShow) {
		if (!firstShow) {
			centerShow();
		}
	});
}

PLSShareSourceItem::~PLSShareSourceItem()
{
	delete ui;
}

void PLSShareSourceItem::initInfo(const QVariantMap &source)
{
	mSource = source;

	auto disName = getInfo(mSource, g_nickName, g_nickName);
	QString disElidName = getElidedText(ui->DisnameLabel, disName, ui->DisnameLabel->contentsRect().width());
	ui->DisnameLabel->setText(disElidName);
	auto urlStr = getInfo(mSource, g_shareUrlTemp, DEFAULT_URL);
	if (urlStr.isEmpty()) {
		urlStr = getInfo(mSource, g_shareUrl, DEFAULT_URL);
	}
	ui->GotoBtn->setProperty(SHARE_URL_KEY, urlStr);
	QString disTxt = getElidedText(ui->UrlPtn, urlStr, ui->UrlPtn->contentsRect().width());
	ui->UrlPtn->setText(disTxt);

	auto facebookShareUrl = g_facebookShareUrl.arg(urlStr);
	ui->OpenUrlFaceBook->setProperty(SHARE_URL_KEY, facebookShareUrl);

	auto twitterUrl = g_twitterShareUrl.arg(disName + " Broadcast Share ", urlStr);
	ui->OpenUrlTwitter->setProperty(SHARE_URL_KEY, twitterUrl);

	updatePixmap();
}

void PLSShareSourceItem::onCopyPressed()
{
	auto clipboard = QGuiApplication::clipboard();
	QString url = getInfoOfObject(ui->GotoBtn, SHARE_URL_KEY, QString());
	if (url.isEmpty()) {
		return;
	}
	clipboard->setText(url);
	emit UrlCopied();
	ui->CopyBtn->setEnabled(false);
}

void PLSShareSourceItem::openUrl()
{
	auto obj = dynamic_cast<QObject *>(sender());
	if (obj) {
		QString url = getInfoOfObject(obj, SHARE_URL_KEY, QString());
		if (!QDesktopServices::openUrl(url)) {
			qDebug() << " error on openning url :" << url;
		}
	}
}

void PLSShareSourceItem::on_CloseBtn_clicked()
{
	this->close();
}

void PLSShareSourceItem::changeEvent(QEvent *e)
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

void PLSShareSourceItem::updatePixmap()
{
	QString uuid = getInfo(mSource, g_channelUUID);
	QString userHeaderPic;
	QString platformPic;
	getComplexImageOfChannel(uuid, userHeaderPic, platformPic, "", ".*profile");
	ui->UserIconLabel->setPixmap(userHeaderPic, QSize(110, 110));
	ui->UserIconLabel->setPlatformPixmap(platformPic, QSize(34, 34));
	PLSDpiHelper::dpiDynamicUpdate(this);
}
