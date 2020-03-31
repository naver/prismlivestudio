#pragma once

#include <QObject>

class PLSDateFormate : public QObject {
	Q_OBJECT
public:
	//explicit PLSDateFormate(QObject *parent = nullptr);

	/**
 *  @param 1527840000
 *  @return  02/07/2020 11:40AM UTC+9
 */
	static QString timeStampToUTCString(long timeStamp);
	/**
 *  @param 1527840000
 *  @return  02/07/2020 11:40
 */
	static QString timeStampToShortString(long timeStamp);

	/**
 *  @param 2020-02-29T07:00:00.000Z || 2020-02-29T07:00:00Z
 *  @return 1527840000
 */
	static long timeStringToStamp(QString time);
	/**
 *  @param 1527840000 
 *  @return 2020-02-29T07:00:00Z
 */
	static QString youtubeTimeStampToString(long timeStamp);
	/**
 *  @param 2020-04-14 15:54
 *  @return 1527840000
 */
	static long vliveTimeStringToStamp(QString time);

	/**
 *  @param 1527840000
 *  @return 2020-04-14 15:54
 */
	static QString vliveTimeStampToString(long timeStamp);

	static QString navertvTimeStampToString(qint64 timeStamp);

	static long getNowTimeStamp();
};
