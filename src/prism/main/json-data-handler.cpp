#include "json-data-handler.hpp"
#include <QFile>

bool PLSJsonDataHandler::isValidJsonFormat(const QByteArray &array)
{
	QJsonParseError jsonError;
	if (array.isEmpty()) {
		return false;
	} else {
		QJsonDocument::fromJson(array, &jsonError);
		if (QJsonParseError::NoError == jsonError.error) {
			return true;
		} else {
			return false;
		}
	}
}

bool PLSJsonDataHandler::getValueFromByteArray(const QByteArray &byteArray, const QString &key, QVariant &value)
{
	if (isValidJsonFormat(byteArray)) {
		return getValue(QJsonDocument::fromJson(byteArray).object(), key, value);
	} else {
		return false;
	}
}

bool PLSJsonDataHandler::getValuesFromByteArray(const QByteArray &byteArray, const QString &key, QVariantMap &values)
{
	if (isValidJsonFormat(byteArray)) {
		return getValues(QJsonDocument::fromJson(byteArray).object(), key, values);
	} else {
		return false;
	}
}

bool PLSJsonDataHandler::getValuesFromByteArray(const QByteArray &byteArray, const QString &key, QJsonObject &object)
{
	if (isValidJsonFormat(byteArray)) {
		return getValues(QJsonDocument::fromJson(byteArray).object(), key, object);
	} else {
		return false;
	}
}

bool PLSJsonDataHandler::getValuesFromByteArray(const QByteArray &byteArray, const QString &key, QVariantList &value)
{
	if (isValidJsonFormat(byteArray)) {
		return getValues(QJsonDocument::fromJson(byteArray).object(), key, value);
	} else {
		return false;
	}
}

QByteArray PLSJsonDataHandler::getJsonByteArrayFromMap(const QVariantMap &map)
{
	QJsonDocument doc;
	QJsonObject obj;
	QVariantMap::const_iterator it;
	for (it = map.constBegin(); it != map.end(); ++it) {
		obj.insert(it.key(), it.value().toJsonValue());
	}
	doc.setObject(obj);
	return doc.toJson();
}

bool PLSJsonDataHandler::getValue(const QJsonObject &jsonObject, const QString &key, QVariant &value)
{
	for (auto tem : jsonObject.keys()) {
		QJsonValue v = jsonObject.value(tem);
		if (v.isObject()) {
			getValue(jsonObject.value(tem).toObject(), key, value);
		} else {
			if (key == tem) {
				value = v.toVariant();
				return true;
			} else {
				continue;
			}
		}
	}
	return false;
}

bool PLSJsonDataHandler::getValues(const QJsonObject &jsonObject, const QString &key, QVariantList &values)
{
	for (auto tem : jsonObject.keys()) {
		QJsonValue v = jsonObject.value(tem);
		if (v.isObject()) {
			getValues(jsonObject.value(tem).toObject(), key, values);
		} else if (v.isArray()) {
			if (key == tem) {
				values = v.toArray().toVariantList();
				return true;
			} else {
				getArray(v.toArray(), key, values);
			}
		} else {
			continue;
		}
	}
	return false;
}

bool PLSJsonDataHandler::getValues(const QJsonObject &jsonObject, const QString &key, QJsonObject &object)
{
	for (auto tem : jsonObject.keys()) {
		if (tem == key) {
			QJsonValue v = jsonObject.value(tem);
			if (v.isObject()) {
				object = v.toObject();
				return true;
			} else {
				continue;
			}
		} else {
			continue;
		}
	}
	return false;
}
bool PLSJsonDataHandler::getValues(const QJsonObject &jsonObject, const QString &key, QVariantMap &values)
{
	for (auto tem : jsonObject.keys()) {
		QJsonValue v = jsonObject.value(tem);
		if (v.isObject() && key == tem) {
			values = v.toObject().toVariantMap();
			return true;
		} else {
			getValues(jsonObject.value(tem).toObject(), key, values);
		}
	}
	return false;
}
void PLSJsonDataHandler::getArray(const QJsonArray &array, const QString &key, QVariantList &values)
{
	for (auto arr : array) {
		if (arr.isObject()) {
			getValues(arr.toObject(), key, values);
		} else if (arr.isArray()) {
			getArray(arr.toArray(), key, values);
		} else {
			continue;
		}
	}
}
bool PLSJsonDataHandler::saveJsonFile(const QByteArray &array, const QString &path)
{
	QFile file(path);
	if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
		return false;
	}
	file.write(array);
	file.close();
	return true;
}

bool PLSJsonDataHandler::getJsonArrayFromFile(QByteArray &array, const QString &path)
{
	QFile file(path);
	if (!file.open(QFile::ReadOnly | QIODevice::Text)) {
		return false;
	}
	array = file.readAll();
	file.close();
	return true;
}
