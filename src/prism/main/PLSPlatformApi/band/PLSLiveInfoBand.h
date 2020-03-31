#ifndef PLSLIVEINFOBAND_H
#define PLSLIVEINFOBAND_H

#include "..\PLSLiveInfoBase.h"
#include <qevent.h>

class PLSPlatformBand;
namespace Ui {
class PLSLiveInfoBand;
}

class PLSLiveInfoBand : public PLSLiveInfoBase {
	Q_OBJECT

public:
	explicit PLSLiveInfoBand(PLSPlatformBase *pPlatformBase, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSLiveInfoBand();
	QString getDescription() const;
	bool isModify();

private slots:
	void okButtonClicked();
	void cancelButtonClicked();
	void textChangeHandler();

private:
	Ui::PLSLiveInfoBand *ui;
	QString m_uuid;
	QString m_description;
	PLSPlatformBand *platform;
	bool m_isModify;
};

#endif // PLSLIVEINFOBAND_H
