#include <QMap>
#include <QVector>
#include <QString>
#define name2str(name) (#name)

typedef struct RtmpDestination {
	QString streamName;
	QString rtmpUrl;
	bool exposure;
	int order;
} RtmpDestination;

typedef struct Common {
	QString stage;
	QString regionCode;
	QString deviceType;
} Common;
typedef struct channelAttr {
	QString id;
	uint order;
} channelAttr;

typedef struct ChannelAttrs {
	QVector<channelAttr> attrs;
} ChannelAttrs;

typedef struct GPopChannelDataList {
	QMap<QString, ChannelAttrs> channels;
} GPopChannelDataList;

typedef struct BeautyFilterPreset {
	QString id;
	uint order;
	uint version;
	QString url;
} eautyFilterPreset;
typedef struct Category {
	uint id;
	QString nameKr;
	QString nameEn;
} Category;
typedef struct YoutubeCategories {
	QVector<Category> categories;
} YoutubeCategories;

typedef struct RequestPolicy {
	QString requestType;
	uint timeoutMS;
} RequestPolicy;
typedef struct StartPolicy {
	QString startType;
	uint waitDurationMS;
	uint retryCount;
} StartPolicy;
typedef struct StatisticPolicy {
	QString statisticType;
	uint reqIntervalMS;
} StatisticPolicy;
typedef struct AbpPolicy {
	QString abpPolicyType;
	uint triggerToastMS;
	uint abpExecuteIntervalMS;
	uint bitrateDownThresholdMS;
	uint audioBufferDropThresholdMS;
	float targetBitrateWeight;
	uint videoBitrateStep;
	uint minVideoBitrate;
	uint maxVideoBitrate;
	uint initialVideoBitrate;
	uint initialAudioBitrate;
	uint minBitrateUpThreshold;
	uint maxBitrateUpThreshold;
	float bitrateUpThresholdMultiplier;
	uint bitrateUpDurationThresholdMS;
	uint bitrateUpContinueThresholdMS;
	uint bwStableThresholdMS;
	uint tuningBitrateStepRange;
	float tuningThresholdWeight;
	uint movingAverageSize;
	uint forcedTerminationMS;
} AbpPolicy;

typedef struct Publish {
	QString publishType;
	QString netPolicy;
	QMap<QString, AbpPolicy> abpPolicys;
} Publish;
typedef struct SnsCallbackUrl {
	QString callbackUrlType;
	QString url;
} SnsCallbackUrl;

typedef struct Policy {
	bool isN1HCExposure;
	QStringList gameCaptureBlackList;
	QVector<uint> camFpsList;
	bool isExposeCoachmark;
	QMap<QString, RequestPolicy> requests;
	QMap<QString, StartPolicy> statrPolicys;
	QMap<QString, StatisticPolicy> statistics;
	QMap<QString, StatisticPolicy> chats;
	QMap<QString, Publish> publishs;
	QMap<QString, SnsCallbackUrl> snscallbackurls;
} Policy;
typedef struct InitialData {
	QVector<GPopChannelDataList> channelLists;
	QVector<BeautyFilterPreset> beautyFilterPresets;
	YoutubeCategories youtube;

} InitialData;
typedef struct Pc {
	QString logLevel;
	InitialData initialData;
	Policy policy;

} Pc;

typedef struct Optional {
	Common common;
	Pc pc;
} Optional;
typedef struct Fallback {
	QString url;
	QString ssl;
} Fallback;
typedef struct Connection {
	QString url;
	QString ssl;
	Fallback fallback;

} Connection;
typedef struct GPopInfo {
	Connection connection;
	Optional optional;
	long long buildData;
} GpopInfo;
typedef struct VliveNotice {
	QString version;
	bool isNeedNotice;
	QString pageUrl;
};
