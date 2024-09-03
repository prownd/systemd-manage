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

#include "udevrulefileinformation.h"

#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextStream>

UdevRuleFileInformation::UdevRuleFileInformation(QWidget *parent) :
    QWidget(parent)
{
    //frameless window
    //setWindowFlags(Qt::FramelessWindowHint);
    init();
}

UdevRuleFileInformation::UdevRuleFileInformation(QString udevRuleFilePath, QWidget *parent) :
    QWidget(parent)
{
    m_udevRuleFile = udevRuleFilePath;
    init();
}

UdevRuleFileInformation::~UdevRuleFileInformation()
{
}

void UdevRuleFileInformation::init()
{
    setWindowTitle(QObject::tr("model window"));
    setWindowModality(Qt::ApplicationModal); //set block model
    setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
    createMainWindow();
    createUdevRuleFileView();
    createUdevRuleFileTextEdit();
    //this->setFixedSize(1200,800);
}

void UdevRuleFileInformation::createMainWindow()
{
    m_mainWindow = new QWidget(this);
    m_mainVLayout = new QVBoxLayout(this);
}
void UdevRuleFileInformation::createUdevRuleFileView()
{
    m_udevRuleFileLabel = new QLabel;
    m_udevRuleFileLabel->setText(tr("config file: %1").arg(m_udevRuleFile));
    m_mainVLayout->addWidget(m_udevRuleFileLabel);

    m_udevRuleFileTextEdit = new QTextEdit(m_mainWindow);
    m_udevRuleFileTextEdit->setText(tr("this is unit configure content item"));
    m_udevRuleFileTextEdit->setText(tr("unit config file detail"));
    m_udevRuleFileTextEdit->setFixedSize(900,700);
    m_udevRuleFileTextEdit->setReadOnly(true);
    m_udevRuleFileTextEdit->setLineWrapMode(QTextEdit::NoWrap);

    m_mainVLayout->addWidget(m_udevRuleFileTextEdit);

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

    connect(m_editBtn, &QPushButton::clicked, this, &UdevRuleFileInformation::handleEditBtnClick);
    connect(m_resetBtn, &QPushButton::clicked, this, &UdevRuleFileInformation::handleResetBtnClick);
    connect(m_applyBtn, &QPushButton::clicked, this, &UdevRuleFileInformation::handleApplyBtnClick);
    connect(m_cancelBtn, &QPushButton::clicked, this, &UdevRuleFileInformation::handleCancelBtnClick);
}

void UdevRuleFileInformation::createUdevRuleFileTextEdit()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(14);

    m_udevRuleFileTextEdit->setFont(font);

    highlighter = new Highlighter(m_udevRuleFileTextEdit->document());

    if (!m_udevRuleFile.isEmpty())
    {
        QFile file(m_udevRuleFile);
        if (file.open(QFile::ReadOnly | QFile::Text)){
            m_contentByteArray = file.readAll();
            m_udevRuleFileTextEdit->setPlainText(m_contentByteArray);
        }
        file.close();
    }
}

void UdevRuleFileInformation::handleEditBtnClick()
{
    m_udevRuleFileTextEdit->setReadOnly(false);
    m_resetBtn->setEnabled(true);

}

void UdevRuleFileInformation::handleResetBtnClick()
{
    m_resetBtn->setEnabled(true);
    m_udevRuleFileTextEdit->setPlainText(m_contentByteArray);
}

void UdevRuleFileInformation::handleApplyBtnClick()
{
    if (!m_udevRuleFile.isEmpty())
    {
        QFile file(m_udevRuleFile);
        if (! file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::critical(this, tr("Error"), tr("Can not save file"));
            return;
        }
        QTextStream out(&file);
        out << m_udevRuleFileTextEdit->toPlainText();
        file.close();
    }
}

void UdevRuleFileInformation::handleCancelBtnClick()
{
    this->close();
}
