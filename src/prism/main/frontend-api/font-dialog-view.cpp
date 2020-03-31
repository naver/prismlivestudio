#include "font-dialog-view.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVariant>
#include <QPushButton>
#include <QEvent>

#include "combobox.hpp"
#include "dialogbuttonbox.hpp"

class PLSFontDialogImpl : public QFontDialog {
public:
	PLSFontDialogImpl(PLSFontDialogView *cdv, const QFont &initial, QWidget *parent) : QFontDialog(initial, parent), fontDialogView(cdv)
	{
		setWindowFlags(Qt::Widget);

		if (QDialogButtonBox *buttonBox = findChild<QDialogButtonBox *>(); buttonBox) {
			PLSDialogButtonBox::updateStandardButtonsStyle(buttonBox);
		}
		for (auto &child : findChildren<QComboBox *>()) {
			child->setView(new PLSComboBoxListView());
		}
		for (auto &child : findChildren<QVBoxLayout *>()) {
			child->setSpacing(10);
		}
	}
	~PLSFontDialogImpl() {}

	void done(int result) override
	{
		QFontDialog::done(result);
		fontDialogView->done(result);
	}

private:
	PLSFontDialogView *fontDialogView;
};

PLSFontDialogView::PLSFontDialogView(QWidget *parent, PLSDpiHelper dpiHelper) : PLSFontDialogView(parent->font(), parent, dpiHelper) {}

PLSFontDialogView::PLSFontDialogView(const QFont &initial, QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper)
{
	dpiHelper.setCss(this, {PLSCssIndex::QFontDialog, PLSCssIndex::PLSFontDialogView});
	dpiHelper.setInitSize(this, QSize(630, 638));

	impl = new PLSFontDialogImpl(this, initial, this->content());

	QHBoxLayout *l = new QHBoxLayout(this->content());
	l->setSpacing(0);
	l->setMargin(0);
	l->addWidget(impl);

	connect(impl, &QFontDialog::currentFontChanged, this, &PLSFontDialogView::currentFontChanged);
	connect(impl, &QFontDialog::fontSelected, this, &PLSFontDialogView::fontSelected);
}

PLSFontDialogView::~PLSFontDialogView() {}

QFontDialog *PLSFontDialogView::fontDialog() const
{
	return impl;
}

void PLSFontDialogView::setCurrentFont(const QFont &font)
{
	impl->setCurrentFont(font);
}

QFont PLSFontDialogView::currentFont() const
{
	return impl->currentFont();
}

QFont PLSFontDialogView::selectedFont() const
{
	return impl->selectedFont();
}

void PLSFontDialogView::setOption(FontDialogOption option, bool on)
{
	impl->setOption(option, on);
}

bool PLSFontDialogView::testOption(FontDialogOption option) const
{
	return impl->testOption(option);
}

void PLSFontDialogView::setOptions(FontDialogOptions options)
{
	impl->setOptions(options);
}

PLSFontDialogView::FontDialogOptions PLSFontDialogView::options() const
{
	return impl->options();
}

void PLSFontDialogView::setVisible(bool visible)
{
	impl->setVisible(visible);
	PLSDialogView::setVisible(visible);
}

QFont PLSFontDialogView::getFont(bool *ok, QWidget *parent)
{
	QFont initial;
	return PLSFontDialogView::getFont(ok, initial, parent, QString(), 0);
}

QFont PLSFontDialogView::getFont(bool *ok, const QFont &initial, QWidget *parent, const QString &title, FontDialogOptions options)
{
	PLSFontDialogView dlg(initial, parent);
	dlg.setOptions(options);
	if (!title.isEmpty()) {
		dlg.setWindowTitle(title);
	}

	int ret = (dlg.exec() || (options & QFontDialog::NoButtons));
	if (ok) {
		*ok = !!ret;
	}
	if (ret) {
		return dlg.selectedFont();
	} else {
		return initial;
	}
}

void PLSFontDialogView::done(int result)
{
	PLSDialogView::done(result);
}

void PLSFontDialogView::showEvent(QShowEvent *event)
{
	moveToCenter();
	PLSDialogView::showEvent(event);
}
