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

#ifndef CONFIGFILEINFORMATION_H
#define CONFIGFILEINFORMATION_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLabel>
#include "systemdmanagerinterface.h"
#include "highlighter.h"

class ConfigFileInformation : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigFileInformation(QWidget *parent = nullptr);
    explicit ConfigFileInformation(QString configFilePath = "", QWidget *parent = nullptr);
    ~ConfigFileInformation();
    void init();
    void createMainWindow();
    void createConfigFileView();
    void createConfigFileTextEdit();

private:
    QWidget * m_mainWindow ;
    QVBoxLayout * m_mainVLayout ;
    QHBoxLayout * m_mainHLayout ;

    SystemdManagerInterface * m_systemdManagerInterface;
    QVector<conffile> m_confFileList;

    QTextEdit *   m_configFileTextEdit;
    //QPlainTextEdit * m_configFileTextEdit;
    Highlighter * highlighter;
    QString m_configFile;

    QHBoxLayout * m_btnHLayout;
    QPushButton * m_editBtn;
    QPushButton * m_resetBtn;
    QPushButton * m_applyBtn;
    QPushButton * m_cancelBtn;
    QLabel * m_configFileLabel;

    QByteArray m_contentByteArray;

public Q_SLOTS:
    void handleEditBtnClick();
    void handleResetBtnClick();
    void handleApplyBtnClick();
    void handleCancelBtnClick();

};

#endif // CONFIGFILEINFORMATION_H
