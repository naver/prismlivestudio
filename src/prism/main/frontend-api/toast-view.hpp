#ifndef PLSTOASTVIEW_HPP
#define PLSTOASTVIEW_HPP

#include "dialog-view.hpp"
#include "frontend-api.h"

#include <QTimer>

namespace Ui {
class PLSToastView;
}

/**
 * @class PLSToastView
 * @brief common popup toast
 */
class FRONTEND_API PLSToastView : public PLSDialogView {
	Q_OBJECT
	Q_PROPERTY(Icon icon READ getIcon WRITE setIcon)
	Q_PROPERTY(QPixmap pixmap READ getPixmap WRITE setPixmap)

public:
	enum Icon { Error = 1, Warning = 2 };
	Q_ENUM(Icon)

public:
	explicit PLSToastView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSToastView();

public:
	Icon getIcon() const;
	void setIcon(Icon icon);

	QPixmap getPixmap() const;
	void setPixmap(const QPixmap &pixmap);

	void setMessage(const QString &message);

	void show(Icon icon, const QString &message, int autoHide = 0);

private slots:
	void autoHide();

private:
	Ui::PLSToastView *ui;
	Icon icon;
	QTimer *timer;
};

#endif // PLSTOASTVIEW_HPP
