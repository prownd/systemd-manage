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

#include "settingswindow.h"
#include "configfile.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QTranslator>
#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include <QListView>


QString settingWindowtPushButtonLabelStyleSheet1(R"(
/* QPushButton */
QPushButton {
    border-top: 0px outset transparent;
    border-bottom: 0px outset transparent;
    border-right: 1px outset transparent;
    border-left: 6px outset transparent;
    border-radius:6px;
    background-color: #dedede;
    width: 190px;
    height: 40px;
    margin: 0px 0px;
}

QPushButton:checked {
    background-color: #00ff7e;
}

/* press and checked */
QPushButton:pressed:checked {
    border-left: 0px outset rgb(93, 95, 97);
    background-color: #00ff7e;
}

QPushButton:hover:!checked{
    background-color: #c7c7c7;
}

QLabel {
    font: 14pt;
    margin: 2px 2px 1px 1px;
}

)");

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent)
{
    //frameless window
    setWindowFlags(Qt::FramelessWindowHint);
    m_configSettings = new ConfigSetting();
    CreateSettingView();
}

SettingsWindow::~SettingsWindow()
{
}

void SettingsWindow::CreateSettingView()
{
    m_mainVLayout = new  QVBoxLayout;
    m_subLangHLayout = new  QHBoxLayout();

    m_langLabel = new QLabel();
    m_langLabel->setText(tr("Language:"));
    m_langLabel->setStyleSheet(settingWindowtPushButtonLabelStyleSheet1);

    m_languageList = QStringList{tr("zh_CN"), tr("en_US"),
                                 tr("de_DE"), tr("ru_RU"),
                                 tr("fr_FR"), tr("ja_JP"),
                                 tr("zh_TW")};

    m_languageListComboBox = new QComboBox();
    m_languageListComboBox->addItems(m_languageList);
    m_languageListComboBox->setCurrentIndex(0);

    m_languageListComboBox->setView(new  QListView());

    QString lang = m_configSettings->getValue("Language").toString();
    if(!lang.isEmpty()){
        m_languageListComboBox->setCurrentText(lang);
    }

    m_confirmLangBtn = new QPushButton();
    m_confirmLangBtn->setText(tr("Save Setting"));
    m_confirmLangBtn->setStyleSheet(settingWindowtPushButtonLabelStyleSheet1);

    m_subLangHLayout->addWidget(m_langLabel);
    m_subLangHLayout->addWidget(m_languageListComboBox);
    m_subLangHLayout->addWidget(m_confirmLangBtn);
    m_subLangHLayout->addStretch(0);
    m_subLangHLayout->setContentsMargins(50,0,0,0);

    m_mainVLayout->addLayout(m_subLangHLayout);
    m_mainVLayout->addStretch(0);

    this->setLayout(m_mainVLayout);

    QObject::connect(m_confirmLangBtn,&QPushButton::clicked,this,&SettingsWindow::confirmSelectLanguage);
}

void SettingsWindow::confirmSelectLanguage()
{
    //int cur_index = m_languageListComboBox->currentIndex();
    QString cur_indexText = m_languageListComboBox->currentText();
    m_configSettings->setValue("Language", cur_indexText);

    // QMessageBox::StandardButton box;
    // box = QMessageBox::information(this, tr("Language change require restart"), tr("The language change will take effect after restart?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    // if(box==QMessageBox::No) {
    //     return;
    // }else if(box==QMessageBox::Yes) {
    //     qDebug()<<"restart it after language change";
    //     qApp->quit();
    //     QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    // }

    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information, tr("Language change require restart"), tr("The language change will take effect after restart?"), QMessageBox::Yes | QMessageBox::No);
    msgBox->button(QMessageBox::Yes)->setText(tr("Yes"));
    msgBox->button(QMessageBox::No)->setText(tr("No"));
    msgBox->setDefaultButton(QMessageBox::No);
    int res = msgBox->exec();
    if(QMessageBox::Yes == res){
        //qDebug()<<"restart it after language change";
        qApp->quit();
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    }else if(QMessageBox::No == res){
        return;
    }
}

void SettingsWindow::resizeEvent(QResizeEvent *event){
    resize(width(), height());
    update();
}
