/*
 * @fine      FreeNetworkReplyGuard
 * @brief     QNetworkReply object uninitialize and delete
 * @date      2019-10-10
 * @author    Bing Cheng
 * @attention

 * @version   v1.0
 * @modify
 */

#ifndef FREENETWORKREPLYGUARD_H
#define FREENETWORKREPLYGUARD_H

#include <QNetworkReply>
#include <QTimer>
#include <QHttpMultiPart>

class FreeNetworkReplyGuard {
public:
	explicit FreeNetworkReplyGuard(QNetworkReply *reply, QTimer *t, QHttpMultiPart *&multilPart) : m_reply(reply), m_timer(t), part(multilPart) {}

	~FreeNetworkReplyGuard()
	{
		m_reply->abort();
		m_reply->close();
		m_reply->deleteLater();
		m_timer->deleteLater();
		part = nullptr;
	}

private:
	QNetworkReply *m_reply;
	QTimer *m_timer;
	QHttpMultiPart *&part;
};

#endif // FREENETWORKREPLYGUARD_H
