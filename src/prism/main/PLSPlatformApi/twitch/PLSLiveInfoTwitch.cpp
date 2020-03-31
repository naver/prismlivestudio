#include "PLSLiveInfoTwitch.h"

#include <QObject>
#include <QNetworkReply>
#include <QDebug>
#include <QComboBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "PLSHttpApi/PLSHttpHelper.h"
#include "../PLSPlatformApi.h"
#include "../PLSLiveInfoDialogs.h"
#include "alert-view.hpp"
#include "PLSChannelDataAPI.h"
#include "ChannelCommonFunctions.h"

PLSLiveInfoTwitch::PLSLiveInfoTwitch(PLSPlatformBase *pPlatformBase, QWidget *parent, PLSDpiHelper dpiHelper) : PLSLiveInfoBase(pPlatformBase, parent, dpiHelper), ui(new Ui::PLSLiveInfoTwitch())
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSLiveInfoTwitch});
	dpiHelper.setFixedSize(this, {720, 550});
	dpiHelper.notifyDpiChanged(this, [this](double, double, bool firstShow) {
		if (firstShow) {
			QMetaObject::invokeMethod(
				this, [this] { pls_flush_style(ui->lineEditCategory); }, Qt::QueuedConnection);
		}
	});

	ui->setupUi(content());

	connect(ui->pushButtonOk, &QPushButton::clicked, this, &PLSLiveInfoTwitch::doOk);
	connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui->lineEditTitle, &QLineEdit::textChanged, this, &PLSLiveInfoTwitch::titleEdited);
	connect(ui->lineEditCategory, &QLineEdit::textChanged, this, &PLSLiveInfoTwitch::doUpdateOkState);
	connect(ui->comboBoxServers, SIGNAL(currentIndexChanged(int)), SLOT(doUpdateOkState()));

	pushButtonClear = new QPushButton(ui->lineEditCategory);
	pushButtonClear->setObjectName("pushButtonClear");
	pushButtonClear->setCursor(Qt::ArrowCursor);
	pushButtonClear->setFocusPolicy(Qt::NoFocus);

	pushButtonSearch = new QPushButton(ui->lineEditCategory);
	pushButtonSearch->setObjectName("pushButtonSearch");
	pushButtonSearch->setFocusPolicy(Qt::NoFocus);

	auto layoutCategory = new QHBoxLayout(ui->lineEditCategory);
	layoutCategory->setSpacing(9);
	layoutCategory->setContentsMargins(0, 0, 9, 0);
	layoutCategory->addStretch();

	layoutCategory->addWidget(pushButtonClear);
	layoutCategory->addWidget(pushButtonSearch);
	pushButtonClear->hide();

	ui->lineEditCategory->installEventFilter(this);

	listCategory = new PLSLiveInfoTwitchListWidget(this);
	listCategory->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
	listCategory->setAttribute(Qt::WA_ShowWithoutActivating, true);
	listCategory->hide();
	dpiHelper.notifyDpiChanged(listCategory, [=](double dpi) { listCategory->setIconSize(PLSDpiHelper::calculate(dpi, QSize(38, 52))); });

	content()->setFocusPolicy(Qt::StrongFocus);
	connect(qApp, &QApplication::focusChanged, listCategory, [this](const QWidget *old, const QWidget *now) {
		if (ui->lineEditCategory == now) {
			if (!ui->lineEditCategory->text().isEmpty()) {
				pushButtonClear->show();
			}
			if (listCategory->count() > 0) {
				showListCategory();
			}
		} else if (nullptr == now || (listCategory != now && listCategory != now->parent())) {
			pushButtonClear->hide();
			listCategory->hide();
			ui->lineEditCategory->clearFocus();
		}
	});
	connect(ui->lineEditCategory, &QLineEdit::textChanged, this, [this](const QString &text) {
		if (text.isEmpty()) {
			pushButtonClear->hide();

			listCategory->clear();
			listCategory->hide();
		} else {
			if (ui->lineEditCategory->hasFocus() && pushButtonClear->isHidden()) {
				pushButtonClear->show();
			}

			auto pPlatformTwitch = PLS_PLATFORM_TWITCH;
			pPlatformTwitch->requestCategory(text);
		}
	});

	connect(pushButtonClear, &QPushButton::clicked, this, [this]() { ui->lineEditCategory->setText(QString()); });

	connect(listCategory, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
		ui->lineEditCategory->setText(item->text());
		listCategory->hide();
	});

	auto pPlatformTwitch = PLS_PLATFORM_TWITCH;
	pPlatformTwitch->setAlertParent(this);
	connect(pPlatformTwitch, &PLSPlatformTwitch::onGetChannel, this, &PLSLiveInfoTwitch::refreshChannel);
	connect(pPlatformTwitch, &PLSPlatformTwitch::onGetServer, this, &PLSLiveInfoTwitch::refreshServer);
	connect(pPlatformTwitch, &PLSPlatformTwitch::onGetCategory, this, &PLSLiveInfoTwitch::doGetCategory);
	connect(pPlatformTwitch, &PLSPlatformTwitch::onUpdateChannel, this, &PLSLiveInfoTwitch::doUpdateChannel);
	connect(pPlatformTwitch, &PLSPlatformTwitch::closeDialogByExpired, this, &QDialog::reject);

	updateStepTitle(ui->pushButtonOk);
	doUpdateOkState();

	if (PLS_PLATFORM_API->isPrepareLive()) {
		ui->horizontalLayout->addWidget(ui->pushButtonOk);
	}
}

PLSLiveInfoTwitch::~PLSLiveInfoTwitch()
{
	delete ui;
}

void PLSLiveInfoTwitch::refreshChannel(PLSPlatformApiResult value)
{
	if (PLSPlatformApiResult::PAR_SUCCEED != value) {
		hideLoading();

		if (!PLS_PLATFORM_API->isPrepareLive()) {
			reject();
		}

		return;
	}

	auto pPlatformTwitch = PLS_PLATFORM_TWITCH;
	auto title = QString::fromStdString(pPlatformTwitch->getTitle());
	auto category = QString::fromStdString(pPlatformTwitch->getCategory());
	ui->lineEditTitle->setText(title);
	ui->lineEditCategory->setText(category);
	auto uuid = pPlatformTwitch->getChannelUUID();
	PLSCHANNELS_API->setValueOfChannel(uuid, ChannelData::g_catogry, category);
	ui->lineEditTitle->setCursorPosition(0);
}

void PLSLiveInfoTwitch::refreshServer(PLSPlatformApiResult value)
{
	hideLoading();
	if (PLSPlatformApiResult::PAR_SUCCEED != value) {
		if (!PLS_PLATFORM_API->isPrepareLive()) {
			reject();
		}

		return;
	}

	auto pPlatformTwitch = PLS_PLATFORM_TWITCH;
	ui->comboBoxServers->clear();
	for (const auto &item : pPlatformTwitch->getServerNames()) {
		if (item._Starts_with("DEPRECATED")) {
			continue;
		}
		ui->comboBoxServers->addItem(QString::fromStdString(item));
	}

	if (pPlatformTwitch->getServerIndex() < ui->comboBoxServers->count()) {
		ui->comboBoxServers->setCurrentIndex(pPlatformTwitch->getServerIndex());
	}

	ui->comboBoxServers->setEnabled(!PLS_PLATFORM_API->isLiving());
}

void PLSLiveInfoTwitch::showListCategory()
{
	listCategory->move(content()->mapToGlobal({ui->lineEditCategory->x(), ui->lineEditCategory->y() + ui->lineEditCategory->height()}));
	listCategory->resize(ui->lineEditCategory->width(), PLSDpiHelper::calculate(this, 56) * min(5, listCategory->count()) + PLSDpiHelper::calculate(this, 3));
	listCategory->show();
}

void PLSLiveInfoTwitch::doOk()
{
	if (isModified()) {
		showLoading(content());

		auto pPlatformTwitch = PLS_PLATFORM_TWITCH;
		auto category = ui->lineEditCategory->text();
		pPlatformTwitch->saveSettings(ui->lineEditTitle->text().toStdString(), category.toStdString(), ui->comboBoxServers->currentIndex());
		PLSCHANNELS_API->setValueOfChannel(pPlatformTwitch->getChannelUUID(), ChannelData::g_catogry, category);
		return;
	}

	accept();
}

void PLSLiveInfoTwitch::doGetCategory(QJsonObject root, const QString &request)
{
	if (root.isEmpty() || ui->lineEditCategory->text() != request) {
		return;
	}

	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(code)

		if (data.isEmpty()) {
			return;
		}

		auto item = reinterpret_cast<QLabel *>(context);
		if (nullptr != item) {
			QPixmap icon;
			icon.loadFromData(data);
			item->setPixmap(icon);
		}
	};

	listCategory->clear();

	auto games = root["games"].toArray();
	for (auto item : games) {
		auto game = item.toObject();
		auto id = game["_id"].toInt();
		auto name = game["name"].toString();
		auto url = game["box"].toObject()["small"].toString();

		auto item = new QListWidgetItem(name);

		auto root = new QWidget();
		auto layout = new QHBoxLayout(root);

		layout->setContentsMargins(QMargins());
		layout->setSpacing(PLSDpiHelper::calculate(PLSDpiHelper::getDpi(this), 10));

		auto icon = new QLabel();
		icon->setObjectName("categoryIconLabel");
		icon->setAttribute(Qt::WA_Hover, false);
		icon->setFixedSize(listCategory->iconSize());
		icon->setScaledContents(true);
		static QPixmap tmp("data/prism-studio/images/404_boxart-52x72.jpg");
		icon->setPixmap(tmp);
		layout->addWidget(icon);

		auto textLbael = new QLabel(name);
		textLbael->setObjectName("categoryLabel");
		layout->addWidget(textLbael);

		listCategory->addItem(item);
		listCategory->setItemWidget(item, root);

		PLSNetworkReplyBuilder builder(url);
		PLS_HTTP_HELPER->connectFinished(builder.get(), icon, _onSucceed, nullptr, nullptr, icon);
	}

	if (qApp->focusWidget() == ui->lineEditCategory) {
		showListCategory();
	}
}

void PLSLiveInfoTwitch::showEvent(QShowEvent *event)
{
	Q_UNUSED(event)

	showLoading(content());

	PLS_PLATFORM_TWITCH->requestChannel(true);

	PLSLiveInfoBase::showEvent(event);
};

bool PLSLiveInfoTwitch::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == ui->lineEditCategory) {
		if (event->type() == QEvent::FocusIn) {
			pushButtonSearch->setStyleSheet("image: url(:/images/btn-search-on-normal.svg);");
		} else if (event->type() == QEvent::FocusOut) {
			pushButtonSearch->setStyleSheet(QString());
		}
	}

	return __super::eventFilter(watched, event);
}

void PLSLiveInfoTwitch::doUpdateChannel(PLSPlatformApiResult value)
{
	hideLoading();

	if (PLSPlatformApiResult::PAR_SUCCEED == value) {
		accept();
	}
}

bool PLSLiveInfoTwitch::isModified()
{
	auto pPlatformTwitch = PLS_PLATFORM_TWITCH;
	return ui->lineEditTitle->text().toStdString() != pPlatformTwitch->getTitle() || ui->lineEditCategory->text().toStdString() != pPlatformTwitch->getCategory() ||
	       ui->comboBoxServers->currentIndex() != pPlatformTwitch->getServerIndex();
}

void PLSLiveInfoTwitch::doUpdateOkState()
{
	if (ui->lineEditTitle->text().trimmed().isEmpty() || ui->comboBoxServers->currentIndex() == -1) {
		ui->pushButtonOk->setEnabled(false);
		return;
	}

	ui->pushButtonOk->setEnabled(PLS_PLATFORM_API->isPrepareLive() || isModified());
}

void PLSLiveInfoTwitch::titleEdited()
{
	static const int TwitchTitleLengthLimit = 140;
	bool isLargeToMax = false;
	if (ui->lineEditTitle->text().length() > TwitchTitleLengthLimit) {
		isLargeToMax = true;
		ui->lineEditTitle->setText(ui->lineEditTitle->text().left(TwitchTitleLengthLimit));
	}
	if (isLargeToMax) {
		auto pPlatformTwitch = PLS_PLATFORM_TWITCH;
		const auto channelName = pPlatformTwitch->getInitData().value(ChannelData::g_channelName).toString();
		PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.Title.Length.Check.arg").arg(TwitchTitleLengthLimit).arg(channelName));
	}

	doUpdateOkState();
}
