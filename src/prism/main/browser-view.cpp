#include "browser-view.hpp"
#include "ui_PLSBrowserView.h"
#include "pls-common-define.hpp"
#include <QHBoxLayout>
#include <QEventLoop>

#include "window-basic-main.hpp"

extern QCef *cef;

PLSBrowserView::PLSBrowserView(const QUrl &url, QWidget *parent) : PLSBrowserView(nullptr, url, nullptr, parent) {}

PLSBrowserView::PLSBrowserView(const QUrl &url, const std::map<std::string, std::string> &headers, QWidget *parent) : PLSBrowserView(nullptr, url, headers, QString(), nullptr, parent) {}

PLSBrowserView::PLSBrowserView(QJsonObject *res, const QUrl &url, PLSResultCheckingCallback callback, QWidget *parent)
	: PLSBrowserView(res, url, std::map<std::string, std::string>(), QString(), nullptr, parent)
{
}

PLSBrowserView::PLSBrowserView(QJsonObject *res, const QUrl &url, const std::map<std::string, std::string> &headers, const QString &pannelCookieName, PLSResultCheckingCallback callback,
			       QWidget *parent)
	: WidgetDpiAdapter(parent), ui(new Ui::PLSBrowserView), result(res), uri(url.toString().toStdString()), resultCheckingCallback(callback), cefWidget(nullptr), browser_panel_cookies(nullptr)
{
	ui->setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

	if (!cef) {
		emit doneSignal(QDialog::Rejected);
		return;
	}
	browser_panel_cookies = PLSBasic::getBrowserPannelCookieMgr(pannelCookieName);
	//PLSBasic::InitBrowserPanelSafeBlock();
	cefWidget = cef->create_widget(this, uri, pls_get_offline_javaScript(), browser_panel_cookies, headers);
	if (!cefWidget) {
		emit doneSignal(QDialog::Rejected);
		return;
	}

	// QObject::connect(cefWidget, SIGNAL(titleChanged(const QString &)), this, SLOT(setWindowTitle(const QString &)));

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(cefWidget);
	setWindowIcon(QIcon(""));
	setWindowTitle(ONE_SPACE);

	PLSDpiHelper dpiHelper;
	dpiHelper.setInitSize(this, {800, 600});

	QObject::connect(this, &PLSBrowserView::doneSignal, this, &PLSBrowserView::done, Qt::QueuedConnection);
	QObject::connect(cefWidget, SIGNAL(urlChanged(const QString &)), this, SLOT(urlChanged(const QString &)));
}

PLSBrowserView::~PLSBrowserView()
{
	delete ui;
}

void PLSBrowserView::resizeEvent(QResizeEvent *event)
{

	WidgetDpiAdapter::resizeEvent(event);
	QMetaObject::invokeMethod(
		this,
		[=]() {
			if (m_dpi != PLSDpiHelper::getDpi(this)) {
				QPoint _topLeft = this->frameGeometry().topLeft();
				QPoint newPoint = QPoint(_topLeft.x() + 1, _topLeft.y());
				move(newPoint);
				move(_topLeft);
				m_dpi = PLSDpiHelper::getDpi(this);
			}
		},
		Qt::QueuedConnection);
}

void PLSBrowserView::urlChanged(const QString &url)
{
	if (!result || !resultCheckingCallback) {
		return;
	}

	enum { InitValue, QuitByDoneSignal, QuitByVisitAllCookies };

	struct EventLoop {
		std::atomic<int> quitFlag = InitValue;
		QEventLoop el;
		QMap<QString, QString> cookies;
	};

	std::shared_ptr<EventLoop> eventLoop(new EventLoop());

	auto conn = connect(this, &PLSBrowserView::doneSignal, &eventLoop->el, [eventLoop]() {
		if (eventLoop->quitFlag == InitValue) {
			eventLoop->quitFlag = QuitByDoneSignal;
			eventLoop->el.quit();
		}
	});

	browser_panel_cookies->visitAllCookies(
		[eventLoop](const char *name, const char *value, const char *domain, const char *path, bool complete, void *context) {
			if (name && value) {
				eventLoop->cookies.insert(QString::fromUtf8(name), QString::fromUtf8(value));
			}

			if (complete && (eventLoop->quitFlag == InitValue)) {
				eventLoop->quitFlag = QuitByVisitAllCookies;
				QMetaObject::invokeMethod(&eventLoop->el, &QEventLoop::quit);
			}
		},
		nullptr);

	eventLoop->el.exec();

	disconnect(conn);

	if (eventLoop->quitFlag != QuitByVisitAllCookies) {
		return;
	}

	switch (resultCheckingCallback(*result, url, eventLoop->cookies)) {
	case PLSResultCheckingResult::Ok:
		emit doneSignal(QDialog::Accepted);
		break;
	case PLSResultCheckingResult::Close:
		emit doneSignal(QDialog::Rejected);
		break;
	default:
		break;
	}
}
