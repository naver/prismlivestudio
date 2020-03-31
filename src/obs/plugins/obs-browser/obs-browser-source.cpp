/******************************************************************************
 Copyright (C) 2014 by John R. Bradley <jrb@turrettech.com>
 Copyright (C) 2018 by Hugh Bailey ("Jim") <jim@obsproject.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "obs-browser-source.hpp"
#include "browser-client.hpp"
#include "browser-scheme.hpp"
#include "wide-string.hpp"
#include "interaction/interaction_manager.h"
#include <util/threading.h>
#include <QApplication>
#include <util/dstr.h>
#include <functional>
#include <thread>
#include <mutex>

//PRISM/Wangshaohui/20201027/#4892/save url settings
#include <base64/base64.hpp>

#ifdef USE_QT_LOOP
#include <QEventLoop>
#include <QThread>
#endif

using namespace std;

extern bool QueueCEFTask(std::function<void()> task);

static mutex browser_list_mutex;
static BrowserSource *first_browser = nullptr;

void SendBrowserVisibility(CefRefPtr<CefBrowser> browser, bool isVisible)
{
	if (!browser)
		return;

#if ENABLE_WASHIDDEN
	if (isVisible) {
		browser->GetHost()->WasHidden(false);
		browser->GetHost()->Invalidate(PET_VIEW);
	} else {
		browser->GetHost()->WasHidden(true);
	}
#endif

	CefRefPtr<CefProcessMessage> msg =
		CefProcessMessage::Create("Visibility");
	CefRefPtr<CefListValue> args = msg->GetArgumentList();
	args->SetBool(0, isVisible);
	SendBrowserProcessMessage(browser, PID_RENDERER, msg);
}

BrowserSource::BrowserSource(obs_data_t *, obs_source_t *source_)
	: source(source_)
{
	/* defer update */
	obs_source_update(source, nullptr);

	lock_guard<mutex> lock(browser_list_mutex);
	p_prev_next = &first_browser;
	next = first_browser;
	if (first_browser)
		first_browser->p_prev_next = &next;
	first_browser = this;

	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
	{
		signal_handler_connect_ref(
			obs_source_get_signal_handler(source), "rename",
			BrowserSource::SourceRenamed, this);
		interaction_ui =
			INTERACTION_PTR(new BrowserInteractionMain(this));
		InteractionManager::Instance()->OnSourceCreated(this);
	}
}

BrowserSource::~BrowserSource()
{
	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
	{
		blog(LOG_INFO, "Destrcuture function of BrowserSource:%p",
		     this);

		signal_handler_disconnect(obs_source_get_signal_handler(source),
					  "rename",
					  BrowserSource::SourceRenamed, this);
		InteractionManager::Instance()->OnSourceDeleted(this);

		if (interaction_display) {
			blog(LOG_ERROR,
			     "Display is not cleared. BrowserSource:%p", this);
			assert(NULL == interaction_display &&
			       "display exist !");
		}
		DestroyInteraction();
	}

	DestroyBrowser();
	DestroyTextures();

	lock_guard<mutex> lock(browser_list_mutex);
	if (next)
		next->p_prev_next = p_prev_next;
	*p_prev_next = next;
}

void BrowserSource::ExecuteOnBrowser(BrowserFunc func, bool async)
{
	if (!async) {
#ifdef USE_QT_LOOP
		if (QThread::currentThread() == qApp->thread()) {
			if (!!cefBrowser)
				func(cefBrowser);
			return;
		}
#endif
		os_event_t *finishedEvent;
		os_event_init(&finishedEvent, OS_EVENT_TYPE_AUTO);
		bool success = QueueCEFTask([&]() {
			if (!!cefBrowser)
				func(cefBrowser);
			os_event_signal(finishedEvent);
		});
		if (success) {
			os_event_wait(finishedEvent);
		}
		os_event_destroy(finishedEvent);
	} else {
		CefRefPtr<CefBrowser> browser = cefBrowser;
		if (!!browser) {
#ifdef USE_QT_LOOP
			QueueBrowserTask(cefBrowser, func);
#else
			QueueCEFTask([=]() { func(browser); });
#endif
		}
	}
}

bool BrowserSource::CreateBrowser()
{
	return QueueCEFTask([this]() {
#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
		if (hwaccel) {
			obs_enter_graphics();
			tex_sharing_avail = gs_shared_texture_available();
			obs_leave_graphics();
		}
#else
		bool hwaccel = false;
#endif

		//PRISM/Wangshaohui/20200811/#3784/for cef interaction
		interaction_ui->CreateInteractionUI();
		PostInteractionTitle();

		struct obs_video_info ovi;
		obs_get_video_info(&ovi);

		//PRISM/Wangshaohui/20201021/#5271/for cef hardware accelerate
		CefRefPtr<BrowserClient> browserClient = new BrowserClient(
			this, hwaccel && tex_sharing_avail && use_hardware,
			reroute_audio,
			INTERACTION_WEAK_PTR(
				interaction_ui)); //PRISM/Wangshaohui/20200811/#3784/for cef interaction

		CefWindowInfo windowInfo;
#if CHROME_VERSION_BUILD < 3071
		windowInfo.transparent_painting_enabled = true;
#endif
		windowInfo.width = width;
		windowInfo.height = height;
		windowInfo.windowless_rendering_enabled = true;

#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
		//PRISM/Wangshaohui/20201021/#5271/for cef hardware accelerate
		windowInfo.shared_texture_enabled = hwaccel && use_hardware;
#endif

		CefBrowserSettings cefBrowserSettings;

#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
		if (!fps_custom) {
			windowInfo.external_begin_frame_enabled = true;
			cefBrowserSettings.windowless_frame_rate = 0;
		} else {
			cefBrowserSettings.windowless_frame_rate = fps;
		}
#else
		cefBrowserSettings.windowless_frame_rate = fps;
#endif

#if ENABLE_LOCAL_FILE_URL_SCHEME
		if (is_local) {
			/* Disable web security for file:// URLs to allow
			 * local content access to remote APIs */
			cefBrowserSettings.web_security = STATE_DISABLED;
		}
#endif

		cefBrowser = CefBrowserHost::CreateBrowserSync(
			windowInfo, browserClient, url, cefBrowserSettings,
#if CHROME_VERSION_BUILD >= 3770
			CefRefPtr<CefDictionaryValue>(),
#endif
			nullptr);
#if CHROME_VERSION_BUILD >= 3683
		if (reroute_audio)
			cefBrowser->GetHost()->SetAudioMuted(true);
#endif

		SendBrowserVisibility(cefBrowser, is_showing);

		//PRISM/Wangshaohui/20200811/#3784/for cef interaction
		interaction_ui->SetInteractionInfo(width, height, cefBrowser);
	});
}

void BrowserSource::DestroyBrowser(bool async)
{
	//PRISM/WangShaohui/20200729/NoIssue/for debugging destroy browser source
	blog(LOG_INFO, "DestroyBrowser for '%s', async:%s",
	     obs_source_get_name(source), async ? "yes" : "no");

	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
	ExecuteOnInteraction(
		[](INTERACTION_PTR interaction) {
			interaction->SetInteractionInfo(0, 0, NULL);
		},
		true);

	ExecuteOnBrowser(
		[](CefRefPtr<CefBrowser> cefBrowser) {
			CefRefPtr<CefClient> client =
				cefBrowser->GetHost()->GetClient();
			BrowserClient *bc =
				reinterpret_cast<BrowserClient *>(client.get());

			//PRISM/WangShaohui/20200729/NoIssue/for debugging destroy browser source
			blog(LOG_INFO,
			     "DestroyBrowser is invoked, BrowserClient:%p", bc);

			if (bc) {
				bc->bs = nullptr;
			}

			/*
		 * This stops rendering
		 * http://magpcss.org/ceforum/viewtopic.php?f=6&t=12079
		 * https://bitbucket.org/chromiumembedded/cef/issues/1363/washidden-api-got-broken-on-branch-2062)
		 */
			cefBrowser->GetHost()->WasHidden(true);
			cefBrowser->GetHost()->CloseBrowser(true);
		},
		async);

	cefBrowser = nullptr;
}

void BrowserSource::ClearAudioStreams()
{
	QueueCEFTask([this]() {
		audio_streams.clear();
		std::lock_guard<std::mutex> lock(audio_sources_mutex);
		audio_sources.clear();
	});
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
/*
void BrowserSource::SendMouseClick(const struct obs_mouse_event *event,
				   int32_t type, bool mouse_up,
				   uint32_t click_count)
{
	uint32_t modifiers = event->modifiers;
	int32_t x = event->x;
	int32_t y = event->y;

	ExecuteOnBrowser(
		[=](CefRefPtr<CefBrowser> cefBrowser) {
			CefMouseEvent e;
			e.modifiers = modifiers;
			e.x = x;
			e.y = y;
			CefBrowserHost::MouseButtonType buttonType =
				(CefBrowserHost::MouseButtonType)type;
			cefBrowser->GetHost()->SendMouseClickEvent(
				e, buttonType, mouse_up, click_count);
		},
		true);
}

void BrowserSource::SendMouseMove(const struct obs_mouse_event *event,
				  bool mouse_leave)
{
	uint32_t modifiers = event->modifiers;
	int32_t x = event->x;
	int32_t y = event->y;

	ExecuteOnBrowser(
		[=](CefRefPtr<CefBrowser> cefBrowser) {
			CefMouseEvent e;
			e.modifiers = modifiers;
			e.x = x;
			e.y = y;
			cefBrowser->GetHost()->SendMouseMoveEvent(e,
								  mouse_leave);
		},
		true);
}

void BrowserSource::SendMouseWheel(const struct obs_mouse_event *event,
				   int x_delta, int y_delta)
{
	uint32_t modifiers = event->modifiers;
	int32_t x = event->x;
	int32_t y = event->y;

	ExecuteOnBrowser(
		[=](CefRefPtr<CefBrowser> cefBrowser) {
			CefMouseEvent e;
			e.modifiers = modifiers;
			e.x = x;
			e.y = y;
			cefBrowser->GetHost()->SendMouseWheelEvent(e, x_delta,
								   y_delta);
		},
		true);
}

void BrowserSource::SendFocus(bool focus)
{
	ExecuteOnBrowser(
		[=](CefRefPtr<CefBrowser> cefBrowser) {
			cefBrowser->GetHost()->SendFocusEvent(focus);
		},
		true);
}

void BrowserSource::SendKeyClick(const struct obs_key_event *event, bool key_up)
{
	uint32_t modifiers = event->modifiers;
	std::string text = event->text;
	uint32_t native_vkey = event->native_vkey;

	ExecuteOnBrowser(
		[=](CefRefPtr<CefBrowser> cefBrowser) {
			CefKeyEvent e;
			e.windows_key_code = native_vkey;
			e.native_key_code = 0;

			e.type = key_up ? KEYEVENT_KEYUP : KEYEVENT_RAWKEYDOWN;

			if (!text.empty()) {
				wstring wide = to_wide(text);
				if (wide.size())
					e.character = wide[0];
			}

			//e.native_key_code = native_vkey;
			e.modifiers = modifiers;

			cefBrowser->GetHost()->SendKeyEvent(e);
			if (!text.empty() && !key_up) {
				e.type = KEYEVENT_CHAR;
				e.windows_key_code = e.character;
				e.native_key_code = native_vkey;
				cefBrowser->GetHost()->SendKeyEvent(e);
			}
		},
		true);
}
*/

void BrowserSource::SetShowing(bool showing)
{
	is_showing = showing;

	if (shutdown_on_invisible) {
		if (showing) {
			Update();
		} else {
			DestroyBrowser(true);
		}
	} else {
#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
		if (showing && !fps_custom) {
			reset_frame = false;
		}
#endif

		SendBrowserVisibility(cefBrowser, showing);
	}
}

void BrowserSource::SetActive(bool active)
{
	ExecuteOnBrowser(
		[=](CefRefPtr<CefBrowser> cefBrowser) {
			CefRefPtr<CefProcessMessage> msg =
				CefProcessMessage::Create("Active");
			CefRefPtr<CefListValue> args = msg->GetArgumentList();
			args->SetBool(0, active);
			SendBrowserProcessMessage(cefBrowser, PID_RENDERER,
						  msg);
		},
		true);
}

void BrowserSource::Refresh()
{
	ExecuteOnBrowser(
		[](CefRefPtr<CefBrowser> cefBrowser) {
			cefBrowser->ReloadIgnoreCache();
		},
		true);
}

#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
inline void BrowserSource::SignalBeginFrame()
{
	if (reset_frame) {
		ExecuteOnBrowser(
			[](CefRefPtr<CefBrowser> cefBrowser) {
				cefBrowser->GetHost()->SendExternalBeginFrame();
			},
			true);

		reset_frame = false;
	}
}
#endif

//PRISM/Zhangdewen/20200901/#for chat source
void BrowserSource::onBrowserLoadEnd() {}

void BrowserSource::Update(obs_data_t *settings)
{
	if (settings) {
		bool n_is_local;
		int n_width;
		int n_height;
		bool n_fps_custom;
		int n_fps;
		bool n_shutdown;
		bool n_restart;
		bool n_reroute;
		std::string n_url;
		std::string n_css;

		//PRISM/Wangshaohui/20201021/#5271/for cef hardware accelerate
		bool n_hareware;

		n_is_local = obs_data_get_bool(settings, "is_local_file");
		n_width = (int)obs_data_get_int(settings, "width");
		n_height = (int)obs_data_get_int(settings, "height");
		n_fps_custom = obs_data_get_bool(settings, "fps_custom");
		n_fps = (int)obs_data_get_int(settings, "fps");
		n_shutdown = obs_data_get_bool(settings, "shutdown");
		n_restart = obs_data_get_bool(settings, "restart_when_active");
		n_css = obs_data_get_string(settings, "css");
		n_url = obs_data_get_string(settings,
					    n_is_local ? "local_file" : "url");
		n_reroute = obs_data_get_bool(settings, "reroute_audio");

		//PRISM/Wangshaohui/20201021/#5271/for cef hardware accelerate
		n_hareware = obs_data_get_bool(settings, "hardware_accelerate");

		if (n_is_local && !n_url.empty()) {
			n_url = CefURIEncode(n_url, false);

#ifdef _WIN32
			size_t slash = n_url.find("%2F");
			size_t colon = n_url.find("%3A");

			if (slash != std::string::npos &&
			    colon != std::string::npos && colon < slash)
				n_url.replace(colon, 3, ":");
#endif

			while (n_url.find("%5C") != std::string::npos)
				n_url.replace(n_url.find("%5C"), 3, "/");

			while (n_url.find("%2F") != std::string::npos)
				n_url.replace(n_url.find("%2F"), 3, "/");

#if !ENABLE_LOCAL_FILE_URL_SCHEME
			/* http://absolute/ based mapping for older CEF */
			n_url = "http://absolute/" + n_url;
#elif defined(_WIN32)
			/* Widows-style local file URL:
			 * file:///C:/file/path.webm */
			n_url = "file:///" + n_url;
#else
			/* UNIX-style local file URL:
			 * file:///home/user/file.webm */
			n_url = "file://" + n_url;
#endif
		}

#if ENABLE_LOCAL_FILE_URL_SCHEME
		if (astrcmpi_n(n_url.c_str(), "http://absolute/", 16) == 0) {
			/* Replace http://absolute/ URLs with file://
			 * URLs if file:// URLs are enabled */
			n_url = "file:///" + n_url.substr(16);
			n_is_local = true;
		}
#endif

		if (n_is_local == is_local && n_width == width &&
		    n_height == height && n_fps_custom == fps_custom &&
		    n_fps == fps && n_shutdown == shutdown_on_invisible &&
		    n_restart == restart && n_css == css && n_url == url &&
		    n_reroute == reroute_audio && n_hareware == use_hardware) {
			return;
		}

		is_local = n_is_local;
		width = n_width;
		height = n_height;
		fps = n_fps;
		fps_custom = n_fps_custom;
		shutdown_on_invisible = n_shutdown;
		reroute_audio = n_reroute;
		restart = n_restart;
		css = n_css;
		url = n_url;

		//PRISM/Wangshaohui/20201021/#5271/for cef hardware accelerate
		use_hardware = n_hareware;

		//PRISM/Wangshaohui/20201027/#4892/save url settings
		blog(LOG_INFO, "---------------------------------");
		blog(LOG_INFO,
		     "[Browser : '%s'] web updated: \n"
		     "is_local_file: %d \n"
		     "url: %s \n"
		     "resolution: %d x %d \n"
		     "reroute_audio: %d \n"
		     "shutdown_when_invisible: %d \n"
		     "refresh_when_active: %d \n"
		     "hardware_accelerate: %d",
		     obs_source_get_name(source), n_is_local,
		     base64_encode(n_url).c_str(), n_width, n_height, n_reroute,
		     n_shutdown, n_restart, n_hareware);
		blog(LOG_INFO, "---------------------------------");

		obs_source_set_audio_active(source, reroute_audio);
	}

	DestroyBrowser(true);
	DestroyTextures();
	ClearAudioStreams();
	if (!shutdown_on_invisible || obs_source_showing(source))
		create_browser = true;

	first_update = false;
}

void BrowserSource::Tick()
{
	if (create_browser && CreateBrowser())
		create_browser = false;
#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
	if (!fps_custom)
		reset_frame = true;
#endif

	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
	HWND top_hwnd = interaction_ui->GetInteractionMain();
	if (::IsWindow(top_hwnd)) {
		if (is_interaction_showing) {
			OnInteractionShow(top_hwnd);
		} else {
			OnInteractionHide(top_hwnd);
		}
	}
}

extern void ProcessCef();

void BrowserSource::Render()
{
	bool flip = false;
#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
	//PRISM/Wangshaohui/20201021/#5271/for cef hardware accelerate
	flip = hwaccel && use_hardware;
#endif

	if (texture) {
		gs_effect_t *effect =
			obs_get_base_effect(OBS_EFFECT_PREMULTIPLIED_ALPHA);
		while (gs_effect_loop(effect, "Draw"))
			obs_source_draw(texture, 0, 0, 0, 0, flip);
	}

#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
	SignalBeginFrame();
#elif USE_QT_LOOP
	ProcessCef();
#endif
}

void ExecuteOnAllBrowsers(BrowserFunc func)
{
	lock_guard<mutex> lock(browser_list_mutex);

	BrowserSource *bs = first_browser;
	while (bs) {
		BrowserSource *bsw = reinterpret_cast<BrowserSource *>(bs);
		bsw->ExecuteOnBrowser(func, true);
		bs = bs->next;
	}
}

//PRISM/Zhangdewen/20200901/#for chat source
void ExecuteOnOneBrowser(obs_source_t *source, BrowserFunc func)
{
	lock_guard<mutex> lock(browser_list_mutex);

	for (BrowserSource *bs = first_browser; bs; bs = bs->next) {
		BrowserSource *bsw = reinterpret_cast<BrowserSource *>(bs);
		if (bsw->source == source) {
			bsw->ExecuteOnBrowser(func, true);
			return;
		}
	}
}

void DispatchJSEvent(std::string eventName, std::string jsonString)
{
	ExecuteOnAllBrowsers([=](CefRefPtr<CefBrowser> cefBrowser) {
		CefRefPtr<CefProcessMessage> msg =
			CefProcessMessage::Create("DispatchJSEvent");
		CefRefPtr<CefListValue> args = msg->GetArgumentList();

		args->SetString(0, eventName);
		args->SetString(1, jsonString);
		SendBrowserProcessMessage(cefBrowser, PID_RENDERER, msg);
	});
}

//PRISM/Zhangdewen/20200901/#for chat source
void DispatchJSEvent(obs_source_t *source, std::string eventName,
		     std::string jsonString)
{
	ExecuteOnOneBrowser(source, [=](CefRefPtr<CefBrowser> cefBrowser) {
		CefRefPtr<CefProcessMessage> msg =
			CefProcessMessage::Create("DispatchJSEvent");
		CefRefPtr<CefListValue> args = msg->GetArgumentList();

		args->SetString(0, eventName);
		args->SetString(1, jsonString);
		SendBrowserProcessMessage(cefBrowser, PID_RENDERER, msg);
	});
}

//PRISM/Zhangdewen/20200901/#for chat source
void DispatchJSEvent(BrowserSource *source, std::string eventName,
		     std::string jsonString)
{
	source->ExecuteOnBrowser(
		[=](CefRefPtr<CefBrowser> cefBrowser) {
			CefRefPtr<CefProcessMessage> msg =
				CefProcessMessage::Create("DispatchJSEvent");
			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			args->SetString(0, eventName);
			args->SetString(1, jsonString);
			SendBrowserProcessMessage(cefBrowser, PID_RENDERER,
						  msg);
		},
		true);
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
void BrowserSource::ShowInteraction(bool show)
{
	// WARNING:
	// In this function, we'd better not add any lock !

	if (is_interaction_showing != show) {
		if (show) {
			blog(LOG_INFO,
			     "Request show interaction for '%s', BrowserSource:%p",
			     obs_source_get_name(source), this);
		} else {
			blog(LOG_INFO,
			     "Request hide interaction for '%s', BrowserSource:%p",
			     obs_source_get_name(source), this);
		}
	}

	if (is_interaction_showing && show) {
		is_interaction_reshow = true;
	} else {
		is_interaction_reshow = false;
	}

	is_interaction_showing = show;
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
void BrowserSource::DestroyInteraction()
{
	// Here we firstly post a message to make sure the window can be destroied soon.
	interaction_ui->PostDestroyInteractionUI();

	blog(LOG_INFO, "To run DestroyInteraction for '%s'",
	     obs_source_get_name(source));

	ExecuteOnInteraction(
		[](INTERACTION_PTR interaction) {
			blog(LOG_INFO,
			     "DestroyInteraction is invoked, address:%p",
			     interaction.get());

			interaction->SetInteractionInfo(0, 0, NULL);
			interaction->WaitDestroyInteractionUI();
		},
		false); // Here we must wait interaction window to be destroied, because its instance will be freed soon.
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
bool BrowserSource::CreateDisplay(HWND hWnd, int cx, int cy)
{
	assert(NULL == interaction_display);
	assert(NULL == reference_source);

	reference_source = obs_get_source_by_name(obs_source_get_name(source));
	if (!reference_source) {
		assert(false);
		return false;
	}

	gs_init_data info = {};
	info.cx = cx;
	info.cy = cy;
	info.format = GS_BGRA;
	info.zsformat = GS_ZS_NONE;
	info.window.hwnd = hWnd;

	interaction_display = obs_display_create(&info, VIEW_BK_RENDER_COLOR);
	assert(interaction_display);
	if (!interaction_display) {
		obs_source_release(reference_source);
		reference_source = NULL;
		return false;
	}

	display_cx = cx;
	display_cy = cy;

	obs_display_add_draw_callback(interaction_display,
				      BrowserSource::DrawPreview, this);

	blog(LOG_INFO,
	     "Added display for interaction for '%s', BrowserSource:%p",
	     obs_source_get_name(source), this);

	return true;
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
void BrowserSource::ClearDisplay()
{
	if (interaction_display) {
		obs_display_remove_draw_callback(
			interaction_display, BrowserSource::DrawPreview, this);

		obs_display_destroy(interaction_display);

		interaction_display = NULL;
		display_cx = 0;
		display_cy = 0;

		blog(LOG_INFO,
		     "Removed display for interaction for '%s', BrowserSource:%p",
		     obs_source_get_name(source), this);
	}

	if (reference_source) {
		obs_source_release(reference_source);
		reference_source = NULL;
	}
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
void BrowserSource::OnInteractionShow(HWND top_hwnd)
{
	if (!::IsWindowVisible(top_hwnd)) {
		is_interaction_reshow = false;
		ExecuteOnInteraction(
			[](INTERACTION_PTR interaction) {
				interaction->ShowInteractionUI(true);
			},
			true);
	} else {
		if (is_interaction_reshow) {
			is_interaction_reshow = false;
			ExecuteOnInteraction(
				[](INTERACTION_PTR interaction) {
					interaction->BringUIToTop();
				},
				true);
		}

		if (interaction_ui->IsResizing()) {
			if (interaction_display &&
			    obs_display_enabled(interaction_display)) {
				obs_display_set_enabled(interaction_display,
							false);
			}
			// We won't render display while resizing interaction to avoid UI flicker
			return;
		} else {
			if (interaction_display &&
			    !obs_display_enabled(interaction_display)) {
				obs_display_set_enabled(interaction_display,
							true);
			}
		}

		HWND view_hwnd = interaction_ui->GetInteractionView();

		RECT rc;
		GetClientRect(view_hwnd, &rc);

		int cx = RectWidth(rc);
		int cy = RectHeight(rc);

		if (!interaction_display) {
			CreateDisplay(view_hwnd, cx, cy);
		} else {
			if (cx != display_cx || cy != display_cy) {
				obs_display_resize(interaction_display, cx, cy);
				display_cx = cx;
				display_cy = cy;
			}
		}
	}
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
void BrowserSource::OnInteractionHide(HWND top_hwnd)
{
	if (::IsWindowVisible(top_hwnd)) {
		ExecuteOnInteraction(
			[](INTERACTION_PTR interaction) {
				interaction->ShowInteractionUI(false);
			},
			true);
	}

	ClearDisplay();
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
void BrowserSource::ExecuteOnInteraction(InteractionFunc func, bool async)
{
	if (!async) {
#ifdef USE_QT_LOOP
		if (QThread::currentThread() == qApp->thread()) {
			if (!!cefBrowser)
				func(cefBrowser);
			return;
		}
#endif
		os_event_t *finishedEvent;
		os_event_init(&finishedEvent, OS_EVENT_TYPE_AUTO);

		bool success = QueueCEFTask([&]() {
			if (!!interaction_ui)
				func(interaction_ui);
			os_event_signal(finishedEvent);
		});

		if (success) {
			os_event_wait(finishedEvent);
		}
		os_event_destroy(finishedEvent);

	} else {
		INTERACTION_PTR ui = interaction_ui;
		if (!!ui) {
#ifdef USE_QT_LOOP
			QueueBrowserTask(interaction_ui, func);
#else
			QueueCEFTask([=]() { func(ui); });
#endif
		}
	}
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
void BrowserSource::PostInteractionTitle()
{
	const char *name = obs_source_get_name(source);
	if (name && *name) {
		const char *fmt = obs_module_text("Basic.InteractionWindow");
		if (fmt && *fmt) {
			QString title = QString(fmt).arg(name);
			interaction_ui->PostWindowTitle(
				title.toStdString().c_str());
		}
	}
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
void BrowserSource::SourceRenamed(void *data, calldata_t *params)
{
	BrowserSource *self = (BrowserSource *)data;
	self->PostInteractionTitle();
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
void BrowserSource::DrawPreview(void *data, uint32_t cx, uint32_t cy)
{
	BrowserSource *window = static_cast<BrowserSource *>(data);

	if (!window->source)
		return;

	if (window->interaction_ui->IsResizing()) {
		return;
	}

	uint32_t sourceCX = max(obs_source_get_width(window->source), 1u);
	uint32_t sourceCY = max(obs_source_get_height(window->source), 1u);

	int x, y;
	float scale;
	GetScaleAndCenterPos(sourceCX, sourceCY, cx, cy, x, y, scale);

	int newCX = int(scale * float(sourceCX));
	int newCY = int(scale * float(sourceCY));

	gs_viewport_push();
	gs_projection_push();
	gs_ortho(0.0f, float(sourceCX), 0.0f, float(sourceCY), -100.0f, 100.0f);
	gs_set_viewport(x, y, newCX, newCY);

	obs_source_video_render(window->source);

	gs_projection_pop();
	gs_viewport_pop();
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
bool BrowserSource::SetBrowserData(void *src, obs_data_t *data)
{
	if (!data) {
		assert(false);
		return false;
	}

	if (!src) {
		// set global params
		BrowserInteractionMain::SetGlobalParam(data);
		return true;
	} else {
		const char *method = obs_data_get_string(data, "method");
		if (!method) {
			assert(false);
			return false;
		}

		BrowserSource *source = reinterpret_cast<BrowserSource *>(src);
		if (0 == strcmp(method, "ShowInteract")) {
			source->ShowInteraction(true);
			return true;
		} else if (0 == strcmp(method, "HideInterct")) {
			source->ShowInteraction(false);
			return true;
		} else {
			return false;
		}
	}
}

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
void BrowserSource::GetBrowserData(void *src, obs_data_t *data)
{
	if (!data) {
		return;
	}

	if (!src) {
		// get global params
		obs_data_set_int(data, "interaction_cx",
				 BrowserInteractionMain::user_size.cx);
		obs_data_set_int(data, "interaction_cy",
				 BrowserInteractionMain::user_size.cy);
	}
}
