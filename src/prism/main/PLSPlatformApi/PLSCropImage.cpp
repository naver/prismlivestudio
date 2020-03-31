#include "PLSCropImage.h"
#include "ui_PLSCropImage.h"

#include "ChannelCommonFunctions.h"

static const double SCALE_FACTOR = 1000.0;

template<typename... T> static void setTranslucent(T... w)
{
	QFrame *ws[] = {w...};
	for (int i = 0; i < sizeof...(T); ++i) {
		ws[i]->setFrameShape(QFrame::NoFrame);
		ws[i]->setStyleSheet("background-color: rgba(0, 0, 0, 0.5)");
		ws[i]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	}
}
static int leastEqual(int value, int least)
{
	return (value >= least) ? value : least;
}
static int mostEqual(int value, int most)
{
	return (value <= most) ? value : most;
}
static QRect calcImageRect(const QSize &previewSize, const QSize &imageSize, const QRect &middleRect)
{
	if (imageSize.width() * middleRect.height() < imageSize.height() * middleRect.width()) {
		int displayHeight = leastEqual(qRound(imageSize.height() * middleRect.width() / double(imageSize.width())), middleRect.height());
		return QRect(middleRect.x(), mostEqual((previewSize.height() - displayHeight) / 2, middleRect.y()), middleRect.width(), displayHeight);
	} else {
		int displayWidth = leastEqual(qRound(imageSize.width() * middleRect.height() / double(imageSize.height())), middleRect.width());
		return QRect(mostEqual((previewSize.width() - displayWidth) / 2, middleRect.x()), middleRect.y(), displayWidth, middleRect.height());
	}
}
static void initImageSize(QLabel *image, QSlider *slider, const QRect &imageRect, double &minScaleFactor, double maxScaleFactor, double originalImageWidth, double previewWidth)
{
	image->setGeometry(imageRect);

	minScaleFactor = imageRect.width() / originalImageWidth;

	slider->blockSignals(true);
	slider->setRange(int(minScaleFactor * SCALE_FACTOR), int(maxScaleFactor * SCALE_FACTOR));
	slider->blockSignals(false);

	double curScaleFactor = previewWidth / originalImageWidth;
	slider->setValue(int(curScaleFactor * SCALE_FACTOR));
}

PLSCropImage::PLSCropImage(const QString &imageFilePath, const QSize &cropImageSize, QWidget *parent, PLSDpiHelper dpiHelper) : PLSCropImage(QPixmap(imageFilePath), cropImageSize, parent, dpiHelper)
{
}

PLSCropImage::PLSCropImage(const QPixmap &originalImage_, const QSize &cropImageSize_, QWidget *parent, PLSDpiHelper dpiHelper)
	: PLSDialogView(parent, dpiHelper), ui(new Ui::PLSCropImage), cropImageSize(cropImageSize_), originalImage(originalImage_)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSCropImage});

	setResizeEnabled(false);

	ui->setupUi(content());
	QMetaObject::connectSlotsByName(this);

	image = new QLabel(ui->preview);
	image->setFrameShape(QFrame::NoFrame);
	image->setScaledContents(true);
	image->setPixmap(originalImage);
	image->move(0, 0);

	image->setMouseTracking(true);
	image->installEventFilter(this);

	QFrame *left = new QFrame(ui->preview);
	QFrame *top = new QFrame(ui->preview);
	QFrame *right = new QFrame(ui->preview);
	QFrame *bottom = new QFrame(ui->preview);
	left->setObjectName("left");
	top->setObjectName("top");
	right->setObjectName("right");
	bottom->setObjectName("bottom");
	setTranslucent(left, top, right, bottom);

	middle = new QFrame(ui->preview);
	middle->setObjectName("middle");
	dpiHelper.setFixedSize(middle, cropImageSize);
	dpiHelper.setStyleSheet(middle, "QFrame { border: /*hdpi*/ 1px solid #effc35; background-color: rgba(216, 216, 216, 0); }");
	middle->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	middle->installEventFilter(this);

	QHBoxLayout *h = new QHBoxLayout(ui->preview);
	h->setMargin(0);
	h->setSpacing(0);
	QVBoxLayout *v = new QVBoxLayout();
	v->setMargin(0);
	v->setSpacing(0);
	v->addWidget(top);
	v->addWidget(middle);
	v->addWidget(bottom);
	h->addWidget(left);
	h->addLayout(v);
	h->addWidget(right);
}

PLSCropImage::~PLSCropImage()
{
	delete ui;
}

void PLSCropImage::setButtons(int buttons)
{
	ui->backButton->setVisible(buttons & Button::Back);
	ui->okButton->setVisible(buttons & Button::Ok);
	ui->cancelButton->setVisible(buttons & Button::Cancel);
}

int PLSCropImage::cropImage(QPixmap &cropedImage, const QString &imageFilePath, const QSize &cropImageSize, int buttons, QWidget *parent)
{
	PLSCropImage cropImage(imageFilePath, cropImageSize, parent);
	cropImage.setButtons(buttons);
	int button = cropImage.exec();
	if (button == Button::Ok) {
		cropedImage = cropImage.cropedImage;
	}
	return button;
}

int PLSCropImage::cropImage(QPixmap &cropedImage, const QPixmap &originalImage, const QSize &cropImageSize, int buttons, QWidget *parent)
{
	PLSCropImage cropImage(originalImage, cropImageSize, parent);
	cropImage.setButtons(buttons);
	int button = cropImage.exec();
	if (button == Button::Ok) {
		cropedImage = cropImage.cropedImage;
	}
	return button;
}

void PLSCropImage::on_slider_valueChanged(int value)
{
	QRect oldImageRect = image->geometry();
	QRect middleRect = middle->geometry();
	QPoint oldCenter = image->mapFromParent(middleRect.center());
	double scaleFactor = value / SCALE_FACTOR;
	QSize imageSize = originalImage.size() * scaleFactor;
	imageSize.setWidth(leastEqual(imageSize.width(), middleRect.width()));
	imageSize.setHeight(leastEqual(imageSize.height(), middleRect.height()));
	QRect imageRect(oldImageRect.topLeft() + oldCenter * (1 - imageSize.width() / double(oldImageRect.width())), imageSize);
	if (imageRect.contains(middleRect)) {
		image->setGeometry(imageRect);
		return;
	}

	if (imageRect.left() > middleRect.left()) {
		imageRect.moveLeft(middleRect.left());
	}
	if (imageRect.top() > middleRect.top()) {
		imageRect.moveTop(middleRect.top());
	}
	if (imageRect.right() < middleRect.right()) {
		imageRect.moveRight(middleRect.right());
	}
	if (imageRect.bottom() < middleRect.bottom()) {
		imageRect.moveBottom(middleRect.bottom());
	}
	image->setGeometry(imageRect);
}

void PLSCropImage::on_smallButton_clicked()
{
	ui->slider->setValue(int(minScaleFactor * SCALE_FACTOR));
}
void PLSCropImage::on_largeButton_clicked()
{
	ui->slider->setValue(int(PLSDpiHelper::calculate(this, maxScaleFactor) * SCALE_FACTOR));
}
void PLSCropImage::on_backButton_clicked()
{
	done(Button::Back);
}
void PLSCropImage::on_okButton_clicked()
{
	QRect middleRect = middle->geometry();
	QRect grabRect = QRect(image->mapFromParent(middleRect.topLeft()), middleRect.size());
	cropedImage = QPixmap::grabWidget(image, grabRect);
	done(Button::Ok);
}
void PLSCropImage::on_cancelButton_clicked()
{
	done(Button::Cancel);
}

bool PLSCropImage::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == middle) {
		switch (event->type()) {
		case QEvent::Resize: {
			QSize previewSize = ui->preview->size();
			QRect imageRect = calcImageRect(previewSize, originalImage.size(), QRect(middle->pos(), dynamic_cast<QResizeEvent *>(event)->size()));
			initImageSize(image, ui->slider, imageRect, minScaleFactor, PLSDpiHelper::calculate(this, maxScaleFactor), originalImage.width(), previewSize.width());
			break;
		}
		case QEvent::Move: {
			QSize previewSize = ui->preview->size();
			QRect imageRect = calcImageRect(previewSize, originalImage.size(), QRect(dynamic_cast<QMoveEvent *>(event)->pos(), middle->size()));
			initImageSize(image, ui->slider, imageRect, minScaleFactor, PLSDpiHelper::calculate(this, maxScaleFactor), originalImage.width(), previewSize.width());
			break;
		}
		}
	} else if (watched == image) {
		switch (event->type()) {
		case QEvent::MouseButtonPress: {
			dragingImage = true;
			dragStartPos = dynamic_cast<QMouseEvent *>(event)->globalPos();
			setCursor(Qt::PointingHandCursor);
			break;
		}
		case QEvent::MouseButtonRelease: {
			dragingImage = false;
			setCursor(Qt::ArrowCursor);
			break;
		}
		case QEvent::MouseMove: {
			QPoint globalPos = dynamic_cast<QMouseEvent *>(event)->globalPos();
			if (dragingImage && globalPos != dragStartPos) {
				QPoint pos = image->pos() + (globalPos - dragStartPos);
				if (QRect(pos, image->size()).contains(middle->geometry())) {
					image->move(pos);
				}
				dragStartPos = globalPos;
			}
			break;
		}
		}
	}
	return PLSDialogView::eventFilter(watched, event);
}
