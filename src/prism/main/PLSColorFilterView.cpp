#include "PLSColorFilterView.h"
#include "ui_PLSColorFilterView.h"

#include "pls-common-define.hpp"
#include "obs-properties.h"
#include "qt-wrappers.hpp"
#include "frontend-api.h"
#include "json-data-handler.hpp"
#include "log.h"
#include "log/module_names.h"
#include "action.h"

#include <QDir>
#include <QLabel>
#include <QImage>
#include <QStyle>
#include <QScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QJsonArray>
#include <QJsonObject>

#define ORIGINAL_THUMBNAIL_PNG "original_thumbnail.png"
#define ORIGINAL_PNG "original.png"
#define N1_THUMBNAIL_PNG "01_N1_thumbnail.png"
#define N1_PNG "01_N1.png"
#define N1_SHORT_TITLE "N1"
#define COLOR_FILTER_TEXT_FONT_SIZE 11
#define COLOR_FILTER_TEXT_COLOR "#FFFFFF"
#define COLOR_FILTER_TEXT_MARGIN_TOP 53

const int btnXCoordinateOffset = 4;

PLSColorFilterView::PLSColorFilterView(obs_source_t *source_, QWidget *parent) : QFrame(parent), source(source_), ui(new Ui::PLSColorFilterView)
{
	PLSDpiHelper dpiHelper;

	ui->setupUi(this);
	ui->horizontalLayout->setAlignment(Qt::AlignLeft);

	// dpiHelper.setFixedSize(this, {FILTERS_DISPLAY_VIEW_MAX_WIDTH, COLOR_FILTERS_IMAGE_FIXED_WIDTH});
	GetDefaultColorFilterPath();
	LoadDefaultColorFilter();

	leftScrollBtn = new QPushButton(this);
	leftScrollBtn->setObjectName(OBJECT_NAME_LEFT_SCROLL_BTN);
	connect(leftScrollBtn, &QPushButton::clicked, this, &PLSColorFilterView::OnLeftScrollBtnClicked);

	rightScrollBtn = new QPushButton(this);
	rightScrollBtn->setObjectName(OBJECT_NAME_RIGHT_SCROLL_BTN);
	connect(rightScrollBtn, &QPushButton::clicked, this, &PLSColorFilterView::OnRightScrollBtnClicked);

	dpiHelper.notifyDpiChanged(this, [=](double dpi) {
		leftScrollBtn->move(PLSDpiHelper::calculate(dpi, 5), PLSDpiHelper::calculate(dpi, 15));
		rightScrollBtn->move(this->width() - rightScrollBtn->width() - PLSDpiHelper::calculate(this, btnXCoordinateOffset), this->height() / 2 - rightScrollBtn->height() / 2);
	});
}

PLSColorFilterView::~PLSColorFilterView()
{
	delete ui;
}

void PLSColorFilterView::ResetColorFilter()
{
	QString defaultFilter = pls_get_color_filter_dir_path().append(ORIGINAL_PNG);
	UpdateFilterItemStyle(defaultFilter);
	auto bar = ui->scrollArea->horizontalScrollBar();
	bar->setValue(0);
	ResetConfigFile();
	UpdateColorFilterSource(defaultFilter);
}

void PLSColorFilterView::UpdateColorFilterValue(const int &value)
{
	for (auto &item : ImageVec) {
		if (!item) {
			continue;
		}
		if (item->GetClickedState()) {
			item->SetValue(value);
			WriteConfigFile(item);
			break;
		}
	}
}

OBSSource PLSColorFilterView::GetSource()
{
	return source;
}

void PLSColorFilterView::showEvent(QShowEvent *event)
{
	QFrame::showEvent(event);
	SetDefaultCurrentColorFilter();
}

void PLSColorFilterView::resizeEvent(QResizeEvent *event)
{
	if (leftScrollBtn)
		leftScrollBtn->move(PLSDpiHelper::calculate(this, btnXCoordinateOffset), this->height() / 2 - rightScrollBtn->height() / 2);
	if (rightScrollBtn)
		rightScrollBtn->move(this->width() - rightScrollBtn->width() - PLSDpiHelper::calculate(this, btnXCoordinateOffset), this->height() / 2 - rightScrollBtn->height() / 2);
	QFrame::resizeEvent(event);
}

void PLSColorFilterView::OnLeftScrollBtnClicked()
{
	auto bar = ui->scrollArea->horizontalScrollBar();
	bar->setValue(bar->value() - GetScrollAreaWidth());
}

void PLSColorFilterView::OnRightScrollBtnClicked()
{
	auto bar = ui->scrollArea->horizontalScrollBar();
	bar->setValue(bar->value() + GetScrollAreaWidth());
}

void PLSColorFilterView::OnColorFilterImageMousePressd(PLSColorFilterImage *image)
{
	if (!image) {
		return;
	}
	PLS_UI_STEP(MAINFILTER_MODULE, image->GetPath().toStdString().c_str(), ACTION_CLICK);
	UpdateFilterItemStyle(image);
	UpdateColorFilterSource(image->GetPath());
}

int PLSColorFilterView::GetScrollAreaWidth()
{
	double dpi = PLSDpiHelper::getDpi(this);
	int value = this->width() / PLSDpiHelper::calculate(dpi, COLOR_FILTERS_IMAGE_FIXED_WIDTH + 10);
	return value * PLSDpiHelper::calculate(dpi, COLOR_FILTERS_IMAGE_FIXED_WIDTH);
}

void PLSColorFilterView::CreateColorFilterImage(const QString &name, const QString &shortTitle, const QString &filterPath, const QString &thumbnailPath)
{
	PLSColorFilterImage *filterImage = new PLSColorFilterImage(name, shortTitle, filterPath, thumbnailPath, this);
	if (0 == currentPath.compare(filterPath)) {
		filterImage->SetClickedState(true);
	}

	connect(filterImage, &PLSColorFilterImage::MousePressd, this, &PLSColorFilterView::OnColorFilterImageMousePressd);
	QImage image;
	if (image.load(thumbnailPath)) {
		QPixmap pixmap = QPixmap::fromImage(image).scaled(QSize(this->width(), this->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		filterImage->SetPixMap(pixmap);
		filterImage->setObjectName(OBJECT_NAME_COLOR_FILTER_LABEL);
		ImageVec.push_back(filterImage);

		if (0 == thumbnailPath.compare(pls_get_color_filter_dir_path().append(ORIGINAL_THUMBNAIL_PNG))) {
			ui->horizontalLayout->insertWidget(0, filterImage);
		} else {
			ui->horizontalLayout->addWidget(filterImage);
		}
	}
}

void PLSColorFilterView::UpdateFilterItemStyle(PLSColorFilterImage *image)
{
	if (!image) {
		return;
	}

	for (auto &item : ImageVec) {
		if (!item) {
			continue;
		}
		if (image == item) {
			item->SetClickedState(true);
			int value;
			if (GetValueFromConfigFile(item->GetName(), value)) {
				emit ColorFilterValueChanged(value, image->GetPath() == pls_get_color_filter_dir_path() + ORIGINAL_PNG);
			}
		} else {
			item->SetClickedState(false);
		}
	}
}

void PLSColorFilterView::UpdateFilterItemStyle(const QString &imagePath)
{
	for (auto &item : ImageVec) {
		if (!item) {
			continue;
		}
		if (imagePath == item->GetPath()) {
			UpdateFilterItemStyle(item);
			break;
		}
	}
}

void PLSColorFilterView::UpdateColorFilterSource(const QString &path)
{
	obs_properties_t *props = obs_source_properties(source);
	QString name = obs_source_get_name(source);
	obs_property_t *property = obs_properties_first(props);
	while (property) {
		obs_property_type type = obs_property_get_type(property);
		if (OBS_PROPERTY_PATH == type) {
			const char *name = obs_property_name(property);
			OBSData settings = obs_source_get_settings(source);
			if (path == pls_get_color_filter_dir_path() + ORIGINAL_PNG) {
				emit OriginalPressed(true);
			} else {
				emit OriginalPressed(false);
			}
			obs_data_set_string(settings, name, QT_TO_UTF8(path));
			obs_source_update(source, settings);
			break;
		}
		obs_property_next(&property);
	}
	obs_properties_destroy(props);
}

void PLSColorFilterView::LoadDefaultColorFilter()
{
	QDir dir(pls_get_color_filter_dir_path());

	// origin thumbnail + N1
	CreateColorFilterImage(ORIGINAL_PNG, "", pls_get_color_filter_dir_path() + ORIGINAL_PNG, pls_get_color_filter_dir_path() + ORIGINAL_THUMBNAIL_PNG);
	CreateColorFilterImage(N1_PNG, N1_SHORT_TITLE, pls_get_color_filter_dir_path() + N1_PNG, pls_get_color_filter_dir_path() + N1_THUMBNAIL_PNG);

	QByteArray array;
	int index = 1;
	if (PLSJsonDataHandler::getJsonArrayFromFile(array, pls_get_color_filter_dir_path() + COLOR_FILTER_JSON_FILE)) {
		QJsonObject objectJson = QJsonDocument::fromJson(array).object();
		const QJsonArray &jsonArray = objectJson.value(HTTP_ITEMS).toArray();
		for (auto item : jsonArray) {
			QString name = item[HTTP_ITEM_ID].toString();
			QString title = item[TITLE].toString();
			QString shortTitle = item[SHORT_TITLE].toString();
			QString number{};
			if (index < COLOR_FILTER_ORDER_NUMBER) {
				number = number.append("0").append(QString::number(index++));
			} else {
				number = number.append(QString::number(index++));
			}
			QString colorFilterName = number + "_" + name + "_" + shortTitle + COLOR_FILTER_IMAGE_FORMAT_PNG;
			QString path = pls_get_color_filter_dir_path() + colorFilterName;
			QString thumbnailName = pls_get_color_filter_dir_path() + number + "_" + name + "_" + shortTitle + COLOR_FILTER_THUMBNAIL + COLOR_FILTER_IMAGE_FORMAT_PNG;

			CreateColorFilterImage(colorFilterName, shortTitle, path, thumbnailName);
		}
	}

	WriteConfigFile();
}

void PLSColorFilterView::GetDefaultColorFilterPath()
{
	obs_properties_t *props = obs_source_properties(this->source);
	if (!props) {
		return;
	}
	QString name = obs_source_get_name(this->source);
	obs_property_t *property = obs_properties_first(props);
	while (property) {
		obs_property_type type = obs_property_get_type(property);
		if (OBS_PROPERTY_PATH == type) {
			const char *name = obs_property_name(property);
			OBSData settings = obs_source_get_settings(source);
			currentPath = obs_data_get_string(settings, name);
		}

		if (OBS_PROPERTY_INT == type) {
			const char *name = obs_property_name(property);
			OBSData settings = obs_source_get_settings(source);
			currentValue = obs_data_get_int(settings, name);
		}
		obs_property_next(&property);
	}
	obs_properties_destroy(props);
}

void PLSColorFilterView::SetDefaultCurrentColorFilter()
{
	bool isFind = false;
	for (auto &image : ImageVec) {
		if (image && image->GetClickedState()) {
			isFind = true;
			if (image->GetPath() == pls_get_color_filter_dir_path() + ORIGINAL_PNG) {
				emit OriginalPressed(true);
			} else {
				emit OriginalPressed(false);
			}
			break;
		}
	}

	if (!isFind) {
		emit OriginalPressed(true);
		UpdateFilterItemStyle(pls_get_color_filter_dir_path().append(ORIGINAL_PNG));
	}
}

bool PLSColorFilterView::WriteConfigFile(PLSColorFilterImage *image)
{
	if (image) {
		return SetValueToConfigFile(image->GetName(), image->GetValue());
	}

	obs_data_array_t *filtersArray = obs_data_array_create();
	for (auto &tmpImage : ImageVec) {
		if (!tmpImage) {
			continue;
		}

		obs_data_t *data = obs_data_create();
		obs_data_set_string(data, "name", tmpImage->GetName().toStdString().c_str());

		int value;
		if (GetValueFromConfigFile(tmpImage->GetName(), value)) {
			obs_data_set_int(data, "value", value);
		} else {
			obs_data_set_int(data, "value", tmpImage->GetValue());
		}

		if (tmpImage->GetPath() == currentPath && tmpImage->GetName() != ORIGINAL_PNG) {
			obs_data_set_int(data, "value", currentValue);
		}

		obs_data_array_push_back(filtersArray, data);
		obs_data_release(data);
	}

	obs_data_t *privateData = obs_source_get_private_settings(source);
	obs_data_set_array(privateData, "color_filter", filtersArray);
	obs_data_array_release(filtersArray);
	obs_data_release(privateData);

	return true;
}

bool PLSColorFilterView::ResetConfigFile()
{
	obs_data_array_t *filtersArray = obs_data_array_create();
	for (auto &tmpImage : ImageVec) {
		if (!tmpImage) {
			continue;
		}

		obs_data_t *data = obs_data_create();
		obs_data_set_string(data, "name", tmpImage->GetName().toStdString().c_str());
		obs_data_set_int(data, "value", COLOR_FILTER_DENSITY_DEFAULT_VALUE);
		obs_data_array_push_back(filtersArray, data);
		obs_data_release(data);
	}

	obs_data_t *privateData = obs_source_get_private_settings(source);
	obs_data_set_array(privateData, "color_filter", filtersArray);
	obs_data_array_release(filtersArray);
	obs_data_release(privateData);

	return true;
}

bool PLSColorFilterView::SetValueToConfigFile(const QString &name, const int &value)
{
	obs_data_t *privateData = obs_source_get_private_settings(source);
	if (!privateData) {
		return false;
	}

	obs_data_array_t *filterArray = obs_data_get_array(privateData, "color_filter");
	if (!filterArray) {
		obs_data_release(privateData);
		return false;
	}

	bool find = false;
	size_t num = obs_data_array_count(filterArray);
	for (size_t i = 0; i < num; i++) {
		obs_data_t *data = obs_data_array_item(filterArray, i);
		if (0 == strcmp(name.toStdString().c_str(), obs_data_get_string(data, "name"))) {
			obs_data_set_int(data, "value", value);
			find = true;
			obs_data_release(data);
			break;
		}
		obs_data_release(data);
	}

	obs_data_array_release(filterArray);
	obs_data_release(privateData);

	return find;
}

bool PLSColorFilterView::GetValueFromConfigFile(const QString &name, int &value)
{
	obs_data_t *privateData = obs_source_get_private_settings(source);
	if (!privateData) {
		return false;
	}

	obs_data_array_t *filterArray = obs_data_get_array(privateData, "color_filter");
	if (!filterArray) {
		obs_data_release(privateData);
		return false;
	}

	bool find = false;
	size_t num = obs_data_array_count(filterArray);
	for (size_t i = 0; i < num; i++) {
		obs_data_t *data = obs_data_array_item(filterArray, i);
		if (0 == strcmp(name.toStdString().c_str(), obs_data_get_string(data, "name"))) {
			value = obs_data_get_int(data, "value");
			find = true;
			obs_data_release(data);
			break;
		}
		obs_data_release(data);
	}

	obs_data_array_release(filterArray);
	obs_data_release(privateData);

	return find;
}

PLSColorFilterImage::PLSColorFilterImage(const QString &name_, const QString &shortTitle_, const QString &path_, const QString &thumbnail_, QWidget *parent /*= nullptr*/)
	: QPushButton(parent), name(name_), shortTitle(shortTitle_), path(path_), thumbnail(thumbnail_)
{
	connect(this, &PLSColorFilterImage::ClickedStateChanged, this, &PLSColorFilterImage::SetClickedStyle);
}

QString PLSColorFilterImage::GetPath()
{
	return path;
}

QString PLSColorFilterImage::GetThumbnailPath()
{
	return thumbnail;
}

QString PLSColorFilterImage::GetName()
{
	return name;
}

int PLSColorFilterImage::GetValue()
{
	return value;
}

void PLSColorFilterImage::SetValue(const int &value)
{
	this->value = value;
}

void PLSColorFilterImage::SetClickedState(bool clicked)
{
	if (this->clicked != clicked) {
		this->clicked = clicked;
		emit ClickedStateChanged(clicked);
	}
}

void PLSColorFilterImage::SetPixMap(const QPixmap &pixmap)
{
	this->pixmap = pixmap;
	update();
}

bool PLSColorFilterImage::GetClickedState()
{
	return clicked;
}

void PLSColorFilterImage::SetClickedStyle(bool clicked)
{
	this->setProperty(STATUS_CLICKED, clicked);
	this->style()->unpolish(this);
	this->style()->polish(this);
}

void PLSColorFilterImage::mousePressEvent(QMouseEvent *event)
{
	emit MousePressd(this);
	QPushButton::mousePressEvent(event);
}

void PLSColorFilterImage::paintEvent(QPaintEvent *event)
{
	if (!pixmap.isNull()) {
		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing, true);
		painter.save();

		double dpi = PLSDpiHelper::getDpi(this);

		QPainterPath painterPath;
		painterPath.addRoundedRect(this->rect(), PLSDpiHelper::calculate(dpi, 3.0), PLSDpiHelper::calculate(dpi, 3.0));
		painter.setClipPath(painterPath);
		painter.drawPixmap(painterPath.boundingRect().toRect(), pixmap.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter.setPen(Qt::NoPen);
		painter.drawPath(painterPath);
		painter.restore();

		if (!name.contains(ORIGINAL_PNG)) {
			QFont font;
			font.setPixelSize(PLSDpiHelper::calculate(dpi, COLOR_FILTER_TEXT_FONT_SIZE));
			font.setBold(true);
			painter.setFont(font);
			painter.setPen(QColor(COLOR_FILTER_TEXT_COLOR));

			int widthOfTitle = painter.fontMetrics().width(shortTitle);
			painter.drawText(this->width() / 2 - widthOfTitle / 2, PLSDpiHelper::calculate(dpi, COLOR_FILTER_TEXT_MARGIN_TOP), shortTitle);
		}
	}

	QPushButton::paintEvent(event);
}
