#include "PLSChannelsEntrance.h"
#include <obs-module.h>
#include <obs.h>
#include <QDebug>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QThread>
#include <QToolBar>
#include "ChannelCapsule.h"
#include "ChannelCommonFunctions.h"
#include "LogPredefine.h"
#include "PLSChannelDataAPI.h"
#include "PLSChannelDataHandlerFunctions.h"
#include "PLSChannelsArea.h"
#include "PLSChannelsVirualAPI.h"
#include "frontend-api.h"
#include "log.h"
#include "pls-app.hpp"
#include "signal.h"
#include "window-basic-main.hpp"

void onLiveStateChanged(enum obs_frontend_event event, void *)
{
	switch (event) {
	case OBS_FRONTEND_EVENT_STREAMING_STARTING:
		//PLSCHANNELS_API->sigTrySetBroadcastState(ChannelData::StreamStarting);
		break;
	case OBS_FRONTEND_EVENT_STREAMING_STARTED:
		PLSCHANNELS_API->sigTrySetBroadcastState(ChannelData::StreamStarted);
		break;
	case OBS_FRONTEND_EVENT_STREAMING_STOPPING:
		//PLSCHANNELS_API->sigTrySetBroadcastState(ChannelData::StreamStopping);
		break;
	case OBS_FRONTEND_EVENT_STREAMING_STOPPED:
		//PLSCHANNELS_API->setBroadcastState(ChannelData::StreamStopped);
		break;
		/*recording       start       */
	case OBS_FRONTEND_EVENT_RECORDING_STARTING:
		PLSCHANNELS_API->sigTrySetRecordState(ChannelData::RecordStarting);
		break;
	case OBS_FRONTEND_EVENT_RECORDING_STARTED:
		PLSCHANNELS_API->sigTrySetRecordState(ChannelData::RecordStarted);
		break;
	case OBS_FRONTEND_EVENT_RECORDING_STOPPING:
		PLSCHANNELS_API->sigTrySetRecordState(ChannelData::RecordStopping);
		break;
	case OBS_FRONTEND_EVENT_RECORDING_STOPPED:
		PLSCHANNELS_API->sigTrySetRecordState(ChannelData::RecordStopped);
		break;
	case OBS_FRONTEND_EVENT_RECORDING_PAUSED:
		break;
	case OBS_FRONTEND_EVENT_RECORDING_UNPAUSED:
		break;
		/*recording    end          */
	case OBS_FRONTEND_EVENT_EXIT:
		PLSCHANNELS_API->exitApi();
		break;
	default:
		break;
	}
};

void onUserLoginStateChanged(pls_frontend_event event, const QVariantList &, void *)
{
	switch (event) {
	case pls_frontend_event::PLS_FRONTEND_EVENT_PRISM_LOGOUT:
		//PLSCHANNELS_API->stopAll();
		PLSCHANNELS_API->clearAll();
		PLSCHANNELS_API->saveData();
		break;
	case pls_frontend_event::PLS_FRONTEND_EVENT_PRISM_LOGIN:
		initChannelsData();
	default:
		break;
	}
}

//init channel ui
bool initChannelUI()
{
	auto window = PLSBasic::Get();
	if (window) {
		PRE_LOG(Initialize, INFO);
		auto thread = new QThread;
		//start a thread for data handle
		PLSCHANNELS_API->moveToNewThread(thread);
		thread->start();
		registerAllPlatforms();
		QHBoxLayout *layout = new QHBoxLayout(window->getChannelsContainer());
		layout->setMargin(0);
		layout->setSpacing(0);

		auto channelArea = new PLSChannelsArea;
		layout->addWidget(channelArea);

		obs_frontend_add_event_callback(onLiveStateChanged, nullptr);
		pls_frontend_add_event_callback(onUserLoginStateChanged, nullptr);

		return true;
	}
	return false;
}

void initChannelsData()
{
	HolderReleaser releaser(&PLSChannelDataAPI::holdOnChannelArea);
	PLSCHANNELS_API->resetInitializeState(false);
	PLSCHANNELS_API->updateRtmpGpopInfos();
	PLSCHANNELS_API->updatePlatformsStates();
	PLSCHANNELS_API->sigRefreshAllChannels();
}

QMainWindow *getMainWindow()
{
	return PLSBasic::Get();
}
