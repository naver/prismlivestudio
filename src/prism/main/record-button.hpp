#pragma once

#include <QPushButton>

class RecordButton : public QPushButton {
	Q_OBJECT

public:
	explicit inline RecordButton(QWidget *parent = nullptr) : QPushButton(parent) {}

	virtual void resizeEvent(QResizeEvent *event) override;
};
