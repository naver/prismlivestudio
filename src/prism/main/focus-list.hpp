#pragma once

#include <QListWidget>

class FocusList : public QListWidget {
	Q_OBJECT

public:
	explicit FocusList(QWidget *parent);

protected:
	void focusInEvent(QFocusEvent *event) override;

signals:
	void GotFocus();
};
