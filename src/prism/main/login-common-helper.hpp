/*
 * @fine      LoginCommonHelpers
 * @brief     login module common function
 * @date      2019-10-10
 * @author    Bing Cheng
 * @attention

 * @version   v1.0
 * @modify
 */

#ifndef LOGIN_LOGINCOMMONHELPER_H
#define LOGIN_LOGINCOMMONHELPER_H

#include <QString>
#include <QStackedWidget>

namespace LoginCommonHelpers {
/**
 * @brief isValidEmailByRegExp: check email format
 * @param email
 * @return
 */
bool isValidEmailByRegExp(const QString &email);
/**
 * @brief isValidPasswordByRegExp: check password format
 * @param password
 * @return
 */
bool isValidPasswordByRegExp(const QString &password);
/**
 * @brief isValidNickNameByRegExp: check nickName format
 * @param nickName
 * @return
 */
bool isValidNickNameByRegExp(const QString &nickName);
/**
 * @brief setCurrentWidget: Set the form displayed by the current stack window
 * @param ojbName: display widget object name
 */
void setCurrentStackWidget(QStackedWidget *stackWidget, const QString &ojbName);

QWidget *getCurrentStackWidget(QStackedWidget *stackWidget, const QString &ojbName);

void refreshStyle(QWidget *widget);

void loginResultHandler(QStackedWidget *stackWidget, bool isSuccess);

}; // namespace LoginCommonHelpers

#endif // LOGINCOMMONHELPER_H
