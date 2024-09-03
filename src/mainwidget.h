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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QPushButton>
#include <QToolButton>
#include <QString>

#include "configfilewindow.h"
#include "settingswindow.h"
#include "unitwindow.h"
#include "usersessionwindow.h"
#include "analyzewindow.h"
#include "journalwindow.h"
#include "qprocessinfowindow.h"
#include "udevdevicewindow.h"

#include "aboutwindow.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    typedef enum {
        BM_Unit,
        BM_Session,
        BM_ConfigFile,
        BM_Analyze,
        BM_Journal,
        BM_Process,
        BM_UdevDevice,
        BM_Setting,
        BM_About
    } btnMenuTypes;

    QString addTextSpaceMargin(const QString strText);

private:
    QWidget * centralWidget;
    QWidget * sideBarWidget;

    QHBoxLayout * mainLayout;
    QVBoxLayout * sideBarLayout;

    QButtonGroup *btnGroup;
    UnitWindow unitWnd;
    UserSessionWindow usersessionWnd;
    ConfigFileWindow configfileWnd;
    AnalyzeWindow  analyzeWnd;
    JournalWindow  journalWnd;
    QProcessInfoWidget  processWnd;
    UdevDeviceWindow  udevdeviceWnd;
    SettingsWindow settingsWnd;
    AboutWindow aboutWnd;

    QStackedWidget* stackedWidget;
    QToolButton * btnUnit;
    QToolButton * btnUserSession;
    QToolButton * btnConfigFile;
    QToolButton * btnAnalyze;
    QToolButton * btnJournal;
    QToolButton * btnProcess;
    QToolButton * btnUdevDevice;
    QToolButton * btnSettings;
    QToolButton * btnAbout;

    QToolButton * btnCustom;

    QStringList toolBtnStrList;

protected:
    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual bool eventFilter(QObject * obj, QEvent *event) override;
};
#endif // MAINWIDGET_H
