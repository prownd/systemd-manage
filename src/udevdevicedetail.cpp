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

#include "udevdevicedetail.h""

#include "customlistwidgetitem.h"

#include <QListWidget>
#include <QStringLiteral>
#include <QTextEdit>

UdevDeviceDetail::UdevDeviceDetail(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
}

UdevDeviceDetail::UdevDeviceDetail(UdevDevice udevDevice, QWidget *parent):
    QWidget(parent)
{
    m_udevDevice = udevDevice;
    init();
}

UdevDeviceDetail::~UdevDeviceDetail()
{
}

void UdevDeviceDetail::init()
{
    setWindowTitle(QObject::tr("Hint Model Window"));
    setWindowModality(Qt::ApplicationModal); //setting block
    setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
    //setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Dialog);

    createQProcessWidgetView();
    createQProcessDetailListWidgetViewData();
}

void UdevDeviceDetail::createQProcessWidgetView()
{
    m_udevDeviceDetailWidget = new QWidget(this);
    m_udevDeviceDetailWidget->setFixedSize(800,240);

    m_udevDeviceDetaiLayout = new QHBoxLayout(this);
    m_udevDeviceDetailWidget->setLayout(m_udevDeviceDetaiLayout);
}

void UdevDeviceDetail::createQProcessDetailListWidgetViewData()
{   m_udevDeviceDetailListWidget = new QListWidget;

    if (m_udevDevice.devicePath() != ""){
        QSize  pitemSize(750,40);

        QString processItemName1 = tr("Udev Device Path:");
        QString processItemValue1 = m_udevDevice.devicePath();
        QListWidgetItem* pItem1 = new QListWidgetItem();
        pItem1->setSizeHint(pitemSize);
        m_udevDeviceDetailListWidget->addItem(pItem1);
        custemListWidgetItem* pCustomItem1 = new custemListWidgetItem(processItemName1, processItemValue1);
        m_udevDeviceDetailListWidget->setItemWidget(pItem1, pCustomItem1);

        QString processItemName2 = tr("Udev Device Subsystem:");
        QString processItemValue2 = m_udevDevice.deviceSubsystem();
        QListWidgetItem* pItem2 = new QListWidgetItem();
        pItem2->setSizeHint(pitemSize);
        m_udevDeviceDetailListWidget->addItem(pItem2);
        custemListWidgetItem* pCustomItem2 = new custemListWidgetItem(processItemName2, processItemValue2);
        m_udevDeviceDetailListWidget->setItemWidget(pItem2, pCustomItem2);

        QString processItemName3 = tr("Udev Device Type:");
        QString processItemValue3 = m_udevDevice.deviceType();
        QListWidgetItem* pItem3 = new QListWidgetItem();
        pItem3->setSizeHint(pitemSize);
        m_udevDeviceDetailListWidget->addItem(pItem3);
        custemListWidgetItem* pCustomItem3 = new custemListWidgetItem(processItemName3, processItemValue3);
        m_udevDeviceDetailListWidget->setItemWidget(pItem3, pCustomItem3);

        QString processItemName4 = tr("Udev Device Node:");
        QString processItemValue4 = m_udevDevice.deviceNode();
        QListWidgetItem* pItem4 = new QListWidgetItem();
        pItem4->setSizeHint(pitemSize);
        m_udevDeviceDetailListWidget->addItem(pItem4);
        custemListWidgetItem* pCustomItem4 = new custemListWidgetItem(processItemName4, processItemValue4);
        m_udevDeviceDetailListWidget->setItemWidget(pItem4, pCustomItem4);

        QString processItemName5 = tr("Udev Device Action:");
        QString processItemValue5 = m_udevDevice.deviceAction();
        QListWidgetItem* pItem5 = new QListWidgetItem();
        pItem5->setSizeHint(pitemSize);
        m_udevDeviceDetailListWidget->addItem(pItem5);
        custemListWidgetItem* pCustomItem5 = new custemListWidgetItem(processItemName5, processItemValue5);
        m_udevDeviceDetailListWidget->setItemWidget(pItem5, pCustomItem5);
    }
    m_udevDeviceDetaiLayout->addWidget(m_udevDeviceDetailListWidget);

}

void UdevDeviceDetail::resizeEvent(QResizeEvent *event)
{
    if (nullptr != m_udevDeviceDetailListWidget)
    {
        update();
    }
}
