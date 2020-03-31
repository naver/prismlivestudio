#include "PLSSceneTransitionsView.h"
#include "ui_PLSSceneTransitionsView.h"

#include "qt-wrappers.hpp"
#include "window-namedialog.hpp"
#include "pls-app.hpp"
#include "window-basic-main.hpp"
#include "dialog-view.hpp"
#include "pls-common-define.hpp"
#include "liblog.h"
#include "log/module_names.h"
#include "action.h"
#include "PLSMenu.hpp"
#include "ChannelCommonFunctions.h"
#include "PLSDpiHelper.h"

Q_DECLARE_METATYPE(OBSSource);

PLSSceneTransitionsView::PLSSceneTransitionsView(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper), ui(new Ui::PLSSceneTransitionsView)
{
	ui->setupUi(this->content());
	this->setWindowTitle(QTStr("Basic.Hotkeys.SelectScene"));
	dpiHelper.setCss(this, {PLSCssIndex::QComboBox, PLSCssIndex::QSpinBox, PLSCssIndex::QScrollBar, PLSCssIndex::PLSSceneTransitionsView});
	connect(ui->okBtn, &QPushButton::clicked, this, [=] {
		PLS_UI_STEP(MAINSCENE_MODULE, "Scene Transition OK", ACTION_CLICK);
		this->close();
	});
	connect(ui->transitions, &PLSComboBox::currentTextChanged, this, &PLSSceneTransitionsView::OnCurrentTextChanged);
	connect(ui->transitions, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PLSSceneTransitionsView::on_transitions_currentIndexChanged);
	connect(ui->transitionAdd, &QPushButton::clicked, this, &PLSSceneTransitionsView::on_transitionAdd_clicked);
	connect(ui->transitionRemove, &QPushButton::clicked, this, &PLSSceneTransitionsView::on_transitionRemove_clicked);
	connect(ui->transitionProps, &QPushButton::clicked, this, &PLSSceneTransitionsView::on_transitionProps_clicked);
	connect(ui->transitionDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &PLSSceneTransitionsView::on_transitionDuration_valueChanged);
}

PLSSceneTransitionsView::~PLSSceneTransitionsView()
{
	delete ui;
}

void PLSSceneTransitionsView::InitLoadTransition(obs_data_array_t *transitions, obs_source_t *fadeTransition, const int &transitionDuration, const QString &crrentTransition)
{
	if (transitions)
		LoadTransitions(transitions);

	obs_source_t *curTransition = FindTransition(crrentTransition.toStdString().c_str());
	if (!curTransition)
		curTransition = fadeTransition;

	SetTransitionDurationValue(transitionDuration);
	SetTransition(curTransition);
}

void PLSSceneTransitionsView::InitTransition(obs_source_t *transition)
{
	auto onTransitionStop = [](void *data, calldata_t *) {
		PLSBasic *window = (PLSBasic *)data;
		QMetaObject::invokeMethod(window, "TransitionStopped", Qt::QueuedConnection);
	};

	auto onTransitionFullStop = [](void *data, calldata_t *) {
		PLSBasic *window = (PLSBasic *)data;
		QMetaObject::invokeMethod(window, "TransitionFullyStopped", Qt::QueuedConnection);
	};

	signal_handler_t *handler = obs_source_get_signal_handler(transition);
	main = PLSBasic::Get();

	signal_handler_connect(handler, "transition_video_stop", onTransitionStop, main);
	signal_handler_connect(handler, "transition_stop", onTransitionFullStop, main);
}

obs_source_t *PLSSceneTransitionsView::FindTransition(const char *name)
{
	for (int i = 0; i < ui->transitions->count(); i++) {
		OBSSource tr = ui->transitions->itemData(i).value<OBSSource>();

		const char *trName = obs_source_get_name(tr);
		if (strcmp(trName, name) == 0)
			return tr;
	}
	return nullptr;
}

OBSSource PLSSceneTransitionsView::GetCurrentTransition()
{
	return ui->transitions->currentData().value<OBSSource>();
}

OBSSource PLSSceneTransitionsView::GetTransitionByIndex(const int &index)
{
	return ui->transitions->itemData(index).value<OBSSource>();
}

obs_data_array_t *PLSSceneTransitionsView::SaveTransitions()
{
	obs_data_array_t *transitions = obs_data_array_create();

	for (int i = 0; i < ui->transitions->count(); i++) {
		OBSSource tr = ui->transitions->itemData(i).value<OBSSource>();
		if (!obs_source_configurable(tr))
			continue;

		obs_data_t *sourceData = obs_data_create();
		obs_data_t *settings = obs_source_get_settings(tr);

		obs_data_set_string(sourceData, "name", obs_source_get_name(tr));
		obs_data_set_string(sourceData, "id", obs_obj_get_id(tr));
		obs_data_set_obj(sourceData, "settings", settings);

		obs_data_array_push_back(transitions, sourceData);

		obs_data_release(settings);
		obs_data_release(sourceData);
	}

	return transitions;
}

void PLSSceneTransitionsView::LoadTransitions(obs_data_array_t *transitions)
{
	size_t count = obs_data_array_count(transitions);

	for (size_t i = 0; i < count; i++) {
		obs_data_t *item = obs_data_array_item(transitions, i);
		const char *name = obs_data_get_string(item, "name");
		const char *id = obs_data_get_string(item, "id");
		obs_data_t *settings = obs_data_get_obj(item, "settings");

		obs_source_t *source = obs_source_create_private(id, name, settings);
		if (!obs_obj_invalid(source)) {
			InitTransition(source);
			ui->transitions->addItem(QT_UTF8(name), QVariant::fromValue(OBSSource(source)));
			ui->transitions->setCurrentIndex(ui->transitions->count() - 1);
		}

		obs_data_release(settings);
		obs_data_release(item);
		obs_source_release(source);
	}
}

void PLSSceneTransitionsView::AddTransition()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	QString idStr = action->property("id").toString();
	PLS_UI_STEP(MAIN_TRANSITION_MODULE, idStr.toStdString().c_str(), ACTION_CLICK);

	std::string name;
	QString placeHolderText = QT_UTF8(obs_source_get_display_name(QT_TO_UTF8(idStr)));
	QString format = placeHolderText + " (%1)";
	obs_source_t *source = nullptr;
	int i = 1;

	while ((source = FindTransition(QT_TO_UTF8(placeHolderText)))) {
		placeHolderText = format.arg(++i);
	}

	bool accepted = NameDialog::AskForName(this, QTStr("TransitionNameDlg.Title"), QTStr("TransitionNameDlg.Text"), name, placeHolderText);
	name = QString(name.c_str()).simplified().toStdString();
	if (accepted) {
		if (name.empty()) {
			PLSMessageBox::warning(this, QTStr("NoNameEntered.Title"), QTStr("NoNameEntered.Text"));
			AddTransition();
			return;
		}

		source = FindTransition(name.c_str());
		if (source) {
			PLSMessageBox::warning(this, QTStr("NameExists.Title"), QTStr("NameExists.Text"));

			AddTransition();
			return;
		}

		source = obs_source_create_private(QT_TO_UTF8(idStr), name.c_str(), NULL);
		InitTransition(source);
		ui->transitions->addItem(QT_UTF8(name.c_str()), QVariant::fromValue(OBSSource(source)));
		ui->transitions->setCurrentIndex(ui->transitions->count() - 1);
		main = PLSBasic::Get();
		if (main) {
			main->CreatePropertiesWindow(source, OPERATION_NONE, this);
			main->OnTransitionAdded();
		}
		obs_source_release(source);
	}
}

void PLSSceneTransitionsView::RenameTransition()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	QVariant variant = action->property("transition");
	obs_source_t *transition = variant.value<OBSSource>();

	std::string name;
	QString placeHolderText = QT_UTF8(obs_source_get_name(transition));
	obs_source_t *source = nullptr;

	bool accepted = NameDialog::AskForName(this, QTStr("TransitionNameDlg.Title"), QTStr("TransitionNameDlg.Text"), name, placeHolderText);
	name = QString(name.c_str()).simplified().toStdString();

	if (accepted) {
		if (name.empty()) {
			PLSMessageBox::warning(this, QTStr("NoNameEntered.Title"), QTStr("NoNameEntered.Text"));
			RenameTransition();
			return;
		}

		source = FindTransition(name.c_str());
		if (source) {
			PLSMessageBox::warning(this, QTStr("NameExists.Title"), QTStr("NameExists.Text"));

			RenameTransition();
			return;
		}

		obs_source_set_name(transition, name.c_str());
		int idx = ui->transitions->findData(variant);
		if (idx != -1) {
			ui->transitions->setItemText(idx, QT_UTF8(name.c_str()));
			main = PLSBasic::Get();
			if (main) {
				main->OnTransitionRenamed();
			}
		}
	}
}

void PLSSceneTransitionsView::ClearTransition()
{
	ui->transitions->clear();
}

static inline void SetComboTransition(QComboBox *combo, obs_source_t *tr)
{
	int idx = combo->findData(QVariant::fromValue<OBSSource>(tr));
	if (idx != -1) {
		combo->blockSignals(true);
		combo->setCurrentIndex(idx);
		combo->blockSignals(false);
	}
}

void PLSSceneTransitionsView::SetTransition(OBSSource transition)
{
	obs_source_t *oldTransition = obs_get_output_source(0);

	if (oldTransition && transition) {
		obs_transition_swap_begin(transition, oldTransition);
		if (transition != GetCurrentTransition())
			SetComboTransition(ui->transitions, transition);
		obs_set_output_source(0, transition);
		obs_transition_swap_end(transition, oldTransition);
	} else {
		obs_set_output_source(0, transition);
	}

	if (oldTransition)
		obs_source_release(oldTransition);

	bool fixed = transition ? obs_transition_fixed(transition) : false;
	ui->transitionDurationLabel->setVisible(!fixed);
	ui->transitionDuration->setVisible(!fixed);

	bool configurable = obs_source_configurable(transition);
	ui->transitionRemove->setEnabled(configurable);
	ui->transitionProps->setEnabled(configurable);

	main = PLSBasic::Get();
	if (main) {
		main->OnTransitionSet();
	}
}

void PLSSceneTransitionsView::AddTransitionsItem(std::vector<OBSSource> &transitions)
{
	for (OBSSource &tr : transitions) {
		ui->transitions->addItem(QT_UTF8(obs_source_get_name(tr)), QVariant::fromValue(OBSSource(tr)));
	}
	ui->transitions->setCurrentIndex(0);
}

QComboBox *PLSSceneTransitionsView::GetTransitionCombobox()
{
	return ui->transitions;
}

QSpinBox *PLSSceneTransitionsView::GetTransitionDuration()
{
	return ui->transitionDuration;
}

int PLSSceneTransitionsView::GetTransitionComboBoxCount()
{
	return ui->transitions->count();
}

int PLSSceneTransitionsView::GetTransitionDurationValue()
{
	return ui->transitionDuration->value();
}

void PLSSceneTransitionsView::SetTransitionDurationValue(const int &value)
{
	ui->transitionDuration->setValue(value);
}

void PLSSceneTransitionsView::on_transitions_currentIndexChanged(int index)
{
	Q_UNUSED(index);
	OBSSource transition = GetCurrentTransition();
	SetTransition(transition);
}

void PLSSceneTransitionsView::on_transitionAdd_clicked()
{
	PLS_UI_STEP(MAINSCENE_MODULE, "Add Scene Transition", ACTION_CLICK);
	bool foundConfigurableTransitions = false;
	PLSPopupMenu menu(this);
	size_t idx = 0;
	const char *id;

	while (obs_enum_transition_types(idx++, &id)) {
		if (obs_is_source_configurable(id)) {
			const char *name = obs_source_get_display_name(id);
			QAction *action = new QAction(name, this);
			action->setProperty(PROPERTY_NAME_ID, id);

			connect(action, &QAction::triggered, this, &PLSSceneTransitionsView::AddTransition);

			menu.addAction(action);
			foundConfigurableTransitions = true;
		}
	}

	if (foundConfigurableTransitions)
		menu.exec(QCursor::pos());
}

void PLSSceneTransitionsView::on_transitionRemove_clicked()
{
	PLS_UI_STEP(MAINSCENE_MODULE, "Remove Scene Transition", ACTION_CLICK);

	OBSSource tr = GetCurrentTransition();

	main = PLSBasic::Get();
	if (!tr || !obs_source_configurable(tr) || !main || !main->QueryRemoveSource(tr))
		return;

	int idx = ui->transitions->findData(QVariant::fromValue<OBSSource>(tr));
	if (idx == -1)
		return;

	ui->transitions->removeItem(idx);
	main->onTransitionRemoved(tr);
}

void PLSSceneTransitionsView::on_transitionProps_clicked()
{
	PLS_UI_STEP(MAINSCENE_MODULE, "Scene Transition Property", ACTION_CLICK);

	OBSSource source = GetCurrentTransition();

	if (!obs_source_configurable(source))
		return;

	auto properties = [&]() {
		main = PLSBasic::Get();
		main->CreatePropertiesWindow(source, OPERATION_NONE, this);
	};

	PLSPopupMenu menu(this);

	QAction *action = new QAction(QTStr("Rename"), &menu);
	connect(action, &QAction::triggered, this, &PLSSceneTransitionsView::RenameTransition);
	action->setProperty(PROPERTY_NAME_TRANSITION, QVariant::fromValue(source));
	menu.addAction(action);

	action = new QAction(QTStr("Transition.Properties"), &menu);
	connect(action, &QAction::triggered, properties);
	menu.addAction(action);

	menu.exec(QCursor::pos());
}

void PLSSceneTransitionsView::on_transitionDuration_valueChanged(int value)
{
	PLS_UI_STEP(MAINSCENE_MODULE, "Scene Transition Value Changed", ACTION_CLICK);

	main = PLSBasic::Get();
	if (main) {
		main->OnTransitionDurationValueChanged(value);
	}
}

void PLSSceneTransitionsView::OnCurrentTextChanged(const QString &text)
{
	currentText = text;
}

void PLSSceneTransitionsView::showEvent(QShowEvent *event)
{
	QDialog::showEvent(event);
	ui->transitions->setCurrentText(currentText);
}
