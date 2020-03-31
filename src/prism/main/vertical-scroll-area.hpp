#pragma once

#include "PLSCommonScrollBar.h"

#include <QScrollArea>

class QResizeEvent;

class VScrollArea : public QScrollArea {
	Q_OBJECT

public:
	explicit inline VScrollArea(QWidget *parent = nullptr) : QScrollArea(parent)
	{
		this->setVerticalScrollBar(new PLSCommonScrollBar());
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}

protected:
	virtual void resizeEvent(QResizeEvent *event) override;
};
