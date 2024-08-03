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

#ifndef UNITINFORMATION_H
#define UNITINFORMATION_H

#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QTextEdit>
#include <QDialog>
#include <QLabel>
#include "systemdgeneric.h"
#include "unitmodel.h"
#include "systemdmanagerinterface.h"

#include "highlighter.h"

//class UnitInformation : public QDialog
class UnitInformation : public QWidget
{
    Q_OBJECT

public:
    explicit UnitInformation(QWidget *parent = nullptr);

    explicit UnitInformation(QString unitName = "", QString unitFile = "",  QWidget *parent = nullptr);

    ~UnitInformation();
    void init();

    void createMainWindowView();
    void createTabWidgetView();
    void createUnitConfigView();
    void createUnitLogView();
    void createUnitDependencyView();

    QString invokeCommand(QString cmdstr, QStringList arguments);

private:
    QWidget *    m_pMainWidget ;
    QTabWidget * m_pUnitInfoTabWidget;

    QWidget *  m_pUnitConfigureWidget ;
    QWidget *  m_pUnitLogWidget ;
    QWidget *  m_pUnitDependencyWidget;


    QVBoxLayout* m_mainLayout1;
    //QBoxLayout* m_mainLayout1;

    QVBoxLayout* m_subUnitConfigureVLayout;
    QVBoxLayout* m_subUnitLogVLayout;
    QVBoxLayout* m_subUnitDependencyVLayout;

    QTextEdit * m_unitConfigureTextEdit;
    QTextEdit * m_unitLogTextEdit;
    QTextEdit * m_unitDependencyTextEdit;

    QString m_unitName;
    QString m_unitFile;

    Highlighter *highlighter;

    QHBoxLayout * m_unitConfigBtnHLayout;
    QPushButton * m_unitConfigEditBtn;
    QPushButton * m_unitConfigResetBtn;
    QPushButton * m_unitConfigApplyBtn;
    QPushButton * m_unitConfigCancelBtn;
    QLabel * m_unitConfigLabel;

    QByteArray m_unitConfigContentByteArray;


    SystemdManagerInterface * m_systemdManagerInterface;

protected:
    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;

public Q_SLOTS:
    void handleUnitConfigEditBtnClick();
    void handleUnitConfigResetBtnClick();
    void handleUnitConfigApplyBtnClick();
    void handleUnitConfigCancelBtnClick();
};

#endif // UNITINFORMATION_H
