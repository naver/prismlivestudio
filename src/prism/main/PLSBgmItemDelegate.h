#pragma once

#include "PLSBgmDataManager.h"

#include <QItemDelegate>
#include <QScopedPointer>
#include <QPushButton>
#include <QSvgRenderer>
#include <QAbstractItemView>

class PLSBgmItemDelegate : public QItemDelegate {

	Q_OBJECT
public:
	explicit PLSBgmItemDelegate(QAbstractItemView *view, QFont font, QObject *parent = nullptr);
	virtual ~PLSBgmItemDelegate() override;

	static void setDpi(float dpi);
	// for udpating iconIndex to render loading icons
	static void nextLoadFrame();
	// for setting total frame count
	static void totalFrame(int frameCount);
	static void setCurrentFrame(int frameIndex);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	// painting
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	// for setting row size
	virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	// for dealing mouse event
	virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
	void drawBackground(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void drawProducer(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void drawName(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void drawDeleteIcon(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void drawStateIcon(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void drawDropIndicator(const PLSBgmItemData &data, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void UpdateIndex(const QModelIndex &index);
	QString ConvertIntToTimeString(int seconds) const;

signals:
	void delBtnClicked(const QModelIndex &index);

private:
	// for render svg icons
	QSvgRenderer *svgRendererLoading;
	QSvgRenderer *svgRendererDelBtn;
	QSvgRenderer *svgRendererFlag;
	QSvgRenderer *svgRendererDot;
	// keep a view pointer to update viewport
	QAbstractItemView *view = nullptr;
	// for render loading icons
	static int iconIndex;
	static int frameCount;
	// for adapte dpis
	static float dpi;
	ButtonState deleteBtnState{ButtonState::Normal};
	bool entered = false;
};
