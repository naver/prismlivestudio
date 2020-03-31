#ifndef PLSSCENEITEMVIEW_H
#define PLSSCENEITEMVIEW_H

#include "qt-display.hpp"

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <iostream>
#include <vector>
#include <QResizeEvent>

template<typename PLSRef> struct SignalContainer {
	PLSRef ref;
	std::vector<std::shared_ptr<OBSSignal>> handlers;
};

namespace Ui {
class PLSSceneItemView;
}

class PLSSceneItemView;
class PLSSceneDisplay : public PLSQTDisplay {
	Q_OBJECT
public:
	explicit PLSSceneDisplay(QWidget *parent = nullptr);
	~PLSSceneDisplay();
	void SetRenderFlag(bool state);
	void SetCurrentFlag(bool state);
	void SetSceneData(OBSScene scene);
	void SetDragingState(bool state);
	bool GetRenderState() const;
	void visibleSlot(bool visible) override;

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

private:
	static void RenderScene(void *data, uint32_t cx, uint32_t cy);

private slots:
	void OnRenderChanged(bool state);
	void OnDisplayCreated();
	void CaptureImageFinished(const QString &path);

private:
	void AddRenderCallback();
	void RemoveRenderCallback();

signals:
	void MouseLeftButtonClicked();
	void RenderChanged(bool state);
	void CaptureImageFinishedSignal(const QString &path);

private:
	bool ready{false};
	bool render{false};
	bool current{false};
	bool isDraging{false};
	bool toplevelVisible{true};

	OBSScene scene;
	PLSSceneItemView *parent{};
	QPoint startPos{};
};

class PLSSceneItemView : public QFrame {
	Q_OBJECT
public:
	explicit PLSSceneItemView(const QString &name, OBSScene scene, QWidget *parent = nullptr);
	~PLSSceneItemView();
	void SetData(OBSScene scene);
	void SetSignalHandler(SignalContainer<OBSScene> handler);
	void SetCurrentFlag(bool state = true);
	void SetName(const QString &name);
	void SetRenderFlag(bool state);

	OBSScene GetData();
	QString GetName();
	SignalContainer<OBSScene> GetSignalHandler();
	bool GetCurrentFlag();

public slots:
	void OnRenameOperation();

protected:
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
	virtual bool eventFilter(QObject *object, QEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;
	virtual void enterEvent(QEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;

private slots:
	void OnCurrentItemChanged(bool state);
	void OnMouseButtonClicked();
	void OnModifyButtonClicked();
	void OnDeleteButtonClicked();
	void OnMouseEnterEvent();
	void OnMouseLeaveEvent();
	void OnFinishingEditName();
	void SetContentMargins(bool state);
	void OnCaptureImageFinished(const QString &path);

private:
	void setEnterPropertyState(bool state, QWidget *widget);
	void CreateDrag(const QPoint &startPos, const QString &path);
	QString GetNameElideString();

signals:
	void CurrentItemChanged(bool state);
	void MouseButtonClicked(PLSSceneItemView *item);
	void ModifyButtonClicked(PLSSceneItemView *item);
	void DeleteButtonClicked(PLSSceneItemView *item);
	void FinishingEditName(const QString &name, PLSSceneItemView *item);

private:
	Ui::PLSSceneItemView *ui;
	QPushButton *deleteBtn{};
	OBSScene scene;
	QString name;
	bool current{false};
	bool isFinishEditing{true};
	SignalContainer<OBSScene> handler;
	QPoint startPos;
};

#endif // PLSSCENEITEMVIEW_H
