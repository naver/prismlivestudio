#include "PLSOpenSourceView.h"
#include "ui_PLSOpenSourceView.h"
#include <QTextFrame>
#include <platform.hpp>
#include <QFile>
#include "window-basic-main.hpp"
#include "pls-gpop-data.hpp"
#include "ui-config.h"

#define TEXT_EDIT_TOP_MARGIN 15
#define TEXT_EDIT_LEFT_MARGIN 15
#define TEXT_EDIT_RIGHT_MARGIN 10

extern QCef *cef;
extern QCefCookieManager *panel_cookies;

PLSOpenSourceView::PLSOpenSourceView(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper), ui(new Ui::PLSOpenSourceView)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSOpenSourceView});
	dpiHelper.setFixedSize(this, {720, 458});
	ui->setupUi(this->content());

	QMap<QString, QString> map = PLSGpopData::instance()->getOpenSourceLicenseMap();
	QString ver = QString("v2.1.2");
	if (map.contains(ver)) {
		m_updateInfoUrl = map.value(ver);
	}
	initUI();
	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(on_confirmButton_clicked()));
}

PLSOpenSourceView::~PLSOpenSourceView()
{
	timer->stop();
	delete ui;
}

void PLSOpenSourceView::initUI()
{
	if (!cef) {
		return;
	}

	//delay the cef widge
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
	timer->start(100);
}

void PLSOpenSourceView::onTimeOut()
{
	PLSBasic::InitBrowserPanelSafeBlock();
	if (cef) {
		m_cefWidget = cef->create_widget(nullptr, m_updateInfoUrl.toUtf8().constData(), "", panel_cookies);
		ui->cefBackView->addWidget(m_cefWidget);
		ui->cefBackView->setCurrentWidget(m_cefWidget);
	}
	timer->stop();
}

void PLSOpenSourceView::on_confirmButton_clicked()
{
	this->accept();
}
