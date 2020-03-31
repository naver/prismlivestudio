#ifndef PLSWIDGETDPIADAPTER_HPP
#define PLSWIDGETDPIADAPTER_HPP

#include <functional>
#include <list>
#include <memory>

#include <QWidget>

#include "frontend-api.h"
#include "PLSThemeManager.h"
#include "PLSDpiHelper.h"

Q_DECLARE_METATYPE(QMargins)

class PLSDpiHelper;
class PLSDpiHelperImpl;

class FRONTEND_API PLSWidgetDpiAdapter {
	class WidgetDpiAdapterInfo {
	public:
		WidgetDpiAdapterInfo(QWidget *widget);
		~WidgetDpiAdapterInfo();

	public:
		WidgetDpiAdapterInfo(const WidgetDpiAdapterInfo &widgetDpiAdapterInfo);
		WidgetDpiAdapterInfo &operator=(const WidgetDpiAdapterInfo &widgetDpiAdapterInfo);

		void dpiHelper(PLSWidgetDpiAdapter *adapter, double dpi, bool firstShow, bool updateCss);
		QRect showAsDefaultSize(double dpi, const QRect &normalGeometry, const QRect &screenAvailableGeometry);

	public:
		QWidget *widget;
		std::pair<bool, bool> updateLayout{true, true};
		std::pair<bool, QSize> initSize{false, {}};
		std::pair<std::pair<bool, int>, std::pair<bool, int>> minimumSize{{false, -1}, {false, -1}};
		std::pair<std::pair<bool, int>, std::pair<bool, int>> maximumSize{{false, -1}, {false, -1}};
		std::pair<bool, QMargins> contentMargins{false, {}};
		QList<PLSCssIndex> cssIndexes;
		QString styleSheet;
		std::function<QString(double, bool)> dynamicStyleSheetCallback; // Dynamic Css Callbacks
		QList<std::function<void(double, double, bool)>> callbacks;
		QList<std::function<void(const QRect &)>> sagcCallbacks; // Screen Available Geometry Changed Callbacks
		QMetaObject::Connection connection;
	};

	class ScreenConnection {
	public:
		ScreenConnection(QScreen *screen);
		~ScreenConnection();

	public:
		ScreenConnection(const ScreenConnection &screenConnection);
		ScreenConnection &operator=(const ScreenConnection &screenConnection);

	public:
		QScreen *screen;
		QMetaObject::Connection screenAvailableGeometryChanged;
	};

	enum class FirstShowState { Waiting, InitDpi, InProgress, Complete };

protected:
	PLSWidgetDpiAdapter();
	virtual ~PLSWidgetDpiAdapter();

public:
	std::shared_ptr<WidgetDpiAdapterInfo> getWidgetDpiAdapterInfo(QWidget *widget) const;
	std::shared_ptr<WidgetDpiAdapterInfo> getWidgetDpiAdapterInfo(QWidget *widget, bool &isNew) const;
	std::shared_ptr<ScreenConnection> getScreenConnection(QScreen *screen) const;
	QRect showAsDefaultSize(const QRect &normalGeometry, const QRect &screenAvailableGeometry);

public:
	virtual QWidget *selfWidget() const = 0;
	virtual QByteArray saveGeometry() const = 0;
	virtual bool restoreGeometry(const QByteArray &geometry) = 0;

protected:
	virtual bool needQtProcessDpiChangedEvent() const;
	virtual bool needProcessScreenChangedEvent() const;
	virtual QRect onDpiChanged(double dpi, double oldDpi, const QRect &suggested, bool firstShow);
	virtual void onDpiChanged(double dpi, double oldDpi, bool firstShow);
	virtual void onScreenAvailableGeometryChanged(const QRect &screenAvailableGeometry);
	virtual void onScreenRemoved();
	virtual QRect getSuggestedRect(const QRect &suggested) const;

protected:
	void init(QWidget *widget);
	bool event(QWidget *widget, QEvent *event, std::function<bool(QEvent *)> baseEvent);
	bool nativeEvent(QWidget *widget, const QByteArray &eventType, void *message, long *result, std::function<bool(const QByteArray &, void *, long *)> baseNativeEvent);
	void notifyFirstShow(std::function<void()> callback);
	QByteArray saveGeometry(QWidget *widget) const;
	QByteArray saveGeometry(QWidget *widget, const QRect &geometry) const;
	bool restoreGeometry(QWidget *widget, const QByteArray &array);

private:
	void dpiDynamicUpdate(QWidget *widget, bool updateCss);
	void dpiDynamicUpdateBeforeFirstShow(double dpi, QWidget *widget);
	void firstShowEvent(QWidget *widget);
	QRect dpiChangedEvent(QWidget *widget, double dpi, const QRect &suggested, bool firstShow, bool dynamicUpdate, bool updateCss);
	QRect dpiChangedEventImpl(QWidget *widget, double dpi, double oldDpi, const QRect &suggested, bool firstShow, bool updateCss);
	void screenAvailableGeometryChangedEvent(const QRect &screenAvailableGeometry);
	void checkStatusChanged(PLSDpiHelper::Screen *screen = nullptr);
	void dockWidgetFirstShow(double dpi);

protected:
	FirstShowState firstShowState = FirstShowState::Waiting;
	bool isDpiChanging = false;
	bool isMinimizedDpiChanged = false;
	double dpi = 0.0;
	mutable std::list<std::function<void()>> firstShowCallbacks;
	mutable std::list<std::shared_ptr<WidgetDpiAdapterInfo>> widgetDpiAdapterInfos;
	mutable std::list<std::shared_ptr<ScreenConnection>> screenConnections;
	QMetaObject::Connection screenAddedConnection;
	QMetaObject::Connection screenRemovedConnection;

	friend class PLSDpiHelper;
	friend class PLSDpiHelperImpl;
};

template<typename ParentWidget> class PLSWidgetDpiAdapterHelper : public ParentWidget, public PLSWidgetDpiAdapter {
public:
	using Widget = ParentWidget;
	using WidgetDpiAdapter = PLSWidgetDpiAdapterHelper<ParentWidget>;

protected:
	template<typename... Args> PLSWidgetDpiAdapterHelper(Args &&...args) : ParentWidget(std::forward<Args>(args)...) { PLSWidgetDpiAdapter::init(this); }
	virtual ~PLSWidgetDpiAdapterHelper() {}

public:
	QWidget *selfWidget() const override { return const_cast<WidgetDpiAdapter *>(this); }
	QByteArray saveGeometry() const { return PLSWidgetDpiAdapter::saveGeometry(selfWidget()); }
	bool restoreGeometry(const QByteArray &geometry) { return PLSWidgetDpiAdapter::restoreGeometry(this, geometry); }

protected:
	bool event(QEvent *event) override
	{
		bool retval = PLSWidgetDpiAdapter::event(this, event, [this](QEvent *event) { return Widget::event(event); });
		switch (event->type()) {
		case QEvent::Show:
			visibleSignal(true);
			break;
		case QEvent::Hide:
			visibleSignal(false);
			break;
		}
		return retval;
	}
	bool nativeEvent(const QByteArray &eventType, void *message, long *result) override
	{
		return PLSWidgetDpiAdapter::nativeEvent(this, eventType, message, result,
							[this](const QByteArray &eventType, void *message, long *result) { return Widget::nativeEvent(eventType, message, result); });
	}

protected:
	virtual void visibleSignal(bool /*visible*/) {}
};

#endif // PLSWIDGETDPIADAPTER_HPP
