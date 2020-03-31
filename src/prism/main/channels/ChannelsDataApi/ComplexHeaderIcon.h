#ifndef COMPLEXHEADERICON_H
#define COMPLEXHEADERICON_H

#include <QLabel>

/* this is a complex UI for user profile header */
class ComplexHeaderIcon : public QLabel {
	Q_OBJECT
	Q_PROPERTY(bool Active READ isActive WRITE setActive)
public:
	explicit ComplexHeaderIcon(QWidget *parent = nullptr);
	~ComplexHeaderIcon() override;

	void setPixmap(const QString &pix, const QSize &pixSize, bool sharp = true);
	void setPlatformPixmap(const QString &pix, const QSize &pixSize, bool sharp = true);
	void setActive(bool isActive = true);
	bool isActive() { return mActive; }
	void setUseContentsRect(bool isUseContentsRect = true);
	bool isUseContentsRect() { return mUseContentsRect; }
	void setPadding(int padding);
	int getPadding() { return mPadding; }

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	QString mPixPath;
	QString mPlatPixPath;
	QSize mPixSize;
	QSize mPlatPixSize;
	bool mPixSharp = true;
	bool mPlatPixSharp = true;
	QPixmap mBigPix;
	QPixmap mSmallPix;
	bool mActive = false;
	bool mUseContentsRect = false;
	int mPadding = 6;
};

#endif // COMPLEXHEADERICON_H
