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

#include "configfileinformation.h"

#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>

ConfigFileInformation::ConfigFileInformation(QWidget *parent) :
    QWidget(parent)
{
    //frameless window
    //setWindowFlags(Qt::FramelessWindowHint);
    init();
}

ConfigFileInformation::ConfigFileInformation(QString configFilePath, QWidget *parent) :
    QWidget(parent)
{
    m_configFile = configFilePath;
    init();
}

ConfigFileInformation::~ConfigFileInformation()
{
}

void ConfigFileInformation::init()
{
    setWindowTitle(QObject::tr("model window"));
    setWindowModality(Qt::ApplicationModal); //set block model
    setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
    //setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Tool);

    createMainWindow();
    createConfigFileView();
    createConfigFileTextEdit();
    //this->setFixedSize(1200,800);
}

void ConfigFileInformation::createMainWindow()
{
    m_mainWindow = new QWidget(this);
    m_mainVLayout = new QVBoxLayout(this);
    //m_mainVLayout->addWidget(m_mainWindow);
}
void ConfigFileInformation::createConfigFileView()
{
    m_configFileLabel = new QLabel;
    m_configFileLabel->setText(tr("config file: %1").arg(m_configFile));
    m_mainVLayout->addWidget(m_configFileLabel);

    m_configFileTextEdit = new QTextEdit(m_mainWindow);
    m_configFileTextEdit->setText(tr("this is unit configure content item"));
    m_configFileTextEdit->setText(tr("unit config file detail"));
    m_configFileTextEdit->setFixedSize(900,700);
    m_configFileTextEdit->setReadOnly(true);
    m_configFileTextEdit->setLineWrapMode(QTextEdit::NoWrap);

    m_mainVLayout->addWidget(m_configFileTextEdit);

    m_btnHLayout = new QHBoxLayout(this);

    m_editBtn = new QPushButton(m_mainWindow);
    m_editBtn->setText(tr("Edit"));
    m_editBtn->setFixedSize(140,40);

    m_resetBtn = new QPushButton(m_mainWindow);
    m_resetBtn->setText(tr("Reset"));
    m_resetBtn->setFixedSize(140,40);
    m_resetBtn->setEnabled(false);

    m_applyBtn = new QPushButton(m_mainWindow);
    m_applyBtn->setText(tr("Apply"));
    m_applyBtn->setFixedSize(140,40);

    m_cancelBtn = new QPushButton(m_mainWindow);
    m_cancelBtn->setText(tr("Cancel"));
    m_cancelBtn->setFixedSize(140,40);

    m_btnHLayout->addWidget(m_editBtn);
    m_btnHLayout->addWidget(m_resetBtn);
    m_btnHLayout->addStretch(0);
    m_btnHLayout->addWidget(m_applyBtn);
    m_btnHLayout->addWidget(m_cancelBtn);

    m_mainVLayout->addLayout(m_btnHLayout);

    this->setLayout(m_mainVLayout);

    connect(m_editBtn, &QPushButton::clicked, this, &ConfigFileInformation::handleEditBtnClick);
    connect(m_resetBtn, &QPushButton::clicked, this, &ConfigFileInformation::handleResetBtnClick);
    connect(m_applyBtn, &QPushButton::clicked, this, &ConfigFileInformation::handleApplyBtnClick);
    connect(m_cancelBtn, &QPushButton::clicked, this, &ConfigFileInformation::handleCancelBtnClick);
}

void ConfigFileInformation::createConfigFileTextEdit()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(14);

    m_configFileTextEdit->setFont(font);

    highlighter = new Highlighter(m_configFileTextEdit->document());

    if (!m_configFile.isEmpty())
    {
        QFile file(m_configFile);
        if (file.open(QFile::ReadOnly | QFile::Text)){
            m_contentByteArray = file.readAll();
            m_configFileTextEdit->setPlainText(m_contentByteArray);
        }
        file.close();
    }
}

void ConfigFileInformation::handleEditBtnClick()
{
    m_configFileTextEdit->setReadOnly(false);
    m_resetBtn->setEnabled(true);

}

void ConfigFileInformation::handleResetBtnClick()
{
    m_resetBtn->setEnabled(true);
    m_configFileTextEdit->setPlainText(m_contentByteArray);
}

void ConfigFileInformation::handleApplyBtnClick()
{
    if (!m_configFile.isEmpty())
    {
        QFile file(m_configFile);
        if (! file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::critical(this, tr("Error"), tr("Can not save file"));
            return;
        }
        QTextStream out(&file);
        out << m_configFileTextEdit->toPlainText();
        file.close();
    }
}

void ConfigFileInformation::handleCancelBtnClick()
{
    this->close();
}
