/******************************************************************************
 Copyright (C) 2014 by John R. Bradley <jrb@turrettech.com>
 Copyright (C) 2018 by Hugh Bailey ("Jim") <jim@obsproject.com>

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

#include <obs-module.h>
#include <obs.hpp>

#include "cef-headers.hpp"
#include "browser-config.h"
#include "browser-app.hpp"

#include <unordered_map>
#include <functional>
#include <vector>
#include <string>
#include <mutex>
#include "obs-browser-source.hpp"
#include <qstring.h>
#include <qjsonobject.h>

#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
extern bool hwaccel;
#endif

struct TextMotionSource : public BrowserSource {
	using BrowserSource::BrowserSource;
	void Update(obs_data_t *settings = nullptr);
	virtual void onBrowserLoadEnd() override;
	QString toJsonStr();
	void propertiesEditStart(obs_data_t *settings);
	QString type;
	int isMotionOnce;
	double animateDuration;
	int hAlign = -1;
	int letter = -1;
	QString textColor;
	QString textFamily;
	QString textStyle;
	QString textFontSize;
	QString bkColor;
	QString textLineColor;
	QString textLineSize;
	QString content;
	QString subTitle;
};
