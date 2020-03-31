#include "TextMotionTemplateButton.h"
#include <QMovie>
#include "PLSDpiHelper.h"

TextMotionTemplateButton::TextMotionTemplateButton(QWidget *parent) : QPushButton(parent), ui(new Ui::TextMotionTemplateButton)
{
	ui->setupUi(this);
	layout()->setContentsMargins(3, 3, 3, 3);
	layout()->setAlignment(Qt::AlignCenter);
	ui->templateGifLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	ui->templateGifLabel->setProperty("checked", false);
	connect(this, &QAbstractButton::toggled, [=](bool isChecked) { ui->templateGifLabel->setProperty("checked", isChecked); });
}

TextMotionTemplateButton::~TextMotionTemplateButton()
{
	delete ui;
}

void TextMotionTemplateButton::attachGifResource(const QString &resourcePath, const QString &resourceBackupPath)
{
	ui->templateGifLabel->setMovie(&m_movie);
	m_movie.setCacheMode(QMovie::CacheAll);
	m_movie.setScaledSize(ui->templateGifLabel->size());
	QFileInfo fileInfo(resourcePath);
	if (!fileInfo.exists()) {
		m_movie.setFileName(resourceBackupPath);

	} else {
		m_movie.setFileName(resourcePath);
	}
}

void TextMotionTemplateButton::setTemplateData(const TextMotionTemplateData &data)
{
	m_templateData = data;
}

void TextMotionTemplateButton::setGroupName(const QString &groupName)
{
	m_groupName = groupName;
}

QString TextMotionTemplateButton::getGroupName() const
{
	return m_groupName;
}

void TextMotionTemplateButton::setText(const QString &text)
{
	ui->templateGifLabel->setText(text);
}

void TextMotionTemplateButton::showEvent(QShowEvent *event)
{
	if (m_movie.state() != QMovie::Running) {
		m_movie.start();
	}
	QPushButton::showEvent(event);
}

void TextMotionTemplateButton::hideEvent(QHideEvent *event)
{
	QPushButton::hideEvent(event);
}

void TextMotionTemplateButton::resizeEvent(QResizeEvent *event)
{
	QPushButton::resizeEvent(event);
	m_movie.setScaledSize(ui->templateGifLabel->frameSize());
}
