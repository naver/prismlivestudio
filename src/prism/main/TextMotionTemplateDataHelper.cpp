#include "TextMotionTemplateDataHelper.h"
#include "pls-common-define.hpp"
#include "json-data-handler.hpp"
#include "frontend-api.h"
#include "pls-app.hpp"
#include "TextMotionTemplateButton.h"
#include "../PLSHttpApi/PLSHmacNetworkReplyBuilder.h"
#include "../PLSHttpApi/PLSHttpHelper.h"
#include "log.h"

#include <qbuttongroup.h>
#include <qdir.h>

#define PRISM_TM_TEMPLATE_PATH "PRISMLiveStudio/textmotion/textMotionTemplate.json"
#define PRISM_TEXT_TEMPLATE "text-template"

TextMotionTemplateDataHelper::TextMotionTemplateDataHelper(QObject *parent) : QObject(parent)
{
	const QMap<QString, QVector<TextMotionTemplateData>> &templateInfos = TextMotionRemoteDataHandler::instance()->getTMRemoteData();
	for (auto templateStr : templateInfos.keys()) {
		QButtonGroup *buttonGroup = new QButtonGroup;
		buttonGroup->setExclusive(false);
		m_templateButtons.insert(templateStr, buttonGroup);
	}
}

TextMotionTemplateDataHelper::~TextMotionTemplateDataHelper() {}

void TextMotionTemplateDataHelper::initTemplateButtons()
{
	const QMap<QString, QVector<TextMotionTemplateData>> &templateInfos = TextMotionRemoteDataHandler::instance()->getTMRemoteData();

	for (auto group = m_templateButtons.begin(); group != m_templateButtons.end(); ++group) {
		auto groupTmp = group.value();
		while (groupTmp->buttons().size()) {
			groupTmp->removeButton(groupTmp->buttons().value(0));
		}
	}

	for (auto templateKey : templateInfos.keys()) {
		QButtonGroup *group = m_templateButtons.value(templateKey);
		const QVector<TextMotionTemplateData> templateDatas = templateInfos.value(templateKey);
		for (int index = 0; index != templateDatas.count(); ++index) {
			TextMotionTemplateData data = templateDatas.value(index);
			TextMotionTemplateButton *button = new TextMotionTemplateButton();
			QString id = data.id;
			button->setText(id);
			int idInt = id.split('_').last().toInt();
			button->setProperty("ID", idInt);
			connect(button, &TextMotionTemplateButton::clicked, this, &TextMotionTemplateDataHelper::updateButtonsStyle);
			button->setTemplateData(data);
			button->setGroupName(templateKey);
			button->attachGifResource(data.resourcePath, data.resourceBackupPath);
			group->addButton(button, idInt);
		}
	}
}

QMap<int, QString> TextMotionTemplateDataHelper::getTemplateNames()
{
	return TextMotionRemoteDataHandler::instance()->getTemplateNames();
}

void TextMotionTemplateDataHelper::updateButtonsStyle()
{
	TextMotionTemplateButton *button = static_cast<TextMotionTemplateButton *>(sender());
	QString buttonGroupName = button->getGroupName();

	for (auto templateKey : m_templateButtons.keys()) {
		for (auto _button : m_templateButtons.value(templateKey)->buttons()) {
			if (_button == button && !_button->isChecked()) {
				_button->setChecked(true);
			} else if (_button->isChecked() && _button != button) {
				_button->setChecked(false);
			}
		}
	}
}

QButtonGroup *TextMotionTemplateDataHelper::getTemplateButtons(const QString &templateName)
{
	if (m_templateButtons.find(templateName.toLower()) != m_templateButtons.end()) {
		return m_templateButtons.value(templateName);
	}
	return nullptr;
}

void TextMotionTemplateDataHelper::resetButtonStyle()
{
	for (auto templateKey : m_templateButtons.keys()) {
		for (auto _button : m_templateButtons.value(templateKey)->buttons()) {
			_button->setChecked(false);
		}
	}
}

TextMotionRemoteDataHandler *TextMotionRemoteDataHandler::instance()
{
	static TextMotionRemoteDataHandler remoteDataHandler;
	return &remoteDataHandler;
}

void TextMotionRemoteDataHandler::getTMRemoteDataRequest(const QString &url)
{
	PLS_INFO(PRISM_TEXT_TEMPLATE, "start requeset text template data.");
	m_url = url;
	PLSNetworkAccessManager::getInstance()->createHttpRequest(PLSNetworkAccessManager::Operation::GetOperation, url, false);
}

TextMotionRemoteDataHandler::TextMotionRemoteDataHandler()
{
	connect(PLSNetworkAccessManager::getInstance(), &PLSNetworkAccessManager::replyResultData, this, &TextMotionRemoteDataHandler::onReplyResultData);
	connect(PLSNetworkAccessManager::getInstance(), &PLSNetworkAccessManager::replyErrorDataWithSatusCode, this, &TextMotionRemoteDataHandler::onReplyErrorData);
}

TextMotionRemoteDataHandler::~TextMotionRemoteDataHandler() {}

void TextMotionRemoteDataHandler::initTMData()
{
	m_templateInfos.clear();
	m_templateTabs.clear();
	QDir appDir(qApp->applicationDirPath());
	QString backupGifPath = appDir.absoluteFilePath("data/prism-studio/text_motion/%1");
	QString lang = QString(App()->GetLocale()).split('-')[0];
	QVariantList list;
	PLSJsonDataHandler::getValuesFromByteArray(m_templateJson, "group", list);
	int index = 0;
	for (auto map : list) {
		QString name = map.toMap().value(name2str(groupId)).toString().toLower();
		QJsonArray templateList = map.toMap().value(name2str(items)).toJsonArray();
		QVector<TextMotionTemplateData> templateDatas;
		QVector<QJsonObject> tempateJsons;
		for (auto jsonValue : templateList) {
			QVariantMap templateMap = jsonValue.toObject().toVariantMap();
			TextMotionTemplateData tmData;
			tmData.id = templateMap.value(name2str(itemId)).toString();
			tmData.name = templateMap.value(name2str(title)).toString();

			tmData.resourcePath = QString(pls_get_user_path(CONFIGS_USER_TEXTMOTION_PATH)).arg(QString("%1%2.gif").arg(lang).arg(tmData.name));
			tmData.resourceBackupPath = backupGifPath.arg(QString("%1%2.gif").arg(lang).arg(tmData.name));
			templateDatas.append(tmData);
			tempateJsons.append(jsonValue.toObject());

			//get gif url
			QString gifUrl = templateMap.value(name2str(properties))
						 .toMap()
						 .value("template")
						 .toMap()
						 .value("baseInfoPC")
						 .toMap()
						 .value("adjustableAttribute")
						 .toMap()
						 .value(lang)
						 .toMap()
						 .value(name2str(thumbnailUrl))
						 .toString();
			downLoadGif(gifUrl, lang, tmData.name);
		}
		QButtonGroup *buttonGroup = new QButtonGroup;
		buttonGroup->setExclusive(false);
		//m_templateButtons.insert(name, buttonGroup);
		m_templateInfos.insert(name, templateDatas);
		m_templateTabs.insert(index++, name);
	}
}

void TextMotionRemoteDataHandler::downLoadGif(const QString &urlStr, const QString &language, const QString &id)
{
	PLSNetworkReplyBuilder builder(urlStr);
	auto imageCallback = [](bool ok, const QString &imagePath, void *context) {};
	PLSHttpHelper::downloadImageAsync(builder.get(), this, pls_get_user_path("PRISMLiveStudio/textmotion"), imageCallback, language, id);
}

void TextMotionRemoteDataHandler::onReplyResultData(int statusCode, const QString &url, const QByteArray array)
{
	if (m_url == url && statusCode == HTTP_STATUS_CODE_200) {
		PLS_INFO(PRISM_TEXT_TEMPLATE, "get text template json data success from remote.");
		m_templateJson = array;
		PLSJsonDataHandler::saveJsonFile(m_templateJson, pls_get_user_path(PRISM_TM_TEMPLATE_PATH));
		initTMData();
	}
}

void TextMotionRemoteDataHandler::onReplyErrorData(int statusCode, const QString &url, const QString &body, const QString &errorInfo)
{
	Q_UNUSED(url);
	Q_UNUSED(errorInfo);
	if (m_url == url) {
		PLS_INFO(PRISM_TEXT_TEMPLATE, "get text template json data failed from remote");
		QString templatePath = pls_get_user_path(PRISM_TM_TEMPLATE_PATH);
		if (!QFileInfo(templatePath).exists()) {
			QDir appDir(qApp->applicationDirPath());
			templatePath = appDir.absoluteFilePath("data/prism-studio/text_motion/textMotionTemplate.json");
		}
		PLSJsonDataHandler::getJsonArrayFromFile(m_templateJson, templatePath);
		initTMData();
	}
}

QString TextMotionTemplateDataHelper::findTemplateGroupStr(const int &templateId)
{
	for (auto templateKey : m_templateButtons.keys()) {
		for (auto _button : m_templateButtons.value(templateKey)->buttons()) {
			if (templateId == _button->property("ID").toInt()) {
				return templateKey;
			}
		}
	}
	return QString();
}
