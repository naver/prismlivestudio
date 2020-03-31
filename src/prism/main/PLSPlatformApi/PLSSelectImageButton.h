#ifndef PLSSELECTIMAGEBUTTON_H
#define PLSSELECTIMAGEBUTTON_H

#include <QLabel>

#include "PLSDpiHelper.h"

namespace Ui {
class PLSSelectImageButton;
}

class PLSSelectImageButton : public QLabel {
	Q_OBJECT

public:
	explicit PLSSelectImageButton(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSSelectImageButton();

public:
	QString getImagePath() const;
	void setImagePath(const QString &imagePath);
	void setPixmap(const QPixmap &pixmap, const QSize &size = QSize());

	void setEnabled(bool enabled);

signals:
	void takeButtonClicked();
	void selectButtonClicked();
	void imageSelected(const QString &imageFilePath);

private slots:
	void on_takeButton_clicked();
	void on_selectButton_clicked();

protected:
	virtual bool event(QEvent *event);
	virtual bool eventFilter(QObject *watched, QEvent *event);

private:
	Ui::PLSSelectImageButton *ui;
	QLabel *icon;
	bool mouseHover = false;
	QString imagePath;
};

#endif // PLSSELECTIMAGEBUTTON_H
