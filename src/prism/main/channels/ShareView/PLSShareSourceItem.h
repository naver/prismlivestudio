#ifndef SHARESOURCEITEM_H
#define SHARESOURCEITEM_H

#include <QFrame>
#include "PLSWidgetDpiAdapter.hpp"
#include "ui_PLSShareSourceItem.h"

namespace Ui {
class ShareSourceItem;
}

class PLSShareSourceItem : public PLSWidgetDpiAdapterHelper<QFrame> {
	Q_OBJECT

public:
	/*  parent must't be NULL  */
	explicit PLSShareSourceItem(QWidget *parent);
	~PLSShareSourceItem();
	/*  for initialize data and children views  */
	void initInfo(const QVariantMap &source);

signals:
	/*  used by parent obj  */
	void UrlCopied();

public slots:
	/*  copy to clipboard  */
	void onCopyPressed();

	void openUrl();

	void on_CloseBtn_clicked();

protected:
	void changeEvent(QEvent *e) override;

private:
	/*  update cache header image  */
	void updatePixmap();

private:
	QVariantMap mSource;
	Ui::ShareSourceItem *ui;
};

#endif // SHARESOURCEITEM_H
