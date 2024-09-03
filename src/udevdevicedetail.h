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

#ifndef UDEVDEVICEDETAIL_H
#define UDEVDEVICEDETAIL_H

#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include "qudev.h"

class UdevDeviceDetail : public QWidget
{
    Q_OBJECT

public:
    explicit UdevDeviceDetail(QWidget *parent = nullptr);
    explicit UdevDeviceDetail(UdevDevice udevDevice, QWidget *parent = nullptr);
    ~UdevDeviceDetail();
    void init();
    void createQProcessWidgetView();
    void createQProcessDetailListWidgetViewData();

private:
    UdevDevice m_udevDevice;
    QHBoxLayout *  m_udevDeviceDetaiLayout;
    QWidget * m_udevDeviceDetailWidget;

    QListWidget * m_udevDeviceDetailListWidget;

    QLabel * m_udevDeviceDetaiFieldName1;
    QLabel * m_udevDeviceDetaiFieldValue1;

protected:
    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // UDEVDEVICEDETAIL_H
