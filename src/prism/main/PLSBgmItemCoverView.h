#ifndef PLSBGMITEMCOVERVIEW_H
#define PLSBGMITEMCOVERVIEW_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QMovie>

namespace Ui {
class PLSBgmItemCoverView;
}

class QGraphicsBlurEffect;
class QGraphicsOpacityEffect;

class PLSBgmItemCoverImage : public QLabel {
	Q_OBJECT
public:
	PLSBgmItemCoverImage(QWidget *parent = nullptr);

	void SetPixmap(const QPixmap &pixmap);
	void SetRoundedRect(bool set);

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;

private:
	QPixmap pixmap;
	bool setRoundedRect{false};
};

class PLSScrollingLabel;
class PLSBgmItemCoverView : public QFrame {
	Q_OBJECT

public:
	explicit PLSBgmItemCoverView(QWidget *parent = nullptr);
	~PLSBgmItemCoverView();

	void SetMusicInfo(const QString &title, const QString &producer);
	void SetCoverPath(const QString &coverPath, bool isFreeMusic);
	void SetImage(const QImage &image);
	void ShowPlayingGif(bool show);
	void DpiChanged(double dpi);

protected:
	virtual void resizeEvent(QResizeEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
	void ResizeUI();
signals:
	void CoverPressed(const QPoint &point);

private:
	Ui::PLSBgmItemCoverView *ui;
	PLSBgmItemCoverImage *imageLabel{};
	PLSScrollingLabel *titleLabel{};
	PLSScrollingLabel *producerLabel{};
	QGraphicsBlurEffect *blurEffect{};
	QGraphicsOpacityEffect *opacityEffect{};

	QLabel *playingLabel{};
	QFrame *titleFrame{};
	QFrame *producerFrame{};
	QMovie *movie{};
	QString title{};
	QString producer{};
	QString coverPath{};
	bool isFreeMusic{false};

	QPoint startPoint{};
	bool mousePressed{false};
};

#endif // PLSBGMITEMCOVERVIEW_H
