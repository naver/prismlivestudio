#ifndef PLSDARKTHEME_H
#define PLSDARKTHEME_H

#include <QObject>

class PLSDarkTheme : public QObject {
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "prism.themes.DarkTheme" FILE "dark-theme.json")
public:
	PLSDarkTheme();
	~PLSDarkTheme();
};

#endif // PLSDARKTHEME_H
