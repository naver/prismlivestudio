/******************************************************************************
    Copyright (C) 2015 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "window-namedialog.hpp"
#include "window-basic-main.hpp"
#include "window-basic-filters.hpp"
#include "display-helpers.hpp"
#include "qt-wrappers.hpp"
#include "visibility-item-widget.hpp"
#include "item-widget-helpers.hpp"
#include "pls-app.hpp"
#include "alert-view.hpp"
#include "pls-common-define.hpp"
#include "liblog.h"
#include "action.h"
#include "log/module_names.h"
#include "PLSFiltersItemView.h"
#include "PLSColorFilterView.h"

#include <QCloseEvent>
#include <vector>
#include <string>
#include <QMenu>
#include <QVariant>

using namespace std;

Q_DECLARE_METATYPE(OBSSource);

PLSBasicFilters::PLSBasicFilters(QWidget *parent, OBSSource source_, PLSDpiHelper dpiHelper)
	: PLSDialogView(parent, dpiHelper),
	  ui(new Ui::PLSBasicFilters),
	  source(source_),
	  addSignal(obs_source_get_signal_handler(source), "filter_add", PLSBasicFilters::OBSSourceFilterAdded, this),
	  removeSignal(obs_source_get_signal_handler(source), "filter_remove", PLSBasicFilters::OBSSourceFilterRemoved, this),
	  reorderSignal(obs_source_get_signal_handler(source), "reorder_filters", PLSBasicFilters::OBSSourceReordered, this),
	  removeSourceSignal(obs_source_get_signal_handler(source), "remove", PLSBasicFilters::SourceRemoved, this),
	  renameSourceSignal(obs_source_get_signal_handler(source), "rename", PLSBasicFilters::SourceRenamed, this)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSBasicFilters});
	dpiHelper.setInitSize(this, {FILTERS_VIEW_DEFAULT_WIDTH, FILTERS_VIEW_DEFAULT_HEIGHT});

	ui->setupUi(this->content());
	ui->emptyDesLabel->hide();
	ui->rightLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	QMetaObject::connectSlotsByName(this);

	UpdateFilters();

	signal_handler_connect_ref(obs_source_get_signal_handler(source), "capture_state", PLSQTDisplay::OnSourceCaptureState, ui->preview);
	ui->preview->UpdateSourceState(source);
	ui->preview->AttachSource(source);
	ui->filtersListWidget->SetSource(source);

	const char *name = obs_source_get_name(source);
	setWindowTitle(QTStr("Basic.Filters.Title").arg(QT_UTF8(name)));
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

#ifndef QT_NO_SHORTCUT
	ui->actionRemoveFilter->setShortcut(QApplication::translate("PLSBasicFilters", "Del", nullptr));
#endif // QT_NO_SHORTCUT
	ui->actionRemoveFilter->setShortcutContext(Qt::WidgetWithChildrenShortcut);

	addAction(ui->actionRemoveFilter);
	addAction(ui->actionMoveUp);
	addAction(ui->actionMoveDown);

	installEventFilter(CreateShortcutFilter());

	connect(ui->filtersListWidget, &PLSFiltersListView::RowChanged, this, &PLSBasicFilters::OnFiltersRowChanged);
	connect(ui->filtersListWidget, &PLSFiltersListView::CurrentItemIndexChanged, this, &PLSBasicFilters::OnCurrentItemChanged);

	connect(ui->addFilterButton, &QPushButton::clicked, this, &PLSBasicFilters::OnAddFilterButtonClicked);
	QPushButton *close = ui->buttonBox->button(QDialogButtonBox::Close);
	connect(close, SIGNAL(clicked()), this, SLOT(close()));
	close->setDefault(true);

	ui->buttonBox->button(QDialogButtonBox::Reset)->setText(QTStr("Defaults"));

	connect(ui->buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), this, SLOT(ResetFilters()));

	uint32_t caps = obs_source_get_output_flags(source);

	auto addDrawCallback = [this]() { obs_display_add_draw_callback(ui->preview->GetDisplay(), PLSBasicFilters::DrawPreview, this); };

	enum obs_source_type type = obs_source_get_type(source);
	bool drawable_type = type == OBS_SOURCE_TYPE_INPUT || type == OBS_SOURCE_TYPE_SCENE;

	if ((caps & OBS_SOURCE_VIDEO) != 0) {
		ui->preview->show();
		if (drawable_type)
			connect(ui->preview, &PLSQTDisplay::DisplayCreated, addDrawCallback);
		dpiHelper.setMaximumHeight(ui->widget_container, FILTERS_PROPERTIES_VIEW_MAX_HEIGHT);
	} else {
		ui->preview->setProperty("forceHidden", true);
		ui->preview->hide();
		ui->widget_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		ui->widget_container->resize(this->size());
	}

	QAction *renameAsync = new QAction(ui->filtersWidget);
	renameAsync->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(renameAsync, SIGNAL(triggered()), this, SLOT(RenameFilter()));
	ui->filtersWidget->addAction(renameAsync);

#ifdef __APPLE__
	renameAsync->setShortcut({Qt::Key_Return});
	renameEffect->setShortcut({Qt::Key_Return});
#else
	renameAsync->setShortcut({Qt::Key_F2});
#endif
}

PLSBasicFilters::~PLSBasicFilters()
{
	obs_display_remove_draw_callback(ui->preview->GetDisplay(), PLSBasicFilters::DrawPreview, this);
	signal_handler_disconnect(obs_source_get_signal_handler(source), "capture_state", PLSQTDisplay::OnSourceCaptureState, ui->preview);
	ClearListItems(ui->filtersListWidget);
}

void PLSBasicFilters::Init()
{
	show();
}

OBSSource PLSBasicFilters::GetSource()
{
	return source;
}

void PLSBasicFilters::UpdatePropertiesView(int row)
{
	if (view) {
		updatePropertiesSignal.Disconnect();
		ui->hLayout_property_view->removeWidget(view);
		view->hide();
		view->deleteLater();
		view = nullptr;
	}

	OBSSource filter = ui->filtersListWidget->GetFilter(row);
	if (!filter) {
		ui->emptyDesLabel->show();
		ui->filtersListWidget->hide();
		CleanColorFiltersView();
		return;
	}
	ui->emptyDesLabel->hide();

	bool showColorFiltersPath = UpdateColorFilters(filter);

	obs_data_t *settings = obs_source_get_settings(filter);

	view = new PLSPropertiesView(settings, filter, (PropertiesReloadCallback)obs_source_properties, (PropertiesUpdateCallback)obs_source_update, 0, -1, false, !showColorFiltersPath,
				     colorFilterOriginalPressed);
	view->SetCustomContentMargins(true);
	view->RefreshProperties();

	connect(view, &PLSPropertiesView::ColorFilterValueChanged, this, &PLSBasicFilters::OnColorFilterValueChanged);
	connect(this, &PLSBasicFilters::colorFilterOriginalPressedSignal, view, &PLSPropertiesView::OnColorFilterOriginalPressed);
	updatePropertiesSignal.Connect(obs_source_get_signal_handler(filter), "update_properties", PLSBasicFilters::UpdateProperties, this);
	obs_data_release(settings);
	ui->hLayout_property_view->addWidget(view);
	view->show();

	PLSDpiHelper::dpiDynamicUpdate(view);
}

void PLSBasicFilters::UpdateProperties(void *data, calldata_t *)
{
	PLSPropertiesView *view = static_cast<PLSBasicFilters *>(data)->view;
	if (!view) {
		return;
	}

	view->SetCustomContentMargins(true);
	QMetaObject::invokeMethod(view, "ReloadProperties");
}

void PLSBasicFilters::AddFilter(OBSSource filter, bool reorder)
{
	ui->filtersListWidget->AddFilterItemView(filter, reorder);

	if (reorder) {
		ui->filtersListWidget->SetCurrentItem(0);
	} else {
		ui->filtersListWidget->SetCurrentItem(ui->filtersListWidget->count() - 1);
	}
}

void PLSBasicFilters::RemoveFilter(OBSSource filter)
{
	ui->filtersListWidget->RemoveFilterItemView(filter);
	ui->filtersListWidget->SetCurrentItem(0);

	const char *filterName = obs_source_get_name(filter);
	const char *sourceName = obs_source_get_name(source);
	if (!sourceName || !filterName)
		return;

	const char *filterId = obs_source_get_id(filter);

	blog(LOG_INFO, "User removed filter '%s' (%s) from source '%s'", filterName, filterId, sourceName);

	PLSBasic *main = PLSBasic::Get();
	if (main)
		main->SaveProject();

	this->OnCurrentItemChanged(ui->filtersListWidget->GetCurrentRow());
}

struct FilterOrderInfo {
	int filterIdx = 0;
	PLSBasicFilters *window;

	explicit inline FilterOrderInfo(PLSBasicFilters *window_) : window(window_) {}
};

void PLSBasicFilters::ReorderFilter(PLSFiltersListView *list, obs_source_t *filter, size_t idx)
{
	int count = list->count();
	for (int i = count - 1; i >= 0; i--) {
		QListWidgetItem *listItem = list->item(i);
		OBSSource filterItem = list->GetFilter(i);

		if (filterItem == filter) {
			if (i != static_cast<int>(idx)) {
				bool sel = (list->GetStartDragFilter() == filter);
				PLSFiltersItemView *filtersView = static_cast<PLSFiltersItemView *>(list->itemWidget(listItem));
				if (filtersView) {
					QListWidgetItem *item = new QListWidgetItem;
					item->setData(Qt::UserRole, QVariant::fromValue(filterItem));
					list->insertItem((int)idx, item);
					list->setItemWidget(item, filtersView);
					if (sel) {
						list->setCurrentRow((int)idx);
					}

					delete listItem;
					listItem = nullptr;
				}
			}

			break;
		}
	}
}

void PLSBasicFilters::ReorderFilters()
{
	FilterOrderInfo info(this);

	obs_source_enum_filters(
		source,
		[](obs_source_t *, obs_source_t *filter, void *p) {
			FilterOrderInfo *info = reinterpret_cast<FilterOrderInfo *>(p);
			uint32_t flags;
			bool async;

			flags = obs_source_get_output_flags(filter);
			async = (flags & OBS_SOURCE_ASYNC) != 0;
			info->window->ReorderFilter(info->window->ui->filtersListWidget, filter, info->filterIdx++);
		},
		&info);
}

void PLSBasicFilters::UpdateFilters()
{
	if (!source)
		return;

	ClearListItems(ui->filtersListWidget);
	obs_source_enum_filters(
		source,
		[](obs_source_t *, obs_source_t *filter, void *p) {
			PLSBasicFilters *window = reinterpret_cast<PLSBasicFilters *>(p);

			window->AddFilter(filter);
		},
		this);

	if (0 == ui->filtersListWidget->count()) {
		ui->filtersListWidget->hide();
		ui->emptyDesLabel->show();
	}

	UpdatePropertiesView(ui->filtersListWidget->count() - 1);

	PLSBasic *main = PLSBasic::Get();
	if (main)
		main->SaveProject();
}

bool PLSBasicFilters::UpdateColorFilters(obs_source_t *filter)
{
	// if id is color filter, property view should add color filter view
	const char *id = obs_source_get_id(filter);
	if (id) {
		if (0 == strcmp(id, FILTER_TYPE_ID_APPLYLUT)) {
			//add color filter
			//ui->rightLayout->addWidget(CreateColorFitlersView(filter));
			ui->verticalLayout_container->insertWidget(0, CreateColorFitlersView(filter));
			return true;
		} else {
			for (auto iter = colorFilterMap.begin(); iter != colorFilterMap.end(); iter++) {
				PLSColorFilterView *colorFiltersView = iter->second;
				if (colorFiltersView) {
					colorFiltersView->hide();
				}
			}
		}
	}
	return false;
}

QWidget *PLSBasicFilters::CreateColorFitlersView(obs_source_t *filter)
{
	if (!filter) {
		return nullptr;
	}

	const char *name = obs_source_get_name(filter);

	bool isFind = false;
	PLSColorFilterView *colorFiltersView = nullptr;
	for (auto iter = colorFilterMap.begin(); iter != colorFilterMap.end(); ++iter) {
		PLSColorFilterView *tmpFiltersView = iter->second;
		if (!tmpFiltersView) {
			continue;
		}
		if (0 == iter->first.compare(name)) {
			isFind = true;
			colorFiltersView = tmpFiltersView;
		} else {
			tmpFiltersView->hide();
		}
	}

	if (colorFiltersView && isFind) {
		colorFiltersView->show();
	}

	if (!isFind) {
		PLSColorFilterView *colorFiltersView = new PLSColorFilterView(filter, this);
		connect(colorFiltersView, &PLSColorFilterView::OriginalPressed, this, &PLSBasicFilters::OnColorFilterOriginalPressed);
		connect(colorFiltersView, &PLSColorFilterView::ColorFilterValueChanged, this, &PLSBasicFilters::UpdatePropertyColorFilterValue);

		colorFilterMap[name] = colorFiltersView;
		colorFiltersView->show();
		return colorFiltersView;
	}

	return colorFiltersView;
}

void PLSBasicFilters::CleanColorFiltersView()
{
	for (auto iter = colorFilterMap.begin(); iter != colorFilterMap.end();) {
		PLSColorFilterView *colorFiltersView = iter->second;
		if (colorFiltersView) {
			ui->verticalLayout_container->removeWidget(colorFiltersView);
			colorFiltersView->deleteLater();
			colorFiltersView = nullptr;
		}
		iter = colorFilterMap.erase(iter);
	}
}

void PLSBasicFilters::AddFilterAction(QMenu *popup, const char *type, const QString &tooltip)
{
	const char *name = obs_source_get_display_name(type);

	QAction *popupItem = new QAction(QT_UTF8(name), this);
	popupItem->setToolTip(tooltip);
	popupItem->setData(QT_UTF8(type));
	connect(popupItem, SIGNAL(triggered(bool)), this, SLOT(AddFilterFromAction()));
	popup->addAction(popupItem);
}

QMenu *PLSBasicFilters::CreateAddFilterPopupMenu(QMenu *popup, bool async)
{
	uint32_t sourceFlags = obs_source_get_output_flags(source);

	std::vector<std::vector<FilterTypeInfo>> presetTypeList;
	std::vector<QString> otherTypeList;
	GetFilterTypeList(async, sourceFlags, presetTypeList, otherTypeList);

	// add preset source type
	for (auto iter = presetTypeList.begin(); iter != presetTypeList.end(); ++iter) {
		std::vector<FilterTypeInfo> &subList = *iter;
		for (unsigned i = 0; i < subList.size(); ++i) {
			QString id = subList[i].id;
			QString tooptip = subList[i].tooltip;
			AddFilterAction(popup, id.toStdString().c_str(), tooptip);
		}
	}

	//add other source type
	if (!otherTypeList.empty()) {
		for (int i = 0; i < otherTypeList.size(); i++) {
			AddFilterAction(popup, otherTypeList[i].toStdString().c_str(), "");
		}
	}
	bool foundValues = false;
	if (!presetTypeList.empty() || !otherTypeList.empty()) {
		foundValues = true;
	}

	if (!foundValues) {
		delete popup;
		popup = nullptr;
	}

	return popup;
}

void PLSBasicFilters::AddNewFilter(const char *id)
{
	if (id && *id) {
		QString log = "Filter " + QString(id) + " Clicked In Right Popup Menu";
		PLS_UI_STEP(MAINFILTER_MODULE, log.toStdString().c_str(), ACTION_CLICK);

		obs_source_t *existing_filter;
		string name = obs_source_get_display_name(id);

		NameDialog dialog(this);
		bool success = dialog.AskForName(this, QTStr("Basic.Filters.AddFilter.Title"), QTStr("Basic.FIlters.AddFilter.Text"), name, QT_UTF8(name.c_str()));
		if (!success)
			return;

		name = QString(name.c_str()).simplified().toStdString();
		if (name.empty()) {
			PLSMessageBox::warning(this, QTStr("NoNameEntered.Title"), QTStr("NoNameEntered.Text"));
			AddNewFilter(id);
			return;
		}

		existing_filter = obs_source_get_filter_by_name(source, name.c_str());
		if (existing_filter) {
			PLSMessageBox::warning(this, QTStr("NameExists.Title"), QTStr("NameExists.Text"));
			obs_source_release(existing_filter);
			AddNewFilter(id);
			return;
		}

		obs_source_t *filter = obs_source_create(id, name.c_str(), nullptr, nullptr);
		if (filter) {
			const char *sourceName = obs_source_get_name(source);

			blog(LOG_INFO,
			     "User added filter '%s' (%s) "
			     "to source '%s'",
			     name.c_str(), id, sourceName);

			obs_source_filter_add(source, filter);
			obs_source_release(filter);
		}
	}
}

void PLSBasicFilters::AddFilterFromAction()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (!action)
		return;

	AddNewFilter(QT_TO_UTF8(action->data().toString()));
}

void PLSBasicFilters::closeEvent(QCloseEvent *event)
{
	QDialog::closeEvent(event);
	if (!event->isAccepted())
		return;

	obs_display_remove_draw_callback(ui->preview->GetDisplay(), PLSBasicFilters::DrawPreview, this);

	PLSBasic *main = PLSBasic::Get();
	if (main)
		main->SaveProject();
}

/* PLS Signals */

void PLSBasicFilters::OBSSourceFilterAdded(void *param, calldata_t *data)
{
	PLSBasicFilters *window = reinterpret_cast<PLSBasicFilters *>(param);
	obs_source_t *filter = (obs_source_t *)calldata_ptr(data, "filter");

	QMetaObject::invokeMethod(window, "AddFilter", Q_ARG(OBSSource, OBSSource(filter)), Q_ARG(bool, true));
}

void PLSBasicFilters::OBSSourceFilterRemoved(void *param, calldata_t *data)
{
	PLSBasicFilters *window = reinterpret_cast<PLSBasicFilters *>(param);
	obs_source_t *filter = (obs_source_t *)calldata_ptr(data, "filter");

	QMetaObject::invokeMethod(window, "RemoveFilter", Q_ARG(OBSSource, OBSSource(filter)));
}

void PLSBasicFilters::OBSSourceReordered(void *param, calldata_t *data)
{
	QMetaObject::invokeMethod(reinterpret_cast<PLSBasicFilters *>(param), "ReorderFilters");

	UNUSED_PARAMETER(data);
}

void PLSBasicFilters::SourceRemoved(void *param, calldata_t *data)
{
	UNUSED_PARAMETER(data);

	QMetaObject::invokeMethod(static_cast<PLSBasicFilters *>(param), "close");
}

void PLSBasicFilters::SourceRenamed(void *param, calldata_t *data)
{
	const char *name = calldata_string(data, "new_name");
	QString title = QTStr("Basic.Filters.Title").arg(QT_UTF8(name));

	QMetaObject::invokeMethod(static_cast<PLSBasicFilters *>(param), "setWindowTitle", Q_ARG(QString, title));
}

void PLSBasicFilters::DrawPreview(void *data, uint32_t cx, uint32_t cy)
{
	PLSBasicFilters *window = static_cast<PLSBasicFilters *>(data);

	if (!window->source)
		return;

	uint32_t sourceCX = max(obs_source_get_width(window->source), 1u);
	uint32_t sourceCY = max(obs_source_get_height(window->source), 1u);

	int x, y;
	int newCX, newCY;
	float scale;

	GetScaleAndCenterPos(sourceCX, sourceCY, cx, cy, x, y, scale);

	newCX = int(scale * float(sourceCX));
	newCY = int(scale * float(sourceCY));

	gs_viewport_push();
	gs_projection_push();
	gs_ortho(0.0f, float(sourceCX), 0.0f, float(sourceCY), -100.0f, 100.0f);
	gs_set_viewport(x, y, newCX, newCY);

	obs_source_video_render(window->source);

	gs_projection_pop();
	gs_viewport_pop();
}

/* Qt Slots */
void PLSBasicFilters::OnFiltersRowChanged(const int &pre, const int &next)
{
	if (next == pre) {
		return;
	}
	OBSSource filter = ui->filtersListWidget->GetFilter(pre);
	if (filter)
		obs_source_filter_set_order_by_index(source, filter, next - pre);
}

void PLSBasicFilters::on_actionRemoveFilter_triggered()
{
	OBSSource filter = ui->filtersListWidget->GetFilter(ui->filtersListWidget->GetCurrentRow());
	if (filter) {
		if (ui->filtersListWidget->QueryRemove(this, filter))
			obs_source_filter_remove(source, filter);
	}
}

void PLSBasicFilters::OnAddFilterButtonClicked()
{
	PLS_UI_STEP(MAINFILTER_MODULE, "Add Filter Button", ACTION_CLICK);
	QMenu *popup = new QMenu(QTStr("Add"), this);

	uint32_t flags = obs_source_get_output_flags(source);
	bool audio = (flags & OBS_SOURCE_AUDIO) != 0;
	bool audioOnly = (flags & OBS_SOURCE_VIDEO) == 0;
	bool async = (flags & OBS_SOURCE_ASYNC) != 0;

	bool asyncFiltersVisible = true;
	bool effectFiltersVisible = true;
	if (!async && !audio) {
		asyncFiltersVisible = false;
	}
	if (audioOnly) {
		effectFiltersVisible = false;
	}

	if (asyncFiltersVisible && !effectFiltersVisible) {
		QMenu *asyncAction = new QMenu(QTStr("Basic.Filters.AsyncFilters"), this);
		asyncAction->setToolTipsVisible(true);

		CreateAddFilterPopupMenu(asyncAction, true);

		QAction *effectAction = new QAction(QTStr("Basic.Filters.EffectFilters"));
		effectAction->setEnabled(false);
		popup->addAction(effectAction);
		popup->addMenu(asyncAction);

	} else if (!asyncFiltersVisible && effectFiltersVisible) {
		QAction *asyncAction = new QAction(QTStr("Basic.Filters.AsyncFilters"));
		asyncAction->setEnabled(false);
		QMenu *effectAction = new QMenu(QTStr("Basic.Filters.EffectFilters"), this);
		effectAction->setToolTipsVisible(true);
		CreateAddFilterPopupMenu(effectAction, false);

		popup->addMenu(effectAction);
		popup->addAction(asyncAction);
	} else {
		QMenu *asyncAction = new QMenu(QTStr("Basic.Filters.AsyncFilters"), this);
		asyncAction->setToolTipsVisible(true);
		CreateAddFilterPopupMenu(asyncAction, true);

		QMenu *effectAction = new QMenu(QTStr("Basic.Filters.EffectFilters"), this);
		effectAction->setToolTipsVisible(true);

		CreateAddFilterPopupMenu(effectAction, false);
		popup->addMenu(effectAction);
		popup->addMenu(asyncAction);
	}

	popup->exec(QCursor::pos());
}

void PLSBasicFilters::OnCurrentItemChanged(const int &row)
{
	UpdatePropertiesView(row);
}

void PLSBasicFilters::OnColorFilterOriginalPressed(bool state)
{
	colorFilterOriginalPressed = state;
	emit colorFilterOriginalPressedSignal(state);
}

void PLSBasicFilters::OnColorFilterValueChanged(int value)
{
	int row = ui->filtersListWidget->GetCurrentRow();
	OBSSource filter = ui->filtersListWidget->GetFilter(row);

	if (!filter)
		return;

	for (auto iter = colorFilterMap.begin(); iter != colorFilterMap.end(); iter++) {
		PLSColorFilterView *colorFiltersView = iter->second;
		if (colorFiltersView && colorFiltersView->GetSource() == filter) {
			colorFiltersView->UpdateColorFilterValue(value);
			break;
		}
	}
}

void PLSBasicFilters::UpdatePropertyColorFilterValue(int value, bool isOriginal)
{
	if (view) {
		view->UpdateColorFilterValue(value, isOriginal);
	}
}

void PLSBasicFilters::ResetFilters()
{
	int row = ui->filtersListWidget->GetCurrentRow();
	OBSSource filter = ui->filtersListWidget->GetFilter(row);

	if (!filter)
		return;

	obs_data_t *settings = obs_source_get_settings(filter);
	obs_data_clear(settings);
	obs_data_release(settings);

	if (!view->DeferUpdate())
		obs_source_update(filter, nullptr);

	view->SetCustomContentMargins(true);
	view->ReloadProperties();
	for (auto iter = colorFilterMap.begin(); iter != colorFilterMap.end(); iter++) {
		PLSColorFilterView *colorFiltersView = iter->second;
		if (colorFiltersView && colorFiltersView->GetSource() == filter) {
			colorFiltersView->ResetColorFilter();
			break;
		}
	}
}

void PLSBasicFilters::RenameFilter()
{
	ui->filtersListWidget->RenameCurrentFilter();
}
