/*
* @file		PLSHmacNetworkReplyBuilder.h
* @brief	build a url that need hmac
* @author	wu.longyue@navercorp.com
* @date		2020-01-06
*/

#pragma once

#include "PLSNetworkReplyBuilder.h"

enum class HmacType { HT_NONE, HT_PRISM, HT_VLIVE };

using namespace std;

class PLSHmacNetworkReplyBuilder : public PLSNetworkReplyBuilder {
public:
	PLSHmacNetworkReplyBuilder();
	PLSHmacNetworkReplyBuilder(const QString &value, HmacType hmacType = HmacType::HT_PRISM);
	PLSHmacNetworkReplyBuilder(const QUrl &value, HmacType hmacType = HmacType::HT_PRISM);

	PLSHmacNetworkReplyBuilder &setHmacType(HmacType hmacType = HmacType::HT_PRISM);

protected:
	QUrl buildUrl(const QUrl &url) override;

private:
	HmacType hmacType;
};
