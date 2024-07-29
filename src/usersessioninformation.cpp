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

#include "usersessioninformation.h"

#include "customlistwidgetitem.h"

#include <QListWidget>
#include <QStringLiteral>
#include <QTextEdit>

QString usersessionInformationTableViewStyleSheet1(R"(
QTableView {
    color: black;
    selection-color: red;
    selection-background-color:  #dfeaf8;
    border: 2px outset transparent;
    border-radius: 2px;
    padding: 2px 4px;
}
)");

SessionInformation::SessionInformation(QWidget *parent) :
    QWidget(parent)
{
    //frameless window
    setWindowFlags(Qt::FramelessWindowHint);
    init();
}

SessionInformation::SessionInformation(QString sessionId, QString sessionObjectPath, QWidget *parent):
    QWidget(parent)
{
    m_sessionId = sessionId;
    m_sessionObjectPath = sessionObjectPath;
    init();
}

SessionInformation::~SessionInformation()
{
}

void SessionInformation::init()
{
    setWindowTitle(QObject::tr("Hint Model Window"));
    setWindowModality(Qt::ApplicationModal); //setting block
    setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
    //setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Tool);

    m_systemdManagerInterface=new SystemdManagerInterface;
    qDBusRegisterMetaType<SystemdSession>();
    qDBusRegisterMetaType<SystemdUser>();

    createSessionWidgetView();
    createSessionDetailListWidgetView();
    //createSessionDetailView();
    //this->setFixedSize(1200,800);
}

void SessionInformation::createSessionWidgetView()
{
    m_pSessionWidget = new QWidget(this);
    m_pSessionWidget->setFixedSize(600, 550);

    m_subSessionDetailVLayout = new QVBoxLayout;
    m_pSessionWidget->setLayout(m_subSessionDetailVLayout);
}

void SessionInformation::createSessionDetailView()
{
    QDBusObjectPath session_objectPath;
    session_objectPath.setPath(m_sessionObjectPath);

    m_sessionFieldName1 = new QLabel();
    //m_sessionFieldName1->setText(tr("Session ID: %1").arg(m_sessionId));
    m_sessionFieldName1->setText(tr("Session ID:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName1, 0, 0, 1, 1);

    m_sessionFieldValue1 = new QLabel();
    m_sessionFieldValue1->setText(m_sessionId);
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue1, 0, 1, 1, 1);


    m_sessionFieldName2 = new QLabel();
    m_sessionFieldName2->setText(tr("Session Name:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName2, 1, 0, 1, 1);

    m_sessionFieldValue2 = new QLabel();
    m_sessionFieldValue2->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Name"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue2, 1, 1, 1, 1);

    m_sessionFieldName3 = new QLabel();
    m_sessionFieldName3->setText(tr("Session Objectpath:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName3, 2, 0, 1, 1);

    m_sessionFieldValue3 = new QLabel();
    m_sessionFieldValue3->setText(m_sessionObjectPath);
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue3, 2, 1, 1, 1);

    m_sessionFieldName4 = new QLabel();
    m_sessionFieldName4->setText(tr("Session Active:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName4, 3, 0, 1, 1);

    m_sessionFieldValue4 = new QLabel();
    m_sessionFieldValue4->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("State"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue4, 3, 1, 1, 1);

    m_sessionFieldName5 = new QLabel();
    m_sessionFieldName5->setText(tr("Session Scope:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName5, 4, 0, 1, 1);

    m_sessionFieldValue5 = new QLabel();
    m_sessionFieldValue5->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Scope"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue5, 4, 1, 1, 1);

    m_sessionFieldName6 = new QLabel();
    m_sessionFieldName6->setText(tr("Session Service:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName6, 5, 0, 1, 1);

    m_sessionFieldValue6 = new QLabel();
    m_sessionFieldValue6->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Service"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue6, 5, 1, 1, 1);

    m_sessionFieldName7 = new QLabel();
    m_sessionFieldName7->setText(tr("Session TTY:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName7, 6, 0, 1, 1);

    m_sessionFieldValue7 = new QLabel();
    m_sessionFieldValue7->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("TTY"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue7, 6, 1, 1, 1);

    m_sessionFieldName8 = new QLabel();
    m_sessionFieldName8->setText(tr("Session Timestamp:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName8, 7, 0, 1, 1);

    m_sessionFieldValue8 = new QLabel();
    m_sessionFieldValue8->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Timestamp"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue8, 7, 1, 1, 1);

    m_sessionFieldName9 = new QLabel();
    m_sessionFieldName9->setText(tr("Session Remote Host:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName9, 8, 0, 1, 1);

    m_sessionFieldValue9 = new QLabel();
    m_sessionFieldValue9->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("RemoteHost"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue9, 8, 1, 1, 1);

    m_sessionFieldName10 = new QLabel();
    m_sessionFieldName10->setText(tr("Session Type:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName10, 9, 0, 1, 1);

    m_sessionFieldValue10 = new QLabel();
    m_sessionFieldValue10->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Type"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue10, 9, 1, 1, 1);

    m_sessionFieldName11 = new QLabel();
    m_sessionFieldName11->setText(tr("Session Leader:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName11, 10, 0, 1, 1);

    m_sessionFieldValue11 = new QLabel();
    m_sessionFieldValue11->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Leader"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue11, 10, 1, 1, 1);

    m_sessionFieldName12 = new QLabel();
    m_sessionFieldName12->setText(tr("Session Audit:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName12, 11, 0, 1, 1);

    m_sessionFieldValue12 = new QLabel();
    m_sessionFieldValue12->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Audit"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue12, 11, 1, 1, 1);

    m_sessionFieldName13 = new QLabel();
    m_sessionFieldName13->setText(tr("Session VT Number:"));
    m_subSessionDetailLayout->addWidget(m_sessionFieldName13, 12, 0, 1, 1);

    m_sessionFieldValue13 = new QLabel();
    m_sessionFieldValue13->setText(m_systemdManagerInterface->getDbusProperty(QStringLiteral("VTNr"), logdSession, session_objectPath).toString());
    m_subSessionDetailLayout->addWidget(m_sessionFieldValue13, 12, 1, 1, 1);

    /* setting horizontal space */
    m_subSessionDetailLayout->setHorizontalSpacing(1);
    /* setting vertical space */
    m_subSessionDetailLayout->setVerticalSpacing(1);
    /* setting margin */
    m_subSessionDetailLayout->setContentsMargins(1, 1, 1, 1);
}

void SessionInformation::createSessionDetailListWidgetView()
{
    QDBusObjectPath session_objectPath;
    session_objectPath.setPath(m_sessionObjectPath);

    QListWidget * m_sessionListWidget = new QListWidget;
    QSize  pitemSize(550,40);

    QString sessionFieldName1 = tr("Session ID:");
    QString sessionFieldValue1 = m_sessionId;
    QListWidgetItem* pItem1 = new QListWidgetItem();
    pItem1->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem1);
    custemListWidgetItem* pCustomItem1 = new custemListWidgetItem(sessionFieldName1, sessionFieldValue1);
    m_sessionListWidget->setItemWidget(pItem1, pCustomItem1);

    QString sessionFieldName2 = tr("Session Name:");
    QString sessionFieldValue2 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Name"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem2 = new QListWidgetItem();
    pItem2->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem2);
    custemListWidgetItem* pCustomItem2 = new custemListWidgetItem(sessionFieldName2, sessionFieldValue2);
    m_sessionListWidget->setItemWidget(pItem2, pCustomItem2);

    QString sessionFieldName3 = tr("Session Objectpath:");
    QString sessionFieldValue3 = m_sessionObjectPath;
    QListWidgetItem* pItem3 = new QListWidgetItem();
    pItem3->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem3);
    custemListWidgetItem* pCustomItem3 = new custemListWidgetItem(sessionFieldName3, sessionFieldValue3);
    m_sessionListWidget->setItemWidget(pItem3, pCustomItem3);

    QString sessionFieldName4 = tr("Session Active:");
    QString sessionFieldValue4 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("State"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem4 = new QListWidgetItem();
    pItem4->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem4);
    custemListWidgetItem* pCustomItem4 = new custemListWidgetItem(sessionFieldName4, sessionFieldValue4);
    m_sessionListWidget->setItemWidget(pItem4, pCustomItem4);

    QString sessionFieldName5 = tr("Session Scope:");
    QString sessionFieldValue5 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Scope"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem5 = new QListWidgetItem();
    pItem5->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem5);
    custemListWidgetItem* pCustomItem5 = new custemListWidgetItem(sessionFieldName5, sessionFieldValue5);
    m_sessionListWidget->setItemWidget(pItem5, pCustomItem5);

    QString sessionFieldName6 = tr("Session Service:");
    QString sessionFieldValue6 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Service"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem6 = new QListWidgetItem();
    pItem6->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem6);
    custemListWidgetItem* pCustomItem6 = new custemListWidgetItem(sessionFieldName6, sessionFieldValue6);
    m_sessionListWidget->setItemWidget(pItem6, pCustomItem6);

    QString sessionFieldName7 = tr("Session TTY:");
    QString sessionFieldValue7 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("TTY"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem7 = new QListWidgetItem();
    pItem7->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem7);
    custemListWidgetItem* pCustomItem7 = new custemListWidgetItem(sessionFieldName7, sessionFieldValue7);
    m_sessionListWidget->setItemWidget(pItem7, pCustomItem7);

    QString sessionFieldName8 = tr("Session Timestamp:");
    QString sessionFieldValue8 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Timestamp"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem8 = new QListWidgetItem();
    pItem8->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem8);
    custemListWidgetItem* pCustomItem8 = new custemListWidgetItem(sessionFieldName8, sessionFieldValue8);
    m_sessionListWidget->setItemWidget(pItem8, pCustomItem8);

    QString sessionFieldName9 = tr("Session Remote Host:");
    QString sessionFieldValue9 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("RemoteHost"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem9 = new QListWidgetItem();
    pItem9->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem9);
    custemListWidgetItem* pCustomItem9 = new custemListWidgetItem(sessionFieldName9, sessionFieldValue9);
    m_sessionListWidget->setItemWidget(pItem9, pCustomItem9);

    QString sessionFieldName10 = tr("Session Type:");
    QString sessionFieldValue10 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Type"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem10 = new QListWidgetItem();
    pItem10->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem10);
    custemListWidgetItem* pCustomItem10 = new custemListWidgetItem(sessionFieldName10, sessionFieldValue10);
    m_sessionListWidget->setItemWidget(pItem10, pCustomItem10);

    QString sessionFieldName11 = tr("Session Leader:");
    QString sessionFieldValue11 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Leader"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem11 = new QListWidgetItem();
    pItem11->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem11);
    custemListWidgetItem* pCustomItem11 = new custemListWidgetItem(sessionFieldName11, sessionFieldValue11);
    m_sessionListWidget->setItemWidget(pItem11, pCustomItem11);

    QString sessionFieldName12 = tr("Session Audit:");
    QString sessionFieldValue12 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Audit"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem12 = new QListWidgetItem();
    pItem12->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem12);
    custemListWidgetItem* pCustomItem12 = new custemListWidgetItem(sessionFieldName12, sessionFieldValue12);
    m_sessionListWidget->setItemWidget(pItem12, pCustomItem12);

    QString sessionFieldName13 = tr("Session VT Number:");
    QString sessionFieldValue13 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("VTNr"), logdSession, session_objectPath).toString();
    QListWidgetItem* pItem13 = new QListWidgetItem();
    pItem13->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem13);
    custemListWidgetItem* pCustomItem13 = new custemListWidgetItem(sessionFieldName13, sessionFieldValue13);
    m_sessionListWidget->setItemWidget(pItem13, pCustomItem13);

    m_subSessionDetailVLayout->addWidget(m_sessionListWidget);
}

// Class UserInformation
UserInformation::UserInformation(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
}

UserInformation::UserInformation(QString userId, QString userObjectPath, QWidget *parent):
    QWidget(parent)
{
    m_userId = userId;
    m_userObjectPath = userObjectPath;
    init();
}

UserInformation::~UserInformation()
{
}

void UserInformation::init()
{
    setWindowTitle(QObject::tr("Hint Model Window"));
    setWindowModality(Qt::ApplicationModal); //setting block
    setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
    //setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Dialog);

    m_systemdManagerInterface=new SystemdManagerInterface;
    qDBusRegisterMetaType<SystemdSession>();
    qDBusRegisterMetaType<SystemdUser>();

    createUserWidgetView();
    //createUserDetailView();
    createUserDetailListWidgetViewData();
    //this->setFixedSize(1200,800);
}

void UserInformation::createUserWidgetView()
{
    m_pUsernWidget = new QWidget(this);
    m_pUsernWidget->setFixedSize(600,390);

    m_userLayout = new QHBoxLayout(this);
    m_pUsernWidget->setLayout(m_userLayout);
    //this->setLayout(m_userLayout);
}

void UserInformation::createUserDetailView()
{
    m_userDetailTableView= new QTableView();
    m_userDetailStandItemModel = new QStandardItemModel();

    m_userDetailStandItemModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Param Name")));
    m_userDetailStandItemModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Param Value")));

    m_userDetailTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_userDetailTableView->verticalHeader()->setDefaultSectionSize(40);
    m_userDetailTableView->horizontalHeader()->hide();
    //m_userDetailTableView->resizeColumnsToContents();
    m_userDetailTableView->horizontalHeader()->setStretchLastSection(true);
    m_userDetailTableView->verticalHeader()->hide();
    m_userDetailTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_userDetailTableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_userDetailTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_userDetailTableView->setShowGrid(false);
    m_userDetailTableView->resizeColumnToContents(0);
    m_userDetailTableView->resizeColumnToContents(1);

    //use delegate to set column
    m_pUserViewItemDelegateProxy =new MyStyledItemDelegate(m_pUsernWidget);
    m_userDetailTableView->setItemDelegate(m_pUserViewItemDelegateProxy);
    //m_pUserViewItemDelegateProxy->setColumnWidth(0,400);

    m_userDetailTableView->setModel(m_userDetailStandItemModel);
    m_userDetailTableView->sortByColumn(1, Qt::AscendingOrder);

    m_userLayout->addWidget(m_userDetailTableView);
    setupUserDetailViewData();

    m_userDetailTableView->resizeColumnToContents(0);
    m_userDetailTableView->resizeColumnToContents(1);
    m_pUserViewItemDelegateProxy->setColumnWidth(0,400);
    m_userDetailTableView->setStyleSheet(usersessionInformationTableViewStyleSheet1);
}

void UserInformation::createUserDetailListWidgetViewData()
{
    QDBusObjectPath user_objectPath;
    user_objectPath.setPath(m_userObjectPath);

    m_userListWidget = new QListWidget;

    QSize  pitemSize(550,40);

    QString userItemName1 = tr("User ID:");
    QString userItemValue1 = m_userId;
    QListWidgetItem* pItem1 = new QListWidgetItem();
    pItem1->setSizeHint(pitemSize);
    m_userListWidget->addItem(pItem1);
    custemListWidgetItem* pCustomItem1 = new custemListWidgetItem(userItemName1, userItemValue1);
    m_userListWidget->setItemWidget(pItem1, pCustomItem1);

    QString userItemName2 = tr("User Name:");
    QString userItemValue2 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Name"), logdUser, user_objectPath).toString();
    QListWidgetItem* pItem2 = new QListWidgetItem();
    pItem2->setSizeHint(pitemSize);
    m_userListWidget->addItem(pItem2);
    custemListWidgetItem* pCustomItem2 = new custemListWidgetItem(userItemName2, userItemValue2);
    m_userListWidget->setItemWidget(pItem2, pCustomItem2);

    QString userItemName3 = tr("User ObjectPath:");
    QString userItemValue3 = m_userObjectPath;
    QListWidgetItem* pItem3 = new QListWidgetItem();
    pItem3->setSizeHint(pitemSize);
    m_userListWidget->addItem(pItem3);
    custemListWidgetItem* pCustomItem3 = new custemListWidgetItem(userItemName3, userItemValue3);
    m_userListWidget->setItemWidget(pItem3, pCustomItem3);

    QString userItemName4 = tr("User Gid:");
    QString userItemValue4 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("GID"), logdUser, user_objectPath).toString();
    QListWidgetItem* pItem4 = new QListWidgetItem();
    pItem4->setSizeHint(pitemSize);
    m_userListWidget->addItem(pItem4);
    custemListWidgetItem* pCustomItem4 = new custemListWidgetItem(userItemName4, userItemValue4);
    m_userListWidget->setItemWidget(pItem4, pCustomItem4);

    QString userItemName5 = tr("User State:");
    QString userItemValue5 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("State"), logdUser, user_objectPath).toString();
    QListWidgetItem* pItem5 = new QListWidgetItem();
    pItem5->setSizeHint(pitemSize);
    m_userListWidget->addItem(pItem5);
    custemListWidgetItem* pCustomItem5 = new custemListWidgetItem(userItemName5, userItemValue5);
    m_userListWidget->setItemWidget(pItem5, pCustomItem5);

    QString userItemName6 = tr("User Service:");
    QString userItemValue6 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Service"), logdUser, user_objectPath).toString();
    QListWidgetItem* pItem6 = new QListWidgetItem();
    pItem6->setSizeHint(pitemSize);
    m_userListWidget->addItem(pItem6);
    custemListWidgetItem* pCustomItem6 = new custemListWidgetItem(userItemName6, userItemValue6);
    m_userListWidget->setItemWidget(pItem6, pCustomItem6);

    QString userItemName7 = tr("User Slice:");
    QString userItemValue7 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Slice"), logdUser, user_objectPath).toString();
    QListWidgetItem* pItem7 = new QListWidgetItem();
    pItem7->setSizeHint(pitemSize);
    m_userListWidget->addItem(pItem7);
    custemListWidgetItem* pCustomItem7 = new custemListWidgetItem(userItemName7, userItemValue7);
    m_userListWidget->setItemWidget(pItem7, pCustomItem7);

    QString userItemName8 = tr("User RuntimePath:");
    QString userItemValue8 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("RuntimePath"), logdUser, user_objectPath).toString();
    QListWidgetItem* pItem8 = new QListWidgetItem();
    pItem8->setSizeHint(pitemSize);
    m_userListWidget->addItem(pItem8);
    custemListWidgetItem* pCustomItem8 = new custemListWidgetItem(userItemName8, userItemValue8);
    m_userListWidget->setItemWidget(pItem8, pCustomItem8);

    QString userItemName9 = tr("User Timestamp:");
    QString userItemValue9 = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Timestamp"), logdUser, user_objectPath).toString();
    QListWidgetItem* pItem9 = new QListWidgetItem();
    pItem9->setSizeHint(pitemSize);
    m_userListWidget->addItem(pItem9);
    custemListWidgetItem* pCustomItem9 = new custemListWidgetItem(userItemName9, userItemValue9);
    m_userListWidget->setItemWidget(pItem9, pCustomItem9);

    m_userLayout->addWidget(m_userListWidget);
}

void UserInformation::setupUserDetailViewData()
{
    QDBusObjectPath user_objectPath;
    user_objectPath.setPath(m_userObjectPath);

    QStandardItem *userItemName1 = new QStandardItem(tr("User ID:"));
    QStandardItem *userItemValue1 = new QStandardItem(m_userId);
    m_userDetailStandItemModel->setItem(0, 0, userItemName1);
    m_userDetailStandItemModel->setItem(0, 1, userItemValue1);

    QStandardItem *userItemName2 = new QStandardItem(tr("User Name:"));
    QStandardItem *userItemValue2 = new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Name"), logdUser, user_objectPath).toString());
    m_userDetailStandItemModel->setItem(1, 0, userItemName2);
    m_userDetailStandItemModel->setItem(1, 1, userItemValue2);

    QStandardItem *userItemName3 = new QStandardItem(tr("User ObjectPath:"));
    QStandardItem *userItemValue3 = new QStandardItem(m_userObjectPath);
    m_userDetailStandItemModel->setItem(2, 0, userItemName3);
    m_userDetailStandItemModel->setItem(2, 1, userItemValue3);

    QStandardItem *userItemName4 = new QStandardItem(tr("User Gid:"));
    QStandardItem *userItemValue4 = new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("GID"), logdUser, user_objectPath).toString());
    m_userDetailStandItemModel->setItem(3, 0, userItemName4);
    m_userDetailStandItemModel->setItem(3, 1, userItemValue4);

    QStandardItem *userItemName5 = new QStandardItem(tr("User State:"));
    QStandardItem *userItemValue5 = new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("State"), logdUser, user_objectPath).toString());
    m_userDetailStandItemModel->setItem(4, 0, userItemName5);
    m_userDetailStandItemModel->setItem(4, 1, userItemValue5);

    QStandardItem *userItemName6 = new QStandardItem(tr("User Service:"));
    QStandardItem *userItemValue6 = new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Service"), logdUser, user_objectPath).toString());
    m_userDetailStandItemModel->setItem(5, 0, userItemName6);
    m_userDetailStandItemModel->setItem(5, 1, userItemValue6);

    QStandardItem *userItemName7 = new QStandardItem(tr("User Slice:"));
    QStandardItem *userItemValue7 = new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Slice"), logdUser, user_objectPath).toString());
    m_userDetailStandItemModel->setItem(6, 0, userItemName7);
    m_userDetailStandItemModel->setItem(6, 1, userItemValue7);

    QStandardItem *userItemName8 = new QStandardItem(tr("User RuntimePath:"));
    QStandardItem *userItemValue8 = new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("RuntimePath"), logdUser, user_objectPath).toString());
    m_userDetailStandItemModel->setItem(7, 0, userItemName8);
    m_userDetailStandItemModel->setItem(7, 1, userItemValue8);

    QStandardItem *userItemName9 = new QStandardItem(tr("User Timestamp:"));
    QStandardItem *userItemValue9 = new QStandardItem(m_systemdManagerInterface->getDbusProperty(QStringLiteral("Timestamp"), logdUser, user_objectPath).toString());
    m_userDetailStandItemModel->setItem(8, 0, userItemName9);
    m_userDetailStandItemModel->setItem(8, 1, userItemValue9);
}

void UserInformation::resizeEvent(QResizeEvent *event)
{
    if (nullptr != m_userDetailTableView)
    {
        m_userDetailTableView->resize(width(), height());
        update();
    }
}
