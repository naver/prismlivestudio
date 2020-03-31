#include "PLSLoadingTitleItem.h"
#include "ui_PLSLoadingTitleItem.h"

PLSLoadingTitleItem::PLSLoadingTitleItem(QWidget *parent) : QWidget(parent), ui(new Ui::PLSLoadingTitleItem)
{
	ui->setupUi(this);
	setupTimer();
}

PLSLoadingTitleItem::~PLSLoadingTitleItem()
{
	m_timer->stop();
	m_timer = nullptr;
	delete ui;
}

void PLSLoadingTitleItem::setupTimer()
{
	m_timer = new QTimer();
	m_timer->setInterval(300);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateProgress()));
	m_timer->start();
	updateProgress();
}

void PLSLoadingTitleItem::updateProgress()
{
	static int showIndex = 0;
	showIndex = showIndex > 7 ? 1 : ++showIndex;
	ui->loadingLabel->setStyleSheet(QString("image: url(\":/images/loading-%1.svg\")").arg(showIndex));
	//QApplication::processEvents();
}
