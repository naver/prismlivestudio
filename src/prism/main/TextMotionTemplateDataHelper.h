#ifndef TEXTMOTIONTEMPLATEDATAHELPER_H
#define TEXTMOTIONTEMPLATEDATAHELPER_H
#include <QMap>
#include <QObject>
#include <QString>

#include "network-access-manager.hpp"
#include "pls-net-url.hpp"
#include "frontend-api.h"

class TextMotionTemplateButton;
class QPushButton;

using TextMotionTemplateData = struct TextMotionTemplateData {
	QString id;
	QString name;
	QString resourcePath;
	QString resourceBackupPath;
};

class TextMotionRemoteDataHandler : public QObject {
	Q_OBJECT
public:
	static TextMotionRemoteDataHandler *instance();
	void getTMRemoteDataRequest(const QString &url);
	const inline QMap<QString, QVector<TextMotionTemplateData>> &getTMRemoteData() const { return m_templateInfos; }
	const inline QMap<int, QString> &getTemplateNames() const { return m_templateTabs; }

private:
	TextMotionRemoteDataHandler();
	~TextMotionRemoteDataHandler();
	void initTMData();
	void downLoadGif(const QString &urlStr, const QString &language, const QString &id);

private slots:
	void onReplyResultData(int statusCode, const QString &url, const QByteArray array);

	void onReplyErrorData(int statusCode, const QString &url, const QString &body, const QString &errorInfo);

private:
	QString m_url;
	QByteArray m_templateJson;
	QMap<int, QString> m_templateTabs;
	QMap<QString, QVector<TextMotionTemplateData>> m_templateInfos; //key=type,value=TMDatas
};

class TextMotionTemplateDataHelper : public QObject, public ITextMotionTemplateHelper {
	Q_OBJECT

public:
	explicit TextMotionTemplateDataHelper(QObject *parent = nullptr);
	~TextMotionTemplateDataHelper();

public:
	virtual void initTemplateButtons() override;
	virtual QMap<int, QString> getTemplateNames() override;
	virtual QButtonGroup *getTemplateButtons(const QString &templateName) override;
	virtual void resetButtonStyle() override;
	virtual QString findTemplateGroupStr(const int &templateId) override;

private slots:
	void updateButtonsStyle();

private:
	QMap<QString, QButtonGroup *> m_templateButtons;
};

#endif // TEXTMOTIONTEMPLATEDATAHELPER_H
