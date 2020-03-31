#ifndef PLSREGIONCAPTURE_H
#define PLSREGIONCAPTURE_H

#include <QDialog>
#include <QPen>
#include <QFrame>
#include <QRectF>
#include <QPixmap>
#include <QLabel>

class PLSRegionCapture : public QWidget {
	Q_OBJECT

	struct ScreenShotData {
		QPixmap pixmap;
		QRect geometry;
	};

public:
	explicit PLSRegionCapture(QWidget *parent = nullptr);
	~PLSRegionCapture();
	void StartCapture(uint64_t maxRegionWidth = 0, uint64_t maxRegionHeight = 0);
	QRect GetSelectedRect() const;

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;
	virtual bool eventFilter(QObject *watched, QEvent *event) override;
	virtual void closeEvent(QCloseEvent *event) override;

private:
	void UpdateSelectedRect();
	void drawBackground(QPainter *painter, const QRect &rect);
	void drawCenterLines(QPainter *painter);
	void showMenus();
	void hideMuenus();
	void Init();
	QRect CalculateFrameSize();
	void SetTipCoordinate(int tipTextWidth, int tipTextHeight);
	void SetMenuCoordinate();
	void SetCurrentPosDpi(const QPoint &posGlobal);
	void SetHook();
	void ReleaseHook();
	void SetCoordinateValue();
signals:
	void selectedRegion(const QRect &rect);

private:
	QFrame *menuFrame = nullptr;
	QLabel *labelTip = nullptr;
	QLabel *labelRect;
	QPoint pointPressed;
	QPoint pointReleased;
	QPoint pointMoved;
	QRect menuGeometry;
	QPoint tipPositon;
	bool pressed = false;
	bool overWidth = false;
	bool overHeight = false;
	double screenDpi = 1.0;
	QRect rectWhole;
	QRect rectSelected;
	QPen penLine;
	QPen penBorder;
	QList<ScreenShotData> screenShots;
	uint64_t maxRegionWidth = 0;
	uint64_t maxRegionHeight = 0;
	bool drawing = true;
};

#endif // PLSREGIONCAPTURE_H
