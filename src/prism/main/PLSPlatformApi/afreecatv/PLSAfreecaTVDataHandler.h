#pragma once

#include "../../channels/ChannelsDataApi/PLSChannelDataHandler.h"

class PLSAfreecaTVDataHandler : public ChannelDataBaseHandler {

public:
	PLSAfreecaTVDataHandler();
	virtual ~PLSAfreecaTVDataHandler();

	virtual QString getPlatformName() override;

	virtual bool tryToUpdate(const QVariantMap &srcInfo, UpdateCallback finishedCall) override;
};
