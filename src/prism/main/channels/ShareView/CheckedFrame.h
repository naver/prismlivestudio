#ifndef CHECKEDFRAME_H
#define CHECKEDFRAME_H
#include <QFrame>

/* this is a special frame just for checkable*/

class CheckedFrame : public QFrame {
	Q_OBJECT
	Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY checkedChanged)
public:
	using QFrame::QFrame;
	bool isChecked() { return checked; }
signals:
	void checkedChanged(bool);
public slots:
	void setChecked(bool);

private:
	bool checked = false;
};

#endif // CHECKEDFRAME_H
