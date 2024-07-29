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

#include "aboutwindow.h"
#include "version.h"
#include "customlistwidgetitem.h"
#include <QLabel>

AboutWindow::AboutWindow(QWidget *parent) :
    QWidget(parent)
{
    //frameless window
    setWindowFlags(Qt::FramelessWindowHint);
    CreateView();
}

AboutWindow::~AboutWindow()
{
}

void AboutWindow::CreateView()
{
    m_mainVBoxLayout = new QVBoxLayout;

    QListWidget * m_sessionListWidget = new QListWidget;
    QSize  pitemSize(600,50);

    QString sessionFieldName1 = tr("Application:");
    QString sessionFieldValue1 = APPLICATION_NAME;
    QListWidgetItem* pItem1 = new QListWidgetItem();
    pItem1->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem1);
    custemListWidgetItem* pCustomItem1 = new custemListWidgetItem(sessionFieldName1, sessionFieldValue1);
    m_sessionListWidget->setItemWidget(pItem1, pCustomItem1);

    QString sessionFieldName2 = tr("Version:");
    QString sessionFieldValue2 = APPLICATION_VERSION;
    QListWidgetItem* pItem2 = new QListWidgetItem();
    pItem2->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem2);
    custemListWidgetItem* pCustomItem2 = new custemListWidgetItem(sessionFieldName2, sessionFieldValue2);
    m_sessionListWidget->setItemWidget(pItem2, pCustomItem2);

    QString sessionFieldName3 = tr("Author:");
    QString sessionFieldValue3 = AUTHOR;
    QListWidgetItem* pItem3 = new QListWidgetItem();
    pItem3->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem3);
    custemListWidgetItem* pCustomItem3 = new custemListWidgetItem(sessionFieldName3, sessionFieldValue3);
    m_sessionListWidget->setItemWidget(pItem3, pCustomItem3);

    QString sessionFieldName4 = tr("Email:");
    QString sessionFieldValue4 = EMAIL;
    QListWidgetItem* pItem4 = new QListWidgetItem();
    pItem4->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem4);
    custemListWidgetItem* pCustomItem4 = new custemListWidgetItem(sessionFieldName4, sessionFieldValue4);
    m_sessionListWidget->setItemWidget(pItem4, pCustomItem4);

    QString sessionFieldName5 = tr("License:");
    QString sessionFieldValue5 = LICENSE;
    QListWidgetItem* pItem5 = new QListWidgetItem();
    pItem5->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem5);
    custemListWidgetItem* pCustomItem5 = new custemListWidgetItem(sessionFieldName5, sessionFieldValue5);
    m_sessionListWidget->setItemWidget(pItem5, pCustomItem5);

    QString sessionFieldName6 = tr("Website:");
    QString sessionFieldValue6 = WEBSITE;
    QListWidgetItem* pItem6 = new QListWidgetItem();
    pItem6->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem6);
    custemListWidgetItem* pCustomItem6 = new custemListWidgetItem(sessionFieldName6, sessionFieldValue6);
    m_sessionListWidget->setItemWidget(pItem6, pCustomItem6);

    QString sessionFieldName7 = tr("Credits:");
    QString sessionFieldValue7 = CREDITS.join(", ");
    QListWidgetItem* pItem7 = new QListWidgetItem();
    pItem7->setSizeHint(pitemSize);
    m_sessionListWidget->addItem(pItem7);
    custemListWidgetItem* pCustomItem7 = new custemListWidgetItem(sessionFieldName7, sessionFieldValue7);
    m_sessionListWidget->setItemWidget(pItem7, pCustomItem7);

    QSize  legalItemSize(600,80);
    QString sessionFieldName8 = tr("Legal:");
    QString sessionFieldValue8 = LEGAL;
    QListWidgetItem* pItem8 = new QListWidgetItem();
    pItem8->setSizeHint(legalItemSize);
    m_sessionListWidget->addItem(pItem8);
    custemListWidgetItem* pCustomItem8 = new custemListWidgetItem(sessionFieldName8, sessionFieldValue8);
    m_sessionListWidget->setItemWidget(pItem8, pCustomItem8);

    m_mainVBoxLayout->addWidget(m_sessionListWidget);
    this->setLayout(m_mainVBoxLayout);
}

void AboutWindow::resizeEvent(QResizeEvent *event){
    resize(width(), height());
    update();
}
