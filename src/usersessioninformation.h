/********************************************************************************
 * Copyright (C) 2024 Han Jinpeng <hanjinpeng127@gmail.com>                     *
 * Website: https://github.com/prownd/systemd-manage                            *
 *                                                                              *
 * This file is part of the systemd-manage project.                             *
 * License: GPLv3                                                               *
 *                                                                              *
 * This program is free software: you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by the Free   *
 * Software Foundation, either version 3 of the License, or (at your option)    *
 * any later version.                                                           *
 *                                                                              *
 * This program is distributed in the hope that it will be useful, but WITHOUT  *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for     *
 * more details.                                                                *
 *                                                                              *
 * You should have received a copy of the GNU General Public License along      *
 * with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.  *
 ********************************************************************************/

#ifndef USERSESSIONINFORMATION_H
#define USERSESSIONINFORMATION_H

#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include "systemdgeneric.h"
#include "systemdmanagerinterface.h"
#include "mystyleditemdelegate.h"


class SessionInformation : public QWidget
{
    Q_OBJECT

public:
    explicit SessionInformation(QWidget *parent = nullptr);
    explicit SessionInformation(QString sessionId = "", QString sessionObjectPath = "", QWidget *parent = nullptr);
    ~SessionInformation();
    void setupSessionlist();
    void slotRefreshSessionList();
    void init();
    void createSessionWidgetView();
    void createSessionDetailView();
    void createSessionDetailListWidgetView();

private:
    SystemdManagerInterface * m_systemdManagerInterface;

    QStandardItemModel *m_sessionModel;
    QVector<SystemdSession> m_sessionList;

    QTableView * m_sessionTableView;
    MyStyledItemDelegate * m_pTableViewItemDelegateProxy;

    QString m_sessionId;
    QString m_sessionObjectPath;

    QBoxLayout * m_mainLayout1;
    QGridLayout *  m_subSessionDetailLayout;
    QVBoxLayout *  m_subSessionDetailVLayout;
    QWidget * m_pSessionWidget;

    QLabel * m_sessionFieldName1;
    QLabel * m_sessionFieldValue1;
    QLabel * m_sessionFieldName2;
    QLabel * m_sessionFieldValue2;
    QLabel * m_sessionFieldName3;
    QLabel * m_sessionFieldValue3;
    QLabel * m_sessionFieldName4;
    QLabel * m_sessionFieldValue4;
    QLabel * m_sessionFieldName5;
    QLabel * m_sessionFieldValue5;
    QLabel * m_sessionFieldName6;
    QLabel * m_sessionFieldValue6;
    QLabel * m_sessionFieldName7;
    QLabel * m_sessionFieldValue7;
    QLabel * m_sessionFieldName8;
    QLabel * m_sessionFieldValue8;
    QLabel * m_sessionFieldName9;
    QLabel * m_sessionFieldValue9;
    QLabel * m_sessionFieldName10;
    QLabel * m_sessionFieldValue10;
    QLabel * m_sessionFieldName11;
    QLabel * m_sessionFieldValue11;
    QLabel * m_sessionFieldName12;
    QLabel * m_sessionFieldValue12;
    QLabel * m_sessionFieldName13;
    QLabel * m_sessionFieldValue13;
};

class UserInformation : public QWidget
{
    Q_OBJECT

public:
    explicit UserInformation(QWidget *parent = nullptr);
    explicit UserInformation(QString userId = "", QString userObjectPath = "", QWidget *parent = nullptr);
    ~UserInformation();
    void setupUserlist();
    void init();
    void createUserWidgetView();
    void createUserDetailView();
    void createUserDetailListWidgetViewData();
    void setupUserDetailViewData();

private:
    SystemdManagerInterface * m_systemdManagerInterface;

    QString m_userId;
    QString m_userObjectPath;

    QHBoxLayout *  m_userLayout;
    QWidget * m_pUsernWidget;

    QTableView * m_userDetailTableView;
    QStandardItemModel *m_userDetailStandItemModel;

    QListWidget * m_userListWidget;

    QVector<SystemdSession> m_userList;
    MyStyledItemDelegate * m_pUserViewItemDelegateProxy;

    QLabel * m_userFieldName1;
    QLabel * m_userFieldValue1;
    QLabel * m_userFieldName2;
    QLabel * m_userFieldValue2;
    QLabel * m_userFieldName3;
    QLabel * m_userFieldValue3;
    QLabel * m_userFieldName4;
    QLabel * m_userFieldValue4;

protected:
    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // USERSESSIONINFORMATION_H
