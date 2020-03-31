#pragma once

#include <QString>

#include <dialog-view.hpp>

#include "PLSDpiHelper.h"

class Ui_ScriptsTool;

class ScriptLogWindow : public PLSDialogView {
	Q_OBJECT

	QString lines;
	bool bottomScrolled = true;

	void resizeEvent(QResizeEvent *event) override;

public:
	ScriptLogWindow(PLSDpiHelper dpiHelper = PLSDpiHelper());
	~ScriptLogWindow();

public slots:
	void AddLogMsg(int log_level, QString msg);
	void ClearWindow();
	void Clear();
	void ScrollChanged(int val);
};

class ScriptsTool : public PLSDialogView {
	Q_OBJECT

	Ui_ScriptsTool *ui;
	QWidget *propertiesView = nullptr;
	QWidget *tabButtonsHLine = nullptr;

public:
	ScriptsTool(PLSDpiHelper dpiHelper = PLSDpiHelper());
	~ScriptsTool();

	void RemoveScript(const char *path);
	void ReloadScript(const char *path);
	void RefreshLists();

public slots:
	void on_close_clicked();

	void on_addScripts_clicked();
	void on_removeScripts_clicked();
	void on_reloadScripts_clicked();
	void on_scriptLog_clicked();

	void on_scripts_currentRowChanged(int row);

	void on_pythonPathBrowse_clicked();

protected:
	bool eventFilter(QObject *watched, QEvent *event);
};
