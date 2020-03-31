#pragma once

#include <QMainWindow>

#include <util/config-file.h>

class PLSMainView;

class PLSMainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit inline PLSMainWindow(QWidget *parent) : QMainWindow(parent) {}

	virtual config_t *Config() const = 0;
	virtual bool PLSInit() = 0;

	virtual int GetProfilePath(char *path, size_t size, const char *file) const = 0;

	virtual PLSMainView *getMainView() const = 0;
};
