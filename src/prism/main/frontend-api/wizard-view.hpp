#ifndef PLSWIZARDVIEW_HPP
#define PLSWIZARDVIEW_HPP

#include <QWizard>

#include "frontend-api-global.h"
#include "dialog-view.hpp"

class PLSWizardImpl;

class FRONTEND_API PLSWizardView : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSWizardView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSWizardView();

public:
	using WizardStyle = QWizard::WizardStyle;
	using WizardOption = QWizard::WizardOption;
	using WizardOptions = QWizard::WizardOptions;
	using WizardButton = QWizard::WizardButton;
	using WizardPixmap = QWizard::WizardPixmap;

public:
	QWizard *wizard() const;

	int addPage(QWizardPage *page);
	void setPage(int id, QWizardPage *page);
	void removePage(int id);
	QWizardPage *page(int id) const;
	bool hasVisitedPage(int id) const;
	QList<int> visitedPages() const; // ### Qt 6: visitedIds()?
	QList<int> pageIds() const;
	void setStartId(int id);
	int startId() const;
	QWizardPage *currentPage() const;
	int currentId() const;

	virtual bool validateCurrentPage();
	virtual int nextId() const;

	void setField(const QString &name, const QVariant &value);
	QVariant field(const QString &name) const;

	void setWizardStyle(WizardStyle style);
	WizardStyle wizardStyle() const;

	void setOption(WizardOption option, bool on = true);
	bool testOption(WizardOption option) const;
	void setOptions(WizardOptions options);
	WizardOptions options() const;

	void setButtonText(WizardButton which, const QString &text);
	QString buttonText(WizardButton which) const;
	void setButtonLayout(const QList<WizardButton> &layout);
	void setButton(WizardButton which, QAbstractButton *button);
	QAbstractButton *button(WizardButton which) const;

	void setTitleFormat(Qt::TextFormat format);
	Qt::TextFormat titleFormat() const;
	void setSubTitleFormat(Qt::TextFormat format);
	Qt::TextFormat subTitleFormat() const;
	void setPixmap(WizardPixmap which, const QPixmap &pixmap);
	QPixmap pixmap(WizardPixmap which) const;

	void setSideWidget(QWidget *widget);
	QWidget *sideWidget() const;

	void setDefaultProperty(const char *className, const char *property, const char *changedSignal);

Q_SIGNALS:
	void currentIdChanged(int id);
	void helpRequested();
	void customButtonClicked(int which);
	void pageAdded(int id);
	void pageRemoved(int id);

public Q_SLOTS:
	void back();
	void next();
	void restart();

protected:
	virtual void done(int result);

private:
	PLSWizardImpl *impl;

	friend class PLSWizardImpl;
};

#endif // PLSWIZARDVIEW_HPP
