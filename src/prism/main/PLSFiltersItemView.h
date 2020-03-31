#ifndef PLSFILTERSITEMVIEW_H
#define PLSFILTERSITEMVIEW_H

#include <QFrame>
#include "obs.hpp"

namespace Ui {
class PLSFiltersItemView;
}

class PLSFiltersItemView : public QFrame {
	Q_OBJECT

public:
	explicit PLSFiltersItemView(obs_source_t *source, QWidget *parent = nullptr);
	~PLSFiltersItemView();
	OBSSource GetFilter();
	bool GetCurrentState();
	void SetCurrentItemState(bool state);
	void OnRenameActionTriggered();
	void SetText(const QString &text);

protected:
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void enterEvent(QEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;
	virtual bool eventFilter(QObject *object, QEvent *event) override;
	virtual void contextMenuEvent(QContextMenuEvent *event) override;

private:
	static void OBSSourceEnabled(void *param, calldata_t *data);
	static void OBSSourceRenamed(void *param, calldata_t *data);
	QString GetNameElideString();
	void OnMouseStatusChanged(const QString &status);
	void OnFinishingEditName();
	void CreatePopupMenu();
	void UpdateNameStyle();
	void SetProperty(QWidget *widget, const char *property, const QVariant &value);

private slots:
	void OnVisibilityButtonClicked(bool visible);
	void OnAdvButtonClicked();
	void OnRemoveActionTriggered();
	void SourceEnabled(bool enabled);
	void SourceRenamed(QString name);

signals:
	void FilterRenameTriggered(PLSFiltersItemView *item);
	void FilterRemoveTriggered(PLSFiltersItemView *item);
	void FinishingEditName(const QString &name, PLSFiltersItemView *item);
	void CurrentItemChanged(PLSFiltersItemView *item);

private:
	Ui::PLSFiltersItemView *ui;
	OBSSource source;
	QString name;
	bool current{false};
	bool isFinishEditing{true};

	OBSSignal enabledSignal;
	OBSSignal renamedSignal;
};

#endif // PLSFILTERSITEMVIEW_H
