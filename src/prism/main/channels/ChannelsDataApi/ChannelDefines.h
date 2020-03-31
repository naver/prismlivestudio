#ifndef CHANNEL_DEFINE_H
#define CHANNEL_DEFINE_H

#include <QListWidgetItem>
#include <QMetaType>
#include <functional>
#include "ChannelCapsule.h"

namespace ChannelData {

using ItemPtr = QSharedPointer<QListWidgetItem>;
Q_DECLARE_METATYPE(ItemPtr)

using ChannelsFunction = void (*)(QVariantMap &);
Q_DECLARE_METATYPE(ChannelsFunction)

using ChannelCapsulePtr = QSharedPointer<ChannelCapsule>;
Q_DECLARE_METATYPE(ChannelCapsulePtr)

using TaskFun = std::function<void(const QVariant &)>;
Q_DECLARE_METATYPE(TaskFun)

}

#endif // !CHANNEL_DEFINE_H
