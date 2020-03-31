#include "GiphyDownloader.h"
#include "GiphyWebHandler.h"
#include "PLSFloatScrollBarScrollArea.h"
#include "PLSSearchPopupMenu.h"
#include "PLSToastMsgFrame.h"
#include "action.h"
#include "layout/flowlayout.h"
#include "liblog.h"
#include "log/module_names.h"
#include "main-view.hpp"
#include "pls-app.hpp"
#include "pls-common-define.hpp"
#include "pls-common-language.hpp"
#include "qt-wrappers.hpp"
#include "ui_PLSGipyStickerView.h"
#include <QButtonGroup>
#include <QDesktopWidget>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QMetaObject>
#include <QPainter>
#include <QToolButton>
#include <qmath.h>

const char *TAB_INDEX = "tabIndex";
const char *API_KEY = ""; //You should register a API KEY for your application
const char *API = "https://api.giphy.com/v1/stickers/";
const char *PAGE_TYPE = "pageType";
static const char *GEOMETRY_DATA = "geometryGiphy"; //key of giphy window geometry in global ini
static const char *MAXIMIZED_STATE = "isMaxState";  //key if the giphy window is maximized in global ini
static const char *SHOW_MADE = "showMode";          //key if the giphy window is shown in global ini

QPointer<MovieLabel> g_prevClickedItem = nullptr;
PointerValue g_bindSourcePtr = 0;

const int defaultLimit = 24;
const char *SEARCH_API = "search";
const char *TRENDING_API = "trending";
const int duration = 500;
const int MAX_PREVIEW_FRAME = 6;
const int MAX_LIMIT = 24;
const int MOVIE_ITEM_WIDTH = 78;
const int MOVIE_ITEM_HEIGHT = 78;
const int FLOW_LAYOUT_SPACING = 6;
const int FLOW_LAYOUT_VSPACING = 10;
const int FLOW_LAYOUT_MARGIN_LEFT_RIGHT = 17;
const int FLOW_LAYOUT_MARGIN_TOP_BOTTOM = 19;
const int MAX_INPUT_LENGTH = 100;

void PLSGipyStickerView::OnPrismAppQuit(enum obs_frontend_event event, void *context)
{
	if (event == OBS_FRONTEND_EVENT_EXIT) {
		PLSGipyStickerView *view = static_cast<PLSGipyStickerView *>(context);
		if (view) {
			view->SetExitFlag(true);
			view->SaveStickerJsonData();
			if (!view->getMaxState())
				view->onSaveNormalGeometry();
			view->deleteLater();
		}
	}
}

PointerValue PLSGipyStickerView::ConvertPointer(void *ptr)
{
	return reinterpret_cast<PointerValue>(ptr);
}

PLSGipyStickerView::PLSGipyStickerView(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper), ui(new Ui::PLSGipyStickerView)
{
	pageLimit = defaultLimit;
	this->setCursor(Qt::ArrowCursor);
	dpiHelper.setCss(this, {PLSCssIndex::GiphyStickers, PLSCssIndex::PLSToastMsgFrame});
	qRegisterMetaType<RequestTaskData>("RequestTaskData");
	qRegisterMetaType<DownloadTaskData>("DownloadTaskData");
	ui->setupUi(this->content());
	setHasMaxResButton(true);
	setCaptionButtonMargin(9);
	this->setWindowTitle(QTStr(MIAN_GIPHY_STICKER_TITLE));
	QMetaObject::connectSlotsByName(this);
	obs_frontend_add_event_callback(OnPrismAppQuit, this);
	QFrame *titleFrame = this->findChild<QFrame *>("titleBar");
	if (titleFrame)
		titleFrame->setFocusPolicy(Qt::ClickFocus);
	dpiHelper.notifyDpiChanged(this, [=](double dpi) {
		QFont fontTab = this->font();
		fontTab.setBold(true);
		fontTab.setPixelSize(PLSDpiHelper::calculate(dpi, 14));
		QFontMetrics metrics(fontTab);
		int padding = PLSDpiHelper::calculate(dpi, 10);
		int widthRecent = metrics.width(QTStr("main.giphy.tab.recent"));
		int widthTrending = metrics.width(QTStr("main.giphy.tab.trending"));
		ui->btn_recent->setFixedWidth(widthRecent + 2 * padding);
		ui->btn_trending->setFixedWidth(widthTrending + 2 * padding);
		UpdateLayoutSpacing(recentFlowLayout, dpi);
		UpdateLayoutSpacing(trendingFlowLayout, dpi);
		UpdateLayoutSpacing(searchResultFlowLayout, dpi);
	});
	InitStickerJson();
	Init();
}

PLSGipyStickerView::~PLSGipyStickerView()
{
	threadRequest.quit();
	threadRequest.wait();

	if (timerAutoLoad.isActive())
		timerAutoLoad.stop();

	if (timerSearch.isActive())
		timerSearch.stop();

	if (timerScrollSearch.isActive())
		timerScrollSearch.stop();

	if (timerScrollTrending.isActive())
		timerScrollTrending.stop();

	if (g_prevClickedItem)
		delete g_prevClickedItem;

	obs_frontend_remove_event_callback(OnPrismAppQuit, this);
	delete ui;
}

void PLSGipyStickerView::Init()
{
	this->content()->setMouseTracking(true);
	setMouseTracking(true);
	ui->widget_search->setMouseTracking(true);
	ui->frame_head->setMouseTracking(true);
	ui->widget_container->setMouseTracking(true);
	ui->label_giphy_logo->setMouseTracking(true);
	ui->lineEdit_search->setMaxLength(MAX_INPUT_LENGTH);

	timerAutoLoad.setSingleShot(true);
	connect(&timerAutoLoad, &QTimer::timeout, [=]() {
		needUpdateTrendingList = true;
		UpdateLimit();
		AutoFillContent();
	});

	timerSearch.setSingleShot(true);
	connect(&timerSearch, &QTimer::timeout, [=]() { AutoFillSearchList(0); });

	timerScrollSearch.setSingleShot(true);
	connect(&timerScrollSearch, &QTimer::timeout, [=]() {
		if (searchResultList && !searchKeyword.isEmpty()) {
			QuerySearch(searchKeyword, pageLimit, searchResultList->GetPagination());
		}
	});

	timerScrollTrending.setSingleShot(true);
	connect(&timerScrollTrending, &QTimer::timeout, [=]() {
		if (trendingScrollList)
			QueryTrending(pageLimit, trendingScrollList->GetPagination());
	});
	connect(ui->lineEdit_search, &SearchLineEdit::SearchTrigger, this, &PLSGipyStickerView::OnSearchTigger);
	connect(ui->lineEdit_search, &SearchLineEdit::textEdited, [=](const QString &key) { OnSearchMenuRequested(key.isEmpty()); });
	connect(ui->lineEdit_search, &SearchLineEdit::SearchMenuRequested, this, &PLSGipyStickerView::OnSearchMenuRequested);
	ui->btn_cancel->hide();

	InitMenuList();

	btnGroup = new QButtonGroup(this);
	connect(btnGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &PLSGipyStickerView::OnTabClicked);
	btnGroup->addButton(ui->btn_recent, PLSGipyStickerView::RecentTab);
	btnGroup->addButton(ui->btn_trending, PLSGipyStickerView::TrendingTab);
}

void PLSGipyStickerView::InitCommonForList(ScrollAreaWithNoDataTip *scrollList)
{
	if (scrollList) {
		scrollList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		QSizePolicy sizePolicy_(QSizePolicy::Preferred, QSizePolicy::Expanding);
		sizePolicy_.setHorizontalStretch(0);
		sizePolicy_.setVerticalStretch(0);
		sizePolicy_.setHeightForWidth(scrollList->sizePolicy().hasHeightForWidth());
		scrollList->setSizePolicy(sizePolicy_);
		scrollList->setWidgetResizable(true);
	}
}

void PLSGipyStickerView::InitMenuList()
{
	if (nullptr == searchMenu) {
		searchMenu = new PLSSearchPopupMenu(this);
		connect(searchMenu, &PLSSearchPopupMenu::ItemDelete, this, &PLSGipyStickerView::DeleteHistoryItem);
		connect(searchMenu, &PLSSearchPopupMenu::ItemClicked, [=](const QString &key) {
			searchMenu->hide();
			ui->lineEdit_search->setText(key);
			OnSearchTigger(key);
		});
	}
	searchMenu->hide();
	UpdateMenuList();
}

void PLSGipyStickerView::InitRecentList()
{
	if (nullptr == recentScrollList) {

		recentScrollList = new ScrollAreaWithNoDataTip(this);
		recentScrollList->setMouseTracking(true);
		recentScrollList->SetScrollBarRightMargin(1);
		connect(recentScrollList, &ScrollAreaWithNoDataTip::retryClicked, this, &PLSGipyStickerView::retryRencentList);

		recentScrollList->setObjectName("recentScrollList");
		recentScrollList->SetNoDataTipText(QTStr("main.giphy.list.noData"));
		InitCommonForList(recentScrollList);
		QWidget *widgetContent = new QWidget();
		widgetContent->setMouseTracking(true);
		widgetContent->setObjectName("recentListContainer");
		recentFlowLayout = new FlowLayout(widgetContent, 0, FLOW_LAYOUT_SPACING, FLOW_LAYOUT_VSPACING);
		recentFlowLayout->setItemRetainSizeWhenHidden(false);
		recentFlowLayout->setContentsMargins(FLOW_LAYOUT_MARGIN_LEFT_RIGHT, FLOW_LAYOUT_MARGIN_TOP_BOTTOM, FLOW_LAYOUT_MARGIN_LEFT_RIGHT, FLOW_LAYOUT_MARGIN_TOP_BOTTOM);
		recentFlowLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		recentScrollList->setWidget(widgetContent);
		listWidgets << recentScrollList;
		ui->verticalLayout_container->addWidget(recentScrollList);
	}
}

void PLSGipyStickerView::InitTrendingList()
{
	if (nullptr == trendingScrollList) {
		trendingScrollList = new ScrollAreaWithNoDataTip(this);
		trendingScrollList->setMouseTracking(true);
		trendingScrollList->SetScrollBarRightMargin(1);

		connect(trendingScrollList, &PLSFloatScrollBarScrollArea::ScrolledToEnd, [=]() { timerScrollTrending.start(500); });
		connect(trendingScrollList, &ScrollAreaWithNoDataTip::retryClicked, this, &PLSGipyStickerView::retryTrendingList);

		trendingScrollList->setObjectName("trendingScrollList");
		trendingScrollList->SetNoDataTipText(QTStr("main.giphy.list.noData"));
		InitCommonForList(trendingScrollList);
		QWidget *widgetContent = new QWidget();
		widgetContent->setMouseTracking(true);
		widgetContent->setObjectName("trendingListContainer");
		trendingFlowLayout = new FlowLayout(widgetContent, 0, FLOW_LAYOUT_SPACING, FLOW_LAYOUT_VSPACING);
		trendingFlowLayout->setItemRetainSizeWhenHidden(false);
		trendingFlowLayout->setContentsMargins(FLOW_LAYOUT_MARGIN_LEFT_RIGHT, FLOW_LAYOUT_MARGIN_TOP_BOTTOM, FLOW_LAYOUT_MARGIN_LEFT_RIGHT, FLOW_LAYOUT_MARGIN_TOP_BOTTOM);
		trendingFlowLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		trendingScrollList->setWidget(widgetContent);
		listWidgets << trendingScrollList;
		ui->verticalLayout_container->addWidget(trendingScrollList);
	}
}

void PLSGipyStickerView::InitSearchResultList()
{
	if (nullptr == searchResultList) {
		searchResultList = new ScrollAreaWithNoDataTip(this);
		searchResultList->setMouseTracking(true);
		searchResultList->SetScrollBarRightMargin(1);

		connect(searchResultList, &PLSFloatScrollBarScrollArea::ScrolledToEnd, [=]() { timerScrollSearch.start(500); });
		connect(searchResultList, &ScrollAreaWithNoDataTip::retryClicked, this, &PLSGipyStickerView::retrySearchList);

		searchResultList->setObjectName("searchResultList");
		searchResultList->SetNoDataTipText(QTStr("main.giphy.list.noSearchResult"));
		InitCommonForList(searchResultList);
		QWidget *widgetContent = new QWidget();
		widgetContent->setMouseTracking(true);
		widgetContent->setObjectName("searchResultContent");
		searchResultFlowLayout = new FlowLayout(widgetContent, 0, FLOW_LAYOUT_SPACING, FLOW_LAYOUT_VSPACING);
		searchResultFlowLayout->setItemRetainSizeWhenHidden(false);
		searchResultFlowLayout->setContentsMargins(FLOW_LAYOUT_MARGIN_LEFT_RIGHT, FLOW_LAYOUT_MARGIN_TOP_BOTTOM, FLOW_LAYOUT_MARGIN_LEFT_RIGHT, FLOW_LAYOUT_MARGIN_TOP_BOTTOM);
		searchResultFlowLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		searchResultList->setWidget(widgetContent);
	}
}

void PLSGipyStickerView::ShowTabList(int tabType)
{
	switch (tabType) {
	case PLSGipyStickerView::RecentTab: {
		bool init = (nullptr == recentScrollList);
		InitRecentList();
		ShowPageWidget(recentScrollList);
		if (init || needUpdateRecentList)
			UpdateRecentList();
		break;
	}
	case PLSGipyStickerView::TrendingTab: {
		bool init = (nullptr == trendingScrollList);
		InitTrendingList();
		ShowPageWidget(trendingScrollList);
		if (init || needUpdateTrendingList)
			QTimer::singleShot(0, this, [=]() { AutoFillTrendingList(); });
		break;
	}
	default:
		break;
	}
}

void PLSGipyStickerView::StartWebHandler()
{
	webHandler = new GiphyWebHandler;
	connect(this, &PLSGipyStickerView::request, webHandler, &GiphyWebHandler::GiphyFetch);
	connect(&threadRequest, &QThread::finished, webHandler, &GiphyWebHandler::deleteLater);
	connect(webHandler, &GiphyWebHandler::FetchResult, this, &PLSGipyStickerView::requestRespon, Qt::QueuedConnection);
	connect(webHandler, &GiphyWebHandler::FetchError, this, &PLSGipyStickerView::OnFetchError, Qt::QueuedConnection);
	connect(webHandler, &GiphyWebHandler::LoadingVisible, this, &PLSGipyStickerView::OnLoadingVisible, Qt::QueuedConnection);
	connect(webHandler, &GiphyWebHandler::networkAccssibleChanged, this, &PLSGipyStickerView::OnNetworkAccessibleChanged, Qt::QueuedConnection);
	webHandler->moveToThread(&threadRequest);
	threadRequest.start();
}

void PLSGipyStickerView::StartDownloader()
{
	GiphyDownloader::instance()->Start();
}

QString PLSGipyStickerView::GetRequestUrl(const QString &apiType, int limit, int offset) const
{
	QString url(API);
	url.append(apiType).append("?");
	url.append("api_key=").append(API_KEY);
	url.append("&limit=").append(QString::number(limit));
	url.append("&rating=").append("G");
	url.append("&offset=").append(QString::number(offset));
	const char *currentLang = App()->GetLocale();
	if (0 == strcmp(currentLang, "en-US")) {
		url.append("&lang=").append("en");
	} else if (0 == strcmp(currentLang, "ko-KR")) {
		url.append("&lang=").append("ko");
	} else {
		url.append("&lang=").append("en");
	}
	return url;
}

void PLSGipyStickerView::ClearTrendingList()
{
	if (trendingScrollList)
		trendingScrollList->ResetPagination();
	auto iter = listTrending.begin();
	while (iter != listTrending.end()) {
		trendingFlowLayout->removeWidget(*iter);
		delete *iter;
		*iter = nullptr;
		iter = listTrending.erase(iter);
	}
}

void PLSGipyStickerView::ClearRecentList()
{
	auto iter = listRecent.begin();
	while (iter != listRecent.end()) {
		recentFlowLayout->removeWidget(*iter);
		delete *iter;
		*iter = nullptr;
		iter = listRecent.erase(iter);
	}
}

void PLSGipyStickerView::ClearSearchList()
{
	if (searchResultList)
		searchResultList->ResetPagination();
	auto iter = listSearch.begin();
	while (iter != listSearch.end()) {
		MovieLabel *item = *iter;
		searchResultFlowLayout->removeWidget(item);
		delete item;
		item = nullptr;
		iter = listSearch.erase(iter);
	}
}

void PLSGipyStickerView::ShowPageWidget(QWidget *widget)
{
	if (!widget)
		return;
	for (QWidget *w : listWidgets) {
		if (w && w != widget) {
			w->hide();
		}
	}
	widget->show();
}

void PLSGipyStickerView::AddHistorySearchItem(const QString &keyword)
{
	searchMenu->AddHistoryItem(keyword);
	WriteSearchHistoryToFile(keyword);
}

bool PLSGipyStickerView::WriteRecentStickerDataToFile(const GiphyData &data, bool del)
{
	if (!jsonObjSticker.contains("recentList"))
		return false;

	QJsonValue recent = jsonObjSticker.value("recentList");
	if (!recent.isArray())
		return false;

	QJsonArray array = recent.toArray();
	auto iter = array.begin();
	bool find = false;
	int index = 0;
	while (iter != array.end()) {
		QJsonObject obj = iter->toObject();
		if (obj.value("id").toString() == data.id) {
			find = true;
			break;
		}
		iter++;
		index++;
	}

	auto createNewObj = [data](QJsonObject &obj) {
		obj.insert("id", data.id);
		obj.insert("type", data.type);
		obj.insert("title", data.title);
		obj.insert("rating", data.rating);
		obj.insert("previewUrl", data.previewUrl);
		obj.insert("originalUrl", data.originalUrl);
		obj.insert("preview_width", data.sizePreview.width());
		obj.insert("preview_height", data.sizePreview.height());
		obj.insert("original_width", data.sizeOriginal.width());
		obj.insert("original_height", data.sizeOriginal.height());
	};
	if (find)
		array.removeAt(index);

	if (!del) {
		QJsonObject obj;
		createNewObj(obj);
		array.prepend(obj);
	}
	int size = array.size();
	if (size > MAX_RECENT_STICKER_COUNT)
		array.removeAt(size - 1);
	jsonObjSticker.insert("recentList", array);
	return true;
}

bool PLSGipyStickerView::WriteSearchHistoryToFile(const QString &keyword, bool del)
{
	if (jsonObjSticker.contains("historyList")) {
		QJsonValue recent = jsonObjSticker.value("historyList");
		if (recent.isArray()) {
			QJsonArray array = recent.toArray();
			auto iter = array.begin();
			bool find = false;
			int index = 0;
			QJsonObject temp;
			while (iter != array.end()) {
				QJsonObject obj = iter->toObject();
				if (obj.value("content").toString() == keyword) {
					find = true;
					temp = obj;
					break;
				}
				iter++;
				index++;
			}
			if (find) {
				array.removeAt(index);
				if (!del)
					array.prepend(temp);
			} else {
				if (!del) {
					QJsonObject obj;
					obj.insert("content", keyword);
					array.prepend(obj);
				}
			}
			jsonObjSticker.insert("historyList", array);
		}

		return true;
	}

	return false;
}

void PLSGipyStickerView::WriteTabIndexToConfig(int index)
{
	config_set_int(App()->GlobalConfig(), GIPHY_STICKERS_CONFIG, TAB_INDEX, index);
	config_save(App()->GlobalConfig());
}

bool PLSGipyStickerView::LoadArrayData(const QString &key, QJsonArray &array)
{
	if (jsonObjSticker.contains(key)) {
		QJsonValue recent = jsonObjSticker.value(key);
		if (recent.isArray()) {
			array = recent.toArray();
			return true;
		}
	}

	return false;
}

bool PLSGipyStickerView::CreateJsonFile()
{
	QString userPath = pls_get_user_path(GIPHY_STICKERS_USER_PATH);
	QString userFileName = pls_get_user_path(GIPHY_STICKERS_JSON_FILE);
	QDir dir(userPath);
	if (!dir.exists())
		dir.mkpath(userPath);

	QFile file(userFileName);
	if (!file.open(QIODevice::ReadWrite)) {
		PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "Could not open %s for writing: %s\n", qPrintable(userFileName), qPrintable(file.errorString()));
		return false;
	}

	QJsonDocument document;
	QByteArray json = file.readAll();
	if (json.isEmpty()) {
		QJsonObject obj;
		QJsonArray arryHistoryList, arrayRecommendList, arrayRecentList;
		obj.insert("historyList", arryHistoryList);
		obj.insert("recommendList", arrayRecommendList);
		obj.insert("recentList", arrayRecentList);
		QJsonDocument document;
		document.setObject(obj);
		QByteArray json = document.toJson(QJsonDocument::Indented);
		file.write(json);
	}
	file.close();
	return true;
}

void PLSGipyStickerView::InitStickerJson()
{
	if (!CreateJsonFile())
		return;
	QString userFileName = pls_get_user_path(GIPHY_STICKERS_JSON_FILE);
	QFile file(userFileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "Could not open %s for writing: %s\n", qPrintable(userFileName), qPrintable(file.errorString()));
		return;
	}

	QJsonParseError parseError;
	QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
	if (parseError.error != QJsonParseError::NoError) {
		file.close();
		return;
	}

	jsonObjSticker = document.object();
}

void PLSGipyStickerView::AddRecentStickerList(const QJsonArray &array)
{
	ClearRecentList();
	if (array.size() == 0) {
		recentScrollList->SetNoDataPageVisible(true);
		recentScrollList->SetNoDataTipText(QTStr("main.giphy.list.noData"));
		return;
	}
	recentScrollList->SetNoDataPageVisible(false);
	auto iter = array.begin();
	while (iter != array.end()) {
		QJsonObject obj = iter->toObject();
		MovieLabel *pLabel = new MovieLabel(this);
		connect(pLabel, &MovieLabel::DownLoadError, this, &PLSGipyStickerView::OriginalDownLoadFailed);
		connect(pLabel, &MovieLabel::OriginalDownloaded, [=](const GiphyData &taskData, const QString &fileName, const QVariant &extra) {
			Q_UNUSED(fileName)
			emit StickerApply(fileName, taskData, extra);
			if (WriteRecentStickerDataToFile(taskData))
				needUpdateRecentList = true;
		});
		GiphyData data;
		data.id = obj.value("id").toString();
		data.type = obj.value("type").toString();
		data.title = obj.value("title").toString();
		data.rating = obj.value("rating").toString();
		data.previewUrl = obj.value("previewUrl").toString();
		data.originalUrl = obj.value("originalUrl").toString();
		data.sizePreview = QSize(obj.value("preview_width").toInt(), obj.value("preview_height").toInt());
		data.sizeOriginal = QSize(obj.value("original_width").toInt(), obj.value("original_height").toInt());
		recentFlowLayout->addWidget(pLabel);
		listRecent << pLabel;
		pLabel->SetData(data);
		iter++;
	}
}

void PLSGipyStickerView::UpdateRecentList()
{
	needUpdateRecentList = false;
	if (!recentScrollList)
		return;
	QJsonArray array;
	LoadArrayData("recentList", array);
	AddRecentStickerList(array);
}

void PLSGipyStickerView::UpdateMenuList()
{
	QJsonArray array;
	LoadArrayData("historyList", array);
	for (int i = array.size() - 1; i >= 0; --i) {
		QJsonObject obj = array.at(i).toObject();
		searchMenu->AddHistoryItem(obj.value("content").toString());
	}
}

void PLSGipyStickerView::DeleteHistoryItem(const QString &key)
{
	WriteSearchHistoryToFile(key, true);
}

void PLSGipyStickerView::QuerySearch(const QString &keyword, int limit, int offset)
{
	QString url = GetRequestUrl(SEARCH_API, limit, offset);
	url.append("&q=").append(keyword.toUtf8());
	RequestTaskData task;
	task.randomId = ConvertPointer(searchResultList);
	task.url = url;
	task.keyword = keyword;
	//emit request(task);
	webHandler->Get(task);
}

void PLSGipyStickerView::QueryTrending(int limit, int offset)
{
	QString url = GetRequestUrl(TRENDING_API, limit, offset);
	RequestTaskData task;
	task.randomId = ConvertPointer(trendingScrollList);
	task.url = url;
	//emit request(task);
	webHandler->Get(task);
}

void PLSGipyStickerView::ShowSearchPage()
{
	// hide the trending scroll list to improve the performance of display.
	if (trendingScrollList && trendingScrollList->isVisible())
		trendingScrollList->hide();
	isSearchMode = true;
	ui->btn_cancel->show();
	searchMenu->hide();
	QPoint pos;
	QSize sizeContent;
	GetSearchListGeometry(sizeContent, pos);
	searchResultList->resize(sizeContent);
	searchResultList->move(pos);
	searchResultList->show();
}

void PLSGipyStickerView::ShowErrorToast(const QString &tips)
{
	if (nullptr == networkErrorToast) {
		networkErrorToast = new PLSToastMsgFrame(this);
	}
	double dpi = PLSDpiHelper::getDpi(this);
	UpdateErrorToastPos();
	networkErrorToast->SetMessage(tips);
	networkErrorToast->SetShowWidth(this->width() - 2 * PLSDpiHelper::calculate(dpi, 10));
	networkErrorToast->ShowToast();
}

void PLSGipyStickerView::UpdateErrorToastPos()
{
	if (!networkErrorToast)
		return;
	double dpi = PLSDpiHelper::getDpi(this);
	QPoint pos;
	if (isSearchMode) {
		pos = searchResultList->mapTo(this, QPoint(PLSDpiHelper::calculate(dpi, 10), PLSDpiHelper::calculate(dpi, 10)));
	} else {
		pos = ui->frame_head->mapTo(this, QPoint(PLSDpiHelper::calculate(dpi, 10), ui->frame_head->height() + PLSDpiHelper::calculate(dpi, 10)));
	}
	networkErrorToast->move(pos);
}

void PLSGipyStickerView::HideErrorToast()
{
	if (networkErrorToast)
		networkErrorToast->HideToast();
}

void PLSGipyStickerView::AutoFillContent()
{
	if (searchResultList && searchResultList->isVisible() && !searchKeyword.isEmpty()) {
		AutoFillSearchList(searchResultList->GetPagination());
	} else if (trendingScrollList && trendingScrollList->isVisible()) {
		AutoFillTrendingList();
	}
}

//After window resized,page count should be recalculated.
void PLSGipyStickerView::UpdateLimit()
{
	double dpi = PLSDpiHelper::getDpi(this);
	int newPageCount = GetColumCountByWidth(this->width(), FLOW_LAYOUT_MARGIN_LEFT_RIGHT);
	if (newPageCount < defaultLimit)
		newPageCount = defaultLimit;
	if (newPageCount != pageLimit)
		qDebug() << "Update Limit to: " << newPageCount;
	pageLimit = newPageCount;
}

inline int PLSGipyStickerView::GetColumCountByWidth(int width, int marginLeftRight)
{
	double dpi = PLSDpiHelper::getDpi(this);
	int itemWidth = PLSDpiHelper::calculate(dpi, MOVIE_ITEM_WIDTH);
	int hSpacing = qFloor(dpi * FLOW_LAYOUT_SPACING);
	//marginLeftRight = PLSDpiHelper::calculate(dpi, marginLeftRight);
	int colums = (width - 2 * marginLeftRight + hSpacing) / (hSpacing + itemWidth);
	return colums;
}

inline int PLSGipyStickerView::GetRowCountByHeight(int height, int marginTopBottom)
{
	double dpi = PLSDpiHelper::getDpi(this);
	int itemHeight = PLSDpiHelper::calculate(dpi, MOVIE_ITEM_HEIGHT);
	int vSpacing = PLSDpiHelper::calculate(dpi, FLOW_LAYOUT_VSPACING);
	//marginTopBottom = PLSDpiHelper::calculate(dpi, marginTopBottom);
	int rows = qRound((height - 2 * marginTopBottom + vSpacing) / float(vSpacing + itemHeight));
	return rows;
}

void PLSGipyStickerView::UpdateLayoutSpacing(FlowLayout *layout, double dpi)
{
	if (layout) {
		layout->setHorizontalSpacing(qFloor(dpi * FLOW_LAYOUT_SPACING));
		layout->setverticalSpacing(PLSDpiHelper::calculate(dpi, FLOW_LAYOUT_VSPACING));
	}
}

int PLSGipyStickerView::GetItemCountToFillContent(const QSize &sizeContent, int leftRightMargin, int topBottomMargin, int &rows, int &colums)
{
	colums = GetColumCountByWidth(sizeContent.width(), leftRightMargin);
	rows = GetRowCountByHeight(sizeContent.height(), topBottomMargin);
	int total = colums * rows;
	return total;
}

void PLSGipyStickerView::AutoFillSearchList(int StartPagenation)
{
	int rows, colums;
	QPoint pos;
	QSize sizeContent;
	GetSearchListGeometry(sizeContent, pos);
	int leftRightMargin = searchResultFlowLayout->contentsMargins().left();
	int topBottomMargin = searchResultFlowLayout->contentsMargins().top();
	GetItemCountToFillContent(sizeContent, leftRightMargin, topBottomMargin, rows, colums);
	int totalCount = ++rows * colums;
	int pagenation = StartPagenation;
	if (totalCount > pagenation) {
		int offset = totalCount - pagenation;
		// the maximum limit of API is 100.
		if (offset > MAX_LIMIT) {
			int requestCount = offset / MAX_LIMIT;

			for (int i = 0; i < requestCount; ++i) {
				QuerySearch(searchKeyword, MAX_LIMIT, pagenation + i * MAX_LIMIT);
			}

			int other = offset % MAX_LIMIT;
			if (other > 0)
				QuerySearch(searchKeyword, other, pagenation + requestCount * MAX_LIMIT);
		} else {
			//offset = (offset > defaultLimit ? offset : defaultLimit);
			QuerySearch(searchKeyword, offset, pagenation);
		}
	}
}

void PLSGipyStickerView::AutoFillTrendingList(bool oneMoreRow)
{
	needUpdateTrendingList = false;
	QSize listContentSize = trendingScrollList->widget()->size();
	if (listContentSize.height() > trendingScrollList->height())
		return;
	int rows, colums;
	int leftRightMargin = trendingFlowLayout->contentsMargins().left();
	int topBottomMargin = trendingFlowLayout->contentsMargins().top();
	GetItemCountToFillContent(listContentSize, leftRightMargin, topBottomMargin, rows, colums);
	int totalCount = (oneMoreRow ? ++rows : rows) * colums;
	int pagenation = trendingScrollList->GetPagination();
	if (totalCount > pagenation) {
		int offset = totalCount - pagenation;
		// the maximum limit of API is 100.
		if (offset > MAX_LIMIT) {
			int requestCount = offset / MAX_LIMIT;
			for (int i = 0; i < requestCount; ++i) {
				QueryTrending(MAX_LIMIT, pagenation + i * MAX_LIMIT);
			}

			int other = offset % MAX_LIMIT;
			if (other > 0)
				QueryTrending(other, pagenation + requestCount * MAX_LIMIT);
		} else {
			//offset = (offset > defaultLimit ? offset : defaultLimit);
			QueryTrending(offset, pagenation);
		}
	}
}

void PLSGipyStickerView::GetSearchListGeometry(QSize &sizeContent, QPoint &position)
{
	QRect geometry;
	double dpi = PLSDpiHelper::getDpi(this);
	int nY = ui->lineEdit_search->mapTo(this, QPoint(0, ui->lineEdit_search->height())).y() + PLSDpiHelper::calculate(dpi, 13);
	position.setX(PLSDpiHelper::calculate(dpi, 1));
	position.setY(nY);
	sizeContent.setWidth(this->width() - PLSDpiHelper::calculate(dpi, 2));
	sizeContent.setHeight(this->height() - nY - PLSDpiHelper::calculate(dpi, 1));
}

void PLSGipyStickerView::AddSearchItem(const GiphyData &data)
{
	if (stopProcess)
		return;
	MovieLabel *pLabel = new MovieLabel(this);
	connect(pLabel, &MovieLabel::OriginalDownloaded, this, &PLSGipyStickerView::OriginalDownload);
	connect(pLabel, &MovieLabel::DownLoadError, this, &PLSGipyStickerView::OriginalDownLoadFailed);
	searchResultFlowLayout->addWidget(pLabel);
	pLabel->SetData(data);
	listSearch << pLabel;
}

void PLSGipyStickerView::ShowRetryPage()
{
	QList<ScrollAreaWithNoDataTip *> listScrollArea;
	listScrollArea << recentScrollList << trendingScrollList << searchResultList;
	for (auto page : listScrollArea) {
		if (page) {
			page->SetShowRetryPage();
			page->SetRetryTipText(QTStr("main.giphy.network.toast.error"));
		}
	}
}

bool PLSGipyStickerView::IsNetworkAccessible()
{
	return (QNetworkAccessManager::Accessible == accessibility);
}

void PLSGipyStickerView::retrySearchList()
{
	if (!searchResultList)
		return;
	PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, "retry button", ACTION_CLICK);
	if (accessibility != QNetworkAccessManager::Accessible) {
		ShowErrorToast(QTStr("main.giphy.network.toast.error"));
		return;
	}
	searchResultList->HideRetryPage();
	AutoFillSearchList(0);

	if (RecentTab == currentType)
		retryRencentList();
	else if (TrendingTab == currentType)
		retryTrendingList();
}

void PLSGipyStickerView::retryTrendingList()
{
	if (!trendingScrollList)
		return;
	PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, "retry button", ACTION_CLICK);
	if (accessibility != QNetworkAccessManager::Accessible) {
		ShowErrorToast(QTStr("main.giphy.network.toast.error"));
		return;
	}
	trendingScrollList->HideRetryPage();
	AutoFillTrendingList();
	needUpdateRecentList = true;
}

void PLSGipyStickerView::retryRencentList()
{
	if (!recentScrollList)
		return;
	PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, "retry button", ACTION_CLICK);
	if (accessibility != QNetworkAccessManager::Accessible) {
		ShowErrorToast(QTStr("main.giphy.network.toast.error"));
		return;
	}
	recentScrollList->HideRetryPage();
	trendingScrollList->HideRetryPage();
	UpdateRecentList();
	needUpdateTrendingList = true;
}

void PLSGipyStickerView::SetSearchData(const ResponData &data)
{
	if (!isSearching)
		return;
	ShowSearchPage();
	int size = data.giphyData.size();
	PLS_INFO(MAIN_GIPHY_STICKER_MODULE, "request search '%s' data size=%d", data.task.keyword.toStdString().c_str(), size);
	bool noData = (size == 0);
	if (data.pageData.offset == 0) {
		stopProcess = true;
		searchResultList->SetNoDataPageVisible(noData);
		searchResultList->SetNoDataTipText(QTStr("main.giphy.list.noSearchResult"));
		ClearSearchList();
		lastSearchResponOffset = -1;
	}
	if (noData)
		return;
	// avoid repeat data
	if (data.pageData.offset <= lastSearchResponOffset) {
		PLS_INFO(MAIN_GIPHY_STICKER_MODULE, "search list error paganation data respon, last offset=%d,current offset=%d", lastSearchResponOffset, data.pageData.offset);
		return;
	}
	lastSearchResponOffset = data.pageData.offset;
	int nextPagination = data.pageData.offset + data.pageData.count;
	searchResultList->SetPagintion(nextPagination);
	if (size > 0 && !data.task.keyword.isEmpty())
		AddHistorySearchItem(data.task.keyword);
	stopProcess = false;
	for (GiphyData data : data.giphyData) {
		if (exit)
			return;
		if (stopProcess)
			break;
		//QApplication::processEvents();
		AddSearchItem(data);
	}
}

void PLSGipyStickerView::onMaxFullScreenStateChanged()
{
	config_set_bool(App()->GlobalConfig(), GIPHY_STICKERS_CONFIG, MAXIMIZED_STATE, getMaxState());
	config_save(App()->GlobalConfig());
}

void PLSGipyStickerView::onSaveNormalGeometry()
{
	config_set_string(App()->GlobalConfig(), GIPHY_STICKERS_CONFIG, GEOMETRY_DATA, saveGeometry().toBase64().constData());
	config_save(App()->GlobalConfig());
}

bool PLSGipyStickerView::IsSignalConnected(const QMetaMethod &signal)
{
	return this->isSignalConnected(signal);
}

void PLSGipyStickerView::SaveStickerJsonData()
{
	QString userFileName = pls_get_user_path(GIPHY_STICKERS_JSON_FILE);
	QFile file(userFileName);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
		PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "Could not open %s for writing: %s\n", qPrintable(userFileName), qPrintable(file.errorString()));
		return;
	}

	int oldSize = file.readAll().size();
	QJsonDocument document;
	document.setObject(jsonObjSticker);
	QByteArray json = document.toJson(QJsonDocument::Indented);
	if (json.size() < oldSize)
		file.resize(json.size());
	file.seek(0);
	file.write(json);
	file.flush();
	file.close();
}

void PLSGipyStickerView::SaveShowModeToConfig()
{
	config_set_bool(App()->GlobalConfig(), GIPHY_STICKERS_CONFIG, "showMode", isVisible());
	config_save(App()->GlobalConfig());
}

void PLSGipyStickerView::InitGeometry()
{
	auto initGeometry = [this](double dpi, bool inConstructor) {
		extern void setGeometrySys(PLSWidgetDpiAdapter * adapter, const QRect &geometry);

		const char *geometry = config_get_string(App()->GlobalConfig(), GIPHY_STICKERS_CONFIG, GEOMETRY_DATA);
		if (!geometry || !geometry[0]) {
			const int defaultWidth = 298;
			const int defaultHeight = 802;
			const int mainRightOffest = 5;
			PLSMainView *mainView = App()->getMainView();
			QPoint mainTopRight = App()->getMainView()->mapToGlobal(QPoint(mainView->frameGeometry().width(), 0));
			geometryOfNormal = QRect(mainTopRight.x() + PLSDpiHelper::calculate(dpi, mainRightOffest), mainTopRight.y(), PLSDpiHelper::calculate(dpi, defaultWidth),
						 PLSDpiHelper::calculate(dpi, defaultHeight));
			setGeometrySys(this, geometryOfNormal);
		} else if (inConstructor) {
			QByteArray byteArray = QByteArray::fromBase64(QByteArray(geometry));
			restoreGeometry(byteArray);
			if (config_get_bool(App()->GlobalConfig(), GIPHY_STICKERS_CONFIG, MAXIMIZED_STATE)) {
				showMaximized();
			}
		}
	};

	PLSDpiHelper dpiHelper;
	dpiHelper.notifyDpiChanged(this, [=](double dpi, double, bool isFirstShow) {
		extern QRect normalShow(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);

		if (isFirstShow) {
			initGeometry(dpi, false);
			if (!isMaxState && !isFullScreenState) {
				normalShow(this, geometryOfNormal);
			}
		}
	});

	initGeometry(PLSDpiHelper::getDpi(this), true);
}

void PLSGipyStickerView::SetBindSourcePtr(PointerValue sourcePtr)
{
	g_bindSourcePtr = sourcePtr;
	if (g_prevClickedItem)
		g_prevClickedItem->SetClicked(false);
}

void PLSGipyStickerView::hideEvent(QHideEvent *event)
{
	showFromProperty = false;
	PLSDialogView::hideEvent(event);
	if (isSearchMode)
		on_btn_cancel_clicked();
	emit visibleChanged(false);

	if (!getMaxState()) {
		onSaveNormalGeometry();
	}

	SaveShowModeToConfig();
}

void PLSGipyStickerView::closeEvent(QCloseEvent *event)
{
	hide();
	event->ignore();
}

void PLSGipyStickerView::resizeEvent(QResizeEvent *event)
{
	if (searchMenu && searchMenu->isVisible()) {
		searchMenu->SetContentWidth(ui->lineEdit_search->width());
		searchMenu->move(ui->lineEdit_search->mapTo(this, QPoint(0, ui->lineEdit_search->height())));
	}

	if (searchResultList && searchResultList->isVisible()) {
		double dpi = PLSDpiHelper::getDpi(this);
		int nY = ui->lineEdit_search->mapTo(this, QPoint(0, ui->lineEdit_search->height())).y() + PLSDpiHelper::calculate(dpi, 13);
		int width = this->width() - PLSDpiHelper::calculate(dpi, 2);
		searchResultList->resize(width, this->height() - nY - PLSDpiHelper::calculate(dpi, 1));
		searchResultList->move(PLSDpiHelper::calculate(dpi, 1), nY);
	}

	if (networkErrorToast && networkErrorToast->isVisible()) {
		double dpi = PLSDpiHelper::getDpi(this);
		networkErrorToast->SetShowWidth(this->width() - 2 * PLSDpiHelper::calculate(dpi, 10));
		UpdateErrorToastPos();
	}

	timerAutoLoad.start(500);
	PLSDialogView::resizeEvent(event);
}

void PLSGipyStickerView::mousePressEvent(QMouseEvent *event)
{
	if (searchMenu && !searchMenu->geometry().contains(event->pos())) {
		searchMenu->hide();
	}
	PLSDialogView::mousePressEvent(event);
}

void PLSGipyStickerView::showEvent(QShowEvent *event)
{
	PLSDialogView::showEvent(event);
	emit visibleChanged(true);
	if (isFirstShow) {
		isFirstShow = false;
		StartWebHandler();
		StartDownloader();
		ui->btn_trending->setChecked(true);
		QTimer::singleShot(0, this, [=]() { OnTabClicked(TrendingTab); });
	}
	SaveShowModeToConfig();
}

bool PLSGipyStickerView::event(QEvent *e)
{
	if (e && e->type() == customType) {
		auto addSticker = static_cast<AddStickerEvent *>(e);
		ResponData data = addSticker->GetData();
		SetSearchData(data);
		return true;
	}
	return PLSDialogView::event(e);
}

void PLSGipyStickerView::OnTabClicked(int index)
{
	ShowTabList(index);
	switch (index) {
	case RecentTab: {
		PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, "Recent Tab", ACTION_CLICK);
		currentType = RecentTab;
		if (!IsNetworkAccessible()) {
			recentScrollList->SetRetryTipText(QTStr("main.giphy.network.toast.error"));
			recentScrollList->SetRetryPageVisible(true);
		}
		break;
	}
	case TrendingTab: {
		PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, "Trending Tab", ACTION_CLICK);
		currentType = TrendingTab;
		if (!IsNetworkAccessible()) {
			trendingScrollList->SetRetryTipText(QTStr("main.giphy.network.toast.error"));
			trendingScrollList->SetRetryPageVisible(true);
		}
		break;
	}
	default:
		currentType = TrendingTab;
		break;
	}

	WriteTabIndexToConfig(index);
}

void PLSGipyStickerView::requestRespon(const ResponData &data)
{
	if (exit)
		return;

	if (data.task.randomId <= 0)
		return;

	HideErrorToast();
	if (data.task.randomId == ConvertPointer(trendingScrollList)) {
		static int lastTrendingResponOffset = -1;
		// avoid repeat data
		if (data.pageData.offset <= lastTrendingResponOffset) {
			PLS_INFO(MAIN_GIPHY_STICKER_MODULE, "trending list error paganation data respon, last offset=%d,current offset=%d", lastTrendingResponOffset, data.pageData.offset);
			return;
		}
		lastTrendingResponOffset = data.pageData.offset;
		int size = data.giphyData.size();
		bool noData = (size == 0);
		if (data.pageData.offset == 0) {
			trendingScrollList->SetNoDataPageVisible(noData);
			trendingScrollList->SetNoDataTipText(QTStr("main.giphy.list.noData"));
			ClearTrendingList();
		}
		if (noData)
			return;
		int nextPagination = data.pageData.offset + data.pageData.count;
		trendingScrollList->SetPagintion(nextPagination);
		for (GiphyData data : data.giphyData) {
			if (exit)
				return;
			QApplication::processEvents();
			MovieLabel *pLabel = new MovieLabel(this);
			connect(pLabel, &MovieLabel::OriginalDownloaded, this, &PLSGipyStickerView::OriginalDownload);
			connect(pLabel, &MovieLabel::DownLoadError, this, &PLSGipyStickerView::OriginalDownLoadFailed);
			trendingFlowLayout->addWidget(pLabel);
			pLabel->SetData(data);
			listTrending << pLabel;
		}
		trendingFlowLayout->update();
	} else if (data.task.randomId == ConvertPointer(searchResultList)) {
		AddStickerEvent *e = new AddStickerEvent(data);
		QApplication::postEvent(this, e);
	}
}

void PLSGipyStickerView::OnFetchError(const RequestTaskData &task, const RequestErrorInfo &errorInfo)
{
	if (task.randomId <= 0)
		return;

	if (searchMenu)
		searchMenu->hide();

	QString errorTips;
	switch (errorInfo.errorType) {
	case RequestErrorType::RequstTimeOut: {
		errorTips = QTStr("main.giphy.network.request.timeout");
		break;
	}
	case RequestErrorType::OtherNetworkError: {
		switch (errorInfo.networkError) {
		case QNetworkReply::ConnectionRefusedError:
		case QNetworkReply::RemoteHostClosedError:
		case QNetworkReply::HostNotFoundError:
			errorTips = QTStr("main.giphy.network.toast.error");
			break;
		default: {
			errorTips = QTStr("main.giphy.network.unknownError");
			ShowErrorToast(errorTips);
			return;
		}
		}
		break;
	}
	default:
		break;
	}

	if (task.randomId == ConvertPointer(trendingScrollList)) {
		trendingScrollList->SetShowRetryPage();
		trendingScrollList->SetNoDataTipText(errorTips);

	} else if (task.randomId == ConvertPointer(searchResultList)) {
		if (searchResultList->isVisible()) {
			searchResultList->SetShowRetryPage();
			searchResultList->SetNoDataTipText(errorTips);
		} else
			ShowErrorToast(errorTips);
	} else {
		ShowErrorToast(errorTips);
	}
}

void PLSGipyStickerView::OnSearchTigger(const QString &keyword)
{
	if (keyword.isEmpty())
		return;
	if (searchMenu)
		searchMenu->hide();
	if (!IsNetworkAccessible()) {
		ShowErrorToast(QTStr("main.giphy.network.toast.error"));
		return;
	}

	//QApplication::removePostedEvents(this, customType);
	webHandler->DiscardTask();

	isSearching = true;
	PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, "Search line edit tiggered", ACTION_CLICK);
	searchKeyword = keyword.simplified();
	InitSearchResultList();

	//QTimer::singleShot(0, this, [=]() { AutoFillSearchList(0); });
	stopProcess = true;
	timerSearch.start(500);
}

void PLSGipyStickerView::OnSearchMenuRequested(bool show)
{
	if (searchMenu) {
		if (show) {
			searchMenu->Show(ui->lineEdit_search->mapTo(this, QPoint(0, ui->lineEdit_search->height())), ui->lineEdit_search->width());
			searchMenu->raise();
		}
		searchMenu->setVisible(show);
	}
}

void PLSGipyStickerView::on_btn_cancel_clicked()
{
	PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, "Search cancel button", ACTION_CLICK);
	bool networkValid = IsNetworkAccessible();
	if (TrendingTab == currentType) {
		trendingScrollList->show();
		if (networkValid)
			retryTrendingList();
	} else if (RecentTab == currentType)
		retryRencentList();
	isSearching = false;
	isSearchMode = false;
	ui->btn_cancel->hide();
	ui->lineEdit_search->clear();
	lastSearchResponOffset = -1;
	searchKeyword.clear();
	if (searchResultList) {
		searchResultList->hide();
		searchResultList->ResetPagination();
		ClearSearchList();
	}

	if (networkErrorToast && networkErrorToast->isVisible()) {
		ShowErrorToast(networkErrorToast->GetMessage());
	}
}

void PLSGipyStickerView::OriginalDownload(const GiphyData &stickerData, const QString &fileName, const QVariant &extra)
{
	emit StickerApply(fileName, stickerData, extra);
	if (WriteRecentStickerDataToFile(stickerData))
		UpdateRecentList();
}

void PLSGipyStickerView::OriginalDownLoadFailed()
{
	if (accessibility != QNetworkAccessManager::Accessible) {
		ShowErrorToast(QTStr("main.giphy.network.toast.error"));
		return;
	}
	ShowErrorToast(QTStr("main.giphy.network.download.faild"));
}

void PLSGipyStickerView::OnNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible)
{
	accessibility = accessible;
	if (QNetworkAccessManager::Accessible == accessible)
		HideErrorToast();
	else {
		ShowRetryPage();
	}
}

void PLSGipyStickerView::OnLoadingVisible(const RequestTaskData &task, bool visible)
{
	if (task.randomId == 0)
		return;

	if (task.randomId == ConvertPointer(trendingScrollList))
		trendingScrollList->SetLoadingVisible(visible);
	else if (task.randomId == ConvertPointer(searchResultList)) {
		if (!searchResultList->isVisible())
			trendingScrollList->SetLoadingVisible(visible);
		else
			searchResultList->SetLoadingVisible(visible);
	}
}

MovieLabel::MovieLabel(QWidget *parent) : QLabel(parent), rectTarget(QRect())
{
	this->setCursor(Qt::ArrowCursor);
	this->setAutoFillBackground(true);
	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::StrongFocus);

	this->setProperty("useDefaultIcon", true);
	pls_flush_style(this);

	labelLoad = new QLabel(this);
	labelLoad->setObjectName("labelIconLoading");
	labelLoad->hide();

	connect(GiphyDownloader::instance(), &GiphyDownloader::downloadResult, this, &MovieLabel::downloadCallback, Qt::QueuedConnection);
	connect(this, SIGNAL(excuteTask(const DownloadTaskData &)), GiphyDownloader::instance(), SLOT(excuteTask(const DownloadTaskData &)));
}

MovieLabel::~MovieLabel()
{
	if (loadingEvent) {
		delete loadingEvent;
		loadingEvent = nullptr;
	}
	timer.stop();
	imageFrames.clear();
}

void MovieLabel::SetData(const GiphyData &data)
{
	giphyData = data;
	DownloadPreview();
}

void MovieLabel::SetShowLoad(bool visible)
{
	if (visible) {
		if (nullptr == loadingEvent)
			loadingEvent = new PLSLoadingEvent;
		loadingEvent->startLoadingTimer(labelLoad);
	} else {
		if (loadingEvent) {
			loadingEvent->stopLoadingTimer();
			delete loadingEvent;
			loadingEvent = nullptr;
		}
	}
	labelLoad->move(this->width() / 2 - labelLoad->width() / 2, this->height() / 2 - labelLoad->height() / 2);
	labelLoad->setVisible(visible);
	labelLoad->raise();
}

void MovieLabel::SetClicked(bool clicked_)
{
	clicked = clicked_;
	this->setProperty(STATUS_CLICKED, clicked_);
	pls_flush_style(this);
}

bool MovieLabel::Clicked()
{
	return clicked;
}

void MovieLabel::DownloadCallback(const TaskResponData &result, void *param)
{
	MovieLabel *movieLabel = reinterpret_cast<MovieLabel *>(param);
	if (movieLabel) {
		QMetaObject::invokeMethod(movieLabel, "downloadCallback", Qt::QueuedConnection, Q_ARG(const TaskResponData &, result));
	}
}

void MovieLabel::DownloadOriginal()
{
	DownloadTaskData currentTask;
	currentTask.SourceSize = giphyData.sizeOriginal;
	currentTask.url = giphyData.originalUrl;
	currentTask.needRetry = false;
	currentTask.uniqueId = giphyData.id;
	currentTask.type = StickerDownloadType::ORIGINAL;
	currentTask.extraData = QVariant::fromValue(g_bindSourcePtr);
	QString fileName;
	if (GiphyDownloader::IsDownloadFileExsit(currentTask, fileName)) {
		QTimer::singleShot(200, this, [=]() {
			SetClicked(false);
			emit OriginalDownloaded(giphyData, fileName, currentTask.extraData);
		});
		return;
	}

	if (nullptr == downloadOriginalObj) {
		downloadOriginalObj = new QObject(this);
	}
	SetShowLoad(true);
	currentTask.randomId = PLSGipyStickerView::ConvertPointer(downloadOriginalObj);
	emit excuteTask(currentTask);
}

void MovieLabel::LoadFrames()
{
	imageFrames.clear();
	QImage image(giphyFileInfo.taskData.SourceSize, QImage::Format_RGB16);
	QImageReader imageReader;
	imageReader.setFileName(giphyFileInfo.fileName);
	imageReader.setQuality(30);
	int imageCount = imageReader.imageCount();
	bool needDownsample = false;
	if (imageCount > MAX_PREVIEW_FRAME) {
		needDownsample = true;
	}
	for (int i = 0; i < imageCount; ++i) {
		if (imageReader.read(&image)) {
			if (needDownsample && i % 2 != 0)
				continue;
			imageFrames.emplace_back(QPixmap::fromImage(image));
		}
	}
	imageDelay = imageReader.nextImageDelay();
}

void MovieLabel::ResizeScale()
{
	scaledSize = giphyFileInfo.taskData.SourceSize;
	if (scaledSize.width() > STICKER_DISPLAY_SIZE || scaledSize.height() > STICKER_DISPLAY_SIZE) {
		float radio = 1.0;
		if (scaledSize.width() > scaledSize.height()) {
			if (scaledSize.width() > 0)
				radio = STICKER_DISPLAY_SIZE / (float)scaledSize.width();

		} else {
			if (scaledSize.height() > 0)
				radio = STICKER_DISPLAY_SIZE / (float)scaledSize.height();
		}
		scaledSize.setWidth((int)(radio * scaledSize.width()));
		scaledSize.setHeight((int)(radio * scaledSize.height()));
	}

	UpdateRect();
}

void MovieLabel::UpdateRect()
{
	double dpi = PLSDpiHelper::getDpi(this);
	int displayWidth = PLSDpiHelper::calculate(dpi, scaledSize.width());
	int displayHeight = PLSDpiHelper::calculate(dpi, scaledSize.height());
	rectTarget.setRect((this->width() - displayWidth) / 2, (this->height() - displayHeight) / 2, displayWidth, displayHeight);
}

void MovieLabel::DownloadPreview()
{
	DownloadTaskData currentTask;
	currentTask.SourceSize = giphyData.sizePreview;
	currentTask.url = giphyData.previewUrl;
	currentTask.uniqueId = giphyData.id;
	currentTask.type = StickerDownloadType::THUMBNAIL;
	currentTask.randomId = PLSGipyStickerView::ConvertPointer(this);
	emit excuteTask(currentTask);
}

void MovieLabel::enterEvent(QEvent *event)
{
	QLabel::enterEvent(event);
}

void MovieLabel::leaveEvent(QEvent *event)
{
	QLabel::leaveEvent(event);
}

void MovieLabel::mousePressEvent(QMouseEvent *ev)
{
	if (ev->button() != Qt::LeftButton) {
		QLabel::mousePressEvent(ev);
		return;
	}

	PLS_UI_STEP(MAIN_GIPHY_STICKER_MODULE, "Sticker movie label", ACTION_CLICK);
	if (g_prevClickedItem && g_prevClickedItem != this) {
		g_prevClickedItem->SetClicked(false);
	}
	this->SetClicked(true);
	g_prevClickedItem = this;
	QLabel::mousePressEvent(ev);
	DownloadOriginal();
}

void MovieLabel::resizeEvent(QResizeEvent *ev)
{
	labelLoad->move(this->width() / 2 - labelLoad->width() / 2, this->height() / 2 - labelLoad->height() / 2);
	UpdateRect();
	QLabel::resizeEvent(ev);
}

void MovieLabel::timerEvent(QTimerEvent *event)
{
	if (this->visibleRegion().isEmpty()) {
		if (!imageFrames.empty())
			imageFrames.clear();
		QLabel::timerEvent(event);
		return;
	}

	if (imageFrames.empty()) {
		LoadFrames();
	}

	if (event->timerId() == timer.timerId()) {
		if (imageFrames.size() > 0) {
			if (step >= imageFrames.size() - 1) {
				step = -1;
			}
			step++;
			update();
		}
	}
	QLabel::timerEvent(event);
}

void MovieLabel::paintEvent(QPaintEvent *event)
{
	QPainter dc(this);
	dc.setRenderHint(QPainter::SmoothPixmapTransform);
	if (imageFrames.size() > 0 && step >= 0 && step < imageFrames.size()) {
		dc.drawPixmap(rectTarget, imageFrames[step]);
	}
	QLabel::paintEvent(event);
}

void MovieLabel::showEvent(QShowEvent *event)
{
	if (imageDelay > 0 && !timer.isActive())
		timer.start(imageDelay, this);
	QLabel::showEvent(event);
}

void MovieLabel::hideEvent(QHideEvent *event)
{
	if (timer.isActive())
		timer.stop();
	imageFrames.clear();
	QLabel::hideEvent(event);
}

void MovieLabel::downloadCallback(const TaskResponData &responData)
{
	if (responData.taskData.randomId == 0)
		return;
	if (responData.taskData.randomId == PLSGipyStickerView::ConvertPointer(this)) {
		if (responData.resultType == ResultStatus::GIPHY_NO_ERROR) {
			if (!responData.fileName.isEmpty() && QFile::exists(responData.fileName)) {
				giphyFileInfo = responData;
				needRetry = false;
				this->setProperty("useDefaultIcon", false);
				pls_flush_style(this);
				ResizeScale();
				LoadFrames();
				timer.start(imageDelay, this);
			} else {
				PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "sticker file name is not exsit,use default icon");
				this->setProperty("useDefaultIcon", true);
				pls_flush_style(this);
			}

		} else {
			PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "sticker download error,use default icon");
			emit DownLoadError();
			this->setProperty("useDefaultIcon", true);
			pls_flush_style(this);
		}
	} else if (responData.taskData.randomId == PLSGipyStickerView::ConvertPointer(downloadOriginalObj)) {
		SetShowLoad(false);
		SetClicked(false);
		if (responData.resultType == ResultStatus::GIPHY_NO_ERROR) {
			QString fileName = responData.fileName;
			if (!fileName.isEmpty()) {
				emit OriginalDownloaded(giphyData, fileName, responData.taskData.extraData);
			} else
				PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "original sticker file name is empty");
		} else {
			PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "original sticker download error");
			emit DownLoadError();
		}
	}
}

SearchLineEdit::SearchLineEdit(QWidget *parent) : QLineEdit(parent)
{
	QHBoxLayout *searchLayout = new QHBoxLayout(this);
	searchLayout->setContentsMargins(0, 8, 9, 10);
	searchLayout->setSpacing(0);
	toolBtnSearch = new QToolButton(this);
	toolBtnSearch->setObjectName("toolBtnSearch");
	toolBtnSearch->setProperty("searchOn", false);
	pls_flush_style(toolBtnSearch);
	searchLayout->addStretch();
	searchLayout->addWidget(toolBtnSearch);
}

SearchLineEdit::~SearchLineEdit() {}

void SearchLineEdit::focusInEvent(QFocusEvent *e)
{
	toolBtnSearch->setProperty("searchOn", true);
	pls_flush_style(toolBtnSearch);
	QLineEdit::focusInEvent(e);
}

void SearchLineEdit::focusOutEvent(QFocusEvent *e)
{
	emit SearchMenuRequested(false);
	toolBtnSearch->setProperty("searchOn", false);
	pls_flush_style(toolBtnSearch);
	QLineEdit::focusOutEvent(e);
}

void SearchLineEdit::keyReleaseEvent(QKeyEvent *event)
{
	if (event->type() == QEvent::KeyRelease) {
		QKeyEvent *keyEvent = reinterpret_cast<QKeyEvent *>(event);
		switch (keyEvent->key()) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
			emit SearchTrigger(this->text());
		}
	}
	QLineEdit::keyReleaseEvent(event);
}

void SearchLineEdit::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		emit SearchMenuRequested(true);
	}
	QLineEdit::mousePressEvent(event);
}

NoDataPage::NoDataPage(QWidget *parent) : QFrame(parent)
{
	this->setCursor(Qt::ArrowCursor);
	QVBoxLayout *layout_main = new QVBoxLayout(this);
	layout_main->setContentsMargins(0, 0, 0, 0);
	layout_main->setSpacing(0);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(18);
	layout->setAlignment(Qt::AlignHCenter);

	iconNoData = new QLabel(this);
	iconNoData->setObjectName("iconNoData");

	textNoDataTip = new QLabel(this);
	textNoDataTip->setAlignment(Qt::AlignCenter);
	textNoDataTip->setWordWrap(true);
	textNoDataTip->setObjectName("textNoData");

	btnRetry = new QPushButton(this);
	btnRetry->setFocusPolicy(Qt::NoFocus);
	connect(btnRetry, &QPushButton::clicked, this, &NoDataPage::retryClicked);
	btnRetry->setObjectName("networkErrorRetryBtn");
	btnRetry->setText(QTStr("main.giphy.network.retry"));

	layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
	layout->addWidget(iconNoData, 0, Qt::AlignHCenter);
	layout->addWidget(textNoDataTip, 0, Qt::AlignHCenter);
	layout->addWidget(btnRetry, 0, Qt::AlignHCenter);
	layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

	spaceBottom = new QLabel(this);
	spaceBottom->setObjectName("noDataPageBottomSpace");

	layout_main->addItem(layout);
	layout_main->addWidget(spaceBottom, 0, Qt::AlignHCenter);

	SetPageType(Page_Nodata);
}

NoDataPage::~NoDataPage() {}

void NoDataPage::SetPageType(int pageType)
{
	iconNoData->setProperty(PAGE_TYPE, pageType);
	pls_flush_style(iconNoData);
	switch (pageType) {
	case Page_Nodata:
		btnRetry->hide();
		break;
	case Page_NetworkError:
		btnRetry->show();
		break;
	default:
		break;
	}
}

void NoDataPage::SetNoDataTipText(const QString &tipText)
{
	if (textNoDataTip) {
		textNoDataTip->setText(tipText);
	}
}

ScrollAreaWithNoDataTip::ScrollAreaWithNoDataTip(QWidget *parent) : PLSFloatScrollBarScrollArea(parent)
{
	this->setCursor(Qt::ArrowCursor);
	pageNoData = new NoDataPage(this);
	connect(pageNoData, &NoDataPage::retryClicked, this, &ScrollAreaWithNoDataTip::retryClicked);
	pageNoData->setObjectName("pageNoData");
	pageNoData->hide();
}

ScrollAreaWithNoDataTip::~ScrollAreaWithNoDataTip() {}

void ScrollAreaWithNoDataTip::ResetPagination()
{
	pagination = 0;
}

void ScrollAreaWithNoDataTip::SetPagintion(int page)
{
	pagination = page;
}

int ScrollAreaWithNoDataTip::GetPagination() const
{
	return pagination;
}

void ScrollAreaWithNoDataTip::ShowNoDataPage()
{
	if (pageNoData) {
		pageNoData->show();
		pageNoData->SetPageType(NoDataPage::Page_Nodata);
		pageNoData->raise();
	}
}

void ScrollAreaWithNoDataTip::HideNoDataPage()
{
	if (pageNoData) {
		pageNoData->hide();
	}
}

void ScrollAreaWithNoDataTip::SetNoDataPageVisible(bool visible)
{
	if (pageNoData) {
		pageNoData->setVisible(visible);
		pageNoData->SetPageType(NoDataPage::Page_Nodata);
		pageNoData->raise();
	}
}

void ScrollAreaWithNoDataTip::SetNoDataTipText(const QString &tipText)
{
	if (pageNoData) {
		pageNoData->SetNoDataTipText(tipText);
	}
}

void ScrollAreaWithNoDataTip::SetShowRetryPage()
{
	if (pageNoData) {
		pageNoData->show();
		pageNoData->SetPageType(NoDataPage::Page_NetworkError);
		pageNoData->raise();
	}
}

void ScrollAreaWithNoDataTip::HideRetryPage()
{
	if (pageNoData) {
		pageNoData->hide();
	}
}

void ScrollAreaWithNoDataTip::SetRetryPageVisible(bool visible)
{
	if (pageNoData) {
		pageNoData->setVisible(visible);
		pageNoData->SetPageType(NoDataPage::Page_NetworkError);
		pageNoData->raise();
	}
}

void ScrollAreaWithNoDataTip::SetRetryTipText(const QString &tipText)
{
	if (pageNoData) {
		pageNoData->SetNoDataTipText(tipText);
	}
}

bool ScrollAreaWithNoDataTip::IsNoDataPageVisible()
{
	return pageNoData->isVisible();
}

void ScrollAreaWithNoDataTip::ShowLoading()
{
	if (nullptr == loadingLayer) {
		loadingLayer = new QFrame(this);
		loadingLayer->setObjectName("scrollAreaLoadingLayer");
		loadingLayer->setMouseTracking(true);
		QVBoxLayout *layout = new QVBoxLayout(loadingLayer);
		QLabel *loadingLabel = new QLabel(loadingLayer);
		loadingLabel->setObjectName("scrollAreaLoadingIcon");
		layout->addWidget(loadingLabel);
		layout->setAlignment(loadingLabel, Qt::AlignCenter);
	}
	UpdateLayerGeometry();
	loadingEvent.startLoadingTimer(loadingLayer->findChild<QLabel *>("scrollAreaLoadingIcon"));
	loadingLayer->show();
	loadingLayer->raise();
}

void ScrollAreaWithNoDataTip::HideLoading()
{
	if (!loadingLayer)
		return;
	loadingLayer->hide();
	loadingEvent.stopLoading();
}

void ScrollAreaWithNoDataTip::SetLoadingVisible(bool visible)
{
	if (visible)
		ShowLoading();
	else
		HideLoading();
}

void ScrollAreaWithNoDataTip::UpdateLayerGeometry()
{
	if (!loadingLayer)
		return;
	double dpi = PLSDpiHelper::getDpi(this);
	loadingLayer->resize(this->width() - PLSDpiHelper::calculate(dpi, 2), this->height() - PLSDpiHelper::calculate(dpi, 1));
	loadingLayer->move(PLSDpiHelper::calculate(dpi, 1), 0);
}

void ScrollAreaWithNoDataTip::resizeEvent(QResizeEvent *event)
{
	if (pageNoData) {
		pageNoData->move(0, 0);
		pageNoData->resize(this->size().width(), this->size().height());
		pageNoData->raise();
	}

	UpdateLayerGeometry();
	PLSFloatScrollBarScrollArea::resizeEvent(event);
}
