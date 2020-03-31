#ifndef PLSFONTDIALOGVIEW_HPP
#define PLSFONTDIALOGVIEW_HPP

#include <QFontDialog>

#include "frontend-api-global.h"
#include "dialog-view.hpp"

class PLSFontDialogImpl;

class FRONTEND_API PLSFontDialogView : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSFontDialogView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSFontDialogView(const QFont &initial, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSFontDialogView();

public:
	using FontDialogOption = QFontDialog::FontDialogOption;
	using FontDialogOptions = QFontDialog::FontDialogOptions;

	QFontDialog *fontDialog() const;

	void setCurrentFont(const QFont &font);
	QFont currentFont() const;

	QFont selectedFont() const;

	void setOption(FontDialogOption option, bool on = true);
	bool testOption(FontDialogOption option) const;
	void setOptions(FontDialogOptions options);
	FontDialogOptions options() const;

	void setVisible(bool visible) override;

	static QFont getFont(bool *ok, QWidget *parent = nullptr);
	static QFont getFont(bool *ok, const QFont &initial, QWidget *parent = nullptr, const QString &title = QString(), FontDialogOptions options = FontDialogOptions());

Q_SIGNALS:
	void currentFontChanged(const QFont &font);
	void fontSelected(const QFont &font);

protected:
	void done(int result) override;
	void showEvent(QShowEvent *event) override;

private:
	PLSFontDialogImpl *impl;

	friend class PLSFontDialogImpl;
};

#endif // PLSFONTDIALOGVIEW_HPP
