#pragma once
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QGlobalStatic>
#include <QMap>
#include <QPluginLoader>
#include <QString>
#include <QVariantMap>
#include "ChannelData_global.h"

using ChannelsMap = QMap<QString, QVariantMap>;
Q_DECLARE_METATYPE(ChannelsMap)

class PLSChannelPublicAPI {

public:
	virtual ~PLSChannelPublicAPI() {}

	/*replace or add only one channel info */
	virtual void setChannelInfos(const QVariantMap &Infos, bool) = 0;

	virtual void addChannelInfo(const QVariantMap &Infos, bool notify = true) = 0;

	/* check if channel exists */
	virtual bool isChannelInfoExists(const QString &) = 0;

	/* check if no channels */
	virtual bool isEmpty() const = 0;

	/*get the channel of given name */
	virtual const QVariantMap getChannelInfo(const QString &channelUUID) = 0;

	/*to check the status of channel */
	virtual int getChannelStatus(const QString &channelUUID) = 0;

	/*get all channels info for copy */
	virtual const ChannelsMap getAllChannelInfo() = 0;

	/* delete channel info of name ,the widget will be deleted later */
	virtual void removeChannelInfo(const QString &channelUUID, bool notify = true, bool notifyServer = true) = 0;

	/*delete all channels info and widgets will be deleted later */
	virtual void clearAll() = 0;
};

Q_DECLARE_INTERFACE(PLSChannelPublicAPI, "PLSChannelPublicAPI.interface")

Q_GLOBAL_STATIC(QVariantMap, plugins);

template<typename PluginType = PLSChannelPublicAPI> auto getChannelAPI(const QString &name = "prism-channel-controller") -> PluginType *
{
	if (plugins->contains(name)) {
		auto obj = plugins->value(name).value<QObject *>();
		return qobject_cast<PluginType *>(obj);
	}

	PluginType *plugin = nullptr;
	QPluginLoader loader(name);
	QObject *interfaceObj = loader.instance();
	auto API = qobject_cast<PluginType *>(interfaceObj);
	if (API) {
		plugin = API;
		plugins->insert(name, QVariant::fromValue(interfaceObj));
	} else {
		qDebug() << " error load plugin " << loader.errorString() << QDir::currentPath() << QCoreApplication::libraryPaths();
	}
	return plugin;
}
