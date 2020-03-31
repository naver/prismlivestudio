#include "DecklinkOutputUI.h"
#include <obs-module.h>
#include <util/platform.h>
#include <util/util.hpp>
#include "decklink-ui-main.h"
#include "../../pls-common-define.hpp"
#include "log.h"
#include "action.h"

#include <QFormLayout>

namespace {

class CustomPropertiesView : public PLSPropertiesView {
	DecklinkOutputUI *m_output;

public:
	explicit CustomPropertiesView(DecklinkOutputUI *output, OBSData settings, const char *type, PropertiesReloadCallback reloadCallback, int minSize = 0, int maxSize = -1)
		: PLSPropertiesView(output, settings, type, reloadCallback, minSize, maxSize), m_output(output)
	{
		RefreshProperties();
	}

	void RefreshProperties()
	{
		PLSPropertiesView::RefreshProperties(
			[=](QWidget *widget) {
				widget->setContentsMargins(0, 0, 0, 0);
				PLSDpiHelper::dpiDynamicUpdate(widget, false);
				if (obs_property_t *property = obs_properties_first(properties.get()); !property) {
					dynamic_cast<QFormLayout *>(widget->layout())->setHorizontalSpacing(0);
				}
			},
			false);
	}
};
}

DecklinkOutputUI::DecklinkOutputUI(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper), ui(new Ui_Output)
{
	dpiHelper.setCss(this, {PLSCssIndex::DecklinkOutputUI});
	dpiHelper.setFixedSize(this, {720, 700});

	setResizeEnabled(false);
	ui->setupUi(this->content());
	QMetaObject::connectSlotsByName(this);
	setSizeGripEnabled(false);

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	propertiesView = nullptr;
	previewPropertiesView = nullptr;
}

void DecklinkOutputUI::ShowHideDialog()
{
	SetupPropertiesView();
	SetupPreviewPropertiesView();

	setVisible(!isVisible());
}

void DecklinkOutputUI::SetupPropertiesView()
{
	if (propertiesView)
		delete propertiesView;

	obs_data_t *settings = obs_data_create();

	OBSData data = load_settings();
	if (data)
		obs_data_apply(settings, data);

	propertiesView = new CustomPropertiesView(this, settings, "decklink_output", (PropertiesReloadCallback)obs_get_output_properties, 65, 65);

	ui->propertiesLayout->addWidget(propertiesView);
	obs_data_release(settings);

	connect(propertiesView, SIGNAL(Changed()), this, SLOT(PropertiesChanged()));
}

void DecklinkOutputUI::SaveSettings()
{
	BPtr<char> modulePath = obs_module_get_config_path(obs_current_module(), "");

	os_mkdirs(modulePath);

	BPtr<char> path = obs_module_get_config_path(obs_current_module(), "decklinkOutputProps.json");

	obs_data_t *settings = propertiesView->GetSettings();
	if (settings)
		obs_data_save_json_safe(settings, path, "tmp", "bak");
}

void DecklinkOutputUI::SetupPreviewPropertiesView()
{
	if (previewPropertiesView)
		delete previewPropertiesView;

	obs_data_t *settings = obs_data_create();

	OBSData data = load_preview_settings();
	if (data)
		obs_data_apply(settings, data);

	previewPropertiesView = new CustomPropertiesView(this, settings, "decklink_output", (PropertiesReloadCallback)obs_get_output_properties, 65, 65);

	ui->previewPropertiesLayout->addWidget(previewPropertiesView);
	obs_data_release(settings);

	connect(previewPropertiesView, SIGNAL(Changed()), this, SLOT(PreviewPropertiesChanged()));
}

void DecklinkOutputUI::SavePreviewSettings()
{
	char *modulePath = obs_module_get_config_path(obs_current_module(), "");

	os_mkdirs(modulePath);

	char *path = obs_module_get_config_path(obs_current_module(), "decklinkPreviewOutputProps.json");

	obs_data_t *settings = previewPropertiesView->GetSettings();
	if (settings)
		obs_data_save_json_safe(settings, path, "tmp", "bak");
}

// Zhang dewen issue:#2416 merge OBS v25.0.8 code.
void DecklinkOutputUI::on_outputButton_clicked()
{
	PLS_PLUGIN_UI_STEP("Decklink Output > Output Start/Stop Button", ACTION_CLICK);
	SaveSettings();
	output_toggle();
}

void DecklinkOutputUI::PropertiesChanged()
{
	SaveSettings();
}

// Zhang dewen issue:#2416 merge OBS v25.0.8 code.
void DecklinkOutputUI::OutputStateChanged(bool active)
{
	QString text;
	if (active) {
		text = QString::fromUtf8(obs_module_text("Stop"));
	} else {
		text = QString::fromUtf8(obs_module_text("Start"));
	}

	ui->outputButton->setChecked(active);
	ui->outputButton->setText(text);
}

// Zhang dewen issue:#2416 merge OBS v25.0.8 code.
void DecklinkOutputUI::on_previewOutputButton_clicked()
{
	PLS_PLUGIN_UI_STEP("Decklink Output > Preview Output Start/Stop Button", ACTION_CLICK);
	SavePreviewSettings();
	preview_output_toggle();
}

void DecklinkOutputUI::PreviewPropertiesChanged()
{
	SavePreviewSettings();
}

// Zhang dewen issue:#2416 merge OBS v25.0.8 code.
void DecklinkOutputUI::PreviewOutputStateChanged(bool active)

{
	QString text;
	if (active) {
		text = QString::fromUtf8(obs_module_text("Stop"));
	} else {
		text = QString::fromUtf8(obs_module_text("Start"));
	}

	ui->previewOutputButton->setChecked(active);
	ui->previewOutputButton->setText(text);
}
