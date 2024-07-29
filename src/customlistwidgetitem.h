#ifndef CUSTOMLISTWIDGETITEM_H
#define CUSTOMLISTWIDGETITEM_H

#include <QWidget>
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

#include <QListWidgetItem>
#include <QStringList>
#include <QList>
#include <QIcon>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class custemListWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit custemListWidgetItem(const QString text1 = "",  const QString text2 = "",  QWidget *parent=nullptr);
    ~custemListWidgetItem();

private slots:
    void openURL(QString url);

private:
    QHBoxLayout * m_mainHLayout;
    QLabel * m_leftLabel1;
    QLabel * m_rightLabel2;
};

#endif // CUSTOMLISTWIDGETITEM_H
