#include "wizard-view.hpp"

#include <QHBoxLayout>
#include <QVariant>

class PLSWizardImpl : public QWizard {
public:
	PLSWizardImpl(PLSWizardView *wv, QWidget *parent, PLSDpiHelper dpiHelper) : QWizard(parent), wizardView(wv) { setWindowFlags(Qt::Widget); }
	~PLSWizardImpl() {}

	bool wizard_validateCurrentPage() { return QWizard::validateCurrentPage(); }
	int wizard_nextId() const { return QWizard::nextId(); }

	void wizard_done(int result) { QWizard::done(result); }

	virtual bool validateCurrentPage() { return wizardView->validateCurrentPage(); }
	virtual int nextId() const { return wizardView->nextId(); }

	virtual void done(int result) { wizardView->done(result); }

private:
	PLSWizardView *wizardView;
};

PLSWizardView::PLSWizardView(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper)
{
	impl = new PLSWizardImpl(this, this->content(), dpiHelper);

	QHBoxLayout *l = new QHBoxLayout(this->content());
	l->setSpacing(0);
	l->setMargin(0);
	l->addWidget(impl);

	connect(impl, &QWizard::currentIdChanged, this, &PLSWizardView::currentIdChanged);
	connect(impl, &QWizard::helpRequested, this, &PLSWizardView::helpRequested);
	connect(impl, &QWizard::customButtonClicked, this, &PLSWizardView::customButtonClicked);
	connect(impl, &QWizard::pageAdded, this, &PLSWizardView::pageAdded);
	connect(impl, &QWizard::pageRemoved, this, &PLSWizardView::pageRemoved);
}

PLSWizardView::~PLSWizardView() {}

QWizard *PLSWizardView::wizard() const
{
	return impl;
}

int PLSWizardView::addPage(QWizardPage *page)
{
	return impl->addPage(page);
}

void PLSWizardView::setPage(int id, QWizardPage *page)
{
	impl->setPage(id, page);
}

void PLSWizardView::removePage(int id)
{
	impl->removePage(id);
}

QWizardPage *PLSWizardView::page(int id) const
{
	return impl->page(id);
}

bool PLSWizardView::hasVisitedPage(int id) const
{
	return impl->hasVisitedPage(id);
}

QList<int> PLSWizardView::visitedPages() const
{
	return impl->visitedPages();
}

QList<int> PLSWizardView::pageIds() const
{
	return impl->pageIds();
}

void PLSWizardView::setStartId(int id)
{
	impl->setStartId(id);
}

int PLSWizardView::startId() const
{
	return impl->startId();
}

QWizardPage *PLSWizardView::currentPage() const
{
	return impl->currentPage();
}

int PLSWizardView::currentId() const
{
	return impl->currentId();
}

bool PLSWizardView::validateCurrentPage()
{
	return impl->wizard_validateCurrentPage();
}

int PLSWizardView::nextId() const
{
	return impl->wizard_nextId();
}

void PLSWizardView::setField(const QString &name, const QVariant &value)
{
	impl->setField(name, value);
}

QVariant PLSWizardView::field(const QString &name) const
{
	return impl->field(name);
}

void PLSWizardView::setWizardStyle(WizardStyle style)
{
	impl->setWizardStyle(style);
}

PLSWizardView::WizardStyle PLSWizardView::wizardStyle() const
{
	return impl->wizardStyle();
}

void PLSWizardView::setOption(WizardOption option, bool on)
{
	impl->setOption(option, on);
}

bool PLSWizardView::testOption(WizardOption option) const
{
	return impl->testOption(option);
}

void PLSWizardView::setOptions(WizardOptions options)
{
	impl->setOptions(options);
}

PLSWizardView::WizardOptions PLSWizardView::options() const
{
	return impl->options();
}

void PLSWizardView::setButtonText(WizardButton which, const QString &text)
{
	impl->setButtonText(which, text);
}

QString PLSWizardView::buttonText(WizardButton which) const
{
	return impl->buttonText(which);
}

void PLSWizardView::setButtonLayout(const QList<WizardButton> &layout)
{
	impl->setButtonLayout(layout);
}

void PLSWizardView::setButton(WizardButton which, QAbstractButton *button)
{
	impl->setButton(which, button);
}
QAbstractButton *PLSWizardView::button(WizardButton which) const
{
	return impl->button(which);
}

void PLSWizardView::setTitleFormat(Qt::TextFormat format)
{
	impl->setTitleFormat(format);
}

Qt::TextFormat PLSWizardView::titleFormat() const
{
	return impl->titleFormat();
}

void PLSWizardView::setSubTitleFormat(Qt::TextFormat format)
{
	impl->setSubTitleFormat(format);
}

Qt::TextFormat PLSWizardView::subTitleFormat() const
{
	return impl->subTitleFormat();
}

void PLSWizardView::setPixmap(WizardPixmap which, const QPixmap &pixmap)
{
	impl->setPixmap(which, pixmap);
}

QPixmap PLSWizardView::pixmap(WizardPixmap which) const
{
	return impl->pixmap(which);
}

void PLSWizardView::setSideWidget(QWidget *widget)
{
	impl->setSideWidget(widget);
}

QWidget *PLSWizardView::sideWidget() const
{
	return impl->sideWidget();
}

void PLSWizardView::setDefaultProperty(const char *className, const char *property, const char *changedSignal)
{
	impl->setDefaultProperty(className, property, changedSignal);
}

void PLSWizardView::back()
{
	impl->back();
}

void PLSWizardView::next()
{
	impl->next();
}

void PLSWizardView::restart()
{
	impl->restart();
}

void PLSWizardView::done(int result)
{
	impl->wizard_done(result);
	PLSDialogView::done(result);
}
