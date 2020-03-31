#include "PLSSelectImageButton.h"
#include "ui_PLSSelectImageButton.h"

#include <ctime>

#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>

#include "log.h"
#include "action.h"

#include "PLSCropImage.h"
#include "PLSTakeCameraSnapshot.h"
#include "ChannelCommonFunctions.h"

#define MIN_PHOTO_WIDTH 440
#define MIN_PHOTO_HEIGHT 245

static void initButton(QPushButton *button)
{
	QHBoxLayout *layout = new QHBoxLayout(button);
	layout->setMargin(0);
	layout->setSpacing(0);
	QLabel *icon = new QLabel(button);
	icon->setObjectName("icon");
	QLabel *text = new QLabel(button->text(), button);
	text->setObjectName("text");
	layout->addWidget(icon);
	layout->addWidget(text);
	button->setText(QString());
}

QString getTempImageFilePath(const QString &suffix)
{
	QDir temp = QDir::temp();
	temp.mkdir("cropedImages");
	temp.cd("cropedImages");
	QString tempImageFilePath = temp.absoluteFilePath(QString("cropedImage-%1").arg(std::time(nullptr)) + suffix);
	return tempImageFilePath;
}

PLSSelectImageButton::PLSSelectImageButton(QWidget *parent, PLSDpiHelper dpiHelper) : QLabel(parent), ui(new Ui::PLSSelectImageButton)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSSelectImageButton});

	ui->setupUi(this);
	initButton(ui->takeButton);
	initButton(ui->selectButton);

	icon = new QLabel(this);
	icon->setObjectName("icon");
	icon->lower();
	icon->installEventFilter(this);

	ui->takeButton->hide();
	ui->selectButton->hide();

	setScaledContents(true);
	setMouseTracking(true);
	ui->takeButton->setMouseTracking(true);
	ui->selectButton->setMouseTracking(true);

	ui->takeButton->installEventFilter(this);
	ui->selectButton->installEventFilter(this);
}

PLSSelectImageButton::~PLSSelectImageButton()
{
	delete ui;
}

QString PLSSelectImageButton::getImagePath() const
{
	return imagePath;
}
void PLSSelectImageButton::setImagePath(const QString &imagePath)
{
	this->imagePath = imagePath;
	if (!imagePath.isEmpty()) {
		setPixmap(QPixmap(imagePath));
	} else {
		setPixmap(QPixmap());
		icon->show();
	}
}
void PLSSelectImageButton::setPixmap(const QPixmap &pixmap, const QSize &size)
{
	if (pixmap.isNull()) {
		QLabel::setPixmap(pixmap);
		icon->show();
		return;
	}

	QPixmap scaled = pixmap.scaled(size.isValid() ? size : this->size());
	QPixmap image(scaled.size());
	image.fill(Qt::transparent);

	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);

	double dpi = PLSDpiHelper::getDpi(this);

	QPainterPath path;
	path.addRoundedRect(image.rect(), PLSDpiHelper::calculate(dpi, 4), PLSDpiHelper::calculate(dpi, 4));
	painter.setClipPath(path);

	painter.drawPixmap(image.rect(), scaled);

	QLabel::setPixmap(image);
	icon->hide();
}

void PLSSelectImageButton::setEnabled(bool enabled)
{
	// QLabel::setEnabled(enabled);
	icon->setEnabled(enabled);
	pls_flush_style(icon);
}

void PLSSelectImageButton::on_takeButton_clicked()
{
	ui->takeButton->hide();
	ui->selectButton->hide();

	emit takeButtonClicked();

	setFocus();

	for (;;) {
		PLSTakeCameraSnapshot takeCameraSnapshot(this);
		QString imageFilePath = takeCameraSnapshot.getSnapshot();
		if (imageFilePath.isEmpty()) {
			break;
		}

		QPixmap cropedImage;
		int button = PLSCropImage::cropImage(cropedImage, imageFilePath, QSize(MIN_PHOTO_WIDTH, MIN_PHOTO_HEIGHT), PLSCropImage::Back | PLSCropImage::Ok, this);
		if (button == PLSCropImage::Back) {
			continue;
		} else if (button != PLSCropImage::Ok || cropedImage.isNull()) {
			break;
		}

		QString cropedImageFile = getTempImageFilePath(".png");
		cropedImage.save(cropedImageFile, "PNG");

		setPixmap(cropedImage);

		this->imagePath = cropedImageFile;
		emit imageSelected(cropedImageFile);
		break;
	}
}
void PLSSelectImageButton::on_selectButton_clicked()
{
	ui->takeButton->hide();
	ui->selectButton->hide();

	emit selectButtonClicked();

	setFocus();

	QString imageDir = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first();
	QWidget *toplevelView = pls_get_toplevel_view(this);
	QString imageFilePath = QFileDialog::getOpenFileName(toplevelView, tr("Browse"), imageDir, "Image Files (*.jpg *.jpeg *.bmp *.png)");
	if (imageFilePath.isEmpty()) {
		return;
	}

	QPixmap originalImagge(imageFilePath);
	if (originalImagge.width() < MIN_PHOTO_WIDTH || originalImagge.height() < MIN_PHOTO_HEIGHT) {
		PLSAlertView::warning(toplevelView, tr("SelectImage.Alert.Title"), tr("SelectImage.Alert.Message.PhotoTooSmall"));
		return;
	}

	QPixmap cropedImage;
	int button = PLSCropImage::cropImage(cropedImage, originalImagge, QSize(MIN_PHOTO_WIDTH, MIN_PHOTO_HEIGHT), PLSCropImage::Ok | PLSCropImage::Cancel, this);
	if (button != PLSCropImage::Ok || cropedImage.isNull()) {
		return;
	}

	QString cropedImageFile = getTempImageFilePath(".png");
	cropedImage.save(cropedImageFile, "PNG");

	setPixmap(cropedImage);

	this->imagePath = cropedImageFile;
	emit imageSelected(cropedImageFile);
}

bool PLSSelectImageButton::event(QEvent *event)
{
	switch (event->type()) {
	case QEvent::Enter:
		if (!mouseHover) {
			mouseHover = true;
			ui->takeButton->setVisible(icon->isEnabled());
			ui->selectButton->setVisible(icon->isEnabled());
		}
		break;
	case QEvent::Leave:
		if (mouseHover) {
			mouseHover = false;
			ui->takeButton->hide();
			ui->selectButton->hide();
		}
		break;
	case QEvent::Resize: {
		QResizeEvent *resizeEvent = dynamic_cast<QResizeEvent *>(event);
		QSize size = resizeEvent->size() - icon->size();
		icon->move(size.width() / 2, size.height() / 2);
		if (!imagePath.isEmpty()) {
			setPixmap(QPixmap(imagePath), resizeEvent->size());
		}
		break;
	}
	}

	return QLabel::event(event);
}
bool PLSSelectImageButton::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == icon) {
		if (event->type() == QEvent::Resize) {
			QSize size = this->size() - icon->size();
			icon->move(size.width() / 2, size.height() / 2);
		}
	}
	return QLabel::eventFilter(watched, event);
}
