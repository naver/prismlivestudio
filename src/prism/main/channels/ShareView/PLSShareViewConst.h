#ifndef SHAREVIEWCONST_H
#define SHAREVIEWCONST_H
#include <QSize>
#include <QString>
namespace shareView {

#define SHARE_TYPE "shareType"
const QString DEFAULT_URL = "";
#define DEFAULT_HEAD_PIC "icon-source-img-off.svg"
#define STEAM_PIC "stremsetting-vlive-s.svg"
#define YOUTUBE_PIC "img-youtube-profile.svg"
#define LIVE_PIC "badge-live-on.svg"
#define FACEBOOK_HOVER_PIC "icon-share-fb-over.svg"
#define FACEBOOK_PIC "icon-share-fb-normal.svg"

#define IMAGE_PATH ":/images"

#define PIC_PATH(pic)                                    \
	{                                                \
		QString(IMAGE_PATH) + "/" + QString(pic) \
	}

#define BUTTONFLAG "buttonFlag"

const QSize g_headerSize(34, 34);
const QSize g_platformSize(17, 17);
const QSize g_buttonSize(213, 60);
const QSize g_iconSize(22, 22);
} // namespace shareView

#endif // SHAREVIEWCONST_H
