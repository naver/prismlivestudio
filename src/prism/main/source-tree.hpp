#pragma once
#include <QList>
#include <QVector>
#include <QPointer>
#include <QListView>
#include <QCheckBox>
#include <QStaticText>
#include <QSvgRenderer>
#include <QAbstractListModel>
#include <QScrollBar>
#include "PLSDpiHelper.h"

class QLabel;
class QCheckBox;
class QLineEdit;
class SourceTree;
class QSpacerItem;
class QHBoxLayout;
class LockedCheckBox;
class VisibilityCheckBox;
class VisibilityItemWidget;

class SourceTreeSubItemCheckBox : public QCheckBox {
	Q_OBJECT
};

class SourceLabel : public QLabel {
	Q_OBJECT

public:
	SourceLabel(QWidget *p) : QLabel(p) {}
	virtual ~SourceLabel() {}

	void setText(const QString &text);
	void setText(const char *text);
	QString GetText();

protected:
	void resizeEvent(QResizeEvent *event) override;
	virtual void paintEvent(QPaintEvent *event) override;

	QString SnapSourceName();

private:
	QString currentText = "";
};

class SourceTreeItem : public QWidget {
	Q_OBJECT

	friend class SourceTree;
	friend class SourceTreeModel;

	void mouseDoubleClickEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	virtual bool eventFilter(QObject *object, QEvent *event) override;

	void Update(bool force);

	void OnIconTypeChanged(QString value);
	void OnSelectChanged(bool isSelected);
	void UpdateRightMargin();

	enum class Type {
		Unknown,
		Item,
		Group,
		SubItem,
	};

	void DisconnectSignals();
	void ReconnectSignals();

	Type type = Type::Unknown;
signals:
	void SelectItemChanged(OBSSceneItem item, bool selected);
	void VisibleItemChanged(OBSSceneItem item, bool visible);

public:
	static void OnSourceCaptureState(void *data, calldata_t *calldata);
	static void BeautySourceStatusChanged(void *data, calldata_t *params);

	static QString GetErrorTips(const char *id, enum obs_source_error error);

	explicit SourceTreeItem(SourceTree *tree, OBSSceneItem sceneitem);
	virtual ~SourceTreeItem();

	bool IsEditing();
	OBSSceneItem SceneItem();

	enum SourceItemBgType { BgDefault = 0, BgPreset, BgCustom };

	void SetBgColor(SourceItemBgType type, void *param);

	void OnMouseStatusChanged(const char *s);
	enum IndicatorType { IndicatorNormal, IndicatorAbove, IndicatorBelow };
	void UpdateIndicator(IndicatorType type);

private:
	QSpacerItem *spacer = nullptr;
	QCheckBox *expand = nullptr;
	VisibilityCheckBox *vis = nullptr;
	LockedCheckBox *lock = nullptr;
	QHBoxLayout *boxLayout = nullptr;
	SourceLabel *label = nullptr;
	QLabel *iconLabel = nullptr;
	QLineEdit *editor = nullptr;

	QLabel *aboveIndicator = nullptr;
	QLabel *belowIndicator = nullptr;

	QSpacerItem *spaceBeforeVis = nullptr;
	QSpacerItem *spaceBeforeLock = nullptr;
	QSpacerItem *rightMargin = nullptr;

	SourceTree *tree;
	OBSSceneItem sceneitem;
	OBSSignal sceneRemoveSignal;
	OBSSignal itemRemoveSignal;
	OBSSignal groupReorderSignal;
	OBSSignal deselectSignal;
	OBSSignal visibleSignal;
	OBSSignal lockedSignal;
	OBSSignal renameSignal;
	OBSSignal removeSignal;

	bool selected;
	bool isScrollShowed;
	bool isItemNormal;
	bool editing;

	virtual void paintEvent(QPaintEvent *event) override;

private slots:
	void Clear();

	void EnterEditMode();
	void ExitEditMode(bool save);

	void VisibilityChanged(bool visible);
	void LockedChanged(bool locked);
	void Renamed(const QString &name);

	void ExpandClicked(bool checked);

	void Deselect();

	void OnSourceScrollShow(bool isShow);

	void UpdateNameColor(bool selected, bool visible);
	void UpdateIcon();
	bool isDshowSourceChangedState(obs_source_t *source);
	void OnVisibleClicked(bool visible);
};

class SourceTreeModel : public QAbstractListModel {
	Q_OBJECT

	friend class SourceTree;
	friend class SourceTreeItem;

	SourceTree *st;
	QVector<OBSSceneItem> items;
	bool hasGroups = false;

	static void PLSFrontendEvent(enum obs_frontend_event event, void *ptr);
	void Clear();
	void SceneChanged();
	void ReorderItems();

	void Add(obs_sceneitem_t *item);
	void Remove(void *item); // in function we should not use item directly to avoid using deleted memory
	OBSSceneItem Get(int idx);
	QString GetNewGroupName();
	void AddGroup();

	void GroupSelectedItems(QModelIndexList &indices);
	void UngroupSelectedGroups(QModelIndexList &indices);

	void ExpandGroup(obs_sceneitem_t *item);
	void CollapseGroup(obs_sceneitem_t *item);

	void UpdateGroupState(bool update);

	int Count();
	QVector<OBSSceneItem> GetItems();
signals:
	void itemRemoves(QVector<OBSSceneItem> items);
	void itemReorder();

public:
	explicit SourceTreeModel(SourceTree *st);
	~SourceTreeModel();

	virtual int rowCount(const QModelIndex &parent) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;

	virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
	virtual Qt::DropActions supportedDropActions() const override;
};

class QSourceScrollBar : public QScrollBar {
	Q_OBJECT

public:
	QSourceScrollBar(QWidget *parent = nullptr) : QScrollBar(parent) {}
	virtual ~QSourceScrollBar() {}

	void hideEvent(QHideEvent *e) { emit SourceScrollShow(false); }
	void showEvent(QShowEvent *e) { emit SourceScrollShow(true); }

signals:
	void SourceScrollShow(bool isShow);
};

class SourceTree : public QListView {
	Q_OBJECT

	bool ignoreReorder = false;

	friend class SourceTreeModel;
	friend class SourceTreeItem;

	QSourceScrollBar *scrollBar;
	SourceTreeItem *preDragOver = nullptr;
	QLabel *noSourceTips;

	void ResetWidgets();
	void UpdateWidget(const QModelIndex &idx, obs_sceneitem_t *item);
	void UpdateWidgets(bool force = false);

	inline SourceTreeModel *GetStm() const { return reinterpret_cast<SourceTreeModel *>(model()); }

	void NotifyItemSelect(obs_sceneitem_t *sceneitem, bool select);

public:
	inline SourceTreeItem *GetItemWidget(int idx)
	{
		QWidget *widget = indexWidget(GetStm()->createIndex(idx, 0));
		return reinterpret_cast<SourceTreeItem *>(widget);
	}

	explicit SourceTree(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());

	inline bool IgnoreReorder() const { return ignoreReorder; }
	inline void Clear() { GetStm()->Clear(); }

	inline void Add(obs_sceneitem_t *item) { GetStm()->Add(item); }
	inline OBSSceneItem Get(int idx) { return GetStm()->Get(idx); }
	inline QString GetNewGroupName() { return GetStm()->GetNewGroupName(); }

	void SelectItem(obs_sceneitem_t *sceneitem, bool select);

	bool MultipleBaseSelected() const;
	bool GroupsSelected() const;
	bool GroupedItemsSelected() const;

	void UpdateIcons();
	int Count();
	QVector<OBSSceneItem> GetItems();

	void ResetDragOver();
	bool GetDestGroupItem(QPoint pos, obs_sceneitem_t *&item_output);
	bool CheckDragSceneToGroup(obs_sceneitem_t *dragItem, obs_sceneitem_t *destGroupItem);
	bool IsValidDrag(obs_sceneitem_t *destGroupItem, QVector<OBSSceneItem> items);

public slots:
	inline void ReorderItems() { GetStm()->ReorderItems(); }
	void Remove(OBSSceneItem item);
	void GroupSelectedItems();
	void UngroupSelectedGroups();
	void AddGroup();
	void Edit(int idx);
	void OnSourceStateChanged(unsigned long long srcPtr);
	void OnBeautySourceStatusChanged(const QString &sourceName, bool status);
	void OnSourceItemRemove(unsigned long long sceneItemPtr);
	void OnSelectItemChanged(OBSSceneItem item, bool selected);
	void OnVisibleItemChanged(OBSSceneItem item, bool visible);

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
	virtual void dragMoveEvent(QDragMoveEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;
	virtual void paintEvent(QPaintEvent *event) override;

	virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
signals:
	void DshowSourceStatusChanged(const QString name, OBSSceneItem item, bool invalid);
	void SelectItemChanged(OBSSceneItem item, bool selected);
	void VisibleItemChanged(OBSSceneItem item, bool visible);
	void itemsRemove(QVector<OBSSceneItem> items);
	void itemsReorder();
	void beautyStatusChanged(const QString &sourceName, bool status);
};
