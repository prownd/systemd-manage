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

#include "usersessionwindow.h"
#include <QDBusArgument>
#include <QDBusMessage>
#include <QDebug>
#include <QStandardItem>
#include <QStringLiteral>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>

#include "qabstractbutton.h"
#include "usersessioninformation.h"

QString sessionTabWidgetStyleSheet1(R"(
QTabBar::tab {
    /* color: rgb(84,2,119);    */
    /* background-image: url(:/pics/wood.jpg); */
    /* border: 1px solid rgb(68,66,64);     */
    /* border-bottom-color: rgb(68,66,64); */
    border-top: 2px outset transparent;
    border-bottom: 6px outset transparent;
    border-right: 6px outset transparent;
    border-left: 6px outset transparent;
    background-color: #ebebeb;

    border-top-left-radius: 4px;
    border-top-right-radius: 4px;

    margin-top: 2px;

    height: 45px;
    width: 160px;
}
QTabWidget::tab-bar {
   alignment: center;
}

QWidget {
   alignment: center;
}

QTabBar::tab:hover {
   margin-top: 5px;
   background-color: #dedede;
}

QTabBar::tab:!selected {
   margin-top: 5px;
}
QTabBar::tab:selected {
   color: rgb(255,0,128);
    background-color: #c7c7c7;
}
)");

UserSessionWindow::UserSessionWindow(QWidget *parent) :
    QWidget(parent)
{
    //frameless window
    setWindowFlags(Qt::FramelessWindowHint);

    initData();
    createTabWidgetView();

    m_systemdManagerInterface=new SystemdManagerInterface;
    qDBusRegisterMetaType<SystemdSession>();
    qDBusRegisterMetaType<SystemdUser>();

    createSessionTableView();
    createUserTableView();
}

UserSessionWindow::~UserSessionWindow()
{
}

void UserSessionWindow::initData()
{
    m_sessionColumnCount = 6;
    m_userColumnCount = 6;
}

void UserSessionWindow::createTabWidgetView()
{
    // init tabwidget
    m_pTabWidget = new QTabWidget(this);
    //m_pTabWidget->setFixedSize(width(), height());
    m_pTabWidget->setMinimumWidth(width());
    m_pTabWidget->setMinimumHeight(height());
    m_pTabWidget->adjustSize();

    // init unit subwidget
    m_pSessionWidget = new QWidget();
    m_subSessionLayout = new QHBoxLayout(m_pSessionWidget);
    m_pSessionWidget->setLayout(m_subSessionLayout);

    // init session subwidget
    m_pUserWidget = new QWidget();
    m_subUserLayout = new QHBoxLayout(m_pUserWidget);
    m_pUserWidget->setLayout(m_subUserLayout);

    // add all sub widget
    m_pTabWidget->addTab(m_pSessionWidget, tr("Session"));
    m_pTabWidget->addTab(m_pUserWidget, tr("User"));
    //m_pTabWidget->setStyleSheet(sessionTabWidgetStyleSheet1);
}

void UserSessionWindow::createSessionTableView()
{
    m_sessionTableView= new QTableView(this);
    // Sets up the session list initially
    // Register the meta type for storing units
    qDBusRegisterMetaType<SystemdSession>();

    // Setup model for session list
    m_sessionModel = new QStandardItemModel(this);

    // Set header row
    m_sessionModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Session ID")));
    m_sessionModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Session Object Path")));
    m_sessionModel->setHorizontalHeaderItem(2, new QStandardItem(tr("State")));
    m_sessionModel->setHorizontalHeaderItem(3, new QStandardItem(tr("User ID")));
    m_sessionModel->setHorizontalHeaderItem(4, new QStandardItem(tr("User Name")));
    m_sessionModel->setHorizontalHeaderItem(5, new QStandardItem(tr("Seat ID")));
    m_sessionTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_sessionTableView->verticalHeader()->setDefaultSectionSize(40);
    m_sessionTableView->resizeColumnsToContents();
    m_sessionTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //set tableview delegate to modify column
    m_pSessionTableViewItemDelegateProxy = new MyStyledItemDelegate(this);
    m_sessionTableView->setItemDelegate(m_pSessionTableViewItemDelegateProxy);
    m_pSessionTableViewItemDelegateProxy->setColumnWidth(1,600);

    m_sessionTableView->verticalHeader()->hide();

    m_sessionTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_sessionTableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_sessionTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_sessionTableView->setColumnHidden(m_sessionColumnCount-1, true);

    //auto fill columns width
    m_sessionTableView->horizontalHeader()->setStretchLastSection(true);

    // Set model for QTableView (should be called after headers are set)
    m_sessionTableView->setModel(m_sessionModel);
    m_sessionTableView->setMinimumWidth(width());
    m_sessionTableView->setMinimumHeight(height());
    m_sessionTableView->adjustSize();
    m_sessionTableView->setMouseTracking(true);

    m_subSessionLayout->addWidget(m_sessionTableView);

    //create contextMenu
    m_sessionTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_sessionTableView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(sessionCustomMenuRequested(QPoint)));

    connect(m_sessionTableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotSessionTableRowDoubleClicked(const QModelIndex &)));
    connect(m_sessionTableView, SIGNAL(entered(QModelIndex)), this, SLOT(slotSessionTableRowTooltip(QModelIndex)));

    // Add all the sessions
    slotRefreshSessionList();
}

void UserSessionWindow::slotSessionTableRowTooltip(const QModelIndex index)
{
    int curRow=index.row();
    const QModelIndex& curCellIndex = m_sessionModel->index(curRow,0);
    QString sessionID = m_sessionModel->data(curCellIndex).toString();
    const QModelIndex& curCellIndex2 = m_sessionModel->index(curRow, 1);
    QString sessionObjectPathStr = m_sessionModel->data(curCellIndex2).toString();

    QDBusObjectPath session_objectPath;
    session_objectPath.setPath(sessionObjectPathStr);
    QString sessionName=m_systemdManagerInterface->getDbusProperty(QStringLiteral("Name"), logdSession, session_objectPath).toString();
    QString sessionActive = m_systemdManagerInterface->getDbusProperty(QStringLiteral("State"), logdSession, session_objectPath).toString();
    QString sessionScope = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Scope"), logdSession, session_objectPath).toString();
    QString sessionService = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Service"), logdSession, session_objectPath).toString();
    QString sessionTTY = m_systemdManagerInterface->getDbusProperty(QStringLiteral("TTY"), logdSession, session_objectPath).toString();
    QString sessionTimestamp = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Timestamp"), logdSession, session_objectPath).toString();
    QString sessionRemoteHost = m_systemdManagerInterface->getDbusProperty(QStringLiteral("RemoteHost"), logdSession, session_objectPath).toString();
    QString sessionType = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Type"), logdSession, session_objectPath).toString();
    QString sessionLeader = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Leader"), logdSession, session_objectPath).toString();
    QString sessionAudit = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Audit"), logdSession, session_objectPath).toString();
    QString sessionVTNumber = m_systemdManagerInterface->getDbusProperty(QStringLiteral("VTNr"), logdSession, session_objectPath).toString();

    QString toolTipText;
    toolTipText.append(QStringLiteral("<FONT COLOR=DarkCyan>"));
    toolTipText.append(QStringLiteral("<b>Session ID: %1</b><hr>").arg(sessionID));
    toolTipText.append(QStringLiteral("<b>Session Object Path: %1</b><hr>").arg(sessionObjectPathStr));
    toolTipText.append(QStringLiteral("<b>Session Name: %1</b><hr>").arg(sessionName));
    toolTipText.append(QStringLiteral("<b>Session Active: %1</b><hr>").arg(sessionActive));
    toolTipText.append(QStringLiteral("<b>Session Scope: %1</b><hr>").arg(sessionScope));
    toolTipText.append(QStringLiteral("<b>Session Service: %1</b><hr>").arg(sessionService));
    toolTipText.append(QStringLiteral("<b>Session TTY: %1</b><hr>").arg(sessionTTY));
    toolTipText.append(QStringLiteral("<b>Session Timestamp: %1</b><hr>").arg(sessionTimestamp));
    toolTipText.append(QStringLiteral("<b>Session Remote Host: %1</b><hr>").arg(sessionRemoteHost));
    toolTipText.append(QStringLiteral("<b>Session Type: %1</b><hr>").arg(sessionType));
    toolTipText.append(QStringLiteral("<b>Session Leader: %1</b><hr>").arg(sessionLeader));
    toolTipText.append(QStringLiteral("<b>Session Audit: %1</b><hr>").arg(sessionAudit));
    toolTipText.append(QStringLiteral("<b>Session VT Number: %1</b><hr>").arg(sessionVTNumber));
    toolTipText.append(QStringLiteral("</FONT"));

    m_sessionModel->itemFromIndex(index)->setToolTip(toolTipText);
}

void UserSessionWindow::slotSessionTableRowDoubleClicked(const QModelIndex index)
{
    QModelIndexList selectedIndexes = m_sessionTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow=curRowIndex.row();
        const QModelIndex& curCellIndex = m_sessionModel->index(curRow,0);
        QString sessionID = m_sessionModel->data(curCellIndex).toString();

        const QModelIndex& curCellIndex2 = m_sessionModel->index(curRow, 1);
        QString sessionObjectPath = m_sessionModel->data(curCellIndex2).toString();

        SessionInformation * sessionInformationWnd = new SessionInformation(sessionID, sessionObjectPath, this);
        sessionInformationWnd->setWindowTitle(QObject::tr("Session Status Information"));
        sessionInformationWnd->setWindowModality(Qt::ApplicationModal);
        sessionInformationWnd->setAttribute(Qt::WA_ShowModal, true); //property, true:model false:non model
        sessionInformationWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
        sessionInformationWnd->show();
    }
}

void UserSessionWindow::sessionCustomMenuRequested(QPoint pos)
{
    QModelIndex clickedIndex = m_sessionTableView->indexAt(pos);
    if (clickedIndex.isValid()){    // if index valid, continue
        QMenu *sessionMenu=new QMenu(this);

        QAction*  sessionStatusAction = new QAction(tr("Session Status"), this);
        sessionMenu->addAction(sessionStatusAction);

        sessionMenu->addSeparator();

        QAction* sessionLockAction = new QAction(tr("Session Lock"), this);
        sessionMenu->addAction(sessionLockAction);

        QAction* sessionUnlockAction = new QAction(tr("Session Unlock"), this);
        sessionMenu->addAction(sessionUnlockAction);

        QAction* sessionActivateAction = new QAction(tr("Session Activate"), this);
        sessionMenu->addAction(sessionActivateAction);

        sessionMenu->addSeparator();

        QAction* sessionTerminateAction = new QAction(tr("Session Terminate"), this);
        sessionMenu->addAction(sessionTerminateAction);

        sessionMenu->addSeparator();

        QAction* sessionRefreshAction = new QAction(tr("Refresh Session"), this);
        sessionMenu->addAction(sessionRefreshAction);

        connect(sessionStatusAction, &QAction::triggered, this, &UserSessionWindow::handleSessionStatusAction);
        connect(sessionLockAction, &QAction::triggered, this, &UserSessionWindow::handleSessionLockAction);
        connect(sessionUnlockAction, &QAction::triggered, this, &UserSessionWindow::handleSessionUnlockAction);
        connect(sessionActivateAction, &QAction::triggered, this, &UserSessionWindow::handleSessionActivateAction);
        connect(sessionTerminateAction, &QAction::triggered, this, &UserSessionWindow::handleSessionTerminateAction);
        connect(sessionRefreshAction, &QAction::triggered, this, &UserSessionWindow::handleSessionRefreshAction);
        sessionMenu->popup(m_sessionTableView->viewport()->mapToGlobal(pos));
    }
}

void UserSessionWindow::handleSessionStatusAction()
{
    QModelIndexList selectedIndexes = m_sessionTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow=curRowIndex.row();
        const QModelIndex& curCellIndex = m_sessionModel->index(curRow,0);
        QString sessionID = m_sessionModel->data(curCellIndex).toString();

        const QModelIndex& curCellIndex2 = m_sessionModel->index(curRow, 1);
        QString sessionObjectPath = m_sessionModel->data(curCellIndex2).toString();

        SessionInformation * sessionInformationWnd = new SessionInformation(sessionID, sessionObjectPath, this);
        sessionInformationWnd->setWindowTitle(QObject::tr("Session Status Information"));
        sessionInformationWnd->setWindowModality(Qt::ApplicationModal);
        sessionInformationWnd->setAttribute(Qt::WA_ShowModal, true); //property, true:model false:non model
        //sessionInformationWnd->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        sessionInformationWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
        sessionInformationWnd->show();
    }
}

void UserSessionWindow::handleSessionLockAction()
{
    // QMessageBox::StandardButton box;
    // box = QMessageBox::warning(this, tr("Warning"), tr("Are you sure to Lock Session?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    // if(box==QMessageBox::No)
    //     return;
    // else if(box==QMessageBox::Yes)
    // {
    //     qDebug()<<"Lock Session";
    // }

    QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, tr("Warning"), tr("Are you sure to Lock Session?"), QMessageBox::Yes|QMessageBox::No);
    msgBox->button(QMessageBox::Yes)->setText(tr("Yes"));
    msgBox->button(QMessageBox::No)->setText(tr("No"));
    msgBox->setDefaultButton(QMessageBox::No);
    int res = msgBox->exec();
    if(QMessageBox::Yes == res) {
        qDebug()<<"Lock Session";
    }else if(QMessageBox::No == res) {
        return;
    }

    QModelIndexList selectedIndexes = m_sessionTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_sessionModel->index(curRow,0);
        QString sessionId = m_sessionModel->data(curCellIndex).toString();
        m_systemdManagerInterface->lockSessin(sessionId);
    }
}

void UserSessionWindow::handleSessionUnlockAction()
{
    QModelIndexList selectedIndexes = m_sessionTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_sessionModel->index(curRow,0);
        QString sessionId = m_sessionModel->data(curCellIndex).toString();
        m_systemdManagerInterface->unlockSessin(sessionId);
    }
}

void UserSessionWindow::handleSessionActivateAction()
{
    QModelIndexList selectedIndexes = m_sessionTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_sessionModel->index(curRow,0);
        QString sessionId = m_sessionModel->data(curCellIndex).toString();
        m_systemdManagerInterface->activateSession(sessionId);
    }
}

void UserSessionWindow::handleSessionTerminateAction()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, tr("Warning"), tr("Are you sure to Terminate Session?"), QMessageBox::Yes|QMessageBox::No);
    msgBox->button(QMessageBox::Yes)->setText(tr("Yes"));
    msgBox->button(QMessageBox::No)->setText(tr("No"));
    msgBox->setDefaultButton(QMessageBox::No);
    int res = msgBox->exec();
    if(QMessageBox::Yes == res) {
        qDebug()<<"Terminal Session";
    }else if(QMessageBox::No == res) {
        return;
    }

    QModelIndexList selectedIndexes = m_sessionTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_sessionModel->index(curRow,0);
        QString sessionId = m_sessionModel->data(curCellIndex).toString();
        m_systemdManagerInterface->terminateSession(sessionId);
    }
}

void UserSessionWindow::handleSessionRefreshAction()
{
    slotRefreshSessionList();
    m_sessionTableView->resizeColumnsToContents();
}

void UserSessionWindow::createUserTableView()
{
    m_userTableView= new QTableView(this);
    qDBusRegisterMetaType<SystemdUser>();
    // Setup model for session list
    m_userModel = new QStandardItemModel(this);
    // Set header row
    m_userModel->setHorizontalHeaderItem(0, new QStandardItem(tr("User ID")));
    m_userModel->setHorizontalHeaderItem(1, new QStandardItem(tr("User Name")));
    m_userModel->setHorizontalHeaderItem(2, new QStandardItem(tr("User Object Path"))); // This column is hidden
    m_userModel->setHorizontalHeaderItem(3, new QStandardItem(tr("User State")));
    m_userModel->setHorizontalHeaderItem(4, new QStandardItem(tr("Group ID")));
    m_userModel->setHorizontalHeaderItem(5, new QStandardItem(tr("User Slice")));
    m_userTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_userTableView->verticalHeader()->setDefaultSectionSize(40);
    m_userTableView->resizeColumnsToContents();
    m_userTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //set tableview delegate to modify column
    m_pUserTableViewItemDelegateProxy = new MyStyledItemDelegate(this);
    m_userTableView->setItemDelegate(m_pUserTableViewItemDelegateProxy);
    m_pUserTableViewItemDelegateProxy->setColumnWidth(1,200);
    m_userTableView->verticalHeader()->hide();
    m_userTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_userTableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_userTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_userTableView->setColumnHidden(m_userColumnCount-1, true);

    //auto fill columns width
    m_userTableView->horizontalHeader()->setStretchLastSection(true);

    // Set model for QTableView (should be called after headers are set)
    m_userTableView->setModel(m_userModel);

    m_userTableView->setMinimumWidth(width());
    m_userTableView->setMinimumHeight(height());
    m_userTableView->adjustSize();
    m_userTableView->setMouseTracking(true);

    m_subUserLayout->addWidget(m_userTableView);

    //create contextMenu
    m_userTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_userTableView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(userCustomMenuRequested(QPoint)));

    connect(m_userTableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotUserTableRowDoubleClicked(const QModelIndex &)));
    connect(m_userTableView, SIGNAL(entered(QModelIndex)), this, SLOT(slotUserTableRowTooltip(QModelIndex)));

    // Add all the sessions
    slotRefreshUserList();

}

void UserSessionWindow::slotUserTableRowTooltip(const QModelIndex index)
{
    int curRow=index.row();
    const QModelIndex& curCellIndex = m_userModel->index(curRow,0);
    QString userID = m_userModel->data(curCellIndex).toString();

    const QModelIndex& curCellIndex2 = m_userModel->index(curRow, 2);
    QString userObjectPathStr = m_userModel->data(curCellIndex2).toString();

    QDBusObjectPath user_objectPath;
    user_objectPath.setPath(userObjectPathStr);

    QString userName = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Name"), logdUser, user_objectPath).toString();
    QString userGid = m_systemdManagerInterface->getDbusProperty(QStringLiteral("GID"), logdUser, user_objectPath).toString();
    QString userState = m_systemdManagerInterface->getDbusProperty(QStringLiteral("State"), logdUser, user_objectPath).toString();
    QString userService = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Service"), logdUser, user_objectPath).toString();
    QString userSlice = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Slice"), logdUser, user_objectPath).toString();
    QString userRuntimePath = m_systemdManagerInterface->getDbusProperty(QStringLiteral("RuntimePath"), logdUser, user_objectPath).toString();
    QString userTimestamp = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Timestamp"), logdUser, user_objectPath).toString();

    QString toolTipText;
    toolTipText.append(QStringLiteral("<FONT COLOR=DarkCyan>"));
    toolTipText.append(QStringLiteral("<b>User ID: %1</b><hr>").arg(userID));
    toolTipText.append(QStringLiteral("<b>User Object Path: %1</b><hr>").arg(userObjectPathStr));
    toolTipText.append(QStringLiteral("<b>User Name: %1</b><hr>").arg(userName));
    toolTipText.append(QStringLiteral("<b>User Gid: %1</b><hr>").arg(userGid));
    toolTipText.append(QStringLiteral("<b>User State: %1</b><hr>").arg(userState));
    toolTipText.append(QStringLiteral("<b>User Service: %1</b><hr>").arg(userService));
    toolTipText.append(QStringLiteral("<b>User Slice: %1</b><hr>").arg(userSlice));
    toolTipText.append(QStringLiteral("<b>User RuntimePath: %1</b><hr>").arg(userRuntimePath));
    toolTipText.append(QStringLiteral("<b>User Timestamp: %1</b><hr>").arg(userTimestamp));

    m_userModel->itemFromIndex(index)->setToolTip(toolTipText);
}

void UserSessionWindow::slotUserTableRowDoubleClicked(const QModelIndex index)
{
    QModelIndexList selectedIndexes = m_userTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >= 1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_userModel->index(curRow,0);
        QString userID = m_userModel->data(curCellIndex).toString();

        const QModelIndex& curCellIndex2 = m_userModel->index(curRow, 2);
        QString userObjectPath = m_userModel->data(curCellIndex2).toString();

        UserInformation * userInformationWnd = new UserInformation(userID, userObjectPath, this);
        userInformationWnd->setWindowTitle(QObject::tr("Show User Information"));
        userInformationWnd->setWindowModality(Qt::ApplicationModal);
        userInformationWnd->setAttribute(Qt::WA_ShowModal, true); //property, true:model false:non model
        userInformationWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
        userInformationWnd->show();
    }
}

void UserSessionWindow::userCustomMenuRequested(QPoint pos)
{
    QModelIndex clickedIndex = m_userTableView->indexAt(pos);
    if (clickedIndex.isValid()){
        QMenu *userMenu=new QMenu(this);

        QAction*  userStatusAction = new QAction(tr("User Status"), this);
        userMenu->addAction(userStatusAction);

        userMenu->addSeparator();

        QAction* userTerminateAction = new QAction(tr("Terminate User"), this);
        userMenu->addAction(userTerminateAction);

        userMenu->addSeparator();

        QAction* userRefreshAction = new QAction(tr("Refresh User"), this);
        userMenu->addAction(userRefreshAction);

        connect(userStatusAction, &QAction::triggered, this, &UserSessionWindow::handleUserStatusAction);
        connect(userTerminateAction, &QAction::triggered, this, &UserSessionWindow::handleUserTerminateAction);
        connect(userRefreshAction, &QAction::triggered, this, &UserSessionWindow::handleUserRefreshAction);

        userMenu->popup(m_userTableView->viewport()->mapToGlobal(pos));
    }
}

void UserSessionWindow::handleUserStatusAction()
{
    QModelIndexList selectedIndexes = m_userTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >= 1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_userModel->index(curRow,0);
        QString userID = m_userModel->data(curCellIndex).toString();

        const QModelIndex& curCellIndex2 = m_userModel->index(curRow, 2);
        QString userObjectPath = m_userModel->data(curCellIndex2).toString();

        UserInformation * userInformationWnd = new UserInformation(userID, userObjectPath, this);
        userInformationWnd->setWindowTitle(QObject::tr("Show User Information"));
        userInformationWnd->setWindowModality(Qt::ApplicationModal);
        userInformationWnd->setAttribute(Qt::WA_ShowModal, true); //property, true:model false:non model
        userInformationWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
        userInformationWnd->show();
    }
}

void UserSessionWindow::handleUserTerminateAction()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, tr("Warning"), tr("Are you sure to Terminate User?"), QMessageBox::Yes|QMessageBox::No);
    msgBox->button(QMessageBox::Yes)->setText(tr("Yes"));
    msgBox->button(QMessageBox::No)->setText(tr("No"));
    msgBox->setDefaultButton(QMessageBox::No);
    int res = msgBox->exec();
    if(QMessageBox::Yes == res) {
        qDebug()<<"Terminal User";
    }else if(QMessageBox::No == res) {
        return;
    }

    QModelIndexList selectedIndexes = m_userTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_userModel->index(curRow,0);
        uint userId = m_userModel->data(curCellIndex).toUInt();
        m_systemdManagerInterface->terminateUser(userId);
    }
}

void UserSessionWindow::handleUserRefreshAction()
{
    slotRefreshUserList();
    m_userTableView->resizeColumnsToContents();
    m_userTableView->setMinimumWidth(width());
    m_userTableView->setMinimumHeight(height());
    m_userTableView->adjustSize();
}

void UserSessionWindow::slotRefreshSessionList()
{
    // clear list
    m_sessionList.clear();

    // get an updated list of sessions via dbus
    QDBusMessage dbusreply = m_systemdManagerInterface->invokeDbusMethod(QStringLiteral("ListSessions"), logdMgr);

    // extract the list of sessions from the reply
    const QDBusArgument arg = dbusreply.arguments().at(0).value<QDBusArgument>();
    if (arg.currentType() == QDBusArgument::ArrayType)
    {
        arg.beginArray();
        while (!arg.atEnd())
        {
            SystemdSession session;
            arg >> session;
            m_sessionList.append(session);
        }
        arg.endArray();
    }

    // Iterate through the new list and compare to model
    for (const SystemdSession &s : m_sessionList) {
        // This is needed to get the "State" property
        QList<QStandardItem *> items = m_sessionModel->findItems(s.session_id, Qt::MatchExactly, 0);
        if (items.isEmpty())
        {
            // New session discovered so add it to the model
            QList<QStandardItem *> row;
            row <<
                new QStandardItem(s.session_id) <<
                new QStandardItem(s.session_path.path()) <<
                new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("State"), logdSession, s.session_path).toString()) <<
                new QStandardItem(QString::number(s.user_id)) <<
                new QStandardItem(s.user_name) <<
                new QStandardItem(s.seat_id);
            m_sessionModel->appendRow(row);
        } else {
            m_sessionModel->item(items.at(0)->row(), 2)->setData(m_systemdManagerInterface->getDbusProperty(QStringLiteral("State"), logdSession, s.session_path).toString(), Qt::DisplayRole);
        }
    }

    // Check to see if any sessions were removed
    if (m_sessionModel->rowCount() != m_sessionList.size())
    {
        QList<QPersistentModelIndex> indexes;
        // Loop through model and compare to retrieved m_sessionList
        for (int row = 0; row < m_sessionModel->rowCount(); ++row)
        {
            SystemdSession session;
            session.session_id = m_sessionModel->index(row,0).data().toString();
            if (!m_sessionList.contains(session))
            {
                // Add removed units to list for deletion
                // qDebug() << "Unit removed: " << systemUnitModel->index(row,0).data().toString();
                indexes << m_sessionModel->index(row,0);
            }
        }
        // Delete the identified units from model
        for (const QPersistentModelIndex &i : indexes)
            m_sessionModel->removeRow(i.row());
    }

    // Update the text color in model
    //QColor newcolor;
    for (int row = 0; row < m_sessionModel->rowCount(); ++row)
    {
        /*
        QBrush newcolor;
        const KColorScheme scheme(QPalette::Normal);
        if (m_sessionModel->data(m_sessionModel->index(row,2), Qt::DisplayRole) == QLatin1String("active"))
            newcolor = scheme.foreground(KColorScheme::PositiveText);
        else if (m_sessionModel->data(m_sessionModel->index(row,2), Qt::DisplayRole) == QLatin1String("closing"))
            newcolor = scheme.foreground(KColorScheme::InactiveText);
        else
            newcolor = scheme.foreground(KColorScheme::NormalText);

        for (int col = 0; col < m_sessionModel->columnCount(); ++col)
            m_sessionModel->setData(m_sessionModel->index(row,col), QVariant(newcolor), Qt::ForegroundRole);
        */
    }
    m_sessionTableView->resizeColumnsToContents();
}

void UserSessionWindow::slotRefreshUserList()
{
    // clear list
    m_userList.clear();

    // get an updated list of sessions via dbus
    QDBusMessage dbusreply = m_systemdManagerInterface->invokeDbusMethod(QStringLiteral("ListUsers"), logdMgr);

    // extract the list of sessions from the reply
    const QDBusArgument arg = dbusreply.arguments().at(0).value<QDBusArgument>();
    if (arg.currentType() == QDBusArgument::ArrayType)
    {
        arg.beginArray();
        while (!arg.atEnd())
        {
            SystemdUser user;
            arg >> user;
            m_userList.append(user);
        }
        arg.endArray();
    }

    // Iterate through the new list and compare to model
    for (const SystemdUser &u : m_userList) {
        // This is needed to get the "State" property

        QList<QStandardItem *> items = m_userModel->findItems(QString::number(u.user_id), Qt::MatchExactly, 0);


        if (items.isEmpty())
        {
            // New session discovered so add it to the model
            /*
            QVariant qvar = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Sessions"), logdUser, u.user_path);
            QVector<QStringList> user_sessions= qvar.value<QVector<QStringList>>();
            for(const QStringList & ql:user_sessions){
                qDebug()<< ql;
            }*/

            QList<QStandardItem *> row;
            row <<
                new QStandardItem(QString::number(u.user_id)) <<
                new QStandardItem(u.user_name) <<
                new QStandardItem(u.user_path.path()) <<
                new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("State"), logdUser, u.user_path).toString())  <<
                new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("GID"), logdUser, u.user_path).toString())  <<
                new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Slice"), logdUser, u.user_path).toString()) ;
                //<<  new QStandardItem(u.user_sessions);
            m_userModel->appendRow(row);
        } else {
            m_userModel->item(items.at(0)->row(), 2)->setData(m_systemdManagerInterface->getDbusProperty(QStringLiteral("State"), logdUser, u.user_path).toString(), Qt::DisplayRole);
        }
    }

    // Check to see if any sessions were removed
    if (m_userModel->rowCount() != m_userList.size())
    {
        QList<QPersistentModelIndex> indexes;
        // Loop through model and compare to retrieved m_sessionList
        for (int row = 0; row < m_userModel->rowCount(); ++row)
        {
            SystemdUser user;
            user.user_id = m_userModel->index(row,0).data().toInt();
            if (!m_userList.contains(user))
            {
                // Add removed units to list for deletion
                // qDebug() << "Unit removed: " << systemUnitModel->index(row,0).data().toString();
                indexes << m_userModel->index(row,0);
            }
        }
        // Delete the identified units from model
        for (const QPersistentModelIndex &i : indexes)
            m_userModel->removeRow(i.row());
    }

    // Update the text color in model
    for (int row = 0; row < m_userModel->rowCount(); ++row)
    {
        /*
        QBrush newcolor;
        const KColorScheme scheme(QPalette::Normal);
        if (m_sessionModel->data(m_sessionModel->index(row,2), Qt::DisplayRole) == QLatin1String("active"))
            newcolor = scheme.foreground(KColorScheme::PositiveText);
        else if (m_sessionModel->data(m_sessionModel->index(row,2), Qt::DisplayRole) == QLatin1String("closing"))
            newcolor = scheme.foreground(KColorScheme::InactiveText);
        else
            newcolor = scheme.foreground(KColorScheme::NormalText);

        for (int col = 0; col < m_sessionModel->columnCount(); ++col)
            m_sessionModel->setData(m_sessionModel->index(row,col), QVariant(newcolor), Qt::ForegroundRole);
        */
    }
    m_userTableView->resizeColumnsToContents();
}

void UserSessionWindow::resizeEvent(QResizeEvent *event)
{
    if (nullptr != m_pTabWidget)
    {
        //qDebug()<<"m_pSystemUnitWidget resize width:"<< m_pSystemUnitWidget->width() << " height:" << m_pSystemUnitWidget->height();
        m_pTabWidget->resize(width(), height());
        update();
    }
}
