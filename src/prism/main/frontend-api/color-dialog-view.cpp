#include "color-dialog-view.hpp"

#include <QHBoxLayout>
#include <QVariant>
#include <QPushButton>
#include <QStyle>

#include "dialogbuttonbox.hpp"

class PLSColorDialogImpl : public QColorDialog {
public:
	PLSColorDialogImpl(PLSColorDialogView *cdv, const QColor &initial, QWidget *parent, PLSDpiHelper dpiHelper) : QColorDialog(initial, parent), colorDialogView(cdv)
	{
		setWindowFlags(Qt::Widget);

		layout()->setSizeConstraint(QLayout::SetDefaultConstraint);

		if (QDialogButtonBox *buttonBox = findChild<QDialogButtonBox *>(); buttonBox) {
			PLSDialogButtonBox::updateStandardButtonsStyle(buttonBox);
		}
	}
	~PLSColorDialogImpl() {}

	void done(int result) override
	{
		QColorDialog::done(result);
		colorDialogView->done(result);
	}

private:
	PLSColorDialogView *colorDialogView;
};

PLSColorDialogView::PLSColorDialogView(QWidget *parent, PLSDpiHelper dpiHelper) : PLSColorDialogView(Qt::white, parent, dpiHelper) {}

PLSColorDialogView::PLSColorDialogView(const QColor &initial, QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper)
{
	dpiHelper.setCss(this, {PLSCssIndex::QColorDialog, PLSCssIndex::PLSColorDialogView});
	dpiHelper.setInitSize(this, QSize(630, 635));

	impl = new PLSColorDialogImpl(this, initial, this->content(), dpiHelper);

	QHBoxLayout *l = new QHBoxLayout(this->content());
	l->setSpacing(0);
	l->setMargin(0);
	l->addWidget(impl);

	connect(impl, &QColorDialog::currentColorChanged, this, &PLSColorDialogView::currentColorChanged);
	connect(impl, &QColorDialog::colorSelected, this, &PLSColorDialogView::colorSelected);
}

PLSColorDialogView::~PLSColorDialogView() {}

QColorDialog *PLSColorDialogView::colorDialog() const
{
	return impl;
}

void PLSColorDialogView::setCurrentColor(const QColor &color)
{
	impl->setCurrentColor(color);
}

QColor PLSColorDialogView::currentColor() const
{
	return impl->currentColor();
}

QColor PLSColorDialogView::selectedColor() const
{
	return impl->selectedColor();
}

void PLSColorDialogView::setOption(ColorDialogOption option, bool on)
{
	impl->setOption(option, on);
}

bool PLSColorDialogView::testOption(ColorDialogOption option) const
{
	return impl->testOption(option);
}

void PLSColorDialogView::setOptions(ColorDialogOptions options)
{
	impl->setOptions(options);
}

PLSColorDialogView::ColorDialogOptions PLSColorDialogView::options() const
{
	return impl->options();
}

void PLSColorDialogView::setVisible(bool visible)
{
	impl->setVisible(visible);
	PLSDialogView::setVisible(visible);
}

QColor PLSColorDialogView::PLSColorDialogView::getColor(const QColor &initial, QWidget *parent, const QString &title, ColorDialogOptions options)
{
	PLSColorDialogView dlg(initial, parent);
	if (!title.isEmpty()) {
		dlg.setWindowTitle(title);
	}
	dlg.setOptions(options);
	dlg.exec();
	return dlg.selectedColor();
}

int PLSColorDialogView::customCount()
{
	return PLSColorDialogImpl::customCount();
}

QColor PLSColorDialogView::customColor(int index)
{
	return PLSColorDialogImpl::customColor(index);
}

void PLSColorDialogView::setCustomColor(int index, QColor color)
{
	PLSColorDialogImpl::setCustomColor(index, color);
}

QColor PLSColorDialogView::standardColor(int index)
{
	return PLSColorDialogImpl::standardColor(index);
}

void PLSColorDialogView::setStandardColor(int index, QColor color)
{
	PLSColorDialogImpl::setStandardColor(index, color);
}

void PLSColorDialogView::done(int result)
{
	PLSDialogView::done(result);
}

void PLSColorDialogView::showEvent(QShowEvent *event)
{
	//sizeToContent();
	moveToCenter();
	PLSDialogView::showEvent(event);
}
