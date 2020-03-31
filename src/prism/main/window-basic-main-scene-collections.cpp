/******************************************************************************
    Copyright (C) 2015 by Hugh Bailey <obs.jim@gmail.com>

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

#include <obs.hpp>
#include <util/util.hpp>
#include <QVariant>
#include <QFileDialog>
#include <QStandardPaths>
#include "item-widget-helpers.hpp"
#include "window-basic-main.hpp"
#include "window-namedialog.hpp"
#include "qt-wrappers.hpp"
#include "log.h"
#include "action.h"
#include "log/module_names.h"
#include "PLSSceneDataMgr.h"

using namespace std;

void EnumSceneCollections(std::function<bool(const char *, const char *)> &&cb)
{
	char path[512];
	os_glob_t *glob;

	int ret = GetConfigPath(path, sizeof(path), "PRISMLiveStudio/basic/scenes/*.json");
	if (ret <= 0) {
		blog(LOG_WARNING, "Failed to get config path for scene "
				  "collections");
		return;
	}

	if (os_glob(path, 0, &glob) != 0) {
		blog(LOG_WARNING, "Failed to glob scene collections");
		return;
	}

	for (size_t i = 0; i < glob->gl_pathc; i++) {
		const char *filePath = glob->gl_pathv[i].path;

		if (glob->gl_pathv[i].directory)
			continue;

		obs_data_t *data = obs_data_create_from_json_file_safe(filePath, "bak");
		std::string name = obs_data_get_string(data, "name");

		/* if no name found, use the file name as the name
		 * (this only happens when switching to the new version) */
		if (name.empty()) {
			name = strrchr(filePath, '/') + 1;
			name.resize(name.size() - 5);
		}

		obs_data_release(data);

		if (!cb(name.c_str(), filePath))
			break;
	}

	os_globfree(glob);
}

bool PLSBasic::SceneCollectionExists(const char *findName)
{
	bool found = false;
	QList<QAction *> menuActions = ui->sceneCollectionMenu->actions();
	for (int i = 0; i < menuActions.count(); i++) {
		QVariant v = menuActions[i]->property("name");
		if (v.typeName() != nullptr && 0 == strcmp(findName, v.toString().toStdString().c_str())) {
			found = true;
			return true;
		}
	}

	return found;
}

static bool GetUnusedSceneCollectionFile(std::string &name, std::string &file)
{
	char path[512];
	int ret;

	if (!GetFileSafeName(name.c_str(), file)) {
		blog(LOG_WARNING, "Failed to create safe file name for '%s'", name.c_str());
		return false;
	}

	ret = GetConfigPath(path, sizeof(path), "PRISMLiveStudio/basic/scenes/");
	if (ret <= 0) {
		blog(LOG_WARNING, "Failed to get scene collection config path");
		return false;
	}

	file.insert(0, path);

	if (!GetClosestUnusedFileName(file, "json")) {
		blog(LOG_WARNING, "Failed to get closest file name for %s", file.c_str());
		return false;
	}

	file.erase(file.size() - 5, 5);
	file.erase(0, strlen(path));
	return true;
}

bool PLSBasic::GetSceneCollectionName(QWidget *parent, std::string &name, std::string &file, const char *oldName)
{
	bool rename = oldName != nullptr;
	const char *title;
	const char *text;
	char path[512];
	size_t len;
	int ret;

	if (rename) {
		title = Str("Basic.Main.RenameSceneCollection.Title");
		text = Str("Basic.Main.AddSceneCollection.Text");
	} else {
		title = Str("Basic.Main.AddSceneCollection.Title");
		text = Str("Basic.Main.AddSceneCollection.Text");
	}

	for (;;) {
		bool success = NameDialog::AskForName(parent, title, text, name, QT_UTF8(oldName));
		if (!success) {
			return false;
		}

		name = QString(name.c_str()).simplified().toStdString();
		if (name.empty()) {
			PLSMessageBox::warning(parent, QTStr("NoNameEntered.Title"), QTStr("NoNameEntered.Text"));
			continue;
		}
		if (SceneCollectionExists(name.c_str())) {
			PLSMessageBox::warning(parent, QTStr("NameExists.Title"), QTStr("NameExists.Text"));
			continue;
		}
		break;
	}

	if (!GetUnusedSceneCollectionFile(name, file)) {
		return false;
	}

	return true;
}

bool PLSBasic::AddSceneCollection(bool create_new, const QString &qname)
{
	std::string name;
	std::string file;

	if (qname.isEmpty()) {
		if (!GetSceneCollectionName(this, name, file))
			return false;
	} else {
		if (SceneCollectionExists(qname.toStdString().c_str()))
			return false;
		if (!GetUnusedSceneCollectionFile(name, file)) {
			return false;
		}
	}

	SaveProjectNow();

	if (!create_new) {
		QString oldFile = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollectionFile");
		PLSSceneDataMgr::Instance()->MoveSrcToDest(oldFile, QString::fromStdString(file));
	}

	config_set_string(App()->GlobalConfig(), "Basic", "SceneCollection", name.c_str());
	config_set_string(App()->GlobalConfig(), "Basic", "SceneCollectionFile", file.c_str());
	if (create_new) {
		CreateDefaultScene(false);
	}
	SaveProjectNow();
	RefreshSceneCollections();

	blog(LOG_INFO, "Added scene collection '%s' (%s, %s.json)", name.c_str(), create_new ? "clean" : "duplicate", file.c_str());
	blog(LOG_INFO, "------------------------------------------------");

	UpdateTitleBar();

	if (api) {
		api->on_event(OBS_FRONTEND_EVENT_SCENE_COLLECTION_LIST_CHANGED);
		api->on_event(OBS_FRONTEND_EVENT_SCENE_COLLECTION_CHANGED);
	}

	return true;
}

void PLSBasic::RefreshSceneCollections()
{
	QList<QAction *> menuActions = ui->sceneCollectionMenu->actions();
	int count = 0;

	for (int i = 0; i < menuActions.count(); i++) {
		QVariant v = menuActions[i]->property("file_name");
		if (v.typeName() != nullptr)
			delete menuActions[i];
	}

	const char *cur_name = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollection");

	auto addCollection = [&](const char *name, const char *path) {
		std::string file = strrchr(path, '/') + 1;
		file.erase(file.size() - 5, 5);
		QAction *action = new QAction(QT_UTF8(name), this);
		action->setProperty("file_name", QT_UTF8(path));
		action->setProperty("name", QT_UTF8(name));
		connect(action, &QAction::triggered, this, &PLSBasic::ChangeSceneCollection);
		action->setCheckable(true);
		action->setChecked(strcmp(name, cur_name) == 0);
		ui->sceneCollectionMenu->addAction(action);
		count++;
		return true;
	};

	EnumSceneCollections(addCollection);

	/* force saving of first scene collection on first run, otherwise
	 * no scene collections will show up */
	if (!count) {
		long prevDisableVal = disableSaving;

		disableSaving = 0;
		SaveProjectNow();
		disableSaving = prevDisableVal;

		EnumSceneCollections(addCollection);
	}

	ui->actionRemoveSceneCollection->setEnabled(count > 1);

	PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());

	main->ui->actionPasteFilters->setEnabled(false);
	main->ui->actionPasteRef->setEnabled(false);
	main->ui->actionPasteDup->setEnabled(false);
}

void PLSBasic::on_actionNewSceneCollection_triggered()
{
	PLS_UI_STEP(MAINMENU_MODULE, "Main Menu Scene Collection New", ACTION_CLICK);
	AddSceneCollection(true);
}

void PLSBasic::on_actionDupSceneCollection_triggered()
{
	PLS_UI_STEP(MAINMENU_MODULE, "Main Menu Scene Collection Duplicate", ACTION_CLICK);
	AddSceneCollection(false);
}

void PLSBasic::on_actionRenameSceneCollection_triggered()
{
	PLS_UI_STEP(MAINMENU_MODULE, "Main Menu Scene Collection Rename", ACTION_CLICK);

	std::string name;
	std::string file;

	std::string oldFile = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollectionFile");
	const char *oldName = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollection");

	bool success = GetSceneCollectionName(this, name, file, oldName);
	if (!success)
		return;

	config_set_string(App()->GlobalConfig(), "Basic", "SceneCollection", name.c_str());
	config_set_string(App()->GlobalConfig(), "Basic", "SceneCollectionFile", file.c_str());
	PLSSceneDataMgr::Instance()->MoveSrcToDest(QString::fromStdString(oldFile), QString::fromStdString(file));
	SaveProjectNow();

	char path[512];
	int ret = GetConfigPath(path, 512, "PRISMLiveStudio/basic/scenes/");
	if (ret <= 0) {
		blog(LOG_WARNING, "Failed to get scene collection config path");
		return;
	}

	oldFile.insert(0, path);
	oldFile += ".json";
	os_unlink(oldFile.c_str());
	oldFile += ".bak";
	os_unlink(oldFile.c_str());

	blog(LOG_INFO, "------------------------------------------------");
	blog(LOG_INFO, "Renamed scene collection to '%s' (%s.json)", name.c_str(), file.c_str());
	blog(LOG_INFO, "------------------------------------------------");

	UpdateTitleBar();
	RefreshSceneCollections();

	if (api) {
		api->on_event(OBS_FRONTEND_EVENT_SCENE_COLLECTION_LIST_CHANGED);
		api->on_event(OBS_FRONTEND_EVENT_SCENE_COLLECTION_CHANGED);
	}
}

void PLSBasic::on_actionRemoveSceneCollection_triggered()
{
	PLS_UI_STEP(MAINMENU_MODULE, "Main Menu Scene Collection Delete", ACTION_CLICK);

	std::string newName;
	std::string newPath;

	std::string oldFile = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollectionFile");
	std::string oldName = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollection");

	auto cb = [&](const char *name, const char *filePath) {
		if (strcmp(oldName.c_str(), name) != 0) {
			newName = name;
			newPath = filePath;
			return false;
		}

		return true;
	};

	EnumSceneCollections(cb);

	/* this should never be true due to menu item being grayed out */
	if (newPath.empty())
		return;

	QString text = QTStr("ConfirmRemove.Text.title");
	PLSAlertView::Button button = PLSAlertView::Button::NoButton;
	if (0 == strcmp(App()->GetLocale(), "ko-KR")) {
		button = PLSMessageBox::question(this, QTStr("ConfirmRemove.Title"), QString::fromStdString(oldName), text, PLSAlertView::Button::Ok | PLSAlertView::Button::Cancel);
	} else {
		button = PLSMessageBox::question(this, QTStr("ConfirmRemove.Title"), text, QString::fromStdString(oldName), PLSAlertView::Button::Ok | PLSAlertView::Button::Cancel);
	}

	if (PLSAlertView::Button::Ok != button) {
		return;
	}

	char path[512];
	int ret = GetConfigPath(path, 512, "PRISMLiveStudio/basic/scenes/");
	if (ret <= 0) {
		blog(LOG_WARNING, "Failed to get scene collection config path");
		return;
	}

	oldFile.insert(0, path);
	oldFile += ".json";
	os_unlink(oldFile.c_str());
	oldFile += ".bak";
	os_unlink(oldFile.c_str());

	Load(newPath.c_str());
	RefreshSceneCollections();

	const char *newFile = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollectionFile");

	blog(LOG_INFO,
	     "Removed scene collection '%s' (%s.json), "
	     "switched to '%s' (%s.json)",
	     oldName.c_str(), oldFile.c_str(), newName.c_str(), newFile);
	blog(LOG_INFO, "------------------------------------------------");

	UpdateTitleBar();

	if (api) {
		api->on_event(OBS_FRONTEND_EVENT_SCENE_COLLECTION_LIST_CHANGED);
		api->on_event(OBS_FRONTEND_EVENT_SCENE_COLLECTION_CHANGED);
	}
}

void PLSBasic::on_actionImportSceneCollection_triggered()
{
	PLS_UI_STEP(MAINMENU_MODULE, "Main Menu Scene Collection Import", ACTION_CLICK);

	char path[512];

	QString qhome = QDir::homePath();

	int ret = GetConfigPath(path, 512, "PRISMLiveStudio/basic/scenes/");
	if (ret <= 0) {
		blog(LOG_WARNING, "Failed to get scene collection config path");
		return;
	}

	QString qfilePath = QFileDialog::getOpenFileName(this, QTStr("Basic.MainMenu.SceneCollection.Import"), qhome, "JSON Files (*.json)");

	QFileInfo finfo(qfilePath);
	QString qfilename = finfo.fileName();
	QString qpath = QT_UTF8(path);
	QFileInfo destinfo(QT_UTF8(path) + qfilename);

	if (!qfilePath.isEmpty() && !qfilePath.isNull()) {
		string absPath = QT_TO_UTF8(finfo.absoluteFilePath());
		OBSData scenedata = obs_data_create_from_json_file(absPath.c_str());
		obs_data_release(scenedata);

		QString name = qfilename.mid(0, qfilename.lastIndexOf('.'));
		QString suffix = qfilename.mid(qfilename.lastIndexOf('.'));

		QString origName = name;
		string file;
		int inc = 1;

		while (SceneCollectionExists(name.toStdString().c_str())) {
			name = origName + " (" + QString::number(++inc) + ")";
		}

		obs_data_set_string(scenedata, "name", name.toStdString().c_str());

		if (!GetFileSafeName(name.toStdString().c_str(), file)) {
			blog(LOG_WARNING,
			     "Failed to create "
			     "safe file name for '%s'",
			     name.toStdString().c_str());
			return;
		}

		string filePath = std::string(path) + name.toStdString();

		if (!GetClosestUnusedFileName(filePath, "json")) {
			blog(LOG_WARNING,
			     "Failed to get "
			     "closest file name for %s",
			     file.c_str());
			return;
		}

		obs_data_save_json_safe(scenedata, filePath.c_str(), "tmp", "bak");
		RefreshSceneCollections();
	}
}

void PLSBasic::on_actionExportSceneCollection_triggered()
{
	PLS_UI_STEP(MAINMENU_MODULE, "Main Menu Scene Collection Export", ACTION_CLICK);

	SaveProjectNow();

	char path[512];

	QString home = QDir::homePath();

	QString currentFile = QT_UTF8(config_get_string(App()->GlobalConfig(), "Basic", "SceneCollectionFile"));

	int ret = GetConfigPath(path, 512, "PRISMLiveStudio/basic/scenes/");
	if (ret <= 0) {
		blog(LOG_WARNING, "Failed to get scene collection config path");
		return;
	}

	QString exportFile = QFileDialog::getSaveFileName(this, QTStr("Basic.MainMenu.SceneCollection.Export"), home + "/" + currentFile, "JSON Files (*.json)");

	string file = QT_TO_UTF8(exportFile);

	if (!exportFile.isEmpty() && !exportFile.isNull()) {
		if (QFile::exists(exportFile))
			QFile::remove(exportFile);

		QFile::copy(path + currentFile + ".json", exportFile);
	}
}

void PLSBasic::UpdateSceneCollection(QAction *action)
{
	if (!action)
		return;
	LoadSceneCollection(action);
}

void PLSBasic::LoadSceneCollection(QAction *action)
{
	if (!action)
		return;

	std::string fileName;
	fileName = QT_TO_UTF8(action->property("file_name").value<QString>());
	if (fileName.empty())
		return;

	const char *oldName = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollection");
	if (action->text().compare(QT_UTF8(oldName)) == 0) {
		action->setChecked(true);
		return;
	}

	SaveProjectNow();
	Load(fileName.c_str());
	RefreshSceneCollections();

	const char *newName = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollection");
	const char *newFile = config_get_string(App()->GlobalConfig(), "Basic", "SceneCollectionFile");

	blog(LOG_INFO, "Switched to scene collection '%s' (%s.json)", newName, newFile);
	blog(LOG_INFO, "------------------------------------------------");

	UpdateTitleBar();

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_SCENE_COLLECTION_CHANGED);
}

void PLSBasic::ChangeSceneCollection()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	std::string fileName;

	if (!action)
		return;

	LoadSceneCollection(action);
}
