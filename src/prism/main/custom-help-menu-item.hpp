#ifndef CUSTOMHELPMENUITEM_H
#define CUSTOMHELPMENUITEM_H

#include <QWidget>

namespace Ui {
class CustomHelpMenuItem;
}

class CustomHelpMenuItem : public QWidget {
	Q_OBJECT

public:
	explicit CustomHelpMenuItem(const QString &title, QWidget *parent = nullptr);
	~CustomHelpMenuItem();
	void setText(const QString &text);
	void setBadgeVisible(bool visible = false);
	void setItemDisabled(bool disabled);

	// QWidget interface
protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

private:
	Ui::CustomHelpMenuItem *ui;
	bool m_disabled;
};

#endif // CUSTOMHELPMENUITEM_H
