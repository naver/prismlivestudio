#include "PLSToastMsgFrame.h"
#include "PLSDpiHelper.h"
#include "action.h"
#include "liblog.h"
#include "log/module_names.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QStyleOption>

const int TOAST_DISAPPEAR = 5 * 1000;

PLSToastMsgFrame::PLSToastMsgFrame(QWidget *parent) : QFrame(parent)
{
	timerDisappear.setSingleShot(true);
	connect(&timerDisappear, &QTimer::timeout, this, &QFrame::hide);
	this->setCursor(Qt::ArrowCursor);
	btnClose = new QPushButton(this);
	btnClose->setFocusPolicy(Qt::NoFocus);
	btnClose->setObjectName("toastMsgCloseBtn");
	connect(btnClose, &QPushButton::clicked, [=]() {
		PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, "close toast button", ACTION_CLICK);
		if (timerDisappear.isActive())
			timerDisappear.stop();
		this->hide();
	});

	editMessage = new QTextEdit(this);
	editMessage->setReadOnly(true);
	editMessage->setTextInteractionFlags(Qt::NoTextInteraction);
	editMessage->setContextMenuPolicy(Qt::NoContextMenu);
	editMessage->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	editMessage->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	editMessage->setObjectName("toastMsgEdit");
	editMessage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	editMessage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	editMessage->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

	QHBoxLayout *main_layout = new QHBoxLayout(this);
	main_layout->setContentsMargins(10, 10, 10, 10);
	main_layout->setSpacing(0);
	main_layout->addWidget(editMessage);
}

PLSToastMsgFrame::~PLSToastMsgFrame()
{
	if (timerDisappear.isActive())
		timerDisappear.stop();
}

void PLSToastMsgFrame::SetMessage(const QString &message)
{
	QTextOption option;
	option.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	editMessage->document()->setDefaultTextOption(option);
	editMessage->setText(message);
	QTimer::singleShot(0, this, SLOT(resizeToContent()));
}

QString PLSToastMsgFrame::GetMessage() const
{
	return editMessage->toPlainText();
}

void PLSToastMsgFrame::SetShowWidth(int width)
{
	showWidth = width;
	QTimer::singleShot(0, this, SLOT(resizeToContent()));
}

void PLSToastMsgFrame::ShowToast()
{
	this->raise();
	timerDisappear.start(TOAST_DISAPPEAR);
	QTimer::singleShot(0, this, [=]() {
		this->show();
		resizeToContent();
	});
}

void PLSToastMsgFrame::HideToast()
{
	if (timerDisappear.isActive())
		timerDisappear.stop();
	this->hide();
}

void PLSToastMsgFrame::resizeEvent(QResizeEvent *event)
{
	double dip = PLSDpiHelper::getDpi(this);
	int padding = PLSDpiHelper::calculate(dip, 7);
	btnClose->move(this->width() - btnClose->width() - padding, padding);
	btnClose->raise();
	QFrame::resizeEvent(event);
	resizeToContent();
}

void PLSToastMsgFrame::resizeToContent()
{
	auto size = editMessage->document()->size();
	int top, bottom, left, right;
	this->layout()->getContentsMargins(&left, &top, &right, &bottom);
	this->resize(showWidth, size.height() + top + bottom + PLSDpiHelper::calculate(this, 10));
	emit resizeFinished();
}
