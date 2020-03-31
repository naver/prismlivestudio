#include "PLSMenu.hpp"

#include <QAction>
#include <QMenu>
#include <QPushButton>
#include <QActionEvent>
#include <QVariant>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QApplication>
#include <QKeyEvent>
#include <QList>
#include <QIcon>
#include <QEventLoop>
#include <QPushButton>
#include <QWidgetAction>

#include <Windows.h>

#include "log.h"
#include "frontend-api.h"
#include "platform.hpp"
#include "ChannelCommonFunctions.h"

namespace {
const unsigned int altBit = 0x20000000;
const unsigned int keydownBit = 0x40000000;

uint g_ActionUserdata = QObject::registerUserData();
QList<PLSPopupMenu *> g_menuList;
PLSPopupMenu *g_activeMenu = nullptr;
QPushButton *g_menuButton = nullptr;

class ActionUserdata : public QObject, public QObjectUserData {
	Q_OBJECT
public:
	ActionUserdata(bool isMenuAction, PLSPopupMenu *menu, PLSPopupMenu *submenu, PLSPopupMenuItem *menuItem) : m_isMenuAction(isMenuAction), m_menu(menu), m_submenu(submenu), m_menuItem(menuItem)
	{
		if (menu) {
			QObject::connect(menu, &QObject::destroyed, this, &ActionUserdata::clearMenu);
		}

		if (m_submenu) {
			QObject::connect(m_submenu, &QObject::destroyed, this, &ActionUserdata::clearMenuItem);
		}

		if (m_menuItem) {
			QObject::connect(m_menuItem, &QObject::destroyed, this, &ActionUserdata::clearMenuItem);
		}
	}
	virtual ~ActionUserdata() {}

public:
	static ActionUserdata *getActionUserData(QAction *action)
	{
		if (action) {
			return dynamic_cast<ActionUserdata *>(action->userData(g_ActionUserdata));
		}
		return nullptr;
	}

public:
	void showShortcutKey()
	{
		if (m_menuItem && m_menuItem->type() != PLSPopupMenuItem::Type::Separator) {
			dynamic_cast<PLSPopupMenuItemContent *>(m_menuItem)->showShortcutKey();
		}
	}
	void shortcutKeyPress(int key)
	{
		if (m_menuItem && m_menuItem->type() != PLSPopupMenuItem::Type::Separator) {
			dynamic_cast<PLSPopupMenuItemContent *>(m_menuItem)->shortcutKeyPress(key);
		}
	}

public slots:
	void clearMenu() { m_menu = nullptr; }
	void clearSubmenu() { m_submenu = nullptr; }
	void clearMenuItem() { m_menuItem = nullptr; }

public:
	bool m_isMenuAction;
	PLSPopupMenu *m_menu;
	PLSPopupMenu *m_submenu;
	PLSPopupMenuItem *m_menuItem;
};

class HookNativeEvent {
public:
	HookNativeEvent()
	{
		m_keyboardHook = SetWindowsHookExW(WH_KEYBOARD, &keyboardHookProc, GetModuleHandleW(nullptr), GetCurrentThreadId());
		m_mouseHook = SetWindowsHookExW(WH_MOUSE, &mouseHookProc, GetModuleHandleW(nullptr), GetCurrentThreadId());
	}
	~HookNativeEvent()
	{
		if (m_keyboardHook) {
			UnhookWindowsHookEx(m_keyboardHook);
			m_keyboardHook = nullptr;
		}

		if (m_mouseHook) {
			UnhookWindowsHookEx(m_mouseHook);
			m_mouseHook = nullptr;
		}
	}

protected:
	static LRESULT CALLBACK keyboardHookProc(_In_ int code, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		if ((code < 0) || g_menuList.isEmpty() || !(lParam & keydownBit)) {
			return CallNextHookEx(nullptr, code, wParam, lParam);
		}

		if (wParam == VK_MENU) {
			for (PLSPopupMenu *menu : g_menuList) {
				for (QAction *action : menu->actions()) {
					if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData && userData->m_menuItem) {
						userData->showShortcutKey();
					}
				}
			}
		} else if (((wParam >= 0x30) && (wParam <= 0x39) || (wParam >= 0x41) && (wParam <= 0x5A))) {
			for (QAction *action : activeMenu()->actions()) {
				if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData && userData->m_menuItem) {
					userData->shortcutKeyPress(wParam);
				}
			}
		} else if (wParam == VK_LEFT) {
			activeMenu()->leaveSubmenu();
		} else if (wParam == VK_RIGHT) {
			activeMenu()->enterSubmenu();
		} else if (wParam == VK_UP) {
			activeMenu()->hoveredPrevOrLastAction();
		} else if (wParam == VK_DOWN) {
			activeMenu()->hoveredNextOrFirstAction();
		} else if (wParam == VK_RETURN) {
			PLSPopupMenu *menu = activeMenu();
			menu->triggeredSelectedOrFirstAction();
		} else if (wParam == VK_ESCAPE) {
			hideAllPopupMenu();
		}
		return CallNextHookEx(nullptr, code, wParam, lParam);
	}
	static LRESULT CALLBACK mouseHookProc(_In_ int code, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
#if 0
		if (wParam == WM_LBUTTONDOWN) {
			QWidget *widget = QApplication::widgetAt(QCursor::pos());
			if (widget) {
				for (QWidget* p = widget; p; p = p->parentWidget()) {
					qDebug() << p->metaObject()->className() << p->objectName();
				}
			}
		}
#endif

		if ((code < 0) || g_menuList.isEmpty()) {
			return CallNextHookEx(nullptr, code, wParam, lParam);
		}

		if ((wParam == WM_LBUTTONDOWN) || (wParam == WM_LBUTTONUP) || (wParam == WM_LBUTTONDBLCLK) || (wParam == WM_RBUTTONDOWN) || (wParam == WM_RBUTTONUP) || (wParam == WM_RBUTTONDBLCLK) ||
		    (wParam == WM_MBUTTONDOWN) || (wParam == WM_MBUTTONUP) || (wParam == WM_MBUTTONDBLCLK) || (wParam == WM_NCLBUTTONDOWN) || (wParam == WM_NCLBUTTONUP) ||
		    (wParam == WM_NCLBUTTONDBLCLK) || (wParam == WM_NCRBUTTONDOWN) || (wParam == WM_NCRBUTTONUP) || (wParam == WM_NCRBUTTONDBLCLK) || (wParam == WM_NCMBUTTONDOWN) ||
		    (wParam == WM_NCMBUTTONUP) || (wParam == WM_NCMBUTTONDBLCLK)) {
			LPMOUSEHOOKSTRUCT mhs = (LPMOUSEHOOKSTRUCT)lParam;
			if (!isInButton(mhs->pt) && !findMenu(mhs->pt)) {
				hideAllPopupMenu();
			}
		}

		return CallNextHookEx(nullptr, code, wParam, lParam);
	}

	static PLSPopupMenu *activeMenu()
	{
		if (g_activeMenu) {
			return g_activeMenu;
		}

		g_activeMenu = g_menuList.last();
		return g_activeMenu;
	}
	static PLSPopupMenu *findMenu(const POINT &pt)
	{
		for (PLSPopupMenu *menu : g_menuList) {
			RECT rc;
			GetWindowRect((HWND)menu->winId(), &rc);
			if (PtInRect(&rc, pt)) {
				return menu;
			}
		}
		return nullptr;
	}
	static void hideAllPopupMenu()
	{
		for (PLSPopupMenu *menu : g_menuList) {
			menu->hide();
		}
	}
	static bool isInButton(const POINT &pt)
	{
		if (!g_menuButton) {
			return false;
		}

		QRect rc(g_menuButton->mapToGlobal(QPoint(0, 0)), g_menuButton->size());
		return rc.contains(pt.x, pt.y);
	}

	HHOOK m_keyboardHook;
	HHOOK m_mouseHook;
};

void installNativeEventFilter()
{
	static std::unique_ptr<HookNativeEvent> hookNativeEvent;
	if (!hookNativeEvent) {
		hookNativeEvent.reset(new HookNativeEvent);
	}
}

void flushStyle(QAction *action)
{
	if (action) {
		if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData && userData->m_menuItem) {
			pls_flush_style(userData->m_menuItem);
		}
	}
}

void setActionUserData(QAction *action, bool isMenuAction, PLSPopupMenu *menu, PLSPopupMenu *submenu, PLSPopupMenuItem *menuItem)
{
	ActionUserdata *userData = dynamic_cast<ActionUserdata *>(action->userData(g_ActionUserdata));
	if (!userData) {
		action->setUserData(g_ActionUserdata, new ActionUserdata(isMenuAction, menu, submenu, menuItem));
	} else {
		if (menu) {
			QObject::connect(menu, &QObject::destroyed, userData, &ActionUserdata::clearMenu);
			userData->m_menu = menu;
		}

		if (submenu) {
			QObject::connect(submenu, &QObject::destroyed, userData, &ActionUserdata::clearSubmenu);
			userData->m_submenu = submenu;
		}

		if (menuItem) {
			QObject::connect(menuItem, &QObject::destroyed, userData, &ActionUserdata::clearMenuItem);
			userData->m_menuItem = menuItem;
		}
	}
}

bool isMenuAction(QAction *action)
{
	ActionUserdata *userData = ActionUserdata::getActionUserData(action);
	if (userData && userData->m_isMenuAction) {
		return true;
	}
	return false;
}

QRect actionBoundingRect(QAction *action)
{
	ActionUserdata *userData = ActionUserdata::getActionUserData(action);
	if (userData) {
		return QRect(QPoint(userData->m_menu->mapToGlobal(QPoint(0, 0)).x(), userData->m_menuItem->mapToGlobal(QPoint(0, 0)).y()),
			     QSize(userData->m_menu->width(), userData->m_menuItem->height()));
	}
	return QRect(0, 0, 0, 0);
}

int actionIndex(const QList<QAction *> &actions, QAction *action)
{
	return action ? actions.indexOf(action) : -1;
}

void popupSubmenu(PLSPopupMenu *menu, QAction *selected)
{
	for (QAction *action : menu->actions()) {
		ActionUserdata *userData = ActionUserdata::getActionUserData(action);
		if (!userData || !userData->m_isMenuAction) {
			continue;
		}

		if (action == selected) {
			userData->m_submenu->popup(actionBoundingRect(selected).topRight() + QPoint(1, 0), action);
		} else {
			userData->m_submenu->hide();
		}
	}
}

void hideSubmenu(PLSPopupMenu *menu)
{
	if (menu) {
		for (QAction *action : menu->actions()) {
			ActionUserdata *userData = ActionUserdata::getActionUserData(action);
			if (userData && userData->m_isMenuAction && userData->m_submenu) {
				hideSubmenu(userData->m_submenu);
				userData->m_submenu->clearSelectedAction();
				userData->m_submenu->hide();
			}
		}
	}
}

void hideMenuByUserData(ActionUserdata *userData)
{
	if (userData && userData->m_menu) {
		userData->m_menu->hide();
		userData = ActionUserdata::getActionUserData(userData->m_menu->menuAction());
		hideMenuByUserData(userData);
	}
}

PLSPopupMenu *newSubMenu(PLSPopupMenu *menu, QAction *action)
{
	PLSPopupMenu *submenu = new PLSPopupMenu(action, menu);
	submenu->setOwner(menu);
	pls_flush_style(submenu);
	return submenu;
}

PLSPopupMenuItem *newMenuItem(PLSPopupMenu *menu, QAction *action)
{
	if (QWidgetAction *widgetAction = dynamic_cast<QWidgetAction *>(action)) {
		return new PLSPopupMenuItemWidgetAction(widgetAction, menu);
	} else if (action->isSeparator()) {
		return new PLSPopupMenuItemSeparator(action, menu);
	} else if (action->menu()) {
		return new PLSPopupMenuItemMenu(newSubMenu(menu, action)->menuAction(), menu);
	} else if (!isMenuAction(action)) {
		return new PLSPopupMenuItemAction(action, menu);
	} else {
		return new PLSPopupMenuItemMenu(action, menu);
	}
}

bool isPLSPopupMenuType(QObject *object)
{
	if (!object || !object->isWidgetType()) {
		return false;
	}

	QWidget *widget = dynamic_cast<QWidget *>(object);
	if (dynamic_cast<PLSPopupMenu *>(widget) || dynamic_cast<PLSPopupMenuItem *>(widget)) {
		return true;
	} else if (isPLSPopupMenuType(widget->parentWidget())) {
		return true;
	}
	return false;
}

bool isAltPressed()
{
	return GetKeyState(VK_MENU) < 0;
}

QString decodeMenuItemText(const QString &text, int &shortcut)
{
	shortcut = 0;

	if (text.length() < 2) {
		return text;
	}

	if (text.startsWith('&')) {
		shortcut = text.at(1).toUpper().unicode();
		if (isAltPressed()) {
			return QString("<u>%1</u>%2").arg(text.at(1)).arg(text.mid(2));
		}
		return text.mid(1);
	}

	QStringList parts = text.split('&', QString::SkipEmptyParts);
	if (parts.count() == 2) {
		shortcut = parts.at(1).at(0).toUpper().unicode();
		if (isAltPressed()) {
			return QString("%1<u>%2</u>%3").arg(parts.at(0)).arg(parts.at(1).at(0)).arg(parts.at(1).mid(1));
		}
		return parts.at(0) + parts.at(1);
	}
	return text;
}

void setMenuItemProperty(QAction *action, const char *name, bool value)
{
	if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData && userData->m_menuItem) {
		pls_flush_style_recursive(userData->m_menuItem, name, value);
	}
}

QAction *prevAction(const QList<QAction *> &actions, QAction *current)
{
	int index = actions.indexOf(current), count = actions.count();
	if ((index > 0) && (index < count)) {
		return actions.at(index - 1);
	}
	return actions.last();
}

QAction *prevValidAction(const QList<QAction *> &actions, QAction *current)
{
	QAction *_current = current;
	QAction *prev = prevAction(actions, _current);
	while (prev && (prev != current) && (prev->isSeparator() || !prev->isVisible())) {
		prev = prevAction(actions, _current = prev);
	}

	if (!prev->isSeparator() && prev->isVisible()) {
		return prev;
	}
	return nullptr;
}

QAction *nextAction(const QList<QAction *> &actions, QAction *current)
{
	int index = actions.indexOf(current), lastIndex = actions.count() - 1;
	if ((index >= 0) && (index < lastIndex)) {
		return actions.at(index + 1);
	}
	return actions.first();
}

QAction *nextValidAction(const QList<QAction *> &actions, QAction *current)
{
	QAction *_current = current;
	QAction *next = nextAction(actions, _current);
	while (next && (next != current) && (next->isSeparator() || !next->isVisible())) {
		next = nextAction(actions, _current = next);
	}

	if (!next->isSeparator() && next->isVisible()) {
		return next;
	}
	return nullptr;
}

void hoveredAction(QAction *action, bool enterSubmenu)
{
	if (action) {
		if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData && userData->m_menu) {
			userData->m_menu->hovered(action);
			if (enterSubmenu && userData->m_isMenuAction && userData->m_submenu) {
				g_activeMenu = userData->m_submenu;
				g_activeMenu->hoveredNextOrFirstAction();
			}
		}
	}
}

void triggeredAction(QAction *action)
{
	if (action) {
		if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData && userData->m_menu) {
			userData->m_menu->triggered(action);
		}
	}
}
}

bool PLSMenu::event(QEvent *event)
{
	switch (event->type()) {
	case QEvent::ActionAdded: {
		QActionEvent *e = dynamic_cast<QActionEvent *>(event);
		emit actionAdded(e->action(), e->before());
		break;
	}
	case QEvent::ActionRemoved: {
		emit actionRemoved(dynamic_cast<QActionEvent *>(event)->action());
		break;
	}
	}

	return QMenu::event(event);
}

PLSPopupMenuItem::PLSPopupMenuItem(QAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper) : QFrame(menu), m_action(action), m_menu(menu)
{
	setObjectName(action->objectName());
	setActionUserData(action, false, menu, nullptr, this);
	setVisible(action->isVisible());
	QObject::connect(m_action, &QAction::changed, this, [this]() { actionChanged(); });
}

PLSPopupMenuItem::~PLSPopupMenuItem() {}

void PLSPopupMenuItem::actionHovered()
{
	m_action->hover();
}

void PLSPopupMenuItem::actionTriggered()
{
	m_action->trigger();
}

void PLSPopupMenuItem::actionChanged()
{
	setVisible(m_action->isVisible());
}

QString PLSPopupMenuItem::getText(int &shortcutKey, const QFont &font, int width) const
{
	QFontMetrics fm(font);
	QString original = decodeMenuItemText(m_action->text(), shortcutKey);
	QString elidedText = fm.elidedText(original, Qt::ElideRight, width);
	return elidedText;
}

PLSPopupMenuItemSeparator::PLSPopupMenuItemSeparator(QAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper) : PLSPopupMenuItem(action, menu, dpiHelper) {}

PLSPopupMenuItemSeparator::~PLSPopupMenuItemSeparator() {}

PLSPopupMenuItem::Type PLSPopupMenuItemSeparator::type() const
{
	return Type::Separator;
}

PLSPopupMenuItemContent::PLSPopupMenuItemContent(QAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper)
	: PLSPopupMenuItem(action, menu, dpiHelper), m_hasIcon(false), m_isLeftIcon(true), m_isElidedTextProcessed(false), m_layout(), m_icon(), m_text(), m_shortcutKey()
{
	m_layout = new QHBoxLayout(this);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);

	m_icon = new QLabel(this);
	m_icon->setProperty("menuItemRole", "icon");
	m_icon->setVisible(m_hasIcon);

	m_text = new QLabel(decodeMenuItemText(action->text(), m_shortcutKey), this);
	m_text->setProperty("menuItemRole", action->isEnabled() ? "text" : "disabled-text");
	m_text->installEventFilter(this);

	m_layout->addWidget(m_icon);
	m_layout->addWidget(m_text);
	m_layout->addStretch(1);

	if (m_menu->toolTipsVisible()) {
		setAttribute(Qt::WA_AlwaysShowToolTips);
		setToolTip(m_action->toolTip());
	}

	setMouseTracking(true);
	setEnabled(action->isEnabled());
	pls_flush_style_recursive(this, "checked", action->isCheckable() && action->isChecked());

	connect(menu, &PLSPopupMenu::hovered, this, [this](QAction *action) { pls_flush_style_recursive(this, "selected", m_action == action); });
	connect(menu, &PLSPopupMenu::triggered, this, [this](QAction *action) { pls_flush_style_recursive(this, "selected", m_action == action); });

	dpiHelper.notifyDpiChanged(this, [=]() {
		m_isElidedTextProcessed = false;
		m_text->setText(decodeMenuItemText(m_action->text(), m_shortcutKey));
	});
}

PLSPopupMenuItemContent::~PLSPopupMenuItemContent() {}

void PLSPopupMenuItemContent::showShortcutKey()
{
	m_text->setText(decodeMenuItemText(m_action->text(), m_shortcutKey));
}

void PLSPopupMenuItemContent::shortcutKeyPress(int key)
{
	if (m_shortcutKey == key) {
		m_menu->triggered(m_action);
	}
}

bool PLSPopupMenuItemContent::getHasIcon() const
{
	return m_hasIcon;
}

void PLSPopupMenuItemContent::setHasIcon(bool hasIcon)
{
	m_hasIcon = hasIcon;
	m_icon->setVisible(hasIcon);
}

bool PLSPopupMenuItemContent::getIsLeftIcon() const
{
	return m_isLeftIcon;
}

void PLSPopupMenuItemContent::setIsLeftIcon(bool isLeftIcon)
{
	if (m_isLeftIcon == isLeftIcon) {
		return;
	}

	m_isLeftIcon = isLeftIcon;
	if (m_isLeftIcon) {
		m_layout->removeWidget(m_icon);
		m_layout->insertWidget(m_layout->indexOf(m_text), m_icon);
	} else {
		m_layout->removeWidget(m_icon);
		m_layout->insertWidget(m_layout->indexOf(m_text) + 1, m_icon);
	}
}

void PLSPopupMenuItemContent::actionChanged()
{
	PLSPopupMenuItem::actionChanged();
	setProperty("checked", m_action->isCheckable() && m_action->isChecked());

	if (m_menu->toolTipsVisible()) {
		setAttribute(Qt::WA_AlwaysShowToolTips);
		setToolTip(m_action->toolTip());
	}

	m_isElidedTextProcessed = false;
	m_text->setText(decodeMenuItemText(m_action->text(), m_shortcutKey));
	m_text->setProperty("menuItemRole", m_action->isEnabled() ? "text" : "disabled-text");
	setEnabled(m_action->isEnabled());
}

bool PLSPopupMenuItemContent::eventFilter(QObject *watched, QEvent *event)
{
	bool result = PLSPopupMenuItem::eventFilter(watched, event);
	if (watched == m_text && event->type() == QEvent::Resize && !m_isElidedTextProcessed) {
		m_text->setText(getText(m_shortcutKey, m_text->font(), dynamic_cast<QResizeEvent *>(event)->size().width()));
		m_isElidedTextProcessed = true;
	}
	return result;
}

void PLSPopupMenuItemContent::enterEvent(QEvent *event)
{
	m_menu->hovered(m_action);
	PLSPopupMenuItem::enterEvent(event);
}

void PLSPopupMenuItemContent::mousePressEvent(QMouseEvent *event)
{
	PLSPopupMenuItem::mousePressEvent(event);
}

void PLSPopupMenuItemContent::mouseReleaseEvent(QMouseEvent *event)
{
	m_menu->triggered(m_action);
	PLSPopupMenuItem::mouseReleaseEvent(event);
}

PLSPopupMenuItemAction::PLSPopupMenuItemAction(QAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper) : PLSPopupMenuItemContent(action, menu, dpiHelper)
{
	m_shortcut = new QLabel(action->shortcut().toString(), this);
	m_shortcut->setProperty("menuItemRole", "shortcut");

	m_layout->addWidget(m_shortcut);
}

PLSPopupMenuItemAction::~PLSPopupMenuItemAction() {}

PLSPopupMenuItem::Type PLSPopupMenuItemAction::type() const
{
	return Type::Action;
}

void PLSPopupMenuItemAction::actionHovered()
{
	hideSubmenu(m_menu);
	PLSPopupMenuItemContent::actionHovered();
}

void PLSPopupMenuItemAction::actionTriggered()
{
	m_menu->onHideAllPopupMenu();
	PLSPopupMenuItemContent::actionTriggered();
}

void PLSPopupMenuItemAction::actionChanged()
{
	PLSPopupMenuItemContent::actionChanged();

	m_shortcut->setText(m_action->shortcut().toString());
	pls_flush_style_recursive(this);
}

void PLSPopupMenuItemAction::leaveEvent(QEvent *event)
{
	setProperty("selected", false);
	pls_flush_style_recursive(this);
	PLSPopupMenuItemContent::leaveEvent(event);
}

void PLSPopupMenuItemAction::mouseReleaseEvent(QMouseEvent *event)
{
	hideMenuByUserData(ActionUserdata::getActionUserData(m_action));
	PLSPopupMenuItemContent::mouseReleaseEvent(event);
}

PLSPopupMenuItemWidgetAction::PLSPopupMenuItemWidgetAction(QWidgetAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper) : PLSPopupMenuItem(action, menu, dpiHelper)
{
	QWidget *widget = action->defaultWidget();
	widget->setParent(this);
	pls_flush_style_recursive(this);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	layout->addWidget(widget);
	widget->show();
}

PLSPopupMenuItemWidgetAction::~PLSPopupMenuItemWidgetAction() {}

PLSPopupMenuItemWidgetAction::Type PLSPopupMenuItemWidgetAction::type() const
{
	return Type::WidgetAction;
}

void PLSPopupMenuItemWidgetAction::actionHovered() {}

void PLSPopupMenuItemWidgetAction::actionTriggered() {}

void PLSPopupMenuItemWidgetAction::actionChanged()
{
	PLSPopupMenuItem::actionChanged();
	setEnabled(m_action->isEnabled());
	pls_flush_style_recursive(this);
}

void PLSPopupMenuItemWidgetAction::enterEvent(QEvent *event)
{
	PLSPopupMenuItem::enterEvent(event);
}

void PLSPopupMenuItemWidgetAction::leaveEvent(QEvent *event)
{
	//setProperty("selected", false);
	//pls_flush_style_recursive(this);
	PLSPopupMenuItem::leaveEvent(event);
}

void PLSPopupMenuItemWidgetAction::mouseReleaseEvent(QMouseEvent *event)
{
	//hideMenuByUserData(ActionUserdata::getActionUserData(m_action));
	PLSPopupMenuItem::mouseReleaseEvent(event);
}

PLSPopupMenuItemMenu::PLSPopupMenuItemMenu(QAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper) : PLSPopupMenuItemContent(action, menu, dpiHelper)
{
	m_arrow = new QLabel(this);
	m_arrow->setProperty("menuItemRole", "arrow-normal");

	m_layout->addWidget(m_arrow);
}

PLSPopupMenuItemMenu::~PLSPopupMenuItemMenu() {}

PLSPopupMenuItem::Type PLSPopupMenuItemMenu::type() const
{
	return Type::Menu;
}

void PLSPopupMenuItemMenu::actionHovered()
{
	if (m_action->isEnabled()) {
		popupSubmenu(m_menu, m_action);
	} else {
		hideSubmenu(m_menu);
	}

	PLSPopupMenuItemContent::actionHovered();
}

void PLSPopupMenuItemMenu::actionTriggered()
{
	if (m_action->isEnabled()) {
		popupSubmenu(m_menu, m_action);
	} else {
		hideSubmenu(m_menu);
	}

	PLSPopupMenuItemContent::actionTriggered();
}

void PLSPopupMenuItemMenu::actionChanged()
{
	PLSPopupMenuItemContent::actionChanged();

	m_arrow->setProperty("menuItemRole", m_action->isEnabled() ? "arrow-normal" : "arrow-disable");
	pls_flush_style_recursive(this);
}

void PLSPopupMenuItemMenu::mousePressEvent(QMouseEvent *event)
{
	m_arrow->setProperty("menuItemRole", "arrow-click");
	PLSPopupMenuItemContent::mousePressEvent(event);
}

void PLSPopupMenuItemMenu::mouseReleaseEvent(QMouseEvent *event)
{
	m_arrow->setProperty("menuItemRole", m_action->isEnabled() ? "arrow-normal" : "arrow-disable");
	PLSPopupMenuItemContent::mouseReleaseEvent(event);
}

PLSPopupMenu::PLSPopupMenu(QWidget *parent, PLSDpiHelper dpiHelper) : PLSPopupMenu(false, parent, dpiHelper) {}

PLSPopupMenu::PLSPopupMenu(bool toolTipsVisible, QWidget *parent, PLSDpiHelper dpiHelper) : PLSPopupMenu(QString(), toolTipsVisible, parent, dpiHelper) {}

PLSPopupMenu::PLSPopupMenu(const QString &title, bool toolTipsVisible, QWidget *parent, PLSDpiHelper dpiHelper) : PLSPopupMenu(QIcon(), title, toolTipsVisible, parent, dpiHelper) {}

PLSPopupMenu::PLSPopupMenu(const QIcon &icon, const QString &title, bool toolTipsVisible, QWidget *parent, PLSDpiHelper dpiHelper)
	: PLSPopupMenu(icon, title, new QAction(icon, title), toolTipsVisible, parent, dpiHelper)
{
}

PLSPopupMenu::PLSPopupMenu(QAction *menuAction, QWidget *parent, PLSDpiHelper dpiHelper) : PLSPopupMenu(menuAction, menuAction->menu()->toolTipsVisible(), parent, dpiHelper) {}

PLSPopupMenu::PLSPopupMenu(QAction *menuAction, bool toolTipsVisible, QWidget *parent, PLSDpiHelper dpiHelper)
	: PLSPopupMenu(menuAction->icon(), menuAction->text(), menuAction, toolTipsVisible, parent, dpiHelper)
{
	QMenu *menu = menuAction->menu();
	setObjectName(menu->objectName());
	addActions(menu->actions());
	bindPLSMenu(dynamic_cast<PLSMenu *>(menu));
}

PLSPopupMenu::PLSPopupMenu(QMenu *menu, QWidget *parent, PLSDpiHelper dpiHelper) : PLSPopupMenu(menu, menu->toolTipsVisible(), parent, dpiHelper) {}

PLSPopupMenu::PLSPopupMenu(QMenu *menu, bool toolTipsVisible, QWidget *parent, PLSDpiHelper dpiHelper) : PLSPopupMenu(menu->icon(), menu->title(), toolTipsVisible, parent, dpiHelper)
{
	setObjectName(menu->objectName());
	addActions(menu->actions());
	bindPLSMenu(dynamic_cast<PLSMenu *>(menu));
}

PLSPopupMenu::PLSPopupMenu(PLSMenu *menu, QWidget *parent, PLSDpiHelper dpiHelper) : PLSPopupMenu(menu, menu->toolTipsVisible(), parent, dpiHelper) {}

PLSPopupMenu::PLSPopupMenu(PLSMenu *menu, bool toolTipsVisible, QWidget *parent, PLSDpiHelper dpiHelper) : PLSPopupMenu(menu->icon(), menu->title(), toolTipsVisible, parent, dpiHelper)
{
	setObjectName(menu->objectName());
	addActions(menu->actions());
	bindPLSMenu(menu);
}

PLSPopupMenu::PLSPopupMenu(const QIcon &icon, const QString &title, QAction *menuAction, bool toolTipsVisible, QWidget *parent, PLSDpiHelper dpiHelper)
	: WidgetDpiAdapter(parent, Qt::Tool | Qt::FramelessWindowHint), m_owner(), m_menuAction(menuAction), m_selectedAction(), m_layout(), m_toolTipsVisible(toolTipsVisible)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSPopupMenu});

	setActionUserData(m_menuAction, true, nullptr, this, nullptr);
	setMouseTracking(true);
	m_layout = new QVBoxLayout(this);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_menuAction->setIcon(icon);
	m_menuAction->setText(title);

	connect(qApp, &QApplication::applicationStateChanged, this, &PLSPopupMenu::hide);
	installNativeEventFilter();
	SetAlwaysOnTop(this, "PLSPopupMenu", true);

	connect(this, &PLSPopupMenu::triggered, this, [this](QAction *action) {
		m_selectedAction = action;
		if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData && userData->m_menuItem) {
			userData->m_menuItem->actionTriggered();
		}
	});
	connect(this, &PLSPopupMenu::hovered, this, [this](QAction *action) {
		if (g_activeMenu != this) {
			g_activeMenu = this;
		}

		m_selectedAction = action;
		if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData && userData->m_menuItem) {
			userData->m_menuItem->actionHovered();
		}
	});
	connect(this, &PLSPopupMenu::hideAllPopupMenu, this, &PLSPopupMenu::onHideAllPopupMenu, Qt::QueuedConnection);
}

PLSPopupMenu::~PLSPopupMenu()
{
	if (g_activeMenu == this) {
		g_activeMenu = nullptr;
	}
}

PLSPopupMenu *PLSPopupMenu::owner() const
{
	return m_owner;
}

void PLSPopupMenu::setOwner(PLSPopupMenu *owner)
{
	m_owner = owner;
}

QAction *PLSPopupMenu::menuAction() const
{
	return m_menuAction;
}

QString PLSPopupMenu::title() const
{
	return m_menuAction->text();
}

void PLSPopupMenu::setTitle(const QString &title)
{
	m_menuAction->setText(title);
}

QIcon PLSPopupMenu::icon() const
{
	return m_menuAction->icon();
}

void PLSPopupMenu::setIcon(const QIcon &icon)
{
	m_menuAction->setIcon(icon);
}

QAction *PLSPopupMenu::addAction(const QString &text)
{
	QAction *action = new QAction(text, this);
	addAction(action);
	return action;
}

QAction *PLSPopupMenu::addAction(const QIcon &icon, const QString &text)
{
	QAction *action = new QAction(icon, text, this);
	addAction(action);
	return action;
}

QAction *PLSPopupMenu::addAction(const QString &text, const QObject *receiver, const char *member, const QKeySequence &shortcut)
{
	QAction *action = new QAction(text, this);
	action->setShortcut(shortcut);
	QObject::connect(action, SIGNAL(triggered(bool)), receiver, member);
	addAction(action);
	return action;
}

QAction *PLSPopupMenu::addAction(const QIcon &icon, const QString &text, const QObject *receiver, const char *member, const QKeySequence &shortcut)
{
	QAction *action = new QAction(icon, text, this);
	action->setShortcut(shortcut);
	QObject::connect(action, SIGNAL(triggered(bool)), receiver, member);
	addAction(action);
	return action;
}

QAction *PLSPopupMenu::addMenu(PLSPopupMenu *menu)
{
	QAction *action = menu->menuAction();
	addAction(action);
	return action;
}

PLSPopupMenu *PLSPopupMenu::addMenu(const QString &title)
{
	PLSPopupMenu *menu = new PLSPopupMenu(title, this);
	addMenu(menu);
	return menu;
}

PLSPopupMenu *PLSPopupMenu::addMenu(const QIcon &icon, const QString &title)
{
	PLSPopupMenu *menu = new PLSPopupMenu(icon, title, this);
	addMenu(menu);
	return menu;
}

QAction *PLSPopupMenu::addSeparator()
{
	QAction *action = new QAction(this);
	action->setSeparator(true);
	addAction(action);
	return action;
}

QAction *PLSPopupMenu::insertMenu(QAction *before, PLSPopupMenu *menu)
{
	QAction *action = menu->menuAction();
	insertAction(before, action);
	return action;
}

QAction *PLSPopupMenu::insertSeparator(QAction *before)
{
	QAction *action = new QAction(this);
	action->setSeparator(true);
	insertAction(before, action);
	return action;
}

int PLSPopupMenu::getMarginLeft() const
{
	int left = 0, top = 0, right = 0, bottom = 0;
	m_layout->getContentsMargins(&left, &top, &right, &bottom);
	return left;
}

void PLSPopupMenu::setMarginLeft(int left)
{
	int _left = 0, top = 0, right = 0, bottom = 0;
	m_layout->getContentsMargins(&_left, &top, &right, &bottom);
	m_layout->setContentsMargins(left, top, right, bottom);
	PLSDpiHelper::setDynamicContentsMargins(m_layout, true);
}

int PLSPopupMenu::getMarginTop() const
{
	int left = 0, top = 0, right = 0, bottom = 0;
	m_layout->getContentsMargins(&left, &top, &right, &bottom);
	return top;
}

void PLSPopupMenu::setMarginTop(int top)
{
	int left = 0, _top = 0, right = 0, bottom = 0;
	m_layout->getContentsMargins(&left, &_top, &right, &bottom);
	m_layout->setContentsMargins(left, top, right, bottom);
	PLSDpiHelper::setDynamicContentsMargins(m_layout, true);
}

int PLSPopupMenu::getMarginRight() const
{
	int left = 0, top = 0, right = 0, bottom = 0;
	m_layout->getContentsMargins(&left, &top, &right, &bottom);
	return right;
}

void PLSPopupMenu::setMarginRight(int right)
{
	int left = 0, top = 0, _right = 0, bottom = 0;
	m_layout->getContentsMargins(&left, &top, &_right, &bottom);
	m_layout->setContentsMargins(left, top, right, bottom);
	PLSDpiHelper::setDynamicContentsMargins(m_layout, true);
}

int PLSPopupMenu::getMarginBottom() const
{
	int left = 0, top = 0, right = 0, bottom = 0;
	m_layout->getContentsMargins(&left, &top, &right, &bottom);
	return bottom;
}

void PLSPopupMenu::setMarginBottom(int bottom)
{
	int left = 0, top = 0, right = 0, _bottom = 0;
	m_layout->getContentsMargins(&left, &top, &right, &_bottom);
	m_layout->setContentsMargins(left, top, right, bottom);
	PLSDpiHelper::setDynamicContentsMargins(m_layout, true);
}

void PLSPopupMenu::asButtonPopupMenu(QPushButton *button, const QPoint &offset)
{
	connect(button, &QPushButton::clicked, this, [button, offset, this]() {
		if (!isVisible()) {
			g_menuButton = button;
			onHideAllPopupMenu();
			exec(button, offset);
		} else {
			g_menuButton = nullptr;
			hide();
		}
	});
}

static PLSPopupMenu *getIntersectsMenu(PLSPopupMenu *menu, const QRect &menuRect)
{
	int i = g_menuList.indexOf(menu);
	if (i < 0) {
		return nullptr;
	}

	for (; i >= 0; --i) {
		PLSPopupMenu *popupMenu = g_menuList.at(i);
		if (popupMenu->geometry().intersects(menuRect)) {
			return popupMenu;
		}
	}
	return nullptr;
}

static QPoint calcPopupPos(PLSPopupMenu *menu, const QPoint &pos, QAction *action)
{
	extern QRect getScreenAvailableRect(const QPoint &pt);

	menu->adjustSize();

	QRect savr = getScreenAvailableRect(pos);
	QSize size = menu->size();

	QPoint newPos(pos);
	if ((pos.x() - savr.x() + size.width()) > savr.width()) {
		newPos.setX(pos.x() > size.width() ? pos.x() - size.width() : 0);
	}

	if ((pos.y() - savr.y() + size.height()) > savr.height()) {
		newPos.setY(pos.y() > size.height() ? pos.y() - size.height() : 0);
	}

	if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData && userData->m_menu) {
		if (PLSPopupMenu *intersectsMenu = getIntersectsMenu(userData->m_menu, QRect(newPos, size)); intersectsMenu) {
			if ((pos.x() - savr.x() + size.width()) > savr.width()) {
				newPos.setX(newPos.x() - userData->m_menu->geometry().width());
			} else {
				newPos.setX(newPos.x() - userData->m_menu->geometry().width() - size.width());
			}

			for (intersectsMenu = getIntersectsMenu(intersectsMenu, QRect(newPos, size)); intersectsMenu; intersectsMenu = getIntersectsMenu(intersectsMenu, QRect(newPos, size))) {
				newPos.setX(newPos.x() - intersectsMenu->geometry().width());
			}
		}
	}

	return newPos;
}

void PLSPopupMenu::popup(const QPoint &pos, QAction *action)
{
	if (!g_menuList.contains(this)) {
		g_menuList.append(this);
	}

	clearSelectedAction();
	ensurePolished();
	pls_flush_style_recursive(this);
	move(calcPopupPos(this, pos, action));
	show();
}

void PLSPopupMenu::popup(QPushButton *button, const QPoint &offset, QAction *action)
{
	popup(button->mapToGlobal(QPoint(0, button->size().height())) + offset, action);
}

void PLSPopupMenu::exec(const QPoint &pos, QAction *action)
{
	QEventLoop eventLoop;
	connect(this, &PLSPopupMenu::hidden, &eventLoop, &QEventLoop::quit);
	popup(pos, action);
	g_activeMenu = this;
	eventLoop.exec();
}

void PLSPopupMenu::exec(QPushButton *button, const QPoint &offset, QAction *action)
{
	QEventLoop eventLoop;
	connect(this, &PLSPopupMenu::hidden, &eventLoop, &QEventLoop::quit);
	popup(button, offset, action);
	g_activeMenu = this;
	eventLoop.exec();
}

QAction *PLSPopupMenu::selectedAction() const
{
	return m_selectedAction;
}

void PLSPopupMenu::enterSubmenu()
{
	if (m_selectedAction) {
		hoveredAction(m_selectedAction, true);
		return;
	}

	QList<QAction *> actions = this->actions();
	if (!actions.isEmpty()) {
		hoveredAction(m_selectedAction = actions.first(), true);
	}
}

void PLSPopupMenu::leaveSubmenu()
{
	g_activeMenu = m_owner;
	hideSubmenu(this);
	clearSelectedAction();
	hide();
}

void PLSPopupMenu::triggeredSelectedOrFirstAction()
{
	if (m_selectedAction) {
		triggeredAction(m_selectedAction);
		return;
	}

	QList<QAction *> actions = this->actions();
	if (!actions.isEmpty()) {
		triggeredAction(m_selectedAction = actions.first());
	}
}

void PLSPopupMenu::hoveredPrevOrLastAction()
{
	QList<QAction *> actions = this->actions();
	if (actions.isEmpty()) {
		return;
	}

	QAction *current = nullptr;
	QAction *prev = nullptr;
	if (m_selectedAction) {
		prev = prevValidAction(actions, current = m_selectedAction);
	} else {
		current = prev = actions.last();
		if (current->isSeparator() || !current->isVisible()) {
			prev = prevValidAction(actions, current);
		}
	}

	if (prev) {
		hoveredAction(prev, false);
	}
}

void PLSPopupMenu::hoveredNextOrFirstAction()
{
	QList<QAction *> actions = this->actions();
	if (actions.isEmpty()) {
		return;
	}

	QAction *current = nullptr;
	QAction *next = nullptr;
	if (m_selectedAction) {
		next = nextValidAction(actions, current = m_selectedAction);
	} else {
		current = next = actions.first();
		if (current->isSeparator() || !current->isVisible()) {
			next = nextValidAction(actions, current);
		}
	}

	if (next) {
		hoveredAction(next, false);
	}
}

void PLSPopupMenu::clearSelectedAction()
{
	if (m_selectedAction) {
		setMenuItemProperty(m_selectedAction, "selected", false);
		m_selectedAction = nullptr;
	}
}

void PLSPopupMenu::setSelectedAction(QAction *selectedAction)
{
	hoveredAction(selectedAction, true);
}

void PLSPopupMenu::setHasIcon(QAction *action, bool hasIcon)
{
	setMenuItemProperty(action, "hasIcon", hasIcon);
}

bool PLSPopupMenu::isEmpty() const
{
	for (QAction *action : actions()) {
		if (!action->isSeparator() && action->isVisible()) {
			return false;
		}
	}
	return true;
}

bool PLSPopupMenu::toolTipsVisible() const
{
	return m_toolTipsVisible;
}

void PLSPopupMenu::setToolTipsVisible(bool visible)
{
	m_toolTipsVisible = visible;

	for (QAction *action : actions()) {
		action->changed();
	}
}

void PLSPopupMenu::bindPLSMenu(PLSMenu *menu)
{
	if (!menu) {
		return;
	}

	connect(menu, &PLSMenu::actionAdded, this, [this](QAction *action, QAction *before) { m_layout->insertWidget(actionIndex(actions(), before), newMenuItem(this, action)); });
	connect(menu, &PLSMenu::actionRemoved, this, [this](QAction *action) {
		if (m_selectedAction == action) {
			m_selectedAction = nullptr;
		}

		if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData) {
			if (userData->m_submenu) {
				userData->m_submenu->deleteLater();
				userData->m_submenu = nullptr;
			}
			if (userData->m_menuItem) {
				userData->m_menuItem->hide();
				m_layout->removeWidget(userData->m_menuItem);
				userData->m_menuItem->deleteLater();
				userData->m_menuItem = nullptr;
			}
		}
	});
}
void PLSPopupMenu::onHideAllPopupMenu()
{
	for (PLSPopupMenu *menu : g_menuList) {
		menu->hide();
	}
}

bool PLSPopupMenu::event(QEvent *event)
{
	switch (event->type()) {
	case QEvent::ActionAdded: {
		QActionEvent *e = dynamic_cast<QActionEvent *>(event);
		m_layout->insertWidget(actionIndex(actions(), e->before()), newMenuItem(this, e->action()));
		break;
	}
	case QEvent::ActionRemoved: {
		QAction *action = dynamic_cast<QActionEvent *>(event)->action();
		if (m_selectedAction == action) {
			m_selectedAction = nullptr;
		}

		if (ActionUserdata *userData = ActionUserdata::getActionUserData(action); userData) {
			if (userData->m_submenu) {
				userData->m_submenu->deleteLater();
				userData->m_submenu = nullptr;
			}
			if (userData->m_menuItem) {
				userData->m_menuItem->hide();
				m_layout->removeWidget(userData->m_menuItem);
				userData->m_menuItem->deleteLater();
				userData->m_menuItem = nullptr;
			}
		}
		break;
	}
	}

	return WidgetDpiAdapter::event(event);
}

void PLSPopupMenu::showEvent(QShowEvent *event)
{
	emit shown();
	WidgetDpiAdapter::showEvent(event);
}

void PLSPopupMenu::hideEvent(QHideEvent *event)
{
	g_menuList.removeOne(this);

	if (g_activeMenu == this) {
		g_activeMenu = nullptr;
	}

	clearSelectedAction();
	setMenuItemProperty(m_menuAction, "selected", false);
	// hide all submenus when menu hide
	hideSubmenu(this);

	WidgetDpiAdapter::hideEvent(event);
	emit hidden();
}

bool PLSPopupMenu::needQtProcessDpiChangedEvent() const
{
	return false;
}

bool PLSPopupMenu::needProcessScreenChangedEvent() const
{
	return false;
}

void PLSPopupMenu::onDpiChanged(double dpi, double oldDpi, bool firstShow)
{
	WidgetDpiAdapter::onDpiChanged(dpi, oldDpi, firstShow);
	adjustSize();
}

#include "PLSMenu.moc"
