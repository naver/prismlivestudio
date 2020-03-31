#ifndef PLSBEAUTYFILTERVIEW_H
#define PLSBEAUTYFILTERVIEW_H

#include "PLSBeautyDefine.h"
#include <QScrollArea>

#include "frontend-api/dialog-view.hpp"
#include "obs.hpp"
#include "window-basic-main.hpp"
#include "PLSDpiHelper.h"

namespace Ui {
class PLSBeautyFilterView;
}

class FlowLayout;
class PLSBeautyFaceView;
class PLSBeautyFaceItemView;
class PLSBeautyFilterView : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSBeautyFilterView(const QString &sourceName, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSBeautyFilterView();
	void AddSourceName(const QString &sourceName, OBSSceneItem item);
	void RenameSourceName(OBSSceneItem item, const QString &newName, const QString &prevName);
	void RemoveSourceName(const QString &sourceName, OBSSceneItem item);
	void SetSourceVisible(const QString &sourceName, OBSSceneItem item, bool visible);
	void SetSourceSelect(const QString &sourceName, OBSSceneItem item, bool selected);
	void UpdateSourceList(const QString &sourceName, OBSSceneItem item, const DShowSourceVecType &sourceList);
	void ReorderSourceList(const QString &sourceName, OBSSceneItem item, const DShowSourceVecType &sourceList);
	void InitGeometry();
	void SaveShowModeToConfig();
	void onMaxFullScreenStateChanged() override;
	void onSaveNormalGeometry() override;
	OBSSceneItem GetCurrentItemData();
	void Clear();
	void TurnOffBeauty(obs_source_t *source);

	static void InitIconFileName();
	static QString getIconFileByIndex(int groupIndex, int index);

	void UpdateItemIcon();

public slots:
	void RemoveSourceNameList(bool isCurrentScene, const DShowSourceVecType &list);

protected:
	virtual void showEvent(QShowEvent *event) override;
	virtual void hideEvent(QHideEvent *event) override;
	virtual bool eventFilter(QObject *watcher, QEvent *event) override;
	virtual void closeEvent(QCloseEvent *event) override;

private:
	static void PLSFrontendEvent(enum obs_frontend_event event, void *ptr);
	static void getFilesFromDir(const QString &filePath, QVector<QString> &files);

	template<typename T> void InitSlider(QSlider *slider, const T &min, const T &max, const T &step, Qt::Orientation ori = Qt::Horizontal);
	void LoadBeautyFaceView(OBSSceneItem item);
	void InitBeautyFaceView(const BeautyConfig &beautyConfig);
	void InitSliderAndLineEditConnections(QSlider *slider, QLineEdit *lineEdit, void (PLSBeautyFilterView::*func)(int));
	void CreateBeautyFaceView(const QString &id, int filterType, bool isCustom, bool isCurrent, QString baseName, int filterIndex = 0);
	PLSBeautyFaceItemView *isCustomFaceExisted(const QString &filterId);
	void UpdateBeautyFaceView(OBSSceneItem item);
	void SetRecommendValue(const BeautyConfig &config);
	void OnGetByteArraySuccess(const QByteArray &array, BeautyConfig &beautyConfig);
	void UpdateUI(const QString &filterId);
	void UpdateButtonState(bool isCustom, bool isChanged);
	void WriteBeautyConfigToLocal();
	void SetCheckedState(PLSBeautyFaceItemView *view, bool state);
	void SetCheckedState(const QString &filterId, bool state);
	void SetCurrentClickedFaceView();
	void UpdateBeautyParamToRender(OBSSceneItem item, const QString &filterName);
	void DeleteCustomFaceView(const QString &filterId);
	void DeleteAllBeautyViewInCurrentSource();
	void CreateCustomFaceView(BeautyConfig config);
	OBSSceneItem GetCurrentSourceItem();
	QString GetSourceCurrentFilterName(OBSSceneItem item);
	QString GetSourceCurrentFilterBaseName(OBSSceneItem item);

	void LoadPresetBeautyConfig(OBSSceneItem item);
	void InitLineEdit();
	void setLineEditRegularExpression(QLineEdit *lineEdit);
	bool IsDShowSourceAvailable(const QString &name, OBSSceneItem item);
	bool IsDShowSourceValid(const QString &name, OBSSceneItem item);
	bool IsDShowSourceVisible(const QString &name, OBSSceneItem item);
	bool isSourceComboboxVisible(const QString &sourceName, OBSSceneItem item);
	bool isCurrentSourceExisted(const QString &currentSourceName);
	void ClearSourceComboboxSceneData(const QList<OBSSceneItem> &vec, bool releaseCurrent);
	void AddSourceComboboxList(const DShowSourceVecType &list, bool releaseCurrent = false);
	int GetSourceComboboxVisibleCount();
	void InitConnections();
	void ApplySliderValue(void (PLSBeautyFilterView::*func)(int), int value);
	void CheckeAndSetValidValue(int *value);
	void SetSourceUnSelect(const QString &sourceName, OBSSceneItem item);
	void SetSourceInvisible(const QString &sourceName, OBSSceneItem item);
	void SetCurrentSourceName(const QString &sourceName, OBSSceneItem item);
	int FindText(const QString &sourceName, OBSSceneItem item);
	void RemoveItem(const QString &sourceName, OBSSceneItem item);
	bool SetCurrentIndex(const QString &sourceName, OBSSceneItem item);
	void SaveLastValidValue(QLineEdit *object, int value);
	void LoadPrivateBeautySetting(OBSSceneItem item);
	void SetFaceItem(PLSBeautyFaceItemView *item);

private slots:
	void OnChinSliderValueChanged(int value);
	void OnCheekSliderValueChanged(int value);
	void OnCheekBoneSliderValueChanged(int value);
	void OnEyeSliderValueChanged(int value);
	void OnNoseSliderValueChanged(int value);
	void OnSkinSliderValueChanged(int value);
	void OnSourceComboboxCurrentIndexChanged(int index);
	void OnSaveCustomButtonClicked();
	void OnFaceItemClicked(PLSBeautyFaceItemView *);
	void OnFaceItemEdited(const QString &newId, PLSBeautyFaceItemView *item);
	void OnDeleteButtonClicked();
	void OnResetButtonClicked();
	void OnBeautyEffectStateCheckBoxClicked(int state);
	void OnBeautyFaceCheckboxStateChanged(int state);
	void OnSceneCollectionChanged();
	void OnSceneChanged();
	void OnSceneCopy();
	void OnScrollToCurrentItem(PLSBeautyFaceItemView *item);
	void OnScrollToCurrentItem(const QString &filterId);
	void OnSkinSliderMouseRelease();
	void OnLayoutFinished();
signals:
	void beautyViewVisibleChanged(bool);
	void currentSourceChanged(const QString &name, OBSSceneItem item);

private:
	Ui::PLSBeautyFilterView *ui;
	QString currentSourceName{};
	QString selectFilterName{};
	OBSSceneItem currentSource{};
	bool ignoreChangeIndex{true};
	bool sendActionLog{true};
	enum ResizeReason { ItemDelete = 0, ItemAdded, FlowLayoutChange };
	ResizeReason resizeReason{FlowLayoutChange};

	FlowLayout *flowLayout{};
	QList<PLSBeautyFaceItemView *> listItems;
	static QVector<QVector<QString>> iconFileNameVector;
};
#endif // PLSBEAUTYFILTERVIEW_H
