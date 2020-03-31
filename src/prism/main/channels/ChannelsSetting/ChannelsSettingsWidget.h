#ifndef CHANNELSSETTINGSWIDGET_H
#define CHANNELSSETTINGSWIDGET_H
#include <QSharedPointer>
#include <QStyledItemDelegate>
#include <QVariantMap>
#include <QWidget>
#include "PLSChannelDataAPI.h"
#include "PLSWidgetDpiAdapter.hpp"
#include "dialog-view.hpp"

namespace Ui {
class ChannelsSettingsWidget;
}

using WidgetMap = QMap<QString, QWidget *>;

class ChannelsSettingsWidget : public PLSDialogView {
	Q_OBJECT

public:
	explicit ChannelsSettingsWidget(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~ChannelsSettingsWidget();

	void setChannelsData(const ChannelsMap &datas, const QString &platform = "");

	void setChannelsData(const ChannelsMap &datas, int Index = 0);

	void initializePlatforms(const QStringList &platforms);

	enum ItemDataRole { ChannelItemData = Qt::UserRole + 1 };

public slots:
	void on_ChannelsListCombox_currentIndexChanged(const QString &arg1);
	void onSelectionChanged(const QString &uuid, bool isSelected);
	void on_ApplySettingsBtn_clicked();
	void on_Cancel_clicked();
	void on_GotoLoginBtn_clicked();
	void setGuidePageInfo(const QString &platfom);

protected:
	void changeEvent(QEvent *e) override;
	void closeEvent(QCloseEvent *event) override;

private:
	bool hasSelected();
	void applyChanges();

private:
	Ui::ChannelsSettingsWidget *ui;
	ChannelsMap mLastChannelsInfo;
	ChannelsMap mOriginalInfo;
};

class GeometryDelegate : public QStyledItemDelegate {
	Q_OBJECT
public:
	GeometryDelegate(QWidget *parent = nullptr) {}

	virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override
	{
		auto itemRec = option.rect;
		auto itemCenter = itemRec.center();
		auto editorRec = editor->geometry();
		editorRec.moveCenter(itemCenter);
		editor->setGeometry(editorRec);
	}
};

#endif // CHANNELSSETTINGSWIDGET_H
