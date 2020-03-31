#pragma once

#include <QObject>

#include "frontend-api-global.h"

class FRONTEND_API PLSCancel : public QObject {
	Q_OBJECT

public:
	explicit PLSCancel(QObject *parent = nullptr) : QObject(parent), cancel(false) {}
	~PLSCancel() {}

public:
	operator bool() const { return cancel; }
	PLSCancel &operator=(bool cancel)
	{
		if (this->cancel != cancel) {
			emit cancelSignal(this->cancel = cancel);
		}
		return *this;
	}

signals:
	void cancelSignal(bool);

private:
	bool cancel;
};
