#ifndef PLSCOLORFILTERVIEW_H
#define PLSCOLORFILTERVIEW_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QVariant>

#include "obs.hpp"

namespace Ui {
class PLSColorFilterView;
}

const int COLOR_FILTER_DENSITY_DEFAULT_VALUE = 100;

class PLSColorFilterImage;
class PLSColorFilterView : public QFrame {
	Q_OBJECT

public:
	explicit PLSColorFilterView(obs_source_t *source, QWidget *parent = nullptr);
	~PLSColorFilterView();
	void ResetColorFilter();
	void UpdateColorFilterValue(const int &value);
	OBSSource GetSource();

protected:
	virtual void showEvent(QShowEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;

private slots:
	void OnLeftScrollBtnClicked();
	void OnRightScrollBtnClicked();
	void OnColorFilterImageMousePressd(PLSColorFilterImage *image);

private:
	int GetScrollAreaWidth();
	void CreateColorFilterImage(const QString &name, const QString &shortTitle, const QString &filterPath, const QString &thumbnailPath);
	void UpdateFilterItemStyle(PLSColorFilterImage *image);
	void UpdateFilterItemStyle(const QString &imagePath);
	void UpdateColorFilterSource(const QString &path);
	void LoadDefaultColorFilter();
	void GetDefaultColorFilterPath();
	void SetDefaultCurrentColorFilter();
	bool WriteConfigFile(PLSColorFilterImage *image = nullptr);
	bool ResetConfigFile();
	bool SetValueToConfigFile(const QString &name, const int &value);
	bool GetValueFromConfigFile(const QString &name, int &value);
signals:
	void OriginalPressed(bool state);
	void ColorFilterValueChanged(int value, bool isOriginal);

private:
	Ui::PLSColorFilterView *ui;
	OBSSource source;
	QPushButton *leftScrollBtn{};
	QPushButton *rightScrollBtn{};
	std::vector<PLSColorFilterImage *> ImageVec;
	int currentValue{COLOR_FILTER_DENSITY_DEFAULT_VALUE};
	QString currentPath;
};

class PLSColorFilterImage : public QPushButton {
	Q_OBJECT
public:
	PLSColorFilterImage(const QString &name, const QString &shortTitle, const QString &path, const QString &thumbnail, QWidget *parent = nullptr);
	QString GetPath();
	QString GetThumbnailPath();
	QString GetName();
	int GetValue();
	void SetValue(const int &value);
	void SetClickedState(bool clicked);
	void SetPixMap(const QPixmap &pixmap);
	bool GetClickedState();

protected:
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void paintEvent(QPaintEvent *event) override;

private:
	void SetClickedStyle(bool clicked);

signals:
	void MousePressd(PLSColorFilterImage *image);
	void ClickedStateChanged(bool clicked);

private:
	QString name;
	QString path;
	QString thumbnail;
	QString shortTitle;
	bool clicked{false};
	QPixmap pixmap;
	int value{COLOR_FILTER_DENSITY_DEFAULT_VALUE};
};

#endif // PLSCOLORFILTERVIEW_H
