#pragma once

#include "../../channels/ChannelsDataApi/PLSChannelDataHandler.h"

class PLSVLiveDataHandler : public ChannelDataBaseHandler {

public:
	PLSVLiveDataHandler();
	virtual ~PLSVLiveDataHandler();

	virtual QString getPlatformName();

	virtual bool tryToUpdate(const QVariantMap &srcInfo, UpdateCallback finishedCall);
};
