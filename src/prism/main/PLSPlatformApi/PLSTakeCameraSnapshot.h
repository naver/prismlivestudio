#ifndef PLSTAKECAMERASNAPSHOT_H
#define PLSTAKECAMERASNAPSHOT_H

#include <QHBoxLayout>

#include "dialog-view.hpp"
#include "qt-display.hpp"

namespace Ui {
class PLSTakeCameraSnapshot;
}

class PLSTakeCameraSnapshot;

class PLSTakeCameraSnapshotTakeTimerMask : public QDialog {
	Q_OBJECT

public:
	PLSTakeCameraSnapshotTakeTimerMask(QWidget *parent);
	virtual ~PLSTakeCameraSnapshotTakeTimerMask();

signals:
	void timeout();

public:
	void start(int ctime = 3);

protected:
	bool event(QEvent *event) override;

private:
	QLabel *maskLabel;
};

class PLSTakeCameraSnapshot : public PLSDialogView {
	Q_OBJECT

public:
	PLSTakeCameraSnapshot(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSTakeCameraSnapshot();

public:
	QString getSnapshot();
	static QList<QPair<QString, QString>> getCameraList();

private:
	void init(const QString &camera);
	void ClearSource();
	static void drawPreview(void *data, uint32_t cx, uint32_t cy);
	static void onSourceCaptureState(void *data, calldata_t *calldata);
	static void onSourceImageStatus(void *data, calldata_t *calldata);

private slots:
	void on_cameraList_currentIndexChanged(int index);
	void on_cameraSettingButton_clicked();
	void on_okButton_clicked();
	void on_cancelButton_clicked();
	void onTimerEnd();
	void onSourceCaptureState();
	void onSourceImageStatus(bool status);

protected:
	bool event(QEvent *event) override;
	bool eventFilter(QObject *watched, QEvent *event) override;

private:
	Ui::PLSTakeCameraSnapshot *ui;
	QFrame *cameraUninitMask = nullptr;
	QLabel *cameraUninitMaskText = nullptr;
	PLSTakeCameraSnapshotTakeTimerMask *timerMask = nullptr;
	bool hasError = false;
	bool captureImage = false;
	OBSSource source;
	QString imageFilePath;
	QString errorMessage;
};

#endif // PLSTAKECAMERASNAPSHOT_H
