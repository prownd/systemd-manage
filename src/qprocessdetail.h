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

#ifndef QPROCESSDETAIL_H
#define QPROCESSDETAIL_H

#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include "qprocessinfo.h"

class QProcessDetail : public QWidget
{
    Q_OBJECT

public:
    explicit QProcessDetail(QWidget *parent = nullptr);
    explicit QProcessDetail(quint64 pid = 1, QWidget *parent = nullptr);
    ~QProcessDetail();
    void init();
    void createQProcessWidgetView();
    void createQProcessDetailListWidgetViewData();

private:
    quint64 m_pid;

    QHBoxLayout *  m_processDetaiLayout;
    QWidget * m_processDetailWidget;

    QListWidget * m_processDetailListWidget;

    QLabel * m_processDetaiFieldName1;
    QLabel * m_processDetaiFieldValue1;
    QLabel * m_processDetaiFieldName2;
    QLabel * m_processDetaiFieldValue2;

protected:
    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // QPROCESSDETAIL_H
