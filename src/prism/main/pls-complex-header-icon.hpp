#ifndef PLSCOMPLEXHEADERICON_HPP
#define PLSCOMPLEXHEADERICON_HPP

#include <QFrame>
#include <QLabel>
#include <QBitmap>
#include <QPainter>

/* this is a complex UI for user profile header */

namespace Ui {
class PLSComplexHeaderIcon;
}

class PLSComplexHeaderIcon : public QLabel {
	Q_OBJECT

public:
	explicit PLSComplexHeaderIcon(QWidget *parent = nullptr);
	~PLSComplexHeaderIcon() override;

	void setPixmap(const QString &pix);
	void setPixmap(const QString &pix, const QSize &size);
	void setPlatformPixmap(const QString &pix, const QSize &size);
	void setPlatformPixmap(const QPixmap &pix);

protected:
	void changeEvent(QEvent *e) override;

private:
	Ui::PLSComplexHeaderIcon *ui;
};

#endif // PLSCOMPLEXHEADERICON_HPP
