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

#pragma once

#include <QDialogButtonBox>
#include <memory>
#include <obs.hpp>

#include "dialog-view.hpp"
#include "network-access-manager.hpp"
#include "properties-view.hpp"
#include "PLSFiltersItemView.h"
#include "PLSColorFilterView.h"
#include "PLSMenu.hpp"

class PLSBasic;
class QMenu;
using ColorFilterMap = std::map<QString, PLSColorFilterView *>;

#include "ui_PLSBasicFilters.h"

class PLSBasicFilters : public PLSDialogView {
	Q_OBJECT

private:
	PLSBasic *main;
	std::unique_ptr<Ui::PLSBasicFilters> ui;
	OBSSource source;
	PLSPropertiesView *view = nullptr;
	ColorFilterMap colorFilterMap;
	OBSSignal addSignal;
	OBSSignal removeSignal;
	OBSSignal reorderSignal;

	OBSSignal removeSourceSignal;
	OBSSignal renameSourceSignal;
	OBSSignal updatePropertiesSignal;
	bool displayFixed = true;
	bool colorFilterOriginalPressed = false;

	//------------------------ define preset type list ------------------------
	struct FilterTypeInfo {
		QString id;
		QString tooltip;
		bool visible;
		FilterTypeInfo(QString id_, QString tooltip_, bool isVisible_ = false) : visible(isVisible_), tooltip(tooltip_), id(id_) {}
	};

	void UpdateFilters();
	bool UpdateColorFilters(obs_source_t *filter);
	QWidget *CreateColorFitlersView(obs_source_t *filter);
	void CleanColorFiltersView();
	void AddFilterAction(QMenu *popup, const char *type, const QString &tooltip);
	void UpdatePropertiesView(int row);

	static void OBSSourceFilterAdded(void *param, calldata_t *data);
	static void OBSSourceFilterRemoved(void *param, calldata_t *data);
	static void OBSSourceReordered(void *param, calldata_t *data);
	static void SourceRemoved(void *param, calldata_t *data);
	static void SourceRenamed(void *param, calldata_t *data);
	static void UpdateProperties(void *data, calldata_t *params);
	static void DrawPreview(void *data, uint32_t cx, uint32_t cy);

	QMenu *CreateAddFilterPopupMenu(QMenu *popup, bool async);

	void AddNewFilter(const char *id);
	void ReorderFilter(PLSFiltersListView *list, obs_source_t *filter, size_t idx);
	void GetFilterTypeList(bool isAsync, const uint32_t &sourceFlags, std::vector<std::vector<FilterTypeInfo>> &preset, std::vector<QString> &other);

private slots:
	void AddFilter(OBSSource filter, bool reorder = false);
	void RemoveFilter(OBSSource filter);
	void ReorderFilters();
	void ResetFilters();
	void RenameFilter();

	void AddFilterFromAction();

	void OnFiltersRowChanged(const int &pre, const int &next);
	void on_actionRemoveFilter_triggered();

	void OnAddFilterButtonClicked();
	void OnCurrentItemChanged(const int &row);
	void OnColorFilterOriginalPressed(bool state);
	void OnColorFilterValueChanged(int value);
	void UpdatePropertyColorFilterValue(int value, bool isOriginal);

public:
	explicit PLSBasicFilters(QWidget *parent, OBSSource source_, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSBasicFilters();

	void Init();
	OBSSource GetSource();

protected:
	virtual void closeEvent(QCloseEvent *event) override;

signals:
	void colorFilterOriginalPressedSignal(bool state);
};
