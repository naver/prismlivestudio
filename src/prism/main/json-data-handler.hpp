/*
 * @fine      PLSJsonDataHandler
 * @brief     json parsing and json generation module
 * @date      2019-09-30
 * @author    Bing Cheng
 * @attention

 * @version   v1.0
 * @modify
 */

#ifndef COMMON_HTTP_JSONDATAHANDLER_H
#define COMMON_HTTP_JSONDATAHANDLER_H

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QObject>

class PLSJsonDataHandler {
public:
	/**
     * @brief isValidJsonFormat: Determine if the json format is correct
     * @param array json array
     * @return
     */
	bool static isValidJsonFormat(const QByteArray &array);
	/**
     * @brief getValueFromByteArray: Find the corresponding value according to the key value from bytearray
     * @param byteArray
     * @param key
     * @param value
     * @return
     */
	bool static getValueFromByteArray(const QByteArray &byteArray, const QString &key, QVariant &value);
	bool static getValuesFromByteArray(const QByteArray &byteArray, const QString &key, QVariantMap &values);
	bool static getValuesFromByteArray(const QByteArray &byteArray, const QString &key, QJsonObject &object);

	/**
     * @brief getValuesFromByteArray
     * @param byteArray
     * @param key
     * @param value
     * @return
     */
	bool static getValuesFromByteArray(const QByteArray &byteArray, const QString &key, QVariantList &value);
	/**
     * @brief getJsonByteArrayFromMap: serialize json data
     * @param map
     * @return
     */
	static QByteArray getJsonByteArrayFromMap(const QVariantMap &map);
	bool static saveJsonFile(const QByteArray &array, const QString &path);
	bool static getJsonArrayFromFile(QByteArray &array, const QString &path);

	/**
     * @brief getValue parse json data ,get value by key
     * @param jsonObject
     * @param key
     * @param value
     * @return
     */
	bool static getValue(const QJsonObject &jsonObject, const QString &key, QVariant &value);
	/**
     * @brief getValues: get array values
     * @param jsonObject
     * @param key: array key
     * @param values variantList
     * @return
     */
	bool static getValues(const QJsonObject &jsonObject, const QString &key, QVariantList &values);
	bool static getValues(const QJsonObject &jsonObject, const QString &key, QJsonObject &object);
	bool static getValues(const QJsonObject &jsonObject, const QString &key, QVariantMap &values);

	/**
     * @brief getArray :parse arrayJson
     * @param array
     * @param key
     */
	void static getArray(const QJsonArray &array, const QString &key, QVariantList &values);
};

#endif // JSONDATAHANDLER_H
