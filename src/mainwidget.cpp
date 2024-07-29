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

#include "mainwidget.h"

#include <QSplitter>

QString qWidgetToolButtonStyleSheet1(R"(
/* QToolButton style */
QToolButton{
    border-top: 0px outset transparent;
    border-bottom: 0px outset transparent;
    border-right: 0px outset transparent;
    border-left: 0px outset transparent;

    border-radius:6px;

    background-color: #ebebeb;
    width: 270px;
    height: 45px;
    margin: 0px 0px;
}

/* hover */
QToolButton:hover{
    background-color: #dedede;
}

/* press */
QToolButton:pressed {
    border-left: 0px outset rgb(93, 95, 97);
    background-color: #cdcdcd;
}

/* checked */
QToolButton:checked {
    background-color: #dedede;
}

/* hover and checked */
QToolButton:hover:checked {
    background-color: #d3d3d3;
}

/* press and checked */
QToolButton:pressed:checked {
    border-left: 0px outset rgb(93, 95, 97);
    background-color: #c7c7c7;
}

QPushButton:default {
    border-color: navy; /* make the default button prominent */
    border-radius:6px;
}

/* QWidget style */
QWidget
{
    background-color: #ebebeb;
}
)");

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Systemd Manager tools"));
    setWindowIcon(QIcon(QString(":/icons/systemd-manage.ico")));
    sideBarWidget = new QWidget(this);

    QSize toolBtnSize(36, 36);

    QPixmap pixmapUnit(QString(":/icons/systemd-service.svg"));
    btnUnit = new QToolButton;
    btnUnit->setText(addTextSpaceMargin(tr("Service unit")));
    btnUnit->setToolTip(tr("Systemd unit management"));
    btnUnit->setIcon(pixmapUnit);        //set icon
    btnUnit->setIconSize(toolBtnSize);   //icon size
    //btnUnit->setFixedSize(160,40);
    //btnUnit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); //text under icon
    btnUnit->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); //text beside icon
    //btnUnit->setStyleSheet(qToolButtonStyleSheet1);
    btnUnit->setCheckable(true);
    btnUnit->setChecked(true);
    btnUnit->setAutoExclusive(true);

    QPixmap pixmapSession(QString(":/icons/user-session.svg"));
    btnUserSession = new QToolButton;
    btnUserSession->setText(addTextSpaceMargin(tr("Session")));
    btnUserSession->setToolTip(tr("Systemd session management"));
    btnUserSession->setIcon(pixmapSession);
    btnUserSession->setIconSize(toolBtnSize);
    btnUserSession->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); //text beside icon
    btnUserSession->setCheckable(true);
    btnUserSession->setChecked(false);
    btnUserSession->setAutoExclusive(true);

    QPixmap pixmapConfigFile(QString(":/icons/config-file.svg"));
    btnConfigFile = new QToolButton;
    btnConfigFile->setText(addTextSpaceMargin(tr("Config file")));
    btnConfigFile->setToolTip(tr("systemd config file management"));
    btnConfigFile->setIcon(pixmapConfigFile);
    btnConfigFile->setIconSize(toolBtnSize);
    btnConfigFile->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnConfigFile->setCheckable(true);
    btnConfigFile->setChecked(false);
    btnConfigFile->setAutoExclusive(true);

    QPixmap pixmapAnalyze(QString(":/icons/analysize-statistics.svg"));
    btnAnalyze = new QToolButton;
    btnAnalyze->setText(addTextSpaceMargin(tr("Analysize statistics")));
    btnAnalyze->setToolTip(tr("System analysize and static"));
    btnAnalyze->setIcon(pixmapAnalyze);
    btnAnalyze->setIconSize(toolBtnSize);
    btnAnalyze->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnAnalyze->setCheckable(true);
    btnAnalyze->setChecked(false);
    btnAnalyze->setAutoExclusive(true);

    QPixmap pixmapJournal(QString(":/icons/journal-log.svg"));
    btnJournal = new QToolButton;
    btnJournal->setText(addTextSpaceMargin(tr("Journal Log View")));
    btnJournal->setToolTip(tr("System Journal Log View"));
    btnJournal->setIcon(pixmapJournal);
    btnJournal->setIconSize(toolBtnSize);
    btnJournal->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnJournal->setCheckable(true);
    btnJournal->setChecked(false);
    btnJournal->setAutoExclusive(true);

    QPixmap pixmapProcess(QString(":/icons/process-manage.svg"));
    btnProcess = new QToolButton;
    btnProcess->setText(addTextSpaceMargin(tr("Process Manage")));
    btnProcess->setToolTip(tr("Process Manage"));
    btnProcess->setIcon(pixmapProcess);
    btnProcess->setIconSize(toolBtnSize);
    btnProcess->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnProcess->setCheckable(true);
    btnProcess->setChecked(false);
    btnProcess->setAutoExclusive(true);

    QPixmap pixmapSettings(QString(":/icons/system-setting.svg"));
    btnSettings = new QToolButton;
    btnSettings->setText(addTextSpaceMargin(tr("Setting")));
    btnSettings->setToolTip(tr("Setting and preference"));
    btnSettings->setIcon(pixmapSettings);
    btnSettings->setIconSize(toolBtnSize);
    btnSettings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnSettings->setCheckable(true);
    btnSettings->setChecked(false);
    btnSettings->setAutoExclusive(true);

    QPixmap pixmapAbout(QString(":/icons/version-info.svg"));
    btnAbout = new QToolButton;
    btnAbout->setText(addTextSpaceMargin(tr("About")));
    btnAbout->setToolTip(tr("About Application"));
    btnAbout->setIcon(pixmapAbout);
    btnAbout->setIconSize(toolBtnSize);
    btnAbout->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnAbout->setCheckable(true);
    btnAbout->setChecked(false);
    btnAbout->setAutoExclusive(true);

    btnGroup = new QButtonGroup(this);
    btnGroup->addButton(btnUnit, BM_Unit);
    btnGroup->addButton(btnUserSession, BM_Session);
    btnGroup->addButton(btnConfigFile, BM_ConfigFile);
    btnGroup->addButton(btnAnalyze, BM_Analyze);
    btnGroup->addButton(btnJournal, BM_Journal);
    btnGroup->addButton(btnProcess, BM_Process);
    btnGroup->addButton(btnSettings, BM_Setting);
    btnGroup->addButton(btnAbout, BM_About);
    btnGroup->setExclusive(true);

    stackedWidget= new QStackedWidget(this);
    stackedWidget->addWidget(&unitWnd);
    stackedWidget->addWidget(&usersessionWnd);
    stackedWidget->addWidget(&configfileWnd);
    stackedWidget->addWidget(&analyzeWnd);
    stackedWidget->addWidget(&journalWnd);
    stackedWidget->addWidget(&processWnd);
    stackedWidget->addWidget(&settingsWnd);
    stackedWidget->addWidget(&aboutWnd);

    stackedWidget->setWindowFlags(Qt::FramelessWindowHint);

    QFrame * verticalSeparationLine = new QFrame(this);
    verticalSeparationLine->setFixedWidth(4);
    verticalSeparationLine->setMinimumHeight(height());
    verticalSeparationLine->setFrameShape(QFrame::VLine);
    verticalSeparationLine->setFrameShadow(QFrame::Sunken);

    QVBoxLayout * sideBarLayout = new QVBoxLayout;
    sideBarLayout->setSpacing(5);
    sideBarLayout->setContentsMargins(0, 0, 0, 0);

    sideBarLayout->addWidget(btnUnit);
    sideBarLayout->addWidget(btnUserSession);
    sideBarLayout->addWidget(btnConfigFile);
    sideBarLayout->addWidget(btnAnalyze);
    sideBarLayout->addWidget(btnJournal);
    sideBarLayout->addWidget(btnProcess);
    sideBarLayout->addStretch(0);
    sideBarLayout->addWidget(btnSettings);
    sideBarLayout->addWidget(btnAbout);

    sideBarWidget->setLayout(sideBarLayout);
    sideBarWidget->setStyleSheet(qWidgetToolButtonStyleSheet1);
    sideBarWidget->setWindowFlags(Qt::FramelessWindowHint);

    //default selected page
    btnGroup->button(0)->setChecked(true);
    stackedWidget->setCurrentIndex(0);

    btnSettings->installEventFilter(sideBarWidget);
    sideBarWidget->installEventFilter(this);
    installEventFilter(this);

    QSplitter splitter(Qt::Horizontal);

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(sideBarWidget);
    mainLayout->addWidget(verticalSeparationLine);
    mainLayout->addWidget(stackedWidget, Qt::AlignCenter);
    mainLayout->setSpacing(0);
    mainLayout->addStretch(0);

    setLayout(mainLayout);

    this->resize(1400,900);

    connect(btnGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), stackedWidget, &QStackedWidget::setCurrentIndex);
}

bool MainWidget::eventFilter(QObject * obj, QEvent *event)
{
    if (( event->type() == QEvent::Show || event->type() == QEvent::WindowActivate || event->type() == QEvent::Resize))
    {
        update();
        QPoint pos(0,sideBarWidget->height()-btnSettings->height()-3);
    }
    return false;
}

MainWidget::~MainWidget()
{
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    if (nullptr != sideBarWidget)
    {
        //qDebug()<<"sideBar width:"<< sideBarWidget->width() << " sideBar height:" << sideBarWidget->height();
        sideBarWidget->resize(sideBarWidget->width(), height()-20);
    }

    if (nullptr != stackedWidget)
    {
        stackedWidget->resize(stackedWidget->width(), height()-5);
    }

    update();
}

QString MainWidget::addTextSpaceMargin(const QString strText){
    QString strRet = "  " + strText;
    return strRet;
}
