#include <obs-module.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("prism-sticker-source", "en-US")
MODULE_EXPORT const char *obs_module_description(void)
{
	return "PRISM sticker source which can play GIF animations";
}

extern void RegisterPRISMStickerSource();

bool obs_module_load(void)
{
	RegisterPRISMStickerSource();
	return true;
}
