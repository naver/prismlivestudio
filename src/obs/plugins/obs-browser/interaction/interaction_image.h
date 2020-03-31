#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include <memory>
#include <mutex>
#include <map>

class GDIPlusImage {
public:
	GDIPlusImage();
	virtual ~GDIPlusImage();

	// invoked in interaction's thread (CEF's main thread)
	bool LoadImageFile(const WCHAR *file);
	void RenderImage(Gdiplus::Graphics &graphics, RECT &rc);

private:
	typedef std::shared_ptr<Gdiplus::Image> IMAGE_PTR;
	IMAGE_PTR gdip_image = IMAGE_PTR();
};

typedef std::weak_ptr<GDIPlusImage> GDIP_IMAGE_WEAK_PTR;
typedef std::shared_ptr<GDIPlusImage> GDIP_IMAGE_PTR;

class ImageManager {
protected:
	ImageManager() {}

public:
	static ImageManager *Instance();

	virtual ~ImageManager();

	GDIP_IMAGE_PTR LoadImageFile(const WCHAR *file);
	void ClearImage();

private:
	std::map<std::wstring, GDIP_IMAGE_WEAK_PTR> image_list;
	std::mutex image_lock;
};
