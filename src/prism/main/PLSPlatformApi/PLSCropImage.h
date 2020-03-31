#ifndef PLSCROPIMAGE_H
#define PLSCROPIMAGE_H

#include <QLabel>
#include <QPixmap>

#include "dialog-view.hpp"

namespace Ui {
class PLSCropImage;
}

class PLSCropImage : public PLSDialogView {
	Q_OBJECT

public:
	enum Button { Ok = 0x1000, Cancel = 0x2000, Back = 0x4000 };

private:
	explicit PLSCropImage(const QString &imageFilePath, const QSize &cropImageSize, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSCropImage(const QPixmap &originalImage, const QSize &cropImageSize, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSCropImage();

	void setButtons(int buttons);

public:
	static int cropImage(QPixmap &cropedImage, const QString &imageFilePath, const QSize &cropImageSize, int buttons, QWidget *parent = nullptr);
	static int cropImage(QPixmap &cropedImage, const QPixmap &originalImage, const QSize &cropImageSize, int buttons, QWidget *parent = nullptr);

private slots:
	void on_slider_valueChanged(int value);
	void on_smallButton_clicked();
	void on_largeButton_clicked();
	void on_backButton_clicked();
	void on_okButton_clicked();
	void on_cancelButton_clicked();

protected:
	bool eventFilter(QObject *watched, QEvent *event) override;

private:
	Ui::PLSCropImage *ui;
	const QSize cropImageSize;
	QPixmap originalImage;
	QPixmap cropedImage;
	QLabel *image;
	QFrame *middle;
	bool dragingImage = false;
	QPoint dragStartPos;
	double minScaleFactor = 0.01;
	double maxScaleFactor = 3.0;
	double curScaleFactor = 1;
};

#endif // PLSCROPIMAGE_H
