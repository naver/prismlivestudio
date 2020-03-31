#include <QWidget>

#include "pls-common-define.hpp"

enum class DragDirection { Left, Right, Unknown };

class PLSScrollAreaContent : public QWidget {
	Q_OBJECT
public:
	explicit PLSScrollAreaContent(QWidget *parent = nullptr);
	~PLSScrollAreaContent();
	int Refresh();
	void SetIsDraging(bool state);

protected:
	void paintEvent(QPaintEvent *event) override;
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

private:
	void GetRowColByPos(const int &x, const int &y, const int &width, const int &height, int &row, int &col);
	void GetDirectionByPos(const int &x, const int &y, const int &width, const int &height, const int &row, int &col);
	void SetLinePos(const int &startX, const int &startY, const int &endX, const int &endY);
signals:
	void DragFinished();
	void resizeEventChanged(bool);

private:
	QPoint lineStart;
	QPoint lineEnd;
	bool isDrag{false};
};
