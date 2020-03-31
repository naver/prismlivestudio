#pragma once

#include <QDialogButtonBox>

#include "frontend-api-global.h"

class FRONTEND_API PLSDialogButtonBox : public QDialogButtonBox {
	Q_OBJECT

public:
	explicit PLSDialogButtonBox(QWidget *parent = nullptr);
	~PLSDialogButtonBox();

public:
	static void updateStandardButtonsStyle(QDialogButtonBox *buttonBox);

public:
	void setStandardButtons(StandardButtons buttons);

private slots:
	void onButtonClicked(QAbstractButton *button);

protected:
	virtual bool event(QEvent *event);
};
