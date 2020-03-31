#ifndef PLSLIVEINFOFACEBOOK_H
#define PLSLIVEINFOFACEBOOK_H

#include "..\PLSLiveInfoBase.h"
#include "PLSAPIFacebook.h"

enum class PLSLiveInfoFacebookErrorType {
	PLSLiveInfoFacebookGroupError,
	PLSLiveInfoFacebookPageError,
	PLSLiveInfoFacebookSearchGameError,
};

QT_BEGIN_NAMESPACE
namespace Ui {
class PLSLiveInfoFacebook;
}
QT_END_NAMESPACE

class PLSLiveInfoFacebookListWidget : public PLSWidgetDpiAdapterHelper<QListWidget> {
	Q_OBJECT
public:
	PLSLiveInfoFacebookListWidget(QWidget *parent = nullptr);
	~PLSLiveInfoFacebookListWidget();

protected:
	virtual bool needQtProcessDpiChangedEvent() const;
};

class PLSLiveInfoFacebook : public PLSLiveInfoBase {

	Q_OBJECT

public:
	explicit PLSLiveInfoFacebook(PLSPlatformBase *pPlatformBase, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSLiveInfoFacebook();
	void handleRequestFunctionType(PLSAPIFacebookType type);

private slots:
	void on_cancelButton_clicked();
	void on_okButton_clicked();

private:
	void initComboBoxList();
	void initLineEdit();
	void updateSecondComboBoxTitle();
	void onClickGroupComboBox();
	void onClickPageComboBox();
	void searchGameKeyword(const QString keyword);
	void hideSearchGameList();
	void showSearchGameList();
	void doUpdateOkState();
	void startLiving();
	bool isModified();
	void initPlaceTextHorderColor(QWidget *widget);
	void setupGameListCornerRadius();
	void showNetworkErrorAlert(PLSLiveInfoFacebookErrorType errorType);

protected:
	void showEvent(QShowEvent *event) override;

private:
	Ui::PLSLiveInfoFacebook *ui;
	QFrame *m_frame;
	QListWidget *m_gameListWidget;
	QList<QString> m_expiredObjectList;
	bool m_startLivingApi;
};
#endif // PLSLIVEINFOFACEBOOK_H
