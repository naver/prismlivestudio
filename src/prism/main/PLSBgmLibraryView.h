#ifndef PLSBGMLIBRARYVIEW_H
#define PLSBGMLIBRARYVIEW_H

#include "PLSBgmDataManager.h"
#include "PLSToastMsgFrame.h"
#include "frontend-api/dialog-view.hpp"
#include "loading-event.hpp"
#include "obs.hpp"
#include <QPushButton>
#include <QTimer>

namespace Ui {
class PLSBgmLibraryView;
class PLSBgmLibraryItem;
}

class PLSBgmItemView;
class PLSBgmItemData;
class QLabel;
class CategoryButton;
class PLSFloatScrollBarScrollArea;

class PLSBgmLibraryItem : public QFrame {
	Q_OBJECT
public:
	enum class MusicPlayState { Playing, Pause, Loading, Disabled, Selected, Default };
	Q_ENUM(MusicPlayState)

	explicit PLSBgmLibraryItem(const PLSBgmItemData &data, QWidget *parent = nullptr);
	~PLSBgmLibraryItem();

	void SetData(const PLSBgmItemData &data);
	void SetDurations(const int &full, const int &fiftten, const int &thirty, const int &sixty);
	void SetDurationType(int type);
	void SetDurationTypeSelected(int type);
	void SetDurationTypeClicked(int type);
	void SetDurationTypeSelected(const QString &url);
	void SetCheckedButtonVisible(bool visible);
	void SetDurationButtonVisible();
	void SetGroup(const QString &group);

	inline bool IsCurrentSong(const QString &title, int id) const;
	const PLSBgmItemData &GetData() const;
	QString GetTitle() const;
	QString GetProducer() const;
	QString GetGroup() const;
	QString GetUrl(int type) const;
	int GetDurationType() const;
	int GetDuration(int type) const;
	bool ContainUrl(const QString &url);

	static QString GetNameElideString(const QString &name, QLabel *label);

protected:
	virtual void leaveEvent(QEvent *event) override;
	virtual void enterEvent(QEvent *event) override;
	virtual bool eventFilter(QObject *object, QEvent *event) override;

private:
	void OnMouseEventChanged(const QString &state);
	int GetUrlCount();

private slots:
	void on_listenBtn_clicked();
	void on_checkBtn_clicked();
	void on_addBtn_clicked();
	void on_fifteenButton_clicked(bool checked);
	void on_thirtyButton_clicked(bool checked);
	void on_sixtyButton_clicked(bool checked);
	void on_fullButton_clicked(bool checked);

public slots:
	void onMediaStateChanged(const QString &url, obs_media_state state);

signals:
	void ListenButtonClickedSignal(PLSBgmLibraryItem *item);
	void AddButtonClickedSignal(PLSBgmLibraryItem *item);
	void CheckedButtonClickedSignal(PLSBgmLibraryItem *item);
	void DurationTypeChangeSignal(PLSBgmLibraryItem *item);

private:
	Ui::PLSBgmLibraryItem *ui;
	PLSBgmItemData data;
	bool mouseEnter{false};
	bool isChecked{false};
	int selectedCount = 0;
	PLSLoadingEvent loadingEvent;
};

class PLSBgmLibraryView : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSBgmLibraryView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSBgmLibraryView();
	void initGroup();

protected:
	virtual void resizeEvent(QResizeEvent *event) override;
	virtual void showEvent(QShowEvent *event) override;

private slots:
	void OnListenButtonClicked(PLSBgmLibraryItem *item);
	void OnAddButtonClicked(PLSBgmLibraryItem *item);
	void OnCheckedButtonClicked(PLSBgmLibraryItem *item);
	void OnDurationTypeChanged(PLSBgmLibraryItem *item);
	void OnOkButtonClicked();
	void OnCancelButtonClicked();
	void OnPreGroupButtonClicked();
	void OnNextGroupButtonClicked();
	void OnGroupScrolled();
	void OnGroupButtonClicked(const QString &group);

	static void MediaStateChanged(void *data, calldata_t *calldata);
	static void MediaLoad(void *data, calldata_t *calldata);

	void OnMediaStateChanged(const QString &url, obs_media_state state);
	void OnMediaLoad(const QString &url, bool load);
	void OnNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);
	void OnNetWorkStateChanged(bool isConnected);

private:
	PLSBgmLibraryItem *CreateLibraryBgmItemView(const PLSBgmItemData &data, QWidget *parent = nullptr);
	void CreateListenSource();
	void CreateGroup(const QString &groupName);
	void ScrollToCategoryButton(QPushButton *categoryButton);
	void RefreshSelectedGroupButtonStyle(const QString &groupName);

	// toast
	void initToast();
	void ShowToastView(const QString &text);
	void ResizeToastView();
	void ShowList(const QString &group);
	void CreateMusicList(const QString &group, const BgmLibraryData &listData);
	void UpdateMusiclist(const QString &group);
	void InitButtonState(const QString &group);
	void InitCategoryData();
	void UpdateSelectedString();

signals:
	void AddCachePlayList(const QVector<PLSBgmItemData> &datas);
	void signal_meida_state_changed(const QString &url, obs_media_state state);

private:
	Ui::PLSBgmLibraryView *ui;
	QString selectedGroup{};
	QString listenUrl{};

	bool initGroupFinish{false};
	bool firstListen{true};

	OBSSource sourceAudition = nullptr;
	PLSToastMsgFrame toastView;

	QList<CategoryButton *> listCatgaoryButton;
	std::map<QString, PLSFloatScrollBarScrollArea *> groupWidget;            //key category name
	std::map<QString, std::map<QString, PLSBgmLibraryItem *>> musicListItem; //key category name - value music item

	QNetworkAccessManager networkMonitor;
	bool networkAvailable = true;
};

class CategoryButton : public QPushButton {
	Q_OBJECT
public:
	explicit CategoryButton(QWidget *parent = nullptr) : QPushButton(parent)
	{
		PLSDpiHelper dpiHelper;
		dpiHelper.notifyDpiChanged(this, [=](double dpi) { UpdateSize(dpi); });
	}
	virtual ~CategoryButton(){};

	void SetDisplayText(const QString &text)
	{
		QPushButton::setText(text);
		QTimer::singleShot(0, this, [=]() {
			PLSDpiHelper dpiHelper;
			double dpi = dpiHelper.getDpi(this);
			UpdateSize(dpi);
		});
	}

private:
	void UpdateSize(double dpi)
	{
		int textWidth = this->fontMetrics().boundingRect(text()).width();
		this->setFixedWidth(PLSDpiHelper::calculate(dpi, 30) + textWidth);
	}
};

#endif // PLSBGMLIBRARYVIEW_H
