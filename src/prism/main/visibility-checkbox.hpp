#pragma once

#include <QCheckBox>

class VisibilityCheckBox : public QCheckBox {
	Q_OBJECT
public:
	explicit VisibilityCheckBox(QWidget *parent = nullptr) : QCheckBox(parent) {}
	explicit VisibilityCheckBox(const QString &text, QWidget *parent = nullptr) : QCheckBox(text, parent) {}
};
