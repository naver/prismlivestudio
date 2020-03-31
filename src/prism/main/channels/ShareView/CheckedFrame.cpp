#include "CheckedFrame.h"
#include <QDebug>
#include <QPainter>
#include "ChannelCommonFunctions.h"

void CheckedFrame::setChecked(bool checkState)
{
	// qDebug()<<checkState;
	this->checked = checkState;
	refreshStyle(this);
}
