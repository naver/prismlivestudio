#pragma once

#include <QMenu>
#include <QFrame>
#include <QIcon>

#include "PLSToplevelView.hpp"
#include "PLSDpiHelper.h"

class QMenu;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QWidgetAction;

class PLSPopupMenu;

class PLSMenu : public QMenu {
	Q_OBJECT
public:
	using QMenu::QMenu;

signals:
	void actionAdded(QAction *action, QAction *before);
	void actionRemoved(QAction *action);

protected:
	bool event(QEvent *event) override;
};

class PLSPopupMenuItem : public QFrame {
	Q_OBJECT
public:
	explicit PLSPopupMenuItem(QAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSPopupMenuItem() override;

public:
	enum class Type { Separator, Action, WidgetAction, Menu };

public:
	virtual Type type() const = 0;

protected:
	virtual void actionHovered();
	virtual void actionTriggered();
	virtual void actionChanged();

public:
	QString getText(int &shortcutKey, const QFont &font, int width) const;

protected:
	QAction *m_action;
	PLSPopupMenu *m_menu;

	friend class PLSPopupMenu;
};

class PLSPopupMenuItemSeparator : public PLSPopupMenuItem {
	Q_OBJECT
public:
	explicit PLSPopupMenuItemSeparator(QAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSPopupMenuItemSeparator() override;

public:
	Type type() const override;
};

class PLSPopupMenuItemContent : public PLSPopupMenuItem {
	Q_OBJECT
	Q_PROPERTY(bool hasIcon READ getHasIcon WRITE setHasIcon)
	Q_PROPERTY(bool isLeftIcon READ getIsLeftIcon WRITE setIsLeftIcon)
public:
	explicit PLSPopupMenuItemContent(QAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSPopupMenuItemContent() override;

public:
	void showShortcutKey();
	void shortcutKeyPress(int key);

	bool getHasIcon() const;
	void setHasIcon(bool hasIcon);

	bool getIsLeftIcon() const;
	void setIsLeftIcon(bool isLeftIcon);

protected:
	void actionChanged() override;

	bool eventFilter(QObject *watched, QEvent *event) override;
	void enterEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

protected:
	bool m_hasIcon;
	bool m_isLeftIcon;
	bool m_isElidedTextProcessed;
	QHBoxLayout *m_layout;
	QLabel *m_icon;
	QLabel *m_text;
	int m_shortcutKey;
};

class PLSPopupMenuItemAction : public PLSPopupMenuItemContent {
	Q_OBJECT
public:
	explicit PLSPopupMenuItemAction(QAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSPopupMenuItemAction() override;

public:
	Type type() const override;

protected:
	void actionHovered() override;
	void actionTriggered() override;
	void actionChanged() override;

	void leaveEvent(QEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

protected:
	QLabel *m_shortcut;
};

class PLSPopupMenuItemWidgetAction : public PLSPopupMenuItem {
	Q_OBJECT
public:
	explicit PLSPopupMenuItemWidgetAction(QWidgetAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSPopupMenuItemWidgetAction() override;

public:
	Type type() const override;

protected:
	void actionHovered() override;
	void actionTriggered() override;
	void actionChanged() override;

	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
};

class PLSPopupMenuItemMenu : public PLSPopupMenuItemContent {
	Q_OBJECT
public:
	explicit PLSPopupMenuItemMenu(QAction *action, PLSPopupMenu *menu, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSPopupMenuItemMenu() override;

public:
	Type type() const override;

protected:
	void actionHovered() override;
	void actionTriggered() override;
	void actionChanged() override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

protected:
	QLabel *m_arrow;
};

class PLSPopupMenu : public PLSWidgetDpiAdapterHelper<QFrame> {
	Q_OBJECT
	Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
	Q_PROPERTY(int marginLeft READ getMarginLeft WRITE setMarginLeft)
	Q_PROPERTY(int marginTop READ getMarginTop WRITE setMarginTop)
	Q_PROPERTY(int marginRight READ getMarginRight WRITE setMarginRight)
	Q_PROPERTY(int marginBottom READ getMarginBottom WRITE setMarginBottom)

public:
	explicit PLSPopupMenu(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSPopupMenu(bool toolTipsVisible, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSPopupMenu(const QString &title, bool toolTipsVisible, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSPopupMenu(const QIcon &icon, const QString &title, bool toolTipsVisible, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSPopupMenu(QAction *menuAction, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSPopupMenu(QAction *menuAction, bool toolTipsVisible, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSPopupMenu(QMenu *menu, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSPopupMenu(QMenu *menu, bool toolTipsVisible, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSPopupMenu(PLSMenu *menu, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSPopupMenu(PLSMenu *menu, bool toolTipsVisible, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSPopupMenu(const QIcon &icon, const QString &title, QAction *menuAction, bool toolTipsVisible, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSPopupMenu() override;

	PLSPopupMenu *owner() const;
	void setOwner(PLSPopupMenu *owner);

	QAction *menuAction() const;

	QString title() const;
	void setTitle(const QString &title);

	QIcon icon() const;
	void setIcon(const QIcon &icon);

	using QWidget::addAction;
	QAction *addAction(const QString &text);
	QAction *addAction(const QIcon &icon, const QString &text);
	QAction *addAction(const QString &text, const QObject *receiver, const char *member, const QKeySequence &shortcut = 0);
	QAction *addAction(const QIcon &icon, const QString &text, const QObject *receiver, const char *member, const QKeySequence &shortcut = 0);

	// addAction(QString): Connect to a QObject slot / functor or function pointer (with context)
	template<class Obj, typename Func1>
	inline typename std::enable_if<!std::is_same<const char *, Func1>::value && QtPrivate::IsPointerToTypeDerivedFromQObject<Obj *>::Value, QAction *>::type
	addAction(const QString &text, const Obj *object, Func1 slot, const QKeySequence &shortcut = 0)
	{
		QAction *result = addAction(text);
		result->setShortcut(shortcut);
		connect(result, &QAction::triggered, object, std::move(slot));
		return result;
	}
	// addAction(QString): Connect to a functor or function pointer (without context)
	template<typename Func1> inline QAction *addAction(const QString &text, Func1 slot, const QKeySequence &shortcut = 0)
	{
		QAction *result = addAction(text);
		result->setShortcut(shortcut);
		connect(result, &QAction::triggered, std::move(slot));
		return result;
	}
	// addAction(QIcon, QString): Connect to a QObject slot / functor or function pointer (with context)
	template<class Obj, typename Func1>
	inline typename std::enable_if<!std::is_same<const char *, Func1>::value && QtPrivate::IsPointerToTypeDerivedFromQObject<Obj *>::Value, QAction *>::type
	addAction(const QIcon &actionIcon, const QString &text, const Obj *object, Func1 slot, const QKeySequence &shortcut = 0)
	{
		QAction *result = addAction(actionIcon, text);
		result->setShortcut(shortcut);
		connect(result, &QAction::triggered, object, std::move(slot));
		return result;
	}
	// addAction(QIcon, QString): Connect to a functor or function pointer (without context)
	template<typename Func1> inline QAction *addAction(const QIcon &actionIcon, const QString &text, Func1 slot, const QKeySequence &shortcut = 0)
	{
		QAction *result = addAction(actionIcon, text);
		result->setShortcut(shortcut);
		connect(result, &QAction::triggered, std::move(slot));
		return result;
	}

	QAction *addMenu(PLSPopupMenu *menu);
	PLSPopupMenu *addMenu(const QString &title);
	PLSPopupMenu *addMenu(const QIcon &icon, const QString &title);

	QAction *addSeparator();

	QAction *insertMenu(QAction *before, PLSPopupMenu *menu);
	QAction *insertSeparator(QAction *before);

	int getMarginLeft() const;
	void setMarginLeft(int left);

	int getMarginTop() const;
	void setMarginTop(int top);

	int getMarginRight() const;
	void setMarginRight(int right);

	int getMarginBottom() const;
	void setMarginBottom(int bottom);

	void asButtonPopupMenu(QPushButton *button, const QPoint &offset = QPoint());

	void popup(const QPoint &pos, QAction *action = nullptr);
	void popup(QPushButton *button, const QPoint &offset = QPoint(), QAction *action = nullptr);
	void exec(const QPoint &pos, QAction *action = nullptr);
	void exec(QPushButton *button, const QPoint &offset = QPoint(), QAction *action = nullptr);

	QAction *selectedAction() const;
	void enterSubmenu();
	void leaveSubmenu();
	void triggeredSelectedOrFirstAction();
	void hoveredPrevOrLastAction();
	void hoveredNextOrFirstAction();
	void clearSelectedAction();
	static void setSelectedAction(QAction *selectedAction);
	static void setHasIcon(QAction *action, bool hasIcon);

	bool isEmpty() const;

	bool toolTipsVisible() const;
	void setToolTipsVisible(bool visible);

private:
	void bindPLSMenu(PLSMenu *menu);

signals:
	void triggered(QAction *action);
	void hovered(QAction *action);
	void shown();
	void hidden();
	void hideAllPopupMenu();

private slots:
	void onHideAllPopupMenu();

protected:
	bool event(QEvent *event) override;
	void showEvent(QShowEvent *event) override;
	void hideEvent(QHideEvent *event) override;
	bool needQtProcessDpiChangedEvent() const override;
	bool needProcessScreenChangedEvent() const override;
	void onDpiChanged(double dpi, double oldDpi, bool firstShow) override;

private:
	PLSPopupMenu *m_owner;
	QAction *m_menuAction;
	QAction *m_selectedAction;
	QVBoxLayout *m_layout;
	bool m_toolTipsVisible;

	friend class PLSPopupMenuItem;
	friend class PLSPopupMenuItemContent;
	friend class PLSPopupMenuItemAction;
};
