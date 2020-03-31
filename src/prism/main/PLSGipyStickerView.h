#ifndef PLSGIPYSTICKERVIEW_H
#define PLSGIPYSTICKERVIEW_H

#include "GiphyDefine.h"
#include "PLSFloatScrollBarScrollArea.h"
#include "combobox.hpp"
#include "frontend-api/dialog-view.hpp"
#include "loading-event.hpp"
#include "obs.hpp"
#include <QImage>
#include <QLabel>
#include <QLineEdit>
#include <QMovie>
#include <QMutex>
#include <QProcess>
#include <QThread>

namespace Ui {
class PLSGipyStickerView;
}

class FlowLayout;
class QButtonGroup;
class PLSFloatScrollBarScrollArea;
class ScrollAreaWithNoDataTip;
class FlowLayout;
class GiphyDownloader;
class GiphyWebHandler;
class MovieLabel;
class NoDataPage;
class PLSSearchPopupMenu;
class PLSToastMsgFrame;
class QToolButton;

const static QEvent::Type customType = (QEvent::Type)(QEvent::User + 1);
class AddStickerEvent : public QEvent {
public:
	explicit AddStickerEvent(const ResponData &dataIn) : QEvent(customType) { data = dataIn; }
	virtual ~AddStickerEvent() {}

	ResponData GetData() { return data; }

private:
	ResponData data;
};

class PLSGipyStickerView : public PLSDialogView {
	Q_OBJECT

	enum TabType { None = -1, RecentTab = 1, TrendingTab };

public:
	explicit PLSGipyStickerView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSGipyStickerView();

	void onMaxFullScreenStateChanged() override;
	void onSaveNormalGeometry() override;

	bool IsSignalConnected(const QMetaMethod &signal);

	static void OnPrismAppQuit(enum obs_frontend_event event, void *context);
	static PointerValue ConvertPointer(void *ptr);

	void SaveStickerJsonData();
	void SaveShowModeToConfig();
	void InitGeometry();
	void SetBindSourcePtr(PointerValue sourcePtr);
	void SetExitFlag(bool exit_) { exit = exit_; };

protected:
	virtual void hideEvent(QHideEvent *event) override;
	virtual void closeEvent(QCloseEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void showEvent(QShowEvent *event) override;
	virtual bool event(QEvent *event) override;

private:
	void Init();
	void InitCommonForList(ScrollAreaWithNoDataTip *scrollList);
	void InitMenuList();
	void InitRecentList();
	void InitTrendingList();
	void InitSearchResultList();
	void ShowTabList(int tabType);
	void StartWebHandler();
	void StartDownloader();
	QString GetRequestUrl(const QString &apiType, int limit, int offset) const;
	void ClearTrendingList();
	void ClearRecentList();
	void ClearSearchList();
	void ShowPageWidget(QWidget *widget);
	void AddHistorySearchItem(const QString &keyword);
	bool WriteRecentStickerDataToFile(const GiphyData &data, bool del = false);
	bool WriteSearchHistoryToFile(const QString &keyword, bool del = false);
	void WriteTabIndexToConfig(int index);
	bool LoadArrayData(const QString &key, QJsonArray &array);
	bool CreateJsonFile();
	void InitStickerJson();
	void AddRecentStickerList(const QJsonArray &array);
	void UpdateRecentList();
	void UpdateMenuList();
	void DeleteHistoryItem(const QString &key);

	void QuerySearch(const QString &key, int limit, int offset);
	void QueryTrending(int limit, int offset);
	void ShowSearchPage();
	void ShowErrorToast(const QString &tips);
	void UpdateErrorToastPos();
	void HideErrorToast();
	void AutoFillContent();
	void UpdateLimit();
	inline int GetColumCountByWidth(int width, int marginLeftRight);
	inline int GetRowCountByHeight(int height, int marginTopBottom);
	void UpdateLayoutSpacing(FlowLayout *layout, double dpi);
	int GetItemCountToFillContent(const QSize &sizeContent, int leftRightMargin, int topBottomMargin, int &rows, int &colums);
	void AutoFillSearchList(int pagenation);
	void AutoFillTrendingList(bool oneMoreRow = true);
	void GetSearchListGeometry(QSize &sizeContent, QPoint &position);
	void AddSearchItem(const GiphyData &data);
	void ShowRetryPage();
	bool IsNetworkAccessible();
	void retrySearchList();
	void retryTrendingList();
	void retryRencentList();
	void SetSearchData(const ResponData &data);

private slots:
	void OnTabClicked(int index);
	void requestRespon(const ResponData &data);
	void OnFetchError(const RequestTaskData &task, const RequestErrorInfo &errorInfo);
	void OnSearchTigger(const QString &keyword);
	void OnSearchMenuRequested(bool show);
	void on_btn_cancel_clicked();
	void OriginalDownload(const GiphyData &taskData, const QString &fileName, const QVariant &extra);
	void OriginalDownLoadFailed();
	void OnNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);
	void OnLoadingVisible(const RequestTaskData &task, bool visible);

signals:
	void request(RequestTaskData task);
	void StickerApply(const QString &file, const GiphyData &taskData, const QVariant &extra);
	void StickerApplyForProperty(const QString &fileName);
	void StickerApplyForAddingSource(const QString &fileName);
	void visibleChanged(bool visible);

private:
	Ui::PLSGipyStickerView *ui;
	QButtonGroup *btnGroup;
	ScrollAreaWithNoDataTip *recentScrollList{nullptr};
	ScrollAreaWithNoDataTip *trendingScrollList{nullptr};
	ScrollAreaWithNoDataTip *searchResultList{nullptr};

	FlowLayout *recentFlowLayout{nullptr};
	FlowLayout *trendingFlowLayout{nullptr};
	FlowLayout *searchResultFlowLayout{nullptr};

	PLSSearchPopupMenu *searchMenu{nullptr};
	PLSToastMsgFrame *networkErrorToast{nullptr};

	QThread threadRequest;
	GiphyDownloader *downloader;
	GiphyWebHandler *webHandler;
	QTimer timerAutoLoad;
	QTimer timerSearch;
	QTimer timerScrollSearch;
	QTimer timerScrollTrending;

	QList<QWidget *> listWidgets;
	QEventLoop eventLoop;
	TabType currentType{None};

	QNetworkAccessManager::NetworkAccessibility accessibility{QNetworkAccessManager::Accessible};

	bool showFromProperty{false};
	bool isSearchMode{false};
	bool needUpdateRecentList{false};
	bool needUpdateTrendingList{false};
	bool isFirstShow{true};
	bool isSearching{false};
	bool exit{false};
	int pageLimit{0};
	int lastSearchResponOffset{-1};
	bool stopProcess{false};

	QJsonObject jsonObjSticker;
	QString searchKeyword;

	QList<MovieLabel *> listTrending;
	QList<MovieLabel *> listRecent;
	QList<MovieLabel *> listSearch;
};

class ScrollAreaWithNoDataTip : public PLSFloatScrollBarScrollArea {
	Q_OBJECT

public:
	ScrollAreaWithNoDataTip(QWidget *parent = nullptr);
	~ScrollAreaWithNoDataTip() override;

	void ResetPagination();
	void SetPagintion(int page);
	int GetPagination() const;

	void ShowNoDataPage();
	void HideNoDataPage();
	void SetNoDataPageVisible(bool visible);
	void SetNoDataTipText(const QString &tipText);

	void SetShowRetryPage();
	void HideRetryPage();
	void SetRetryPageVisible(bool visible);
	void SetRetryTipText(const QString &tipText);
	bool IsNoDataPageVisible();

	void ShowLoading();
	void HideLoading();
	void SetLoadingVisible(bool visible);

private:
	void UpdateLayerGeometry();

signals:
	void retryClicked();

protected:
	virtual void resizeEvent(QResizeEvent *event) override;

private:
	NoDataPage *pageNoData{nullptr};
	QFrame *loadingLayer{nullptr};
	PLSLoadingEvent loadingEvent;
	int pagination{0};
};

class MovieLabel : public QLabel {
	Q_OBJECT
public:
	MovieLabel(QWidget *parent);
	~MovieLabel() override;

	void SetData(const GiphyData &taskData);
	void SetShowLoad(bool visible);

	void SetClicked(bool clicked);
	bool Clicked();
	static void DownloadCallback(const TaskResponData &result, void *param);
	void DownloadPreview();

private:
	void DownloadOriginal();
	void LoadFrames();
	void ResizeScale();
	void UpdateRect();

signals:
	void excuteTask(const DownloadTaskData &taskData);
	void Clicked(const GiphyData &taskData);
	void OriginalDownloaded(const GiphyData &taskData, const QString &fileName, const QVariant &extraData = QVariant());
	void DownLoadError();

protected:
	virtual void enterEvent(QEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *ev) override;
	virtual void resizeEvent(QResizeEvent *ev) override;
	virtual void timerEvent(QTimerEvent *event) override;
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void showEvent(QShowEvent *event) override;
	virtual void hideEvent(QHideEvent *event) override;

public slots:
	void downloadCallback(const TaskResponData &responData);

private:
	GiphyData giphyData;
	QLabel *labelLoad;
	QObject *downloadOriginalObj{nullptr};
	TaskResponData giphyFileInfo;
	bool clicked{false};
	QBasicTimer timer;
	int step{-1};
	int imageDelay{0};
	bool needRetry{true};
	std::vector<QPixmap> imageFrames;
	QSize displaySize;
	QSize scaledSize;
	QRect rectTarget;
	QPointer<PLSLoadingEvent> loadingEvent{nullptr};
};

class SearchLineEdit : public QLineEdit {
	Q_OBJECT
public:
	SearchLineEdit(QWidget *parent = nullptr);
	~SearchLineEdit() override;

signals:
	void SearchTrigger(const QString &key);
	void SearchMenuRequested(bool show);

protected:
	virtual void focusInEvent(QFocusEvent *e) override;
	virtual void focusOutEvent(QFocusEvent *e) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;

private:
	QToolButton *toolBtnSearch;
};

class NoDataPage : public QFrame {
	Q_OBJECT
public:
	enum PageType { Page_Nodata = 0, Page_NetworkError };

public:
	NoDataPage(QWidget *parent = nullptr);
	~NoDataPage() override;

	void SetPageType(int pageType);
	void SetNoDataTipText(const QString &tipText);

signals:
	void retryClicked();

private:
	QLabel *iconNoData{nullptr};
	QLabel *textNoDataTip{nullptr};
	QPushButton *btnRetry{nullptr};
	QLabel *spaceBottom{nullptr};
	int topPadding{0};
};

#endif // PLSGIPYSTICKERVIEW_H
