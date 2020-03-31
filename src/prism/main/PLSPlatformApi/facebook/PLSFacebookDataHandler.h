#ifndef PLSFACEBOOKDATAHANDLER_H
#define PLSFACEBOOKDATAHANDLER_H

#include "../../channels/ChannelsDataApi/PLSChannelDataHandler.h"

class PLSFacebookDataHandler : public ChannelDataBaseHandler {
public:
	PLSFacebookDataHandler();
	virtual ~PLSFacebookDataHandler();
	virtual QString getPlatformName();
	virtual bool tryToUpdate(const QVariantMap &srcInfo, UpdateCallback finishedCall);
};

#endif // PLSFACEBOOKDATAHANDLER_H
