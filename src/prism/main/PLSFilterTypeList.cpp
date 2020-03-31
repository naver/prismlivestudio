#include "window-basic-main.hpp"
#include "pls-common-define.hpp"

static bool filter_compatible(bool async, uint32_t sourceFlags, uint32_t filterFlags)
{
	bool filterVideo = (filterFlags & OBS_SOURCE_VIDEO) != 0;
	bool filterAsync = (filterFlags & OBS_SOURCE_ASYNC) != 0;
	bool filterAudio = (filterFlags & OBS_SOURCE_AUDIO) != 0;
	bool audio = (sourceFlags & OBS_SOURCE_AUDIO) != 0;
	bool audioOnly = (sourceFlags & OBS_SOURCE_VIDEO) == 0;
	bool asyncSource = (sourceFlags & OBS_SOURCE_ASYNC) != 0;

	if (async && ((audioOnly && filterVideo) || (!audio && !asyncSource)))
		return false;

	return (async && (filterAudio || filterAsync)) || (!async && !filterAudio && !filterAsync);
}

void PLSBasicFilters::GetFilterTypeList(bool isAsync, const uint32_t &sourceFlags, std::vector<std::vector<FilterTypeInfo>> &preset, std::vector<QString> &other)
{
	std::vector<std::vector<FilterTypeInfo>> filterList = {
		{
			FilterTypeInfo(FILTER_TYPE_ID_NOISEGATE, QTStr("Filter.tooptip.noise.gate")),
			FilterTypeInfo(FILTER_TYPE_ID_NOISE_SUPPRESSION, QTStr("Filter.tooptip.noise.suppression")),
			FilterTypeInfo(FILTER_TYPE_ID_COMPRESSOR, QTStr("Filter.tooptip.compressor")),
			FilterTypeInfo(FILTER_TYPE_ID_LIMITER, QTStr("Filter.tooptip.limiter")),
			FilterTypeInfo(FILTER_TYPE_ID_EXPANDER, QTStr("Filter.tooptip.expander")),
			FilterTypeInfo(FILTER_TYPE_ID_GAIN, QTStr("Filter.tooptip.gain")),
			FilterTypeInfo(FILTER_TYPE_ID_INVERT_POLARITY, QTStr("Filter.tooptip.invert.polarity")),
			FilterTypeInfo(FILTER_TYPE_ID_VIDEODELAY_ASYNC, QTStr("Filter.tooptip.video.delay")),
			FilterTypeInfo(FILTER_TYPE_ID_VSTPLUGIN, QTStr("Filter.tooptip.vst.plugin")),
		},
		{FilterTypeInfo(FILTER_TYPE_ID_CHROMAKEY, QTStr("Filter.tooptip.chromakey")), FilterTypeInfo(FILTER_TYPE_ID_COLOR_KEY_FILTER, QTStr("Filter.tooptip.colorkey")),
		 FilterTypeInfo(FILTER_TYPE_ID_LUMAKEY, QTStr("Filter.tooptip.lumakey")), FilterTypeInfo(FILTER_TYPE_ID_APPLYLUT, QTStr("Filter.tooptip.colorfilter")),
		 FilterTypeInfo(FILTER_TYPE_ID_COLOR_FILTER, QTStr("Filter.tooptip.colorcorrection")), FilterTypeInfo(FILTER_TYPE_ID_SHARPEN, QTStr("Filter.tooptip.sharpen")),
		 FilterTypeInfo(FILTER_TYPE_ID_SCALING_ASPECTRATIO, QTStr("Filter.tooptip.scale.aspectradio")), FilterTypeInfo(FILTER_TYPE_ID_CROP_PAD, QTStr("Filter.tooptip.corp.pad")),
		 FilterTypeInfo(FILTER_TYPE_ID_SCROLL, QTStr("Filter.tooptip.scroll")), FilterTypeInfo(FILTER_TYPE_ID_IMAGEMASK_BLEND, QTStr("Filter.tooptip.image.mask")),
		 FilterTypeInfo(FILTER_TYPE_ID_RENDER_DELAY, QTStr("Filter.tooptip.render.delay")),
		 FilterTypeInfo(FILTER_TYPE_ID_PREMULTIPLIED_ALPHA_FILTER, QTStr("Ndi.Filter.FixAlphaBlending.ToolTip"))}};

	//----------------------- load filter type list --------------------------
	const char *type_str;
	size_t idx = 0;
	while (obs_enum_filter_types(idx++, &type_str)) {
		const char *name = obs_source_get_display_name(type_str);
		uint32_t caps = obs_get_source_output_flags(type_str);

		if ((caps & OBS_SOURCE_DEPRECATED) != 0)
			continue;
		if ((caps & OBS_SOURCE_CAP_DISABLED) != 0)
			continue;

		bool isPresetDefined = false;
		for (unsigned i = 0; i < filterList.size() && !isPresetDefined; ++i) {
			std::vector<FilterTypeInfo> &subList = filterList[i];
			for (unsigned j = 0; j < subList.size() && !isPresetDefined; ++j) {
				FilterTypeInfo &info = subList[j];
				if (0 == strcmp(type_str, info.id.toStdString().c_str())) {
					info.visible = true;
					isPresetDefined = true;
				}
			}
		}

		if (!isPresetDefined)
			other.push_back(type_str);
	}

	//----------------------- remove unfound source type --------------------------
	for (unsigned i = 0; i < filterList.size(); ++i) {
		std::vector<FilterTypeInfo> typeList;
		std::vector<FilterTypeInfo> &subList = filterList[i];
		for (unsigned j = 0; j < subList.size(); ++j) {
			FilterTypeInfo &info = subList[j];
			uint32_t filterFlags = obs_get_source_output_flags(info.id.toStdString().c_str());
			if (!filter_compatible(isAsync, sourceFlags, filterFlags))
				continue;
			if (info.visible)
				typeList.push_back(FilterTypeInfo(info.id, info.tooltip));
		}

		if (!typeList.empty())
			preset.push_back(typeList);
	}
}
