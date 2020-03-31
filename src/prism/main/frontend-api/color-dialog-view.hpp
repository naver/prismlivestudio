#ifndef PLSCOLORDIALOGVIEW_HPP
#define PLSCOLORDIALOGVIEW_HPP

#include "qcolordialog.hpp"

#include "frontend-api-global.h"
#include "dialog-view.hpp"

class PLSColorDialogImpl;

class FRONTEND_API PLSColorDialogView : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSColorDialogView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSColorDialogView(const QColor &initial, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSColorDialogView();

public:
	using ColorDialogOption = QColorDialog::ColorDialogOption;
	using ColorDialogOptions = QColorDialog::ColorDialogOptions;

	QColorDialog *colorDialog() const;

	void setCurrentColor(const QColor &color);
	QColor currentColor() const;

	QColor selectedColor() const;

	void setOption(ColorDialogOption option, bool on = true);
	bool testOption(ColorDialogOption option) const;
	void setOptions(ColorDialogOptions options);
	ColorDialogOptions options() const;

	void setVisible(bool visible) override;

	static QColor getColor(const QColor &initial = Qt::white, QWidget *parent = nullptr, const QString &title = QString(), ColorDialogOptions options = ColorDialogOptions());

	static int customCount();
	static QColor customColor(int index);
	static void setCustomColor(int index, QColor color);
	static QColor standardColor(int index);
	static void setStandardColor(int index, QColor color);

Q_SIGNALS:
	void currentColorChanged(const QColor &color);
	void colorSelected(const QColor &color);

protected:
	void done(int result) override;
	void showEvent(QShowEvent *event) override;

private:
	PLSColorDialogImpl *impl;

	friend class PLSColorDialogImpl;
};

#endif // PLSCOLORDIALOGVIEW_HPP
