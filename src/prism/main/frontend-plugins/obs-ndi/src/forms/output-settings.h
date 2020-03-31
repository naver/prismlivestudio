/*
obs-ndi
Copyright (C) 2016-2018 St閜hane Lepin <steph  name of author

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; If not, see <https://www.gnu.org/licenses/>
*/

#ifndef OUTPUTSETTINGS_H
#define OUTPUTSETTINGS_H

#include "dialog-view.hpp"
#include "ui_output-settings.h"

class OutputSettings : public PLSDialogView {
	Q_OBJECT
public:
	explicit OutputSettings(QWidget *parent = 0, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~OutputSettings();
	void showEvent(QShowEvent *event);
	void ToggleShowHide();

private slots:
	void onFormAccepted();

private:
	Ui::OutputSettings *ui;
};

#endif // OUTPUTSETTINGS_H
