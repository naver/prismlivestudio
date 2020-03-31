#pragma once

#include <util/platform.h>
#include <util/util.hpp>
#include <QWidget>

#include <functional>
#include <string>
#include <map>

struct QCefCookieManager {
	virtual ~QCefCookieManager() {}

	virtual bool DeleteCookies(const std::string &url,
				   const std::string &name) = 0;
	virtual bool SetStoragePath(const std::string &storage_path,
				    bool persist_session_cookies = false) = 0;
	virtual bool FlushStore() = 0;

	typedef std::function<void(bool)> cookie_exists_cb;

	virtual void CheckForCookie(const std::string &site,
				    const std::string &cookie,
				    cookie_exists_cb callback) = 0;
	virtual bool SetCookie(const std::string &url, const std::string &name,
			       const std::string &value,
			       const std::string &domain,
			       const std::string &path, bool isOnlyHttp) = 0;

	typedef std::function<void(const char *name, const char *value,
				   const char *domain, const char *path,
				   bool complete, void *context)>
		read_cookie_cb;
	virtual void visitAllCookies(read_cookie_cb cookie_cb,
				     void *context) = 0;

	virtual void ReadCookies(const std::string &site,
				 read_cookie_cb callback, void *context) = 0;
};

/* ------------------------------------------------------------------------- */

class QCefWidget : public QWidget {
	Q_OBJECT

protected:
	inline QCefWidget(QWidget *parent) : QWidget(parent) {}

public:
	virtual void setURL(const std::string &url) = 0;
	virtual void allowAllPopups(bool allow) = 0;
	virtual void closeBrowser() = 0;

signals:
	void titleChanged(const QString &title);
	void urlChanged(const QString &url);
};

/* ------------------------------------------------------------------------- */

struct QCef {
	virtual ~QCef() {}

	virtual bool init_browser(void) = 0;
	virtual bool initialized(void) = 0;
	virtual bool wait_for_browser_init(void) = 0;

	// OBS Modification:
	// Zhang dewen / 20200211 / Related Issue ID=347
	// Reason: add request headers parameter
	// Solution: modify request headers
	virtual QCefWidget *
	create_widget(QWidget *parent, const std::string &url,
		      const std::string &script = std::string(),
		      QCefCookieManager *cookie_manager = nullptr,
		      const std::map<std::string, std::string> &headers =
			      std::map<std::string, std::string>()) = 0;

	virtual QCefCookieManager *
	create_cookie_manager(const std::string &storage_path,
			      bool persist_session_cookies = false) = 0;

	virtual BPtr<char> get_cookie_path(const std::string &storage_path) = 0;

	virtual void add_popup_whitelist_url(const std::string &url,
					     QObject *obj) = 0;
	virtual void add_force_popup_url(const std::string &url,
					 QObject *obj) = 0;
};

static inline QCef *obs_browser_init_panel(void)
{
#ifdef _WIN32
	void *lib = os_dlopen("obs-browser");
#else
	void *lib = os_dlopen("../obs-plugins/obs-browser");
#endif
	QCef *(*create_qcef)(void) = nullptr;

	if (!lib) {
		return nullptr;
	}

	create_qcef =
		(decltype(create_qcef))os_dlsym(lib, "obs_browser_create_qcef");
	if (!create_qcef)
		return nullptr;

	return create_qcef();
}

static inline int obs_browser_qcef_version(void)
{
#ifdef _WIN32
	void *lib = os_dlopen("obs-browser");
#else
	void *lib = os_dlopen("../obs-plugins/obs-browser");
#endif
	int (*qcef_version)(void) = nullptr;

	if (!lib) {
		return 0;
	}

	qcef_version = (decltype(qcef_version))os_dlsym(
		lib, "obs_browser_qcef_version_export");
	if (!qcef_version)
		return 0;

	return qcef_version();
}
