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

#ifndef UNITWINDOW_H
#define UNITWINDOW_H

#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <customsortfilterproxymodel.h>

#include "systemdgeneric.h"
#include "unitmodel.h"
#include "systemdmanagerinterface.h"
#include "mystyleditemdelegate.h"


class UnitWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UnitWindow(QWidget *parent = nullptr);
    ~UnitWindow();

    void initData();
    void createTabWidgetView();
    void createSystemUnitTableView();
    void createUserUnitTableView();
    void createTimerTableView();

private:
    QTabWidget * m_pTabWidget;

    QWidget * m_pSystemUnitWidget;
    QWidget * m_pUserUnitWidget;
    QWidget * m_pTimerWidget;

    QVBoxLayout *m_subSystemUnitLayout;
    QVBoxLayout *m_subUserUnitLayout;
    QVBoxLayout *m_subTimerLayout;

    QBoxLayout* m_mainLayout1;

    QStandardItemModel *m_systemUnitStandItemModel;
    QStandardItemModel *m_userUnitStandItemModel;
    QStandardItemModel *m_timerStandItemModel;

    QTableView *m_systemUnitTableView;
    QTableView *m_userUnitTableView;
    QTableView *m_timerTableView;

    QComboBox *     m_systemUnitTypeComboBox;
    QCheckBox *     m_unloadSystemUnitCheckBox;
    QCheckBox *     m_inactiveSystemUnitCheckBox;
    QLineEdit *     m_searchSystemUnitLineEdit;
    QHBoxLayout *   m_filterSystemUnitHLayout;
    QLabel *        m_systemUnitCountLabel;
    QHBoxLayout *   m_systemUnitCountHLayout;

    QComboBox *     m_userUnitTypeComboBox;
    QCheckBox *     m_unloadUserUnitCheckBox;
    QCheckBox *     m_inactiveUserUnitCheckBox;
    QLineEdit *     m_searchUserUnitLineEdit;
    QHBoxLayout *   m_filterUserUnitHLayout;
    QLabel *        m_userUnitCountLabel;
    QHBoxLayout *   m_userUnitCountHLayout;

    QHBoxLayout *   m_filterTimerUnitHLayout;
    QLabel  *       m_searchTimerUnitLabel;
    QLineEdit  *    m_searchTimerUnitLineEdit;

    UnitModel *m_systemUnitModel;
    UnitModel *m_userUnitModel;

    CustomSortFilterProxyModel * m_systemUnitFilterModel;
    CustomSortFilterProxyModel * m_userUnitFilterModel;
    CustomSortFilterProxyModel * m_timerUnitFilterModel;

    QVector<SystemdUnit> m_systemUnitsList;
    QVector<SystemdUnit> m_userUnitsList;
    QVector<SystemdSession> m_sessionList;
    QVector<conffile> m_confFileList;

    void setupSystemUnitslist();
    void setupUserUnitslist();
    void setupTimerslist();

    QList<QStandardItem *> buildTimerListRow(const SystemdUnit &unit, const QVector<SystemdUnit> &list, dbusBus bus);

    int m_noActSystemUnits;
    int m_noActUserUnits;
    bool enableUserUnits = true;
    QTimer *timer;

    SystemdManagerInterface * m_systemdManagerInterface;
    MyStyledItemDelegate * m_pTableViewItemDelegate;

    int m_systemUnitColumnCount;
    int m_userUnitColumnCount;
    int m_timerColumnCount;

    const QStringList unitTypeSufx = QStringList{QString(),
                                                 QStringLiteral(".service"),
                                                 QStringLiteral(".automount"),
                                                 QStringLiteral(".device"),
                                                 QStringLiteral(".mount"),
                                                 QStringLiteral(".path"),
                                                 QStringLiteral(".scope"),
                                                 QStringLiteral(".slice"),
                                                 QStringLiteral(".socket"),
                                                 QStringLiteral(".swap"),
                                                 QStringLiteral(".target"),
                                                 QStringLiteral(".timer")};

private Q_SLOTS:
    void slotRefreshUnitsList(bool inital, dbusBus bus);
    void slotRefreshTimerList();
    void slotUpdateTimers();

    void handleSystemUnitStatusAction();
    void handleSystemUnitStartAction();
    void handleSystemUnitStopAction();
    void handleSystemUnitRestartAction();
    void handleSystemUnitEnableAction();
    void handleSystemUnitDisableAction();
    void handleSystemUnitRefreshAction();

    void handleUserUnitStatusAction();
    void handleUserUnitStartAction();
    void handleUserUnitStopAction();
    void handleUserUnitRestartAction();
    void handleUserUnitEnableAction();
    void handleUserUnitDisableAction();
    void handleUserUnitRefreshAction();

    void handleTimerUnitStatusAction();
    void handleTimerUnitStartAction();
    void handleTimerUnitStopAction();
    void handleTimerUnitRestartAction();
    void handleTimerUnitEnableAction();
    void handleTimerUnitDisableAction();
    void handleTimerUnitRefreshAction();

    void updateSystemUnitCount();
    void updateUserUnitCount();

    void slotCheckBoxShowSystemUnits(int state);
    void slotCheckBoxShowUserUnits(int state);

    void slotComboBoxShowUserUnits(int index);
    void slotComboBoxShowSystemUnits(int index);

    void slotLineEditSearchSystemUnitChanged(QString term);
    void slotLineEditSearchUserUnitChanged(QString term);
    void slotLineEditSearchTimerUnitChanged(QString term);

public Q_SLOTS:
    void slotSystemUnitTableRowDoubleClicked(const QModelIndex);
    void slotUserUnitTableRowDoubleClicked(const QModelIndex);
    void slotTimerUnitTableRowDoubleClicked(const QModelIndex);

    void systemUnitCustomMenuRequested(QPoint pos);
    void userUnitCustomMenuRequested(QPoint pos);
    void timerUnitCustomMenuRequested(QPoint pos);

protected:
    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // UNITWINDOW_H
