#include "obs-module.h"
#include "frontend-api.h"

#include "devtool.h"
#include "ui_devtool.h"

#include <QAction>
#include <QApplication>
#include <QStyle>
#include <QMetaEnum>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <Windows.h>

#include <private/qstylesheetstyle_p.h>
#include <private/qcssparser_p.h>

static PLSDevTool *devTool = nullptr;
static bool beginPickWidget = false;

#define DEVTOOL "devtool"

extern QVector<QCss::StyleRule> getStyleRules(QWidget *widget, QStyleSheetStyle *style);

namespace {
class HookNativeEvent {
public:
	HookNativeEvent() { m_mouseHook = SetWindowsHookExW(WH_MOUSE, &mouseHookProc, GetModuleHandleW(nullptr), GetCurrentThreadId()); }
	~HookNativeEvent()
	{
		if (m_mouseHook) {
			UnhookWindowsHookEx(m_mouseHook);
			m_mouseHook = nullptr;
		}
	}

protected:
	static LRESULT CALLBACK mouseHookProc(_In_ int code, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		if (!devTool || !devTool->canPick()) {
			return CallNextHookEx(nullptr, code, wParam, lParam);
		}

		switch (wParam) {
		case WM_LBUTTONDOWN:
			if (devTool && devTool->isPtInPickButton()) {
				beginPickWidget = true;
				devTool->pickingWidget();
			}
			break;
		case WM_LBUTTONUP:
			if (beginPickWidget) {
				beginPickWidget = false;
				devTool->pickWidget(false);
			}
			break;
		case WM_MOUSEMOVE:
			if (devTool && devTool->isVisible() && GetAsyncKeyState(VK_CONTROL) < 0) {
				devTool->pickWidget(true);
			}
			break;
		}

		return CallNextHookEx(nullptr, code, wParam, lParam);
	}

	HHOOK m_mouseHook;
};

void installNativeEventFilter()
{
	static std::unique_ptr<HookNativeEvent> hookNativeEvent;
	if (!hookNativeEvent) {
		hookNativeEvent.reset(new HookNativeEvent);
	}
}

QString toString(const QVector<QCss::Pseudo> &pseudos)
{
	QString str;
	for (int i = 0; i < pseudos.length(); ++i) {
		str.append(QString(":%1").arg(pseudos[i].name));
	}
	return str;
}

QString toString(const QVector<QCss::Value> &values)
{
	QString str;
	for (int i = 0; i < values.length(); ++i) {
		str.append(QString(" %1").arg(values[i].toString()));
	}
	return str;
}

QString toString(const QVector<QCss::AttributeSelector> &attributeSelectors)
{
	QString str;
	for (int i = 0; i < attributeSelectors.length(); ++i) {
		auto &attributeSelector = attributeSelectors[i];
		str.append(QString("[%1=\"%2\"]").arg(attributeSelector.name, attributeSelector.value));
	}
	return str;
}

QString toString(const QStringList &ids)
{
	QString str;
	for (int i = 0; i < ids.length(); ++i) {
		auto &id = ids[i];
		str.append(QString("#%1").arg(id));
	}
	return str;
}

QString toString(const QVector<QCss::Selector> &selectors)
{
	QString str;
	for (int i = 0; i < selectors.length(); ++i) {
		auto &selector = selectors[i];
		for (int j = 0; j < selector.basicSelectors.length(); ++j) {
			auto &bs = selector.basicSelectors[j];
			QString selector = QString("%1%2%3%4 ").arg(bs.elementName, toString(bs.ids), toString(bs.attributeSelectors), toString(bs.pseudos));
			if (selector.length() > 1) {
				str.append(selector);
			}
		}
	}
	if (str.isEmpty()) {
		str.append("* ");
	}
	str.append("{\n");
	return str;
}

QString toString(const QVariant &variant)
{
	switch (variant.type()) {
	case QVariant::Rect: {
		QRect rect = variant.toRect();
		return QString("%1 %2 %3 %4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
	}
	case QVariant::RectF: {
		QRectF rect = variant.toRectF();
		return QString("%1 %2 %3 %4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
	}
	case QVariant::Size: {
		QSize size = variant.toSize();
		return QString("%1 %2").arg(size.width()).arg(size.height());
	}
	case QVariant::SizeF: {
		QSizeF size = variant.toSizeF();
		return QString("%1 %2").arg(size.width()).arg(size.height());
	}
	case QVariant::SizePolicy: {
		QSizePolicy sizePolicy = variant.value<QSizePolicy>();
		QMetaEnum me = QMetaEnum::fromType<QSizePolicy::Policy>();
		return QString("%1 %2").arg(me.valueToKey(sizePolicy.horizontalPolicy())).arg(me.valueToKey(sizePolicy.verticalPolicy()));
	}
	default:
		return variant.toString();
	}
}

void listStyleRules(QStyleSheetStyle *style, QWidget *widget, QTextEdit *props, QTextEdit *css)
{
	struct LessThan {
		bool operator()(const QCss::StyleRule &a, const QCss::StyleRule &b) { return a.order > b.order; }
	};

	QString propsText;
	const QMetaObject *mo = widget->metaObject();
	for (int i = 0, count = mo->propertyCount(); i < count; ++i) {
		QMetaProperty mp = mo->property(i);
		propsText.append(QString("%1: %2\n").arg(mp.name(), toString(widget->property(mp.name()))));
	}
	for (const QByteArray &name : widget->dynamicPropertyNames()) {
		propsText.append(QString("%1: %2\n").arg(name.constData(), toString(widget->property(name.constData()))));
	}
	props->setPlainText(propsText);

	QString cssText;
	QVector<QCss::StyleRule> styleRules = getStyleRules(widget, style);
	// qSort(styleRules.begin(), styleRules.end(), LessThan());
	for (int i = styleRules.length() - 1; i >= 0; --i) {
		auto &styleRule = styleRules[i];
		QString selector = toString(styleRule.selectors);
		cssText.append(!selector.isEmpty() ? selector : "*");
		for (int j = 0; j < styleRule.declarations.length(); ++j) {
			auto &declaration = styleRule.declarations[j];
			cssText.append(QString("    %1:%2;\n").arg(declaration.d->property, toString(declaration.d->values)));
		}
		cssText.append("}\n\n");
	}
	css->setPlainText(cssText);
}

enum DataRole { TI_DataRole_Type = Qt::UserRole + 1, TI_DataRole_Object = Qt::UserRole + 2 };
enum DataType { TI_DataType_Widget, TI_DataType_Layout, TI_DataType_Spacer };

void setItemData(QTreeWidgetItem *item, DataType type, void *data)
{
	item->setData(0, TI_DataRole_Type, type);
	item->setData(0, TI_DataRole_Object, QVariant::fromValue(data));
}
template<typename Type> Type *getItemData(QTreeWidgetItem *)
{
	return nullptr;
}
template<> QWidget *getItemData<QWidget>(QTreeWidgetItem *item)
{
	if (item->data(0, TI_DataRole_Type).toInt() == TI_DataType_Widget) {
		return (QWidget *)item->data(0, TI_DataRole_Object).value<void *>();
	}
	return nullptr;
}
template<> QLayout *getItemData<QLayout>(QTreeWidgetItem *item)
{
	if (item->data(0, TI_DataRole_Type).toInt() == TI_DataType_Layout) {
		return (QLayout *)item->data(0, TI_DataRole_Object).value<void *>();
	}
	return nullptr;
}
template<> QSpacerItem *getItemData<QSpacerItem>(QTreeWidgetItem *item)
{
	if (item->data(0, TI_DataRole_Type).toInt() == TI_DataType_Spacer) {
		return (QSpacerItem *)item->data(0, TI_DataRole_Object).value<void *>();
	}
	return nullptr;
}

void listChildren(QLayout *layout, QTreeWidget *treeWidget, QTreeWidgetItem *parent = nullptr)
{
	QTreeWidgetItem *layoutItem = nullptr;
	if (parent) {
		layoutItem = new QTreeWidgetItem(parent, QStringList() << QString("%1: %2").arg(layout->metaObject()->className(), layout->objectName()));
		setItemData(layoutItem, TI_DataType_Layout, layout);
	} else {
		layoutItem = new QTreeWidgetItem(QStringList() << QString("%1: %2").arg(layout->metaObject()->className(), layout->objectName()));
		setItemData(layoutItem, TI_DataType_Layout, layout);
		treeWidget->addTopLevelItem(layoutItem);
	}

	for (int i = 0, count = layout->count(); i < count; ++i) {
		QLayoutItem *li = layout->itemAt(i);
		if (QSpacerItem *si = dynamic_cast<QSpacerItem *>(li)) {
			QTreeWidgetItem *spacerItem = new QTreeWidgetItem(layoutItem, QStringList() << QString("QSpacerItem: "));
			setItemData(spacerItem, TI_DataType_Spacer, si);
		} else if (QLayout *l = dynamic_cast<QLayout *>(li); l) {
			listChildren(l, treeWidget, layoutItem);
		}
	}
}
void listChildren(QWidget *widget, QTreeWidget *treeWidget, QTreeWidgetItem *parent = nullptr)
{
	for (QObject *child : widget->children()) {
		if (child->isWidgetType()) {
			QWidget *childWidget = dynamic_cast<QWidget *>(child);
			if (parent) {
				QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList() << QString("%1: %2").arg(childWidget->metaObject()->className(), childWidget->objectName()));
				setItemData(item, TI_DataType_Widget, childWidget);
				listChildren(childWidget, treeWidget, item);
			} else {
				QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << QString("%1: %2").arg(childWidget->metaObject()->className(), childWidget->objectName()));
				setItemData(item, TI_DataType_Widget, childWidget);
				treeWidget->addTopLevelItem(item);
				listChildren(childWidget, treeWidget, item);
			}
		} else if (QLayout *layout = dynamic_cast<QLayout *>(child)) {
			listChildren(layout, treeWidget, parent);
		}
	}
}
}

PLSDevTool::PLSDevTool(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper), ui(new Ui::PLSDevTool)
{
	setHasCloseButton(false);
	installNativeEventFilter();
	dpiHelper.setCss(this, {PLSCssIndex::CommonDialog});
	dpiHelper.setStyleSheet(this, "PLSDevTool QLabel { font-size: /*hdpi*/ 14px; color: white; }");
	dpiHelper.setMinimumSize(this, {700, 700});
	dpiHelper.setInitSize(this, {700, 700});
	ui->setupUi(content());
	QMetaObject::connectSlotsByName(this);
	ui->pickWidget->installEventFilter(this);
	dpiHelper.setMinimumHeight(ui->pickWidget, 40);
	connect(ui->pickedWidgetStack, &QListWidget::currentRowChanged, [this](int currentRow) {
		if (currentRow < 0) {
			return;
		}

		QListWidgetItem *item = ui->pickedWidgetStack->item(currentRow);
		currentWidget = (QWidget *)item->data(Qt::UserRole).value<void *>();

		ui->selectedWidgetTree->clear();
		listChildren(currentWidget, ui->selectedWidgetTree);

		QStyle *style = currentWidget->style();
		QStyleSheetStyle *styleSheetStyle = qt_styleSheet(style);
		if (styleSheetStyle) {
			listStyleRules(styleSheetStyle, currentWidget, ui->textEditProperties, ui->pickedWidgetStyle);
		}
	});

	connect(ui->selectedWidgetTree, &QTreeWidget::itemSelectionChanged, [this]() {
		QList<QTreeWidgetItem *> selectedItems = ui->selectedWidgetTree->selectedItems();
		if (selectedItems.isEmpty()) {
			return;
		}

		QTreeWidgetItem *item = selectedItems.first();
		currentWidget = getItemData<QWidget>(item);
		if (currentWidget) {
			QStyle *style = currentWidget->style();
			QStyleSheetStyle *styleSheetStyle = qt_styleSheet(style);
			if (styleSheetStyle) {
				listStyleRules(styleSheetStyle, currentWidget, ui->textEditProperties, ui->pickedWidgetStyle);
			}
		} else if (QLayout *layout = getItemData<QLayout>(item); layout) {
			QString text;
			text.append(QString("contentsMargins {\n"));
			QMargins margins = layout->contentsMargins();
			text.append(QString("    left: %1\n").arg(margins.left()));
			text.append(QString("    top: %1\n").arg(margins.top()));
			text.append(QString("    right: %1\n").arg(margins.right()));
			text.append(QString("    bottom: %1\n").arg(margins.bottom()));
			text.append(QString("}\n\n"));

			if (QBoxLayout *bl = dynamic_cast<QBoxLayout *>(layout); bl) {
				text.append(QString("spacing: %1\n").arg(bl->spacing()));
			} else if (QGridLayout *gl = dynamic_cast<QGridLayout *>(layout); gl) {
				text.append(QString("horizontalSpacing: %1\n").arg(gl->horizontalSpacing()));
				text.append(QString("verticalSpacing: %1\n\n").arg(gl->verticalSpacing()));
				text.append(QString("rowMinimumHeight {\n"));
				for (int row = 0; row < gl->rowCount(); ++row) {
					text.append(QString("%1: %2\n").arg(row + 1).arg(gl->rowMinimumHeight(row)));
				}
				text.append(QString("}\n\n"));
				text.append(QString("columnMinimumWidth {\n"));
				for (int column = 0; column < gl->columnCount(); ++column) {
					text.append(QString("%1: %2\n").arg(column + 1).arg(gl->columnMinimumWidth(column)));
				}
				text.append(QString("}\n"));
			} else if (QFormLayout *fl = dynamic_cast<QFormLayout *>(layout); fl) {
				text.append(QString("horizontalSpacing: %1\n").arg(fl->horizontalSpacing()));
				text.append(QString("verticalSpacing: %1\n").arg(fl->verticalSpacing()));
			}
			ui->pickedWidgetStyle->setPlainText(text);
		} else if (QSpacerItem *spacer = getItemData<QSpacerItem>(item); spacer) {
			QMetaEnum me = QMetaEnum::fromType<QSizePolicy::Policy>();

			QString text;
			text.append(QString("width: %1\n").arg(spacer->sizeHint().width()));
			text.append(QString("height: %1\n").arg(spacer->sizeHint().height()));
			text.append(QString("horizontalPolicy: %1\n").arg(me.valueToKey(spacer->sizePolicy().horizontalPolicy())));
			text.append(QString("verticalPolicy: %1\n").arg(me.valueToKey(spacer->sizePolicy().verticalPolicy())));
			ui->pickedWidgetStyle->setPlainText(text);
		} else {
			ui->pickedWidgetStyle->setPlainText(QString());
		}
	});
}

PLSDevTool::~PLSDevTool()
{
	delete ui;
}

bool PLSDevTool::canPick() const
{
	return ui->canPick->isChecked();
}

bool PLSDevTool::isPtInPickButton() const
{
	return ui->pickWidget->rect().contains(ui->pickWidget->mapFromGlobal(QCursor::pos()));
}

void PLSDevTool::pickingWidget()
{
	ui->pickWidget->setText("Picking ...");
}

void PLSDevTool::pickWidget(bool containsSelf)
{
	ui->pickWidget->setText("Pick Widget");

	QWidget *widget = QApplication::widgetAt(QCursor::pos());
	if (!widget || (pickedWidget == widget) || !containsSelf && (pls_get_toplevel_view(widget) == this)) {
		return;
	}

	pickedWidget = currentWidget = widget;
	if (!styleSheets.contains(widget)) {
		styleSheets[widget] = widget->styleSheet();
	}

	ui->pickedWidgetStack->clear();
	for (QWidget *w = widget; w; w = w->parentWidget()) {
		QListWidgetItem *item = new QListWidgetItem(QString("%1: %2").arg(w->metaObject()->className(), w->objectName()));
		item->setData(Qt::UserRole, QVariant::fromValue<void *>(w));
		ui->pickedWidgetStack->addItem(item);
	}

	QStyle *style = widget->style();
	QStyleSheetStyle *styleSheetStyle = qt_styleSheet(style);
	if (styleSheetStyle) {
		listStyleRules(styleSheetStyle, widget, ui->textEditProperties, ui->pickedWidgetStyle);
	}
}

void PLSDevTool::setParent(QWidget *parent)
{
	if (parentWidget() == parent) {
		return;
	}

	if (parent) {
		connect(parent, &QWidget::destroyed, this, [this]() { setParent(nullptr); });
	}

	QRect geometry = this->geometry();
	bool visible = isVisible();
	PLSDialogView::setParent(parent, windowFlags());
	setVisible(visible);
	PLSDpiHelper::checkStatusChanged(static_cast<PLSWidgetDpiAdapter *>(this));
	setGeometry(geometry);
}

void PLSDevTool::on_clear_clicked()
{
	ui->modifyWidgetStyle->setPlainText(QString());
	if (currentWidget) {
		currentWidget->setStyleSheet(QString());
	}
}

void PLSDevTool::on_replace_clicked()
{
	QString styledSheet = ui->modifyWidgetStyle->toPlainText();
	if (currentWidget) {
		currentWidget->setStyleSheet(styledSheet);
	}
}

void PLSDevTool::on_append_clicked()
{
	QString styledSheet = ui->modifyWidgetStyle->toPlainText();
	if (currentWidget) {
		currentWidget->setStyleSheet(styleSheets[pickedWidget] + styledSheet);
	}
}

void PLSDevTool::on_reset_clicked()
{
	if (currentWidget) {
		currentWidget->setStyleSheet(styleSheets[pickedWidget]);
	}
}

void PLSDevTool::hideEvent(QHideEvent *event)
{
	PLSDialogView::hideEvent(event);
	setParent(nullptr);
}

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(BAND_LOGIN, "en-US")

bool obs_module_load(void)
{
	QWidget *widget = pls_get_main_view();
	devTool = new PLSDevTool(widget);

	QMetaObject::invokeMethod(
		widget, [=]() { devTool->show(); }, Qt::QueuedConnection);
	QObject::connect(
		qApp, &QApplication::focusChanged, widget, []() { devTool->setParent(QApplication::activeModalWidget()); }, Qt::QueuedConnection);
	return true;
}

void obs_module_unload(void)
{
	delete devTool;
	devTool = nullptr;
}

const char *obs_module_name(void)
{
	return DEVTOOL;
}

const char *obs_module_description(void)
{
	return DEVTOOL;
}
