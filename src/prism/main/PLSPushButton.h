#ifndef PLSPUSHBUTTON_H
#define PLSPUSHBUTTON_H

#include <QPushButton>

class PLSPushButton : public QPushButton {
	Q_OBJECT
public:
	explicit PLSPushButton(QWidget *parent = nullptr) : QPushButton(parent) {}
	explicit PLSPushButton(const QString &text_, QWidget *parent = nullptr) : QPushButton(parent), text(text_) {}

	void SetText(const QString &text);
	~PLSPushButton() {}

protected:
	virtual void resizeEvent(QResizeEvent *event) override;

private:
	QString GetNameElideString();

private:
	QString text;
};

#endif // PLSPUSHBUTTON_H
