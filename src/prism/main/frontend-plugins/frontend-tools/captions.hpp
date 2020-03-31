#pragma once

#include <memory>

#include <dialog-view.hpp>

#include "ui_captions.h"

class CaptionsDialog : public PLSDialogView {
	Q_OBJECT

	std::unique_ptr<Ui_CaptionsDialog> ui;

public:
	CaptionsDialog(QWidget *parent, PLSDpiHelper dpiHelper = PLSDpiHelper());

public slots:
	void on_source_currentIndexChanged(int idx);
	void on_enable_clicked(bool checked);
	void on_language_currentIndexChanged(int idx);
	void on_provider_currentIndexChanged(int idx);
};
