#pragma once

#include <memory>

#include "dialog-view.hpp"
#include "ui_PLSAbout.h"

class PLSAbout : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSAbout(QWidget *parent = 0);

	std::unique_ptr<Ui::PLSAbout> ui;

private slots:
	void ShowAbout();
	void ShowAuthors();
	void ShowLicense();
};
