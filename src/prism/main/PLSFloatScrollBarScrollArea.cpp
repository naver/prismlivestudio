#include "PLSFloatScrollBarScrollArea.h"
#include "PLSDpiHelper.h"
#include <QTimer>

PLSFloatScrollBarScrollArea::PLSFloatScrollBarScrollArea(QWidget *parent) : QScrollArea(parent)
{
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	delegateScrollBar = new QScrollBar(Qt::Vertical, this);
	delegateScrollBar->setObjectName("flatScrollBar");
	delegateScrollBar->installEventFilter(this);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &PLSFloatScrollBarScrollArea::OnVerticalScrollBarValueChanged);

	connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, [=]() {
		QTimer::singleShot(0, this, [=]() {
			delegateScrollBar->blockSignals(true);
			UpdateSlider();
			emit ScrollBarRangChanged();
			delegateScrollBar->blockSignals(false);
		});
	});

	connect(delegateScrollBar, &QScrollBar::valueChanged, [=](int value) { this->verticalScrollBar()->setValue(value); });

	connect(delegateScrollBar, &QScrollBar::rangeChanged, [=](int min, int max) {
		this->verticalScrollBar()->blockSignals(true);
		this->verticalScrollBar()->setRange(min, max);
		this->verticalScrollBar()->blockSignals(false);
	});
}

PLSFloatScrollBarScrollArea::~PLSFloatScrollBarScrollArea() {}

void PLSFloatScrollBarScrollArea::SetScrollBarRightMargin(int rightMargin_)
{
	rightMargin = rightMargin_;
}

void PLSFloatScrollBarScrollArea::resizeEvent(QResizeEvent *event)
{
	UpdateSlider();
	QScrollArea::resizeEvent(event);
}

void PLSFloatScrollBarScrollArea::OnVerticalScrollBarValueChanged(int value)
{
	int pageStep = this->verticalScrollBar()->pageStep();
	int singleStep = this->verticalScrollBar()->singleStep();
	int maxValue = this->verticalScrollBar()->maximum();
	int minValue = this->verticalScrollBar()->minimum();
	delegateScrollBar->setPageStep(pageStep);
	delegateScrollBar->setSingleStep(singleStep);
	delegateScrollBar->blockSignals(true);
	delegateScrollBar->setValue(value);
	delegateScrollBar->blockSignals(false);

	if (value == maxValue)
		emit ScrolledToEnd();
	else if (value == minValue)
		emit ScrolledToTop();
}

void PLSFloatScrollBarScrollArea::UpdateSlider()
{
	delegateScrollBar->move(width() - delegateScrollBar->width() - PLSDpiHelper::calculate(this, rightMargin), PLSDpiHelper::calculate(this, 1));
	delegateScrollBar->resize(delegateScrollBar->width(), height() - PLSDpiHelper::calculate(this, 2));
	int min = this->verticalScrollBar()->minimum();
	int max = this->verticalScrollBar()->maximum();
	bool needShow = max > 0;
	int pageStep = this->verticalScrollBar()->pageStep();
	int singleStep = this->verticalScrollBar()->singleStep();
	delegateScrollBar->setVisible(needShow);
	delegateScrollBar->setPageStep(pageStep);
	delegateScrollBar->setSingleStep(singleStep);
	delegateScrollBar->setMinimum(min);
	delegateScrollBar->setMaximum(max);
}
