#ifndef PLSBEAUTYFACEITEMVIEW_H
#define PLSBEAUTYFACEITEMVIEW_H

#include <QFrame>
#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QPointer>
#include <QLabel>

namespace Ui {
class PLSBeautyFaceItemView;
}

class PLSBeautyFaceItemView : public QFrame {
	Q_OBJECT

	Q_PROPERTY(bool checked READ IsChecked WRITE SetChecked)

public:
	explicit PLSBeautyFaceItemView(const QString &id, int filterType, bool isCustom = false, QString baseName = "", QWidget *parent = nullptr);
	~PLSBeautyFaceItemView();

	QString GetFilterId() const;
	void SetFilterId(const QString &filterId);

	void SetChecked(bool isChecked);
	bool IsChecked() const;

	void SetEnabled(bool enable);
	bool IsEnabled() const;

	bool IsCustom() const;
	void SetCustom(bool isCustom);

	void SetBaseId(const QString &baseId);
	QString GetBaseId() const;

	void SetFilterIconPixmap();

	/*
	This method is for telling which image will be used for custom effcet filter item.
	param: 1 for first custom image, 2 for second custom image,3 for third custom image.
	To combine with baseName(for example [baseName="Natural"][filterIndex="?"]),we can ensure which image will be used.
	*/
	void SetFilterIndex(int index);
	void SetFilterType(int type);

private:
	void InitUi();
	void EditFinishOperation(bool cancel);
	QString GetNameElideString(int maxWidth);

protected:
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;
	virtual void enterEvent(QEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;
	virtual bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
	void on_modifyBtn_clicked();

signals:
	void FaceItemClicked(PLSBeautyFaceItemView *);
	void FaceItemIdEdited(const QString &newId, PLSBeautyFaceItemView *);

private:
	QString id;
	QString baseName;
	QString imgName;
	int filterIndex{0};
	int filterType{0};
	bool isCustom{false};
	bool isChecked{false};
	bool isEditting{false};
	bool isEntered{false};
	bool isShown{false};
	QColor layerColor;
	QLabel *customFlagIcon{nullptr};

private:
	Ui::PLSBeautyFaceItemView *ui;
};

//customize a QPushButton for showing filter icon
class FilterItemIcon : public QPushButton {
	Q_OBJECT
	Q_PROPERTY(QColor layerColor READ DisableLayerColor WRITE SetDisableLayerColor)

public:
	explicit FilterItemIcon(QWidget *parent = nullptr);
	~FilterItemIcon();

	void SetPixmap(const QPixmap &pixmap);
	void SetPixmap(const QString &pixpath);

	const QColor DisableLayerColor() const;
	void SetDisableLayerColor(const QColor &color);

private:
	bool isChecked();

protected:
	virtual void paintEvent(QPaintEvent *event) override;

private:
	QColor layerColor{QColor(0, 0, 0, 80)};
	QPixmap iconPixmap;
	bool ischecked;
};

#endif // PLSBEAUTYFACEITEMVIEW_H
