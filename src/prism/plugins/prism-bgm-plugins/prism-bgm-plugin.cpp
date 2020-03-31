#include <obs-module.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("prism-bgm-plugin", "en-US")
MODULE_EXPORT const char *obs_module_description(void)
{
	return "Prism BGM plugin";
}

extern void register_prism_bgm_source();

bool obs_module_load(void)
{
	register_prism_bgm_source();
	return true;
}

const char *obs_module_name(void)
{
	return obs_module_description();
}
