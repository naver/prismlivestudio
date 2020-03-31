#pragma once

#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QStringList>
#include <QScrollArea>

#include "PLSWidgetDpiAdapter.hpp"
#include "PLSDpiHelper.h"

class PLSCompleterPopupList : public PLSWidgetDpiAdapterHelper<QFrame> {
	Q_OBJECT

public:
	explicit PLSCompleterPopupList(QWidget *toplevel, QLineEdit *lineEdit, const QStringList &completions, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSCompleterPopupList() override;

public:
	bool isInCompleter(const QPoint &globalPos);
signals:
	void activated(const QString &text);

public slots:
	void showPopup(const QString &text);

protected:
	bool eventFilter(QObject *watched, QEvent *event) override;

private:
	QLineEdit *lineEdit;
	QScrollArea *scrollArea;
	QList<QLabel *> labels;
};

class PLSCompleter : public QObject {
	Q_OBJECT

private:
	explicit PLSCompleter(QWidget *toplevel, QLineEdit *lineEdit, const QStringList &completions);
	~PLSCompleter() override;

public:
	static PLSCompleter *attachLineEdit(QWidget *toplevel, QLineEdit *lineEdit, const QStringList &completions);
	static void detachLineEdit(QLineEdit *lineEdit);

signals:
	void activated(const QString &text);

protected:
	bool eventFilter(QObject *watched, QEvent *event) override;

private:
	QLineEdit *lineEdit;
	PLSCompleterPopupList *popupList;
	bool eatFocusOut = true;
};
