#pragma once

#include "frontend-api-global.h"

#include <QJsonObject>
#include <QWidget>

enum class PLSPlatformType { Twitch, YouTube, Facebook, Google, Twitter, Naver, Line, NaverTv, Vlive, Band, AfreecaTV, WhaleSpace };

class FRONTEND_API PLSLoginInfo {
protected:
	PLSLoginInfo();
	virtual ~PLSLoginInfo();

public:
	enum class UseFor {
		Prism,   // only for prism login
		Channel, // only for channel login
		Both     // both
	};
	enum class ChannelSupport {
		Account, // only support account login
		Rtmp,    // only support rtmp
		Both     // both
	};

public:
	/**
	  * login module use for
	  * return:
	  *     Prism, Channel, Both
	  */
	virtual UseFor useFor() const = 0;

	/**
	  * login platform
	  * return:
	  *     Twitch, YuTube, Facebook, ...
	  */
	virtual PLSPlatformType platform() const = 0;
	/**
	  * login platform name
	  * return:
	  *     Twitch, YuTube, Facebook, ...
	  */
	virtual QString name() const = 0;
	/**
	  * login platform icon
	  * param:
	  *     [in] useFor: Prism or Channel
	  * return:
	  *     icon full path
	  */
	virtual QString icon(UseFor useFor) const = 0;
	/**
	  * channel login platform support
	  * return:
	  *     Account, Rtmp, Both
	  */
	virtual ChannelSupport channelSupport() const;

	/**
	  * login by account way
	  * param:
	  *     [out] result: result data, for example: token, cookies, ...
	  *     [in] useFor: Prism or Channel
	  *     [in-opt] parent: parent widget
	  * return:
	  *     true for success, false for failed
	  */
	virtual bool loginWithAccount(QJsonObject &result, UseFor useFor, QWidget *parent = nullptr) const = 0;

	/**
	  * get rtmp server url address
	  * return:
	  *     rtmp server url address
	  */
	virtual QString rtmpUrl() const;
	/**
	  * rtmp view
	  * param:
	  *     [out] result: result data, for example: token, cookies, ...
	  *     [in] useFor: Prism or Channel
	  *     [in-opt] parent: parent widget
	  * return:
	  *     true for success, false for failed
	  */
	virtual bool rtmpView(QJsonObject &result, UseFor useFor, QWidget *parent = nullptr) const;

private:
	Q_DISABLE_COPY(PLSLoginInfo)
};
