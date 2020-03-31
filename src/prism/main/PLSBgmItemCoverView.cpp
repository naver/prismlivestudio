#include "PLSBgmItemCoverView.h"
#include "ui_PLSBgmItemCoverView.h"

#include <QPainter>
#include <QPen>
#include <QMovie>
#include <QGraphicsBlurEffect>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>

#include "liblog.h"
#include "log/module_names.h"
#include "action.h"
#include "PLSDpiHelper.h"
#include "pls-common-define.hpp"
#include "PLSScrollingLabel.h"

PLSBgmItemCoverView::PLSBgmItemCoverView(QWidget *parent) : QFrame(parent), ui(new Ui::PLSBgmItemCoverView)
{
	ui->setupUi(this);

	blurEffect = new QGraphicsBlurEffect(ui->backgroundLabel);
	double radius = 90 * this->width() / PLSDpiHelper::calculate(this, 300);
	blurEffect->setBlurRadius(90 * this->width() / PLSDpiHelper::calculate(this, 300));

	opacityEffect = new QGraphicsOpacityEffect(this);
	opacityEffect->setOpacity(0.4);

	ui->backgroundLabel->setGraphicsEffect(opacityEffect);
	ui->backgroundLabel->setGraphicsEffect(blurEffect);

	PLSDpiHelper dpiHelper;
	imageLabel = new PLSBgmItemCoverImage(this);
	imageLabel->SetRoundedRect(true);

	QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
	shadowEffect->setOffset(0, 0);
	shadowEffect->setColor(QColor(0, 0, 0, 128));
	shadowEffect->setBlurRadius(PLSDpiHelper::calculate(this, 5));
	QGraphicsOpacityEffect *opacityEffect_ = new QGraphicsOpacityEffect(this);
	imageLabel->setGraphicsEffect(opacityEffect_);
	opacityEffect_->setOpacity(0.2);
	imageLabel->setGraphicsEffect(shadowEffect);
	imageLabel->setObjectName("imageLabel");

	titleFrame = new QFrame(this);
	titleFrame->setObjectName("titleFrame");
	QHBoxLayout *hTitleLayout = new QHBoxLayout(titleFrame);
	hTitleLayout->setContentsMargins(20, 0, 20, 0);

	titleLabel = new PLSScrollingLabel(this);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setObjectName("titleLabel");
	hTitleLayout->addWidget(titleLabel);

	producerFrame = new QFrame(this);
	producerFrame->setObjectName("producerFrame");
	QHBoxLayout *hProducerLayout = new QHBoxLayout(producerFrame);
	hProducerLayout->setContentsMargins(20, 0, 20, 0);

	producerLabel = new PLSScrollingLabel(this);
	producerLabel->setAlignment(Qt::AlignCenter);
	producerLabel->setObjectName("producerLabel");
	hProducerLayout->addWidget(producerLabel);
	QGraphicsOpacityEffect *proOpacityEffect = new QGraphicsOpacityEffect(this);
	producerLabel->setGraphicsEffect(proOpacityEffect);
	proOpacityEffect->setOpacity(0.6);

	playingLabel = new QLabel(imageLabel);
	playingLabel->hide();
	playingLabel->setObjectName("playingLabel");

	movie = new QMovie(BGM_MUSIC_PLAYING_GIF);
	movie->setScaledSize(QSize(PLSDpiHelper::calculate(this, 16), PLSDpiHelper::calculate(this, 16)));
	playingLabel->setMovie(movie);
}

PLSBgmItemCoverView::~PLSBgmItemCoverView()
{
	if (movie) {
		movie->deleteLater();
		movie = nullptr;
	}
	delete ui;
}

void PLSBgmItemCoverView::SetMusicInfo(const QString &title, const QString &producer)
{
	if (!titleLabel || !producerLabel) {
		return;
	}

	titleLabel->SetText(title);
	producerLabel->SetText(producer);
}

void PLSBgmItemCoverView::SetCoverPath(const QString &coverPath, bool isFreeMusic)
{
	if (coverPath.isEmpty()) {
		return;
	}

	this->coverPath = coverPath;
	QPixmap pixmap(coverPath);
	ui->backgroundLabel->SetPixmap(pixmap);
	if (imageLabel) {
		imageLabel->SetPixmap(pixmap);
	}
	ResizeUI();
}

void PLSBgmItemCoverView::SetImage(const QImage &image)
{
	ui->backgroundLabel->SetPixmap(QPixmap::fromImage(image));
	if (imageLabel) {
		imageLabel->SetPixmap(QPixmap::fromImage(image));
	}
	ResizeUI();
}

void PLSBgmItemCoverView::ShowPlayingGif(bool show)
{
	if (!movie || !playingLabel) {
		return;
	}
	show ? movie->start() : movie->stop();
	playingLabel->setVisible(show);

	QMetaObject::invokeMethod(
		this,
		[=]() {
			if (imageLabel) {
				int margin = PLSDpiHelper::calculate(this, 7);
				playingLabel->setGeometry(margin, imageLabel->height() - margin - playingLabel->height(), playingLabel->width(), playingLabel->height());
			}
		},
		Qt::QueuedConnection);
}

void PLSBgmItemCoverView::DpiChanged(double dpi)
{
	if (movie) {
		movie->setScaledSize(QSize(PLSDpiHelper::calculate(this, 16), PLSDpiHelper::calculate(this, 16)));
	}
}

void PLSBgmItemCoverView::resizeEvent(QResizeEvent *event)
{
	QMetaObject::invokeMethod(this, "ResizeUI", Qt::QueuedConnection);
	QFrame::resizeEvent(event);
}

void PLSBgmItemCoverView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		if (event->pos().x() < PLSDpiHelper::calculate(this, 10) || event->pos().x() > width() - PLSDpiHelper::calculate(this, 10)) {
			QFrame::mousePressEvent(event);
			return;
		}
		startPoint = event->pos();
		mousePressed = true;
	}
}

void PLSBgmItemCoverView::mouseMoveEvent(QMouseEvent *event)
{
	QFrame::mouseMoveEvent(event);
	if (mousePressed) {
		emit CoverPressed(event->pos() - startPoint);
	}
}

void PLSBgmItemCoverView::mouseReleaseEvent(QMouseEvent *event)
{
	mousePressed = false;
	QFrame::mouseReleaseEvent(event);
}

void PLSBgmItemCoverView::ResizeUI()
{
	if (blurEffect) {
		double radius = 90 * this->width() / PLSDpiHelper::calculate(this, 300);
		blurEffect->setBlurRadius(90 * this->width() / PLSDpiHelper::calculate(this, 300));
	}

	if (imageLabel) {
		imageLabel->setGeometry((this->width() - imageLabel->width()) / 2, PLSDpiHelper::calculate(this, 35), imageLabel->width(), imageLabel->height());
	}
	if (titleFrame) {
		titleFrame->setGeometry(0, PLSDpiHelper::calculate(this, 180), this->width(), titleLabel->height());
	}
	if (producerFrame) {
		producerFrame->setGeometry(0, PLSDpiHelper::calculate(this, 209), this->width(), producerLabel->height());
	}
	if (playingLabel) {
		QMetaObject::invokeMethod(
			this,
			[=]() {
				if (imageLabel) {
					int margin = PLSDpiHelper::calculate(this, 7);
					playingLabel->setGeometry(margin, imageLabel->height() - margin - playingLabel->height(), playingLabel->width(), playingLabel->height());
				}
			},
			Qt::QueuedConnection);
	}
}

PLSBgmItemCoverImage::PLSBgmItemCoverImage(QWidget *parent) : QLabel(parent) {}

void PLSBgmItemCoverImage::SetPixmap(const QPixmap &pixmap)
{
	this->pixmap = pixmap;
	update();
}

void PLSBgmItemCoverImage::SetRoundedRect(bool set)
{
	setRoundedRect = set;
}

void PLSBgmItemCoverImage::paintEvent(QPaintEvent *event)
{
	QFrame::paintEvent(event);
	QPainter painter(this);
	double dpi = PLSDpiHelper::getDpi(this);

	if (!pixmap.isNull()) {
		painter.setRenderHints(QPainter::Antialiasing, true);
		if (setRoundedRect) {
			QPainterPath painterPath;
			painterPath.addRoundedRect(this->rect(), PLSDpiHelper::calculate(dpi, 3.0), PLSDpiHelper::calculate(dpi, 3.0));
			painter.setClipPath(painterPath);
			painter.drawPixmap(painterPath.boundingRect().toRect(), pixmap.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			painter.setPen(Qt::NoPen);
			painter.drawPath(painterPath);
		} else {
			painter.drawPixmap(this->rect(), pixmap);
		}
	}
}

void PLSBgmItemCoverImage::resizeEvent(QResizeEvent *event)
{
	QLabel::resizeEvent(event);
}
