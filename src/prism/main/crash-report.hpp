#pragma once

#include <QDialog>

class QPlainTextEdit;

class PLSCrashReport : public QDialog {
	Q_OBJECT

	QPlainTextEdit *textBox;

public:
	explicit PLSCrashReport(QWidget *parent, const char *text);

public slots:
	void ExitClicked();
	void CopyClicked();
};
