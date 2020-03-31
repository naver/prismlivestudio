#ifndef PLSALERTVIEW_HPP
#define PLSALERTVIEW_HPP

#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QMap>

#include "dialog-view.hpp"

namespace Ui {
class PLSAlertView;
}

/**
 * @class PLSAlertView
 * @brief common popup alert
 */
class FRONTEND_API PLSAlertView : public PLSDialogView {
	Q_OBJECT
public:
	using Button = QDialogButtonBox::StandardButton;
	using Buttons = QDialogButtonBox::StandardButtons;

public:
	enum Icon { NoIcon, Information, Question, Warning, Critical };

	struct Result {
		Button button;
		bool isChecked;
	};

public:
	/**
	* @brief      open alert
	* @param[in]  parent        : the parent widget
	* @param[in]  icon          : the icon
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  checkbox      : the checkbox text
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @param[in]  sugsize          : the window sugsize
	*/
	explicit PLSAlertView(QWidget *parent, Icon icon, const QString &title, const QString &message, const QString &checkbox, const Buttons &buttons, Button defaultButton = Button::NoButton,
			      const QSize &sugsize = QSize(), PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSAlertView(QWidget *parent, Icon icon, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons,
			      Button defaultButton = Button::NoButton, const QSize &sugsize = QSize(), PLSDpiHelper dpiHelper = PLSDpiHelper());
	explicit PLSAlertView(Icon icon, const QString &title, const QString &messageTitle, const QString &messageContent, QWidget *parent, PLSAlertView::Buttons buttons,
			      Button defaultButton = Button::NoButton, const QSize &sugsize = QSize(), PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSAlertView();

public:
	/**
	* @brief      open alert
	* @param[in]  parent        : the parent widget
	* @param[in]  message       : the message
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @param[in]  sugsize          : the window sugsize
	* @return     the button that user clicked
	*/
	static Button open(QWidget *parent, const QString &message, const Buttons &buttons, Button defaultButton = Button::NoButton, const QSize &sugsize = QSize());
	static Button open(QWidget *parent, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton = Button::NoButton, const QSize &sugsize = QSize());
	static Button open(const QString &title, const QString &messageTitle, const QString &messageContent, QWidget *parent, const QMap<Button, QString> &buttons,
			   Button defaultButton = Button::NoButton, const QSize &sugsize = QSize());

	/**
	* @brief      open alert
	* @param[in]  parent     : the parent widget
	* @param[in]  title      : the title
	* @param[in]  message    : the message
	* @param[in]  buttons    : the custom buttons
	* @param[in]  sugsize          : the window sugsize
	* @return     the button that user clicked
	*/
	static Button open(QWidget *parent, const QString &title, const QString &message, const Buttons &buttons, Button defaultButton = Button::NoButton, const QSize &sugsize = QSize());
	static Button open(QWidget *parent, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton = Button::NoButton,
			   const QSize &sugsize = QSize());

	/**
	* @brief      open alert
	* @param[in]  parent        : the parent widget
	* @param[in]  icon          : the icon
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @param[in]  sugsize          : the window sugsize
	* @return     the button that user clicked
	*/
	static Button open(QWidget *parent, Icon icon, const QString &title, const QString &message, const Buttons &buttons, Button defaultButton = Button::NoButton, const QSize &sugsize = QSize());
	static Button open(QWidget *parent, Icon icon, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton = Button::NoButton,
			   const QSize &sugsize = QSize());
	static Button open(Icon icon, const QString &title, const QString &messageTitle, const QString &messageContent, QWidget *parent, PLSAlertView::Buttons buttons,
			   Button defaultButton = Button::NoButton, const QSize &sugsize = QSize());

	/**
	* @brief      open alert
	* @param[in]  parent        : the parent widget
	* @param[in]  icon          : the icon
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  checkbox      : the checkbox text
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @param[in]  sugsize          : the window sugsize
	* @return     the button that user clicked and checkbox state
	*/
	static Result open(QWidget *parent, Icon icon, const QString &title, const QString &message, const QString &checkbox, const Buttons &buttons, Button defaultButton = Button::NoButton,
			   const QSize &sugsize = QSize());
	static Result open(QWidget *parent, Icon icon, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons,
			   Button defaultButton = Button::NoButton, const QSize &sugsize = QSize());

	/**
	* @brief      open information alert
	* @param[in]  parent        : the parent widget
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @return     the button that user clicked
	*/
	static Button information(QWidget *parent, const QString &title, const QString &message, Buttons buttons = Button::Ok, Button defaultButton = Button::Ok);
	static Button information(QWidget *parent, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton = Button::NoButton);
	/**
	* @brief      open information alert
	* @param[in]  parent        : the parent widget
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  checkbox      : the checkbox text
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @return     the button that user clicked and checkbox state
	*/
	static Result information(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, Buttons buttons = Button::Ok, Button defaultButton = Button::Ok);
	static Result information(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons,
				  Button defaultButton = Button::NoButton);

	/**
	* @brief      open question alert
	* @param[in]  parent        : the parent widget
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @return     the button that user clicked
	*/
	static Button question(QWidget *parent, const QString &title, const QString &message, Buttons buttons = Buttons(Button::Yes | Button::No), Button defaultButton = Button::NoButton);
	static Button question(QWidget *parent, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton = Button::NoButton);
	static Button question(const QString &title, const QString &messageTitle, const QString &messageContent, QWidget *parent, PLSAlertView::Buttons buttons,
			       Button defaultButton = Button::NoButton);

	/**
	* @brief      open question alert
	* @param[in]  parent        : the parent widget
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  checkbox      : the checkbox text
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @return     the button that user clicked and checkbox state
	*/
	static Result question(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, Buttons buttons = Buttons(Button::Yes | Button::No),
			       Button defaultButton = Button::NoButton);
	static Result question(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons, Button defaultButton = Button::NoButton);

	/**
	* @brief      open warning alert
	* @param[in]  parent        : the parent widget
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @return     the button that user clicked
	*/
	static Button warning(QWidget *parent, const QString &title, const QString &message, Buttons buttons = Button::Ok, Button defaultButton = Button::Ok);
	static Button warning(QWidget *parent, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton = Button::NoButton);
	/**
	* @brief      open warning alert
	* @param[in]  parent        : the parent widget
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  checkbox      : the checkbox text
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @return     the button that user clicked and checkbox state
	*/
	static Result warning(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, Buttons buttons = Button::Ok, Button defaultButton = Button::Ok);
	static Result warning(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons, Button defaultButton = Button::NoButton);

	/**
	* @brief      open critical alert
	* @param[in]  parent        : the parent widget
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @return     the button that user clicked
	*/
	static Button critical(QWidget *parent, const QString &title, const QString &message, Buttons buttons = Button::Ok, Button defaultButton = Button::Ok);
	static Button critical(QWidget *parent, const QString &title, const QString &message, const QMap<Button, QString> &buttons, Button defaultButton = Button::NoButton);
	/**
	* @brief      open critical alert
	* @param[in]  parent        : the parent widget
	* @param[in]  title         : the title
	* @param[in]  message       : the message
	* @param[in]  checkbox      : the checkbox text
	* @param[in]  buttons       : the custom buttons
	* @param[in]  defaultButton : the default button
	* @return     the button that user clicked and checkbox state
	*/
	static Result critical(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, Buttons buttons = Button::Ok, Button defaultButton = Button::Ok);
	static Result critical(QWidget *parent, const QString &title, const QString &message, const QString &checkbox, const QMap<Button, QString> &buttons, Button defaultButton = Button::NoButton);

public:
	bool isChecked() const;

	Qt::TextFormat getTextFormat() const;
	void setTextFormat(Qt::TextFormat format);

	Icon getIcon() const;
	void setIcon(Icon icon);

private slots:
	void onButtonClicked(QAbstractButton *button);
	QString GetNameElideString(const QString &name, QWidget *widget);

protected:
	void showEvent(QShowEvent *event) override;

private:
	Ui::PLSAlertView *ui;
	Icon icon;
	QCheckBox *checkBox;
	QSize sugsize;
};

#endif // PLSALERTVIEW_HPP
