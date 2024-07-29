#include <QWidget>
#include <QListWidgetItem>
#include <QCheckBox>
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

#include <QUrl>
#include <QDesktopServices>
#include "customlistwidgetitem.h"

custemListWidgetItem::custemListWidgetItem(const QString text1, const QString text2, QWidget *parent) :
    QWidget(parent)
{
    m_mainHLayout = new QHBoxLayout;
    m_leftLabel1 = new QLabel;
    m_rightLabel2 = new QLabel;

    m_leftLabel1->adjustSize();
    m_rightLabel2->adjustSize();
    //m_rightLabel2->setWordWrap(true);

    m_leftLabel1->setText(text1);
    m_rightLabel2->setText(text2);


    m_mainHLayout->addWidget(m_leftLabel1);
    m_mainHLayout->addStretch(0);
    m_mainHLayout->addWidget(m_rightLabel2);

    connect(m_rightLabel2, SIGNAL(linkActivated(QString)), this, SLOT(openURL(QString)));

    //this->setFixedSize(400,40);
    this->setLayout(m_mainHLayout);
}

custemListWidgetItem::~custemListWidgetItem()
{
}

void custemListWidgetItem::openURL(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}

