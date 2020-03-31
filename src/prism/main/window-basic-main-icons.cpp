#include <window-basic-main.hpp>
#include <pls-common-define.hpp>

// icon for add source menu
QIcon PLSBasic::GetSourceIcon(const char *id) const
{
	obs_icon_type type = obs_source_get_icon_type(id);

	switch (type) {
	case OBS_ICON_TYPE_IMAGE:
		return GetImageIcon();
	case OBS_ICON_TYPE_COLOR:
		return GetColorIcon();
	case OBS_ICON_TYPE_SLIDESHOW:
		return GetSlideshowIcon();
	case OBS_ICON_TYPE_AUDIO_INPUT:
		return GetAudioInputIcon();
	case OBS_ICON_TYPE_AUDIO_OUTPUT:
		return GetAudioOutputIcon();
	case OBS_ICON_TYPE_DESKTOP_CAPTURE:
	case OBS_ICON_TYPE_REGION:
		return GetDesktopCapIcon();
	case OBS_ICON_TYPE_WINDOW_CAPTURE:
		return GetWindowCapIcon();
	case OBS_ICON_TYPE_GAME_CAPTURE:
		return GetGameCapIcon();
	case OBS_ICON_TYPE_CAMERA:
		return GetCameraIcon();
	case OBS_ICON_TYPE_TEXT:
		return GetTextIcon();
	case OBS_ICON_TYPE_MEDIA:
		return GetMediaIcon();
	case OBS_ICON_TYPE_BROWSER:
		return GetBrowserIcon();
	case OBS_ICON_TYPE_BGM:
		return GetBgmIcon();
	case OBS_ICON_TYPE_GIPHY:
		return GetStickerIcon();
	case OBS_ICON_TYPE_NDI:
		return GetNdiIcon();
	case OBS_ICON_TYPE_TEXT_MOTION:
		return GetTextMotionIcon();
	case OBS_ICON_TYPE_CHAT:
		return GetChatIcon();
	case OBS_ICON_TYPE_CUSTOM:
	default:
		//TODO: Add ability for sources to define custom icons
		if (0 == strcmp(id, SCENE_SOURCE_ID)) {
			return GetSceneIcon();
		} else if (0 == strcmp(id, GROUP_SOURCE_ID)) {
			return GetGroupIcon();
		} else {
			return GetDefaultIcon();
		}
	}
}

void PLSBasic::SetImageIcon(const QIcon &icon)
{
	imageIcon = icon;
}

void PLSBasic::SetColorIcon(const QIcon &icon)
{
	colorIcon = icon;
}

void PLSBasic::SetSlideshowIcon(const QIcon &icon)
{
	slideshowIcon = icon;
}

void PLSBasic::SetAudioInputIcon(const QIcon &icon)
{
	audioInputIcon = icon;
}

void PLSBasic::SetAudioOutputIcon(const QIcon &icon)
{
	audioOutputIcon = icon;
}

void PLSBasic::SetDesktopCapIcon(const QIcon &icon)
{
	desktopCapIcon = icon;
}

void PLSBasic::SetWindowCapIcon(const QIcon &icon)
{
	windowCapIcon = icon;
}

void PLSBasic::SetGameCapIcon(const QIcon &icon)
{
	gameCapIcon = icon;
}

void PLSBasic::SetCameraIcon(const QIcon &icon)
{
	cameraIcon = icon;
}

void PLSBasic::SetTextIcon(const QIcon &icon)
{
	textIcon = icon;
}

void PLSBasic::SetMediaIcon(const QIcon &icon)
{
	mediaIcon = icon;
}

void PLSBasic::SetBrowserIcon(const QIcon &icon)
{
	browserIcon = icon;
}

void PLSBasic::SetGroupIcon(const QIcon &icon)
{
	groupIcon = icon;
}

void PLSBasic::SetSceneIcon(const QIcon &icon)
{
	sceneIcon = icon;
}

void PLSBasic::SetStickerIcon(const QIcon &icon)
{
	stickerIcon = icon;
}

void PLSBasic::SetDefaultIcon(const QIcon &icon)
{
	defaultIcon = icon;
}

void PLSBasic::SetBgmIcon(const QIcon &icon)
{
	bgmIcon = icon;
}

void PLSBasic::SetTextMotionIcon(const QIcon &icon)
{
	textMotionIcon = icon;
}

void PLSBasic::SetChatIcon(const QIcon &icon)
{
	chatIcon = icon;
}

void PLSBasic::SetNdiIcon(const QIcon &icon)
{
	ndiIcon = icon;
}

QIcon PLSBasic::GetImageIcon() const
{
	return imageIcon;
}

QIcon PLSBasic::GetColorIcon() const
{
	return colorIcon;
}

QIcon PLSBasic::GetSlideshowIcon() const
{
	return slideshowIcon;
}

QIcon PLSBasic::GetAudioInputIcon() const
{
	return audioInputIcon;
}

QIcon PLSBasic::GetAudioOutputIcon() const
{
	return audioOutputIcon;
}

QIcon PLSBasic::GetDesktopCapIcon() const
{
	return desktopCapIcon;
}

QIcon PLSBasic::GetWindowCapIcon() const
{
	return windowCapIcon;
}

QIcon PLSBasic::GetGameCapIcon() const
{
	return gameCapIcon;
}

QIcon PLSBasic::GetCameraIcon() const
{
	return cameraIcon;
}

QIcon PLSBasic::GetTextIcon() const
{
	return textIcon;
}

QIcon PLSBasic::GetMediaIcon() const
{
	return mediaIcon;
}

QIcon PLSBasic::GetBrowserIcon() const
{
	return browserIcon;
}

QIcon PLSBasic::GetBgmIcon() const
{
	return bgmIcon;
}

QIcon PLSBasic::GetStickerIcon() const
{
	return stickerIcon;
}

QIcon PLSBasic::GetTextMotionIcon() const
{
	return textMotionIcon;
}

QIcon PLSBasic::GetChatIcon() const
{
	return chatIcon;
}

QIcon PLSBasic::GetNdiIcon() const
{
	return ndiIcon;
}

QIcon PLSBasic::GetGroupIcon() const
{
	return groupIcon;
}

QIcon PLSBasic::GetSceneIcon() const
{
	return sceneIcon;
}

QIcon PLSBasic::GetDefaultIcon() const
{
	return defaultIcon;
}
