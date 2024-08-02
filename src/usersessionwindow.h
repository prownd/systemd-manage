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

#ifndef USERSESSIONWINDOW_H
#define USERSESSIONWINDOW_H

#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QWidget>
#include "systemdgeneric.h"
#include "systemdmanagerinterface.h"
#include "mystyleditemdelegate.h"


class UserSessionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UserSessionWindow(QWidget *parent = nullptr);
    ~UserSessionWindow();

    void initData();
    void createTabWidgetView();

    void createSessionTableView();
    void createUserTableView();

    void slotRefreshSessionList();
    void slotRefreshUserList();
private:
    SystemdManagerInterface * m_systemdManagerInterface;

    QTabWidget* m_pTabWidget;
    QWidget * m_pSessionWidget;
    QHBoxLayout* m_subSessionLayout;
    QWidget * m_pUserWidget;
    QHBoxLayout* m_subUserLayout;

    QTableView * m_sessionTableView;
    QTableView * m_userTableView;
    QStandardItemModel *m_sessionModel;
    QStandardItemModel *m_userModel;
    QVector<SystemdSession> m_sessionList;
    QVector<SystemdUser> m_userList;

    MyStyledItemDelegate * m_pSessionTableViewItemDelegateProxy;
    MyStyledItemDelegate * m_pUserTableViewItemDelegateProxy;

    int m_sessionColumnCount;
    int m_userColumnCount;

protected Q_SLOTS:
    void sessionCustomMenuRequested(QPoint pos);
    void slotSessionTableRowDoubleClicked(const QModelIndex);

    void handleSessionStatusAction();
    void handleSessionLockAction();
    void handleSessionUnlockAction();
    void handleSessionActivateAction();
    void handleSessionTerminateAction();
    void handleSessionRefreshAction();

    void userCustomMenuRequested(QPoint pos);
    void slotUserTableRowDoubleClicked(const QModelIndex);

    void handleUserStatusAction();
    void handleUserTerminateAction();
    void handleUserRefreshAction();
protected:
    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // USERSESSIONWINDOW_H
