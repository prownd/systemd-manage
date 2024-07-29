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

#ifndef CONFIGFILEWINDOW_H
#define CONFIGFILEWINDOW_H

#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QWidget>
#include <QHBoxLayout>
#include "systemdmanagerinterface.h"

class ConfigFileWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigFileWindow(QWidget *parent = nullptr);
    ~ConfigFileWindow();
    void createConfigFileTableView();
    void slotRefreshConfFileList();

private:
    SystemdManagerInterface * m_systemdManagerInterface;

    QStandardItemModel *m_confFileModel;
    QTableView * m_configFileTableView;
    QVector<conffile> m_confFileList;

    QHBoxLayout * m_hBoxLayout ;

protected Q_SLOTS:
    void handleConfigFileStatusAction();
    void handleConfigFileRefreshAction();

    void configFileCustomMenuRequested(QPoint pos);

protected:
    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // CONFIGFILEWINDOW_H
