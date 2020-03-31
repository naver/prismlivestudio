#ifndef TEXTMOTIONTEMPLATEBUTTON_H
#define TEXTMOTIONTEMPLATEBUTTON_H

#include <QPushButton>
#include <QMovie>
#include <QResizeEvent>

#include "TextMotionTemplateDataHelper.h"
#include "ui_TextMotionTemplateButton.h"

namespace Ui {
class TextMotionTemplateButton;
}

class TextMotionTemplateButton : public QPushButton {
	Q_OBJECT

public:
	explicit TextMotionTemplateButton(QWidget *parent = nullptr);
	~TextMotionTemplateButton();

	void attachGifResource(const QString &resourcePath, const QString &resourceBackupPath);
	void setTemplateData(const TextMotionTemplateData &data);
	void setGroupName(const QString &groupName);
	QString getGroupName() const;
	void setText(const QString &text);

	// QWidget interface
protected:
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	Ui::TextMotionTemplateButton *ui;
	QMovie m_movie;
	TextMotionTemplateData m_templateData;
	QString m_groupName;
};

#endif // TEXTMOTIONTEMPLATEBUTTON_H
