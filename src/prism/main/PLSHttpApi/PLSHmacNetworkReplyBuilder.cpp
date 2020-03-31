#include "PLSHmacNetworkReplyBuilder.h"

#include "PLSHttpHelper.h"
#include "pls-net-url.hpp"
#include "frontend-api.h"
#include "pls-common-define.hpp"
#include "ui-config.h"

#define HEADER_PRISM_DEVICE QStringLiteral("X-prism-device")
#define HEADER_PRISM_USERCODE QStringLiteral("X-prism-usercode")
#define HEADER_PRISM_IP QStringLiteral("X-prism-ip")

#define HEADER_WINDOWS_OS QStringLiteral("Windows OS")

PLSHmacNetworkReplyBuilder::PLSHmacNetworkReplyBuilder() {}
PLSHmacNetworkReplyBuilder::PLSHmacNetworkReplyBuilder(const QString &value, HmacType hmacType) : PLSNetworkReplyBuilder(value), hmacType(hmacType) {}
PLSHmacNetworkReplyBuilder::PLSHmacNetworkReplyBuilder(const QUrl &value, HmacType hmacType) : PLSNetworkReplyBuilder(value), hmacType(hmacType) {}

PLSHmacNetworkReplyBuilder &PLSHmacNetworkReplyBuilder::setHmacType(HmacType hmacType)
{
	this->hmacType = hmacType;

	return *this;
}

QUrl PLSHmacNetworkReplyBuilder::buildUrl(const QUrl &url)
{
	auto urlEncrypted = __super::buildUrl(url);

	QString hmacKey;
	switch (hmacType) {
	case HmacType::HT_PRISM: {
		hmacKey = PLS_HMAC_KEY;
		QVariantMap headers;
		pls_http_request_head(headers, true);
		setRawHeaders(headers);
	} break;
	case HmacType::HT_VLIVE:
		hmacKey = PLS_VLIVE_HMAC_KEY;
		break;
	default:
		break;
	}

	if (!hmacKey.isEmpty()) {
		return pls_get_encrypt_url(urlEncrypted.toString(), hmacKey);
	}

	return urlEncrypted;
}
