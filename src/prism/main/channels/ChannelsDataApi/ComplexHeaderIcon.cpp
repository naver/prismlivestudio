#include "ComplexHeaderIcon.h"
#include <QImageReader>
#include <QPaintEvent>
#include <qDebug>
#include "ChannelCommonFunctions.h"
#include "PLSDpiHelper.h"

static void sharpPix(QPixmap &pix, bool isBigPix, bool sharp)
{
	if (!sharp || pix.isNull()) {
		return;
	}

	if (isBigPix && pix.width() != pix.height()) {
		pix = getCubePix(pix);
	}

	QSize pixSize = pix.size();

	QPixmap image(pixSize);
	image.fill(Qt::transparent);

	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);

	QPainterPath path;
	path.addRoundedRect(image.rect(), pixSize.width() / 2.0, pixSize.height() / 2.0);
	painter.setClipPath(path);

	painter.drawPixmap(image.rect(), pix);

	pix = image;
}

ComplexHeaderIcon::ComplexHeaderIcon(QWidget *parent) : QLabel(parent)
{
	PLSDpiHelper dpiHelper;

	dpiHelper.notifyDpiChanged(this, [this](double dpi) {
		loadPixmap(mBigPix, mPixPath, PLSDpiHelper::calculate(dpi, mPixSize));
		sharpPix(mBigPix, true, mPixSharp);
	});

	dpiHelper.notifyDpiChanged(this, [this](double dpi) {
		loadPixmap(mSmallPix, mPlatPixPath, PLSDpiHelper::calculate(dpi, mPlatPixSize));
		sharpPix(mSmallPix, false, mPlatPixSharp);
	});
}

ComplexHeaderIcon::~ComplexHeaderIcon() {}

void ComplexHeaderIcon::setPixmap(const QString &pix, const QSize &pixSize, bool sharp)
{
	mPixPath = pix;
	mPixSize = pixSize;
	mPixSharp = sharp;
	PLSDpiHelper::dpiDynamicUpdate(this);
}

void ComplexHeaderIcon::setPlatformPixmap(const QString &pix, const QSize &pixSize, bool sharp)
{
	mPlatPixPath = pix;
	mPlatPixSize = pixSize;
	mPlatPixSharp = sharp;
	PLSDpiHelper::dpiDynamicUpdate(this);
}

void ComplexHeaderIcon::setActive(bool isActive)
{
	mActive = isActive;
	refreshStyle(this);
}

void ComplexHeaderIcon::setUseContentsRect(bool isUseContentsRect)
{
	mUseContentsRect = isUseContentsRect;
	update();
}

void ComplexHeaderIcon::setPadding(int padding)
{
	mPadding = padding;
	update();
}

void ComplexHeaderIcon::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setBrush(Qt::transparent);
	painter.setRenderHint(QPainter::Antialiasing);
	double dpi = PLSDpiHelper::getDpi(this);
	auto rect = mUseContentsRect ? this->contentsRect() : this->rect();
	auto rec = rect.adjusted(PLSDpiHelper::calculate(dpi, mPadding), PLSDpiHelper::calculate(dpi, mPadding), PLSDpiHelper::calculate(dpi, -mPadding), PLSDpiHelper::calculate(dpi, -mPadding));
	if (!mBigPix.isNull()) {
		painter.save();
		auto tmp = mBigPix.scaled(rec.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		painter.drawPixmap(rec, tmp);
		if (!this->isEnabled()) {
			QBrush brush(QColor(10, 10, 10, 50));
			painter.setBrush(brush);
			painter.drawEllipse(rec);
		}
		painter.restore();
	}
	if (!mSmallPix.isNull()) {
		auto picSize = mSmallPix.size();
		QRect imagerect(rec.bottomRight() - QPoint(picSize.width() - PLSDpiHelper::calculate(dpi, 6), picSize.height()), picSize);
		auto tmp = mSmallPix.scaled(imagerect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		painter.drawPixmap(imagerect, tmp);
		if (!this->isEnabled()) {
			QBrush brush(QColor(10, 10, 10, 50));
			painter.setBrush(brush);
			painter.drawEllipse(imagerect);
		}
	}
}
