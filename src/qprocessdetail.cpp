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

#include "qprocessdetail.h"

#include "customlistwidgetitem.h"

#include <QListWidget>
#include <QStringLiteral>
#include <QTextEdit>

// Class QProcessDetail
QProcessDetail::QProcessDetail(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
}

QProcessDetail::QProcessDetail(quint64 pid, QWidget *parent):
    QWidget(parent)
{
    m_pid = pid;
    init();
}

QProcessDetail::~QProcessDetail()
{
}

void QProcessDetail::init()
{
    setWindowTitle(QObject::tr("Hint Model Window"));
    setWindowModality(Qt::ApplicationModal); //setting block
    setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
    //setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Dialog);

    createQProcessWidgetView();
    createQProcessDetailListWidgetViewData();
}

void QProcessDetail::createQProcessWidgetView()
{
    m_processDetailWidget = new QWidget(this);
    m_processDetailWidget->setFixedSize(600,500);

    m_processDetaiLayout = new QHBoxLayout(this);
    m_processDetailWidget->setLayout(m_processDetaiLayout);
    //this->setLayout(m_userLayout);
}

void QProcessDetail::createQProcessDetailListWidgetViewData()
{   m_processDetailListWidget = new QListWidget;
    QProcessInfo * showProcessInfo = NULL;

    QProcessList list = QProcessInfo::getProcInfoList();
    for (const QProcessInfo& info : list) {
        if (m_pid == info.getPid())
        {
            showProcessInfo = (QProcessInfo *)&info;
            break;
        }
    }
    if (showProcessInfo != NULL){
        QSize  pitemSize(550,40);

        QString processItemName1 = tr("Process Name:");
        QString processItemValue1 = showProcessInfo->getName();
        QListWidgetItem* pItem1 = new QListWidgetItem();
        pItem1->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem1);
        custemListWidgetItem* pCustomItem1 = new custemListWidgetItem(processItemName1, processItemValue1);
        m_processDetailListWidget->setItemWidget(pItem1, pCustomItem1);

        QString processItemName2 = tr("Process ID:");
        QString processItemValue2 = QString::number(m_pid);
        QListWidgetItem* pItem2 = new QListWidgetItem();
        pItem2->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem2);
        custemListWidgetItem* pCustomItem2 = new custemListWidgetItem(processItemName2, processItemValue2);
        m_processDetailListWidget->setItemWidget(pItem2, pCustomItem2);

        QString processItemName3 = tr("User:");
        QString processItemValue3 = showProcessInfo->getUsername();;
        QListWidgetItem* pItem3 = new QListWidgetItem();
        pItem3->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem3);
        custemListWidgetItem* pCustomItem3 = new custemListWidgetItem(processItemName3, processItemValue3);
        m_processDetailListWidget->setItemWidget(pItem3, pCustomItem3);

        QString processItemName4 = tr("Status:");
        QString processItemValue4 = showProcessInfo->getState();;
        QListWidgetItem* pItem4 = new QListWidgetItem();
        pItem4->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem4);
        custemListWidgetItem* pCustomItem4 = new custemListWidgetItem(processItemName4, processItemValue4);
        m_processDetailListWidget->setItemWidget(pItem4, pCustomItem4);

        QString processItemName5 = tr("Memory:");
        QString processItemValue5 = QString::number(showProcessInfo->getMem());
        QListWidgetItem* pItem5 = new QListWidgetItem();
        pItem5->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem5);
        custemListWidgetItem* pCustomItem5 = new custemListWidgetItem(processItemName5, processItemValue5);
        m_processDetailListWidget->setItemWidget(pItem5, pCustomItem5);

        QString processItemName6 = tr("Virtual Memory:");
        QString processItemValue6 = QString::number(showProcessInfo->getVmsize());
        QListWidgetItem* pItem6 = new QListWidgetItem();
        pItem6->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem6);
        custemListWidgetItem* pCustomItem6 = new custemListWidgetItem(processItemName6, processItemValue6);
        m_processDetailListWidget->setItemWidget(pItem6, pCustomItem6);

        QString processItemName7 = tr("Resident Memory:");
        QString processItemValue7 = QString::number(showProcessInfo->getMemres());
        QListWidgetItem* pItem7 = new QListWidgetItem();
        pItem7->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem7);
        custemListWidgetItem* pCustomItem7 = new custemListWidgetItem(processItemName7, processItemValue7);
        m_processDetailListWidget->setItemWidget(pItem7, pCustomItem7);

        QString processItemName8 = tr("Writable Memory:");
        QString processItemValue8 = QString::number(showProcessInfo->getMemwritable());
        QListWidgetItem* pItem8 = new QListWidgetItem();
        pItem8->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem8);
        custemListWidgetItem* pCustomItem8 = new custemListWidgetItem(processItemName8, processItemValue8);
        m_processDetailListWidget->setItemWidget(pItem8, pCustomItem8);

        QString processItemName9 = tr("Shared Memory:");
        QString processItemValue9 = QString::number(showProcessInfo->getMemshared());
        QListWidgetItem* pItem9 = new QListWidgetItem();
        pItem9->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem9);
        custemListWidgetItem* pCustomItem9 = new custemListWidgetItem(processItemName9, processItemValue9);
        m_processDetailListWidget->setItemWidget(pItem9, pCustomItem9);

        QString processItemName10 = tr("CPU:");
        QString processItemValue10 = showProcessInfo->getCPUTime();;
        QListWidgetItem* pItem10 = new QListWidgetItem();
        pItem10->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem10);
        custemListWidgetItem* pCustomItem10 = new custemListWidgetItem(processItemName10, processItemValue10);
        m_processDetailListWidget->setItemWidget(pItem10, pCustomItem10);

        QString processItemName11 = tr("Priority:");
        QString processItemValue11 = showProcessInfo->getNiceLevel(showProcessInfo->getNice());
        QListWidgetItem* pItem11 = new QListWidgetItem();
        pItem11->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem11);
        custemListWidgetItem* pCustomItem11 = new custemListWidgetItem(processItemName11, processItemValue11);
        m_processDetailListWidget->setItemWidget(pItem11, pCustomItem11);

        QString processItemName12 = tr("Command Line:");
        QString processItemValue12 = showProcessInfo->getCommandLine();;
        QListWidgetItem* pItem12 = new QListWidgetItem();
        pItem12->setSizeHint(pitemSize);
        m_processDetailListWidget->addItem(pItem12);
        custemListWidgetItem* pCustomItem12 = new custemListWidgetItem(processItemName12, processItemValue12);
        m_processDetailListWidget->setItemWidget(pItem12, pCustomItem12);
    }
    m_processDetaiLayout->addWidget(m_processDetailListWidget);

}

void QProcessDetail::resizeEvent(QResizeEvent *event)
{
    if (nullptr != m_processDetailWidget)
    {
        update();
    }
}
