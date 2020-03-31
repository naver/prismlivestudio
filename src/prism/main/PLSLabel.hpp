#ifndef PLSLABEL_H
#define PLSLABEL_H

#include <QLabel>

class PLSLabel : public QLabel {
	Q_OBJECT
public:
	explicit PLSLabel(QWidget *parent = nullptr);
	~PLSLabel();
	void SetText(const QString &text);

protected:
	virtual void resizeEvent(QResizeEvent *event) override;

private:
	QString GetNameElideString();

private:
	QString realText{};
};

class CombinedLabel : public QLabel {
	Q_OBJECT

	friend class PLSBgmItemView;

public:
	explicit CombinedLabel(QWidget *parent = nullptr);
	~CombinedLabel();

	void SetShowText(const QString &strText);
	void SetAdditionalText(const QString &strText);
	void SetSpacing(int spacing);
	void SetAdditionalVisible(bool visible);
	void SetMiddleVisible(bool visible);
public slots:
	void UpdataUi();

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	int GetTextSpacing();

private:
	QLabel *additionalLabel = nullptr;
	QLabel *middleLabel = nullptr;
	int spacing = 5;
	QString strText;
	QString strAdditional;
};

#endif // PLSLABEL_H
