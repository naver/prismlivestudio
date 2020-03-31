#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <string>

#include "window-basic-main.hpp"
#include "qt-wrappers.hpp"
#include "action.h"
#include "liblog.h"
#include "log/module_names.h"
#include "pls-common-define.hpp"

using namespace std;

static const char *textExtensions[] = {"txt", "log", nullptr};

static const char *imageExtensions[] = {"bmp", "tga", "png", "jpg", "jpeg", "gif", nullptr};

static const char *htmlExtensions[] = {"htm", "html", nullptr};

static const char *mediaExtensions[] = {"3ga",  "669", "a52", "aac",  "ac3", "adt",  "adts", "aif",  "aifc",  "aiff",  "amb",   "amr", "aob",  "ape", "au",   "awb",  "caf", "dts", "flac", "it",
					"kar",  "m4a", "m4b", "m4p",  "m5p", "mid",  "mka",  "mlp",  "mod",   "mpa",   "mp1",   "mp2", "mp3",  "mpc", "mpga", "mus",  "oga", "ogg", "oma",  "opus",
					"qcp",  "ra",  "rmi", "s3m",  "sid", "spx",  "tak",  "thd",  "tta",   "voc",   "vqf",   "w64", "wav",  "wma", "wv",   "xa",   "xm",  "3g2", "3gp",  "3gp2",
					"3gpp", "amv", "asf", "avi",  "bik", "crf",  "divx", "drc",  "dv",    "evo",   "f4v",   "flv", "gvi",  "gxf", "iso",  "m1v",  "m2v", "m2t", "m2ts", "m4v",
					"mkv",  "mov", "mp2", "mp2v", "mp4", "mp4v", "mpe",  "mpeg", "mpeg1", "mpeg2", "mpeg4", "mpg", "mpv2", "mts", "mtv",  "mxf",  "mxg", "nsv", "nuv",  "ogg",
					"ogm",  "ogv", "ogx", "ps",   "rec", "rm",   "rmvb", "rpl",  "thp",   "tod",   "ts",    "tts", "txd",  "vob", "vro",  "webm", "wm",  "wmv", "wtv",  nullptr};

static string GenerateSourceName(const char *base)
{
	string name;
	int inc = 0;

	for (;; inc++) {
		name = base;

		if (inc) {
			name += " (";
			name += to_string(inc + 1);
			name += ")";
		}

		obs_source_t *source = obs_get_source_by_name(name.c_str());
		if (!source) {
			return name;
		} else {
			obs_source_release(source);
		}
	}
}

void PLSBasic::AddDropSource(const char *data, DropType image)
{
	PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
	obs_data_t *settings = obs_data_create();
	obs_source_t *source = nullptr;
	const char *type = nullptr;
	QString name;

	switch (image) {
	case DropType_RawText:
		obs_data_set_string(settings, "text", data);
#ifdef _WIN32
		type = "text_gdiplus";
#else
		type = "text_ft2_source";
#endif
		break;
	case DropType_Text:
#ifdef _WIN32
		obs_data_set_bool(settings, "read_from_file", true);
		obs_data_set_string(settings, "file", data);
		name = QUrl::fromLocalFile(QString(data)).fileName();
		type = "text_gdiplus";
#else
		obs_data_set_bool(settings, "from_file", true);
		obs_data_set_string(settings, "text_file", data);
		type = "text_ft2_source";
#endif
		break;
	case DropType_Image:
		obs_data_set_string(settings, "file", data);
		name = QUrl::fromLocalFile(QString(data)).fileName();
		type = "image_source";
		break;
	case DropType_Media:
		obs_data_set_string(settings, "local_file", data);
		name = QUrl::fromLocalFile(QString(data)).fileName();
		type = "ffmpeg_source";
		break;
	case DropType_Html:
		obs_data_set_bool(settings, "is_local_file", true);
		obs_data_set_string(settings, "local_file", data);
		name = QUrl::fromLocalFile(QString(data)).fileName();
		type = "browser_source";
		break;
	}

	if (!obs_source_get_display_name(type)) {
		obs_data_release(settings);
		return;
	}

	if (name.isEmpty())
		name = obs_source_get_display_name(type);
	source = obs_source_create(type, GenerateSourceName(QT_TO_UTF8(name)).c_str(), settings, nullptr);
	if (source) {
		OBSScene scene = main->GetCurrentScene();
		obs_scene_add(scene, source);
		obs_source_release(source);
	}

	obs_data_release(settings);
}

void PLSBasic::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void PLSBasic::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}

void PLSBasic::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void PLSBasic::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();

	if (mimeData->hasUrls()) {
		QList<QUrl> urls = mimeData->urls();
		for (int i = 0; i < urls.size(); i++) {
			QString file = urls.at(i).toLocalFile();
			QFileInfo fileInfo(file);

			if (!fileInfo.exists())
				continue;

			// Warning : here we must pass ExcludeUserInputEvents to delay handle user events.
			QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, FEED_UI_MAX_TIME);

			// Warning : while comparing extension, make sure you have converted to lowercase letters
			QString suffixQStr = fileInfo.suffix();
			QByteArray suffixArray = suffixQStr.toLower().toUtf8();
			const char *suffix = suffixArray.constData();

			if (CheckSuffix(file, suffix, textExtensions, DropType_Text)) {
				PLS_UI_STEP(MAINFRAME_MODULE, "Text file", ACTION_DROP);
				continue;
			}

			if (CheckSuffix(file, suffix, htmlExtensions, DropType_Html)) {
				PLS_UI_STEP(MAINFRAME_MODULE, "Html file", ACTION_DROP);
				continue;
			}

			if (CheckSuffix(file, suffix, imageExtensions, DropType_Image)) {
				PLS_UI_STEP(MAINFRAME_MODULE, "Image file", ACTION_DROP);
				continue;
			}

			if (CheckSuffix(file, suffix, mediaExtensions, DropType_Media)) {
				PLS_UI_STEP(MAINFRAME_MODULE, "Media file", ACTION_DROP);
				continue;
			}
		}
	} else if (mimeData->hasText()) {
		PLS_UI_STEP(MAINFRAME_MODULE, "Raw text", ACTION_DROP);
		AddDropSource(QT_TO_UTF8(mimeData->text()), DropType_RawText);
	}
}

bool PLSBasic::CheckSuffix(const QString &file, const char *suffix, const char *extensions[], enum DropType type)
{
	const char **cmp = extensions;
	while (*cmp) {
		if (strcmp(*cmp, suffix) == 0) {
			AddDropSource(QT_TO_UTF8(file), type);
			return true;
		}

		++cmp;
	}

	return false;
}
