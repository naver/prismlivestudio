#include <frontend-internal.hpp>
#include <util/windows/win-version.h>

#include "PLSNetworkMonitor.h"
#include "pls-app.hpp"
#include "qt-wrappers.hpp"
#include "window-basic-main.hpp"
#include "window-basic-main-outputs.hpp"
#include "browser-view.hpp"

#include "network-environment.hpp"
#include "main-view.hpp"
#include <functional>
#include "window-dock-browser.hpp"
#include "pls-common-define.hpp"
#include "pls-gpop-data.hpp"
#include "PLSSceneDataMgr.h"
#include "window-dock-browser.hpp"
#include "PLSPlatformApi.h"
#include "PLSChannelDataAPI.h"
#include "TextMotionTemplateDataHelper.h"

#include <QDir>
#include <QFile>

using namespace std;

Q_DECLARE_METATYPE(OBSScene);
Q_DECLARE_METATYPE(OBSSource);

template<typename T> static T GetPLSRef(QListWidgetItem *item)
{
	return item->data(static_cast<int>(QtDataRole::PLSRef)).value<T>();
}

void EnumProfiles(function<bool(const char *, const char *)> &&cb);
void EnumSceneCollections(function<bool(const char *, const char *)> &&cb);

int getActivedChatChannelCount();

extern volatile bool streaming_active;
extern volatile bool recording_active;
extern volatile bool recording_paused;
extern volatile bool replaybuf_active;

/* ------------------------------------------------------------------------- */

template<typename T> struct PLSStudioCallback {
	T callback;
	void *private_data;

	inline PLSStudioCallback(T cb, void *p) : callback(cb), private_data(p) {}
};

template<typename T> inline size_t GetCallbackIdx(vector<PLSStudioCallback<T>> &callbacks, T callback, void *private_data)
{
	for (size_t i = 0; i < callbacks.size(); i++) {
		PLSStudioCallback<T> curCB = callbacks[i];
		if (curCB.callback == callback && curCB.private_data == private_data)
			return i;
	}

	return (size_t)-1;
}

class BrowserDock;
struct PLSStudioAPI : pls_frontend_callbacks {
	PLSBasic *main;
	vector<PLSStudioCallback<obs_frontend_event_cb>> callbacks;
	vector<PLSStudioCallback<obs_frontend_save_cb>> saveCallbacks;
	vector<PLSStudioCallback<obs_frontend_save_cb>> preloadCallbacks;
	QList<PLSLoginInfo *> loginInfos;
	QList<std::tuple<QList<pls_frontend_event>, pls_frontend_event_cb, void *>> eventCallbacks;
	QMap<QString, QSharedPointer<BrowserDock>> chatDocks;

	explicit inline PLSStudioAPI(PLSBasic *main_) : main(main_) {}

	void *obs_frontend_get_main_window(void) override { return (void *)main; }

	void *obs_frontend_get_main_window_handle(void) override { return (void *)main->winId(); }

	void *obs_frontend_get_system_tray(void) override { return (void *)main->trayIcon.data(); }

	void obs_frontend_get_scenes(struct obs_frontend_source_list *sources) override
	{
		SceneDisplayVector data = PLSSceneDataMgr::Instance()->GetDisplayVector();
		for (auto iter = data.begin(); iter != data.end(); ++iter) {
			PLSSceneItemView *item = iter->second;
			OBSScene scene = item->GetData();
			obs_source_t *source = obs_scene_get_source(scene);

			obs_source_addref(source);
			da_push_back(sources->sources, &source);
		}
	}

	obs_source_t *obs_frontend_get_current_scene(void) override
	{
		OBSSource source;

		if (main->IsPreviewProgramMode()) {
			source = obs_weak_source_get_source(main->programScene);
		} else {
			source = main->GetCurrentSceneSource();
			obs_source_addref(source);
		}
		return source;
	}

	void obs_frontend_set_current_scene(obs_source_t *scene) override
	{
		if (main->IsPreviewProgramMode()) {
			QMetaObject::invokeMethod(main, "TransitionToScene", WaitConnection(), Q_ARG(OBSSource, OBSSource(scene)));
		} else {
			QMetaObject::invokeMethod(main, "SetCurrentScene", WaitConnection(), Q_ARG(OBSSource, OBSSource(scene)), Q_ARG(bool, false));
		}
	}

	void obs_frontend_get_transitions(struct obs_frontend_source_list *sources) override
	{
		QComboBox *box = main->GetTransitionCombobox();
		for (int i = 0; i < box->count(); i++) {
			OBSSource tr = box->itemData(i).value<OBSSource>();

			obs_source_addref(tr);
			da_push_back(sources->sources, &tr);
		}
	}

	obs_source_t *obs_frontend_get_current_transition(void) override
	{
		OBSSource tr = main->GetCurrentTransition();

		obs_source_addref(tr);
		return tr;
	}

	void obs_frontend_set_current_transition(obs_source_t *transition) override { QMetaObject::invokeMethod(main, "SetTransition", Q_ARG(OBSSource, OBSSource(transition))); }

	int obs_frontend_get_transition_duration(void) override { return main->GetTransitionDuration(); }

	void obs_frontend_set_transition_duration(int duration) override { QMetaObject::invokeMethod(main->GetTransitionDurationSpinBox(), "setValue", Q_ARG(int, duration)); }

	void obs_frontend_get_scene_collections(std::vector<std::string> &strings) override
	{
		auto addCollection = [&](const char *name, const char *) {
			strings.emplace_back(name);
			return true;
		};

		EnumSceneCollections(addCollection);
	}

	char *obs_frontend_get_current_scene_collection(void) override
	{
		const char *cur_name = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollection");
		return bstrdup(cur_name);
	}

	void obs_frontend_set_current_scene_collection(const char *collection) override
	{
		QList<QAction *> menuActions = main->ui->sceneCollectionMenu->actions();
		QString qstrCollection = QT_UTF8(collection);

		for (int i = 0; i < menuActions.count(); i++) {
			QAction *action = menuActions[i];
			QVariant v = action->property("file_name");

			if (v.typeName() != nullptr) {
				if (action->text() == qstrCollection) {
					action->trigger();
					break;
				}
			}
		}
	}

	bool obs_frontend_add_scene_collection(const char *name) override
	{
		bool success = false;
		QMetaObject::invokeMethod(main, "AddSceneCollection", WaitConnection(), Q_RETURN_ARG(bool, success), Q_ARG(bool, true), Q_ARG(QString, QT_UTF8(name)));
		return success;
	}

	void obs_frontend_get_profiles(std::vector<std::string> &strings) override
	{
		auto addProfile = [&](const char *name, const char *) {
			strings.emplace_back(name);
			return true;
		};

		EnumProfiles(addProfile);
	}

	char *obs_frontend_get_current_profile(void) override
	{
		const char *name = config_get_string(App()->GlobalConfig(), "Basic", "Profile");
		return bstrdup(name);
	}

	void obs_frontend_set_current_profile(const char *profile) override
	{
		QList<QAction *> menuActions = main->ui->profileMenu->actions();
		QString qstrProfile = QT_UTF8(profile);

		for (int i = 0; i < menuActions.count(); i++) {
			QAction *action = menuActions[i];
			QVariant v = action->property("file_name");

			if (v.typeName() != nullptr) {
				if (action->text() == qstrProfile) {
					action->trigger();
					break;
				}
			}
		}
	}

	void obs_frontend_streaming_start(void) override { QMetaObject::invokeMethod(main, "StartStreaming"); }

	void obs_frontend_streaming_stop(void) override { QMetaObject::invokeMethod(main, "StopStreaming"); }

	bool obs_frontend_streaming_active(void) override { return os_atomic_load_bool(&streaming_active); }

	void obs_frontend_recording_start(void) override { QMetaObject::invokeMethod(main, "StartRecording"); }

	void obs_frontend_recording_stop(void) override { QMetaObject::invokeMethod(main, "StopRecording"); }

	bool obs_frontend_recording_active(void) override { return os_atomic_load_bool(&recording_active); }

	void obs_frontend_recording_pause(bool pause) override { QMetaObject::invokeMethod(main, pause ? "PauseRecording" : "UnpauseRecording"); }

	bool obs_frontend_recording_paused(void) override { return os_atomic_load_bool(&recording_paused); }

	void obs_frontend_replay_buffer_start(void) override { QMetaObject::invokeMethod(main, "StartReplayBuffer"); }

	void obs_frontend_replay_buffer_save(void) override { QMetaObject::invokeMethod(main, "ReplayBufferSave"); }

	void obs_frontend_replay_buffer_stop(void) override { QMetaObject::invokeMethod(main, "StopReplayBuffer"); }

	bool obs_frontend_replay_buffer_active(void) override { return os_atomic_load_bool(&replaybuf_active); }

	void *obs_frontend_add_tools_menu_qaction(const char *name) override
	{
		main->ui->menuTools->setEnabled(true);
		return (void *)main->ui->menuTools->addAction(QT_UTF8(name));
	}

	void obs_frontend_add_tools_menu_item(const char *name, obs_frontend_cb callback, void *private_data) override
	{
		main->ui->menuTools->setEnabled(true);

		auto func = [private_data, callback]() { callback(private_data); };

		QAction *action = main->ui->menuTools->addAction(QT_UTF8(name));
		QObject::connect(action, &QAction::triggered, func);
	}

	void *obs_frontend_add_dock(void *dock) override { return (void *)main->AddDockWidget((QDockWidget *)dock); }

	void obs_frontend_add_event_callback(obs_frontend_event_cb callback, void *private_data) override
	{
		size_t idx = GetCallbackIdx(callbacks, callback, private_data);
		if (idx == (size_t)-1)
			callbacks.emplace_back(callback, private_data);
	}

	void obs_frontend_remove_event_callback(obs_frontend_event_cb callback, void *private_data) override
	{
		size_t idx = GetCallbackIdx(callbacks, callback, private_data);
		if (idx == (size_t)-1)
			return;

		callbacks.erase(callbacks.begin() + idx);
	}

	obs_output_t *obs_frontend_get_streaming_output(void) override
	{
		OBSOutput output = main->outputHandler->streamOutput;
		obs_output_addref(output);
		return output;
	}

	obs_output_t *obs_frontend_get_recording_output(void) override
	{
		OBSOutput out = main->outputHandler->fileOutput;
		obs_output_addref(out);
		return out;
	}

	obs_output_t *obs_frontend_get_replay_buffer_output(void) override
	{
		OBSOutput out = main->outputHandler->replayBuffer;
		obs_output_addref(out);
		return out;
	}

	config_t *obs_frontend_get_profile_config(void) override { return main->basicConfig; }

	config_t *obs_frontend_get_global_config(void) override { return App()->GlobalConfig(); }

	void obs_frontend_save(void) override { main->SaveProject(); }

	void obs_frontend_defer_save_begin(void) override { QMetaObject::invokeMethod(main, "DeferSaveBegin"); }

	void obs_frontend_defer_save_end(void) override { QMetaObject::invokeMethod(main, "DeferSaveEnd"); }

	void obs_frontend_add_save_callback(obs_frontend_save_cb callback, void *private_data) override
	{
		size_t idx = GetCallbackIdx(saveCallbacks, callback, private_data);
		if (idx == (size_t)-1)
			saveCallbacks.emplace_back(callback, private_data);
	}

	void obs_frontend_remove_save_callback(obs_frontend_save_cb callback, void *private_data) override
	{
		size_t idx = GetCallbackIdx(saveCallbacks, callback, private_data);
		if (idx == (size_t)-1)
			return;

		saveCallbacks.erase(saveCallbacks.begin() + idx);
	}

	void obs_frontend_add_preload_callback(obs_frontend_save_cb callback, void *private_data) override
	{
		size_t idx = GetCallbackIdx(preloadCallbacks, callback, private_data);
		if (idx == (size_t)-1)
			preloadCallbacks.emplace_back(callback, private_data);
	}

	void obs_frontend_remove_preload_callback(obs_frontend_save_cb callback, void *private_data) override
	{
		size_t idx = GetCallbackIdx(preloadCallbacks, callback, private_data);
		if (idx == (size_t)-1)
			return;

		preloadCallbacks.erase(preloadCallbacks.begin() + idx);
	}

	void obs_frontend_push_ui_translation(obs_frontend_translate_ui_cb translate) override { App()->PushUITranslation(translate); }

	void obs_frontend_pop_ui_translation(void) override { App()->PopUITranslation(); }

	void obs_frontend_set_streaming_service(obs_service_t *service) override { main->SetService(service); }

	obs_service_t *obs_frontend_get_streaming_service(void) override { return main->GetService(); }

	void obs_frontend_save_streaming_service(void) override { main->SaveService(); }

	bool obs_frontend_preview_program_mode_active(void) override { return main->IsPreviewProgramMode(); }

	void obs_frontend_set_preview_program_mode(bool enable) override { main->SetPreviewProgramMode(enable); }

	void obs_frontend_preview_program_trigger_transition(void) override { QMetaObject::invokeMethod(main, "TransitionClicked"); }

	bool obs_frontend_preview_enabled(void) override { return main->previewEnabled; }

	void obs_frontend_set_preview_enabled(bool enable) override
	{
		if (main->previewEnabled != enable)
			main->EnablePreviewDisplay(enable);
	}

	obs_source_t *obs_frontend_get_current_preview_scene(void) override
	{
		OBSSource source = nullptr;

		if (main->IsPreviewProgramMode()) {
			source = main->GetCurrentSceneSource();
			obs_source_addref(source);
		}

		return source;
	}

	void obs_frontend_set_current_preview_scene(obs_source_t *scene) override
	{
		if (main->IsPreviewProgramMode()) {
			QMetaObject::invokeMethod(main, "SetCurrentScene", Q_ARG(OBSSource, OBSSource(scene)), Q_ARG(bool, false));
		}
	}

	void on_load(obs_data_t *settings) override
	{
		for (size_t i = saveCallbacks.size(); i > 0; i--) {
			auto cb = saveCallbacks[i - 1];
			cb.callback(settings, false, cb.private_data);
		}
	}

	void on_preload(obs_data_t *settings) override
	{
		for (size_t i = preloadCallbacks.size(); i > 0; i--) {
			auto cb = preloadCallbacks[i - 1];
			cb.callback(settings, false, cb.private_data);
		}
	}

	void on_save(obs_data_t *settings) override
	{
		for (size_t i = saveCallbacks.size(); i > 0; i--) {
			auto cb = saveCallbacks[i - 1];
			cb.callback(settings, true, cb.private_data);
		}
	}

	void on_event(enum obs_frontend_event event) override
	{
		if (main->disableSaving)
			return;

		for (size_t i = callbacks.size(); i > 0; i--) {
			auto cb = callbacks[i - 1];
			cb.callback(event, cb.private_data);
		}
	}

	bool pls_register_login_info(PLSLoginInfo *login_info)
	{
		loginInfos.append(login_info);
		return true;
	}

	void pls_unregister_login_info(PLSLoginInfo *login_info) { loginInfos.removeOne(login_info); }

	int pls_get_login_info_count() { return loginInfos.size(); }

	PLSLoginInfo *pls_get_login_info(int index)
	{
		if ((index >= 0) && (index < loginInfos.size())) {
			return loginInfos.at(index);
		}
		return nullptr;
	}

	void del_pannel_cookies(const QString &pannelName) { PLSBasic::delPannelCookies(pannelName); }

	void pls_del_specific_url_cookie(const QString &url, const QString &cookieName) { main->delSpecificUrlCookie(url, cookieName); }

	QJsonObject pls_ssmap_to_json(const QMap<QString, QString> &ssmap)
	{
		QJsonObject result;
		for (auto iter = ssmap.begin(), endIter = ssmap.end(); iter != endIter; ++iter) {
			result.insert(iter.key(), iter.value());
		}
		return result;
	}

	bool pls_browser_view(QJsonObject &result, const QUrl &url, pls_result_checking_callback_t callback, QWidget *parent)
	{
		PLSBrowserView bv(&result, url, callback, parent);
		bv.update();
		return bv.exec() == QDialog::Accepted;
	}

	bool pls_browser_view(QJsonObject &result, const QUrl &url, const std::map<std::string, std::string> &headers, const QString &pannelName, pls_result_checking_callback_t callback,
			      QWidget *parent)
	{
		PLSBrowserView bv(&result, url, headers, pannelName, callback, parent);
		// 2020.08.03 cheng.bing
		//disable hotkey,able hotkey

		App()->DisableHotkeys();
		bool isOk = (bv.exec() == QDialog::Accepted);
		App()->UpdateHotkeyFocusSetting();
		return isOk;
	}

	bool pls_rtmp_view(QJsonObject &result, PLSLoginInfo *login_info, QWidget *parent)
	{
		/*PLSRtmpChannelView rclv(login_info, result, parent);
		return rclv.exec() == QDialog::Accepted;*/
		return false;
	}

	bool pls_channel_login(QJsonObject &result, QWidget *parent) { return false; }
	bool pls_sns_user_info(QJsonObject &result, const QList<QNetworkCookie> &cookies, const QString &urlStr) { return false; }
	bool pls_network_environment_reachable()
	{
		NetworkEnvironment environment;
		return environment.getNetWorkEnvironment();
	}

	bool pls_prism_token_is_vaild(const QString &urlStr) { return false; }
	void pls_prism_change_over_login_view() { return; }
	void pls_prism_logout() { return; }
	QString pls_prism_user_thumbnail_path() { return QString(); }
	Common pls_get_common() { return PLSGpopData::instance()->getCommon(); }
	VliveNotice pls_get_vlive_notice() { return PLSGpopData::instance()->getVliveNotice(); }
	QMap<QString, SnsCallbackUrl> pls_get_snscallback_urls() { return QMap<QString, SnsCallbackUrl>(); }
	Connection pls_get_connection() { return PLSGpopData::instance()->getConnection(); }
	QMap<int, RtmpDestination> pls_get_rtmpDestination() { return PLSGpopData::instance()->getRtmpDestination(); };

	QString pls_get_gcc_data() { return QString(); }
	QString pls_get_prism_token() { return QString(); }
	QString pls_get_prism_email() { return QString(); }
	QString pls_get_prism_thmbanilurl() { return QString(); }
	QString pls_get_prism_nickname() { return QString(); }
	QString pls_get_prism_usercode() { return QString(); }

	QWidget *pls_get_main_view() { return main->getMainView(); }
	QWidget *pls_get_toplevel_view(QWidget *widget)
	{
		if (!widget) {
			return main->getMainView();
		} else if (dynamic_cast<PLSMainView *>(widget) || dynamic_cast<PLSDialogView *>(widget)) {
			return widget;
		}

		while (widget = widget->parentWidget()) {
			if (dynamic_cast<PLSMainView *>(widget) || dynamic_cast<PLSDialogView *>(widget)) {
				return widget;
			}
		}
		return main->getMainView();
	}

	QByteArray pls_get_prism_cookie() { return QByteArray(); }

	void pls_frontend_add_event_callback(pls_frontend_event_cb callback, void *context) { eventCallbacks.append(make_tuple(QList<pls_frontend_event>{}, callback, context)); }
	void pls_frontend_add_event_callback(pls_frontend_event event, pls_frontend_event_cb callback, void *context)
	{
		eventCallbacks.append(make_tuple(QList<pls_frontend_event>{event}, callback, context));
	}
	void pls_frontend_add_event_callback(QList<pls_frontend_event> events, pls_frontend_event_cb callback, void *context) { eventCallbacks.append(make_tuple(events, callback, context)); }
	void pls_frontend_remove_event_callback(pls_frontend_event_cb callback, void *context) { eventCallbacks.removeOne(make_tuple(QList<pls_frontend_event>{}, callback, context)); }
	void pls_frontend_remove_event_callback(pls_frontend_event event, pls_frontend_event_cb callback, void *context)
	{
		eventCallbacks.removeOne(make_tuple(QList<pls_frontend_event>{event}, callback, context));
	}
	void pls_frontend_remove_event_callback(QList<pls_frontend_event> events, pls_frontend_event_cb callback, void *context) { eventCallbacks.removeOne(make_tuple(events, callback, context)); }
	void on_event(pls_frontend_event event, const QVariantList &params)
	{
		for (auto iter = eventCallbacks.begin(); iter != eventCallbacks.end(); ++iter) {
			if (std::get<0>(*iter).isEmpty() || std::get<0>(*iter).contains(event)) {
				std::get<1> (*iter)(event, params, std::get<2>(*iter));
			}
		}
	}

	QString pls_get_theme_dir_path() { return "data/prism-studio/themes/Dark/"; }
	QString pls_get_color_filter_dir_path() { return pls_get_user_path("PRISMLiveStudio/color_filter/"); }

	void pls_toast_message(pls_toast_info_type type, const QString &message, int auto_close)
	{
		QMetaObject::invokeMethod(main, "toastMessage", Q_ARG(pls_toast_info_type, type), Q_ARG(QString, message), Q_ARG(int, auto_close));
	}
	void pls_toast_message(pls_toast_info_type type, const QString &message, const QString &url, const QString &replaceStr, int auto_close)
	{
		QMetaObject::invokeMethod(main, "toastMessage", Q_ARG(pls_toast_info_type, type), Q_ARG(QString, message), Q_ARG(QString, url), Q_ARG(QString, replaceStr), Q_ARG(int, auto_close));
	}
	void pls_toast_clear() { QMetaObject::invokeMethod(main, "toastClear"); }

	void pls_set_main_view_side_bar_user_button_icon(const QIcon &icon) { main->getMainView()->setUserButtonIcon(icon); }

	void pls_load_chat_dock(const QString &objectName, const std::string &url, const std::string &white_popup_url, const std::string &startup_script)
	{
		if (!cef) {
			return;
		}

		PLSBasic::InitBrowserPanelSafeBlock();

		QSharedPointer<BrowserDock> chat;
		if (chatDocks.contains(objectName)) {
			chat = chatDocks[objectName];
		} else {
			chat.reset(new BrowserDock());
			chatDocks.insert(objectName, chat);
		}
		chat->setObjectName(objectName);
		chat->resize(300, 600);
		chat->setMinimumSize(200, 300);
		chat->setWindowTitle(objectName);
		chat->setAllowedAreas(Qt::AllDockWidgetAreas);

		QCefWidget *browser = cef->create_widget(nullptr, url, startup_script, panel_cookies);
		chat->SetWidget(browser);
		if (!white_popup_url.empty()) {
			cef->add_force_popup_url(white_popup_url, chat.data());
		}

		main->addDockWidget(Qt::RightDockWidgetArea, chat.data());
		chat->setVisible(true);
	}

	void pls_unload_chat_dock(const QString &objectName)
	{
		if (chatDocks.contains(objectName)) {
			auto dock = chatDocks[objectName];

			chatDocks.remove(objectName);
		}
	}

	const char *pls_basic_config_get_string(const char *section, const char *name, const char *) { return config_get_string(main->Config(), section, name); }
	int64_t pls_basic_config_get_int(const char *section, const char *name, int64_t) { return config_get_int(main->Config(), section, name); }
	uint64_t pls_basic_config_get_uint(const char *section, const char *name, uint64_t) { return config_get_uint(main->Config(), section, name); }
	bool pls_basic_config_get_bool(const char *section, const char *name, bool) { return config_get_bool(main->Config(), section, name); }
	double pls_basic_config_get_double(const char *section, const char *name, double) { return config_get_double(main->Config(), section, name); }

	pls_check_update_result_t pls_check_update(QString &gcc, bool &is_force, QString &version, QString &file_url, QString &update_info_url)
	{ return pls_check_update_result_t::Failed;
	}

	bool pls_check_lastest_version(QString &update_info_url) { return false; }
	pls_upload_file_result_t pls_upload_contactus_files(const QString &email, const QString &question, const QList<QFileInfo> files) { return pls_upload_file_result_t::NetworkError; }
	bool pls_show_update_info_view(bool is_force, const QString &version, const QString &file_url, const QString &update_info_url, bool is_manual, QWidget *parent)
	{ return false;
	}
	bool pls_download_update(QString &local_file_path, const QString &file_url, PLSCancel &cancel, const std::function<void(qint64 download_bytes, qint64 total_bytes)> &progress)
	{ return false;
	}
	bool pls_install_update(const QString &file_path) { return false; }

	QVariantMap pls_get_new_notice_Info() { return QVariantMap(); }
	QString pls_get_win_os_version()
	{
	return QString();
	}

	bool pls_is_living_or_recording() { return PLS_PLATFORM_API->isGoLiveOrRecording(); }

	// add tools menu seperator
	void pls_add_tools_menu_seperator()
	{
		if (!main->ui->menuTools->isEmpty()) {
			main->ui->menuTools->addSeparator();
		}
	}
	void pls_start_broadcast(bool toStart = true)
	{
		if (toStart) {
			PLSCHANNELS_API->toStartBroadcast();
		} else {
			PLSCHANNELS_API->toStopBroadcast();
		}
	}

	void pls_start_record(bool toStart = true)
	{
		if (toStart) {
			PLSCHANNELS_API->toStartRecord();
		} else {
			PLSCHANNELS_API->toStopRecord();
		}
	}

	ITextMotionTemplateHelper *pls_get_text_motion_template_helper_instance() override { return new TextMotionTemplateDataHelper(); }

	virtual QString pls_get_curreng_language() override { return QString::fromUtf8(App()->GetLocale()); }

	virtual int pls_get_actived_chat_channel_count() override { return getActivedChatChannelCount(); }
	virtual int pls_get_prism_live_seq() override { return 0; }
	virtual bool pls_is_create_souce_in_loading() override { return main->isCreateSouceInLoading; }

	virtual void pls_network_state_monitor(std::function<void(bool)> &&callback) override { QObject::connect(PLSNetworkMonitor::Instance(), &PLSNetworkMonitor::OnNetWorkStateChanged, callback); }
	virtual bool pls_get_network_state() override { return PLSNetworkMonitor::Instance()->IsInternetAvailable(); }
};

pls_frontend_callbacks *InitializeAPIInterface(PLSBasic *main)
{
	pls_frontend_callbacks *api = new PLSStudioAPI(main);
	pls_frontend_set_callbacks_internal(api);
	return api;
}
