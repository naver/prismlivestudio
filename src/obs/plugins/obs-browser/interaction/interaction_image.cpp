#include "interaction_image.h"
#include "interaction_util.hpp"
#include <util/util.hpp>

//--------------------------------------------------------------------------------
GDIPlusImage::GDIPlusImage() {}
GDIPlusImage::~GDIPlusImage() {}

bool GDIPlusImage::LoadImageFile(const WCHAR *file)
{
	assert(file);

	Gdiplus::Image *img = Gdiplus::Image::FromFile(file);
	if (!img || img->GetLastStatus() != Gdiplus::Ok) {
		gdip_image = IMAGE_PTR();
		if (img) {
			delete img;
			img = NULL;
		}
		assert(false);
		return false;
	} else {
		gdip_image = IMAGE_PTR(img);
		return true;
	}
}

void GDIPlusImage::RenderImage(Gdiplus::Graphics &graphics, RECT &rc)
{
	if (!gdip_image.get()) {
		return;
	}

	UINT image_cx = gdip_image->GetWidth();
	UINT image_cy = gdip_image->GetHeight();
	if (0 == image_cx || 0 == image_cy) {
		return;
	}

	Gdiplus::Rect dest;
	dest.X = 0;
	dest.Y = 0;
	dest.Width = interaction::RectWidth(rc);
	dest.Height = interaction::RectHeight(rc);

	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	graphics.SetInterpolationMode(
		Gdiplus::InterpolationModeHighQualityBicubic);
	graphics.SetTextRenderingHint(
		Gdiplus::TextRenderingHintClearTypeGridFit);

	graphics.DrawImage(gdip_image.get(), dest, 0, 0, image_cx, image_cy,
			   Gdiplus::Unit::UnitPixel);
}

//--------------------------------------------------------------------------------
ImageManager *ImageManager::Instance()
{
	static ImageManager instance;
	return &instance;
}

ImageManager::~ImageManager()
{
	assert(image_list.empty());
}

GDIP_IMAGE_PTR ImageManager::LoadImageFile(const WCHAR *file)
{
	assert(file);
	if (!file) {
		return GDIP_IMAGE_PTR();
	}

	std::wstring temp = file;
	GDIP_IMAGE_PTR ret;

	{
		std::lock_guard<std::mutex> lock(image_lock);
		GDIP_IMAGE_WEAK_PTR weak_ptr = image_list[temp];
		ret = weak_ptr.lock();
	}

	if (ret.get()) {
		return ret;
	}

	ret = GDIP_IMAGE_PTR(new GDIPlusImage());
	if (!ret->LoadImageFile(file)) {
		assert(false);
		return GDIP_IMAGE_PTR();
	}

	{
		std::lock_guard<std::mutex> lock(image_lock);
		image_list[temp] = GDIP_IMAGE_WEAK_PTR(ret);
	}

	return ret;
}

void ImageManager::ClearImage()
{
	std::lock_guard<std::mutex> lock(image_lock);
	image_list.clear();
}
