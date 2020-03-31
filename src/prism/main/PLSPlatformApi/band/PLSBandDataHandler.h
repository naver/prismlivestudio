#ifndef PLSBANDDATAHANDLER_H
#define PLSBANDDATAHANDLER_H

#include <qobject.h>

#include "../../channels/ChannelsDataApi/PLSChannelDataHandler.h"

#include "../PLSHttpApi/PLSNetworkReplyBuilder.h"
#include "../PLSHttpApi/PLSHttpHelper.h"

#include "../channels/ChannelsDataApi/PLSChannelDataAPI.h"
#include "../channels/ChannelsDataApi/ChannelCommonFunctions.h"
#include "PLSPlatformBand.h"

class PLSBandDataHandler : public QObject, public ChannelDataBaseHandler {

	Q_OBJECT
public:
	PLSBandDataHandler();
	virtual ~PLSBandDataHandler();

	virtual QString getPlatformName();

	virtual bool tryToUpdate(const QVariantMap &srcInfo, UpdateCallback finishedCall);
	virtual bool isMultiChildren() { return true; }

private:
	void getBandTokenInfo(UpdateCallback finishedCall);
	void getBandCategoryInfo(UpdateCallback finishedCall);
	QPair<bool, QString> getChannelEmblemSync(const QString &url);
	//void getBandUserProfile(UpdateCallback finishedCall);

private:
	QList<QVariantMap> m_bandInfos;
	PLSPlatformBand *m_platformBand;

	QVariantMap m_bandLoginInfo;
};

#endif // PLSBANDDATAHANDLER_H
