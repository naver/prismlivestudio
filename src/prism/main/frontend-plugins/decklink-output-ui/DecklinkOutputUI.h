#pragma once

#include <dialog-view.hpp>

#include "ui_output.h"
#include "../../main/properties-view.hpp"
#include "../main/themes/PLSThemeManager.h"
#include "PLSDpiHelper.h"

class DecklinkOutputUI : public PLSDialogView {
	Q_OBJECT
private:
	PLSPropertiesView *propertiesView;
	PLSPropertiesView *previewPropertiesView;

public slots:
	// Zhang dewen issue:#2416 merge OBS v25.0.8 code.
	void on_outputButton_clicked();
	void PropertiesChanged();
	void OutputStateChanged(bool active);

	void on_previewOutputButton_clicked();
	void PreviewPropertiesChanged();
	void PreviewOutputStateChanged(bool active);

public:
	std::unique_ptr<Ui_Output> ui;
	DecklinkOutputUI(QWidget *parent, PLSDpiHelper dpiHelper = PLSDpiHelper());

	void ShowHideDialog();

	void SetupPropertiesView();
	void SaveSettings();

	void SetupPreviewPropertiesView();
	void SavePreviewSettings();
};
