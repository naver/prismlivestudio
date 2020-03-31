#ifndef PLSFLOATSCROLLBARSCROLLAREA_H
#define PLSFLOATSCROLLBARSCROLLAREA_H

#include <QScrollArea>
#include <QWidget>
#include <QScrollBar>

// a scroll area whose scroll bar floats up.
class PLSFloatScrollBarScrollArea : public QScrollArea {
	Q_OBJECT
public:
	explicit PLSFloatScrollBarScrollArea(QWidget *parent = nullptr);
	virtual ~PLSFloatScrollBarScrollArea();

	QScrollBar *VerticalScrollBar() { return delegateScrollBar; };
	void SetScrollBarRightMargin(int rightMargin);

signals:
	void ScrollBarRangChanged();
	void ScrolledToEnd();
	void ScrolledToTop();

protected:
	virtual void resizeEvent(QResizeEvent *event) override;

private slots:
	void OnVerticalScrollBarValueChanged(int value);

private:
	void UpdateSlider();

private:
	QScrollBar *delegateScrollBar{nullptr};
	int rightMargin{0};
};

#endif // PLSFLOATSCROLLBARSCROLLAREA_H
