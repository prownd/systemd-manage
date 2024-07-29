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

#include "journalwindow.h"
#include "qpushbutton.h"
#include "systemdjournalinterface.h"

#include <QCheckBox>
#include <QDateTimeEdit>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

JournalWindow::JournalWindow(QWidget *parent) :
    QWidget(parent)
{
    //frameless window
    setWindowFlags(Qt::FramelessWindowHint);

    createJournalViewWindow();
}

JournalWindow::~JournalWindow()
{
}

void JournalWindow::createJournalViewWindow()
{
    m_mainVBoxLayout = new QVBoxLayout(this);

    m_datetimeLabel = new QLabel;
    m_datetimeLabel->setText(tr("Date:"));
    m_datetimeLabel->setFixedWidth(120);

    m_dateTimeHBoxLayout = new QHBoxLayout;
    m_startDatetimeCheckBox = new QCheckBox;
    //m_startDatetimeCheckBox->setCheckState(Qt::Checked);
    m_startDatetimeCheckBox->setCheckState(Qt::Unchecked);
    m_startDatetimeCheckBox->setCheckable(true);
    m_startDatetimeCheckBox->setText(tr("Start Time"));
    m_endDatetimeCheckBox = new QCheckBox;
    //m_endDatetimeCheckBox->setCheckState(Qt::Checked);
    m_endDatetimeCheckBox->setCheckState(Qt::Unchecked);
    m_endDatetimeCheckBox->setCheckable(true);
    m_endDatetimeCheckBox->setText(tr("End Time"));

    m_startDatetimeDTEdit = new QDateTimeEdit;
    m_endDatetimeDTEdit = new QDateTimeEdit;
    // QDateTimeEdit default values
    m_startDatetimeDTEdit->setDateTime(QDateTime::currentDateTime().addDays(-1));
    m_startDatetimeDTEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    m_endDatetimeDTEdit->setDateTime(QDateTime::currentDateTime());
    m_endDatetimeDTEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    m_dateTimeHBoxLayout->addWidget(m_datetimeLabel);
    m_dateTimeHBoxLayout->addWidget(m_startDatetimeCheckBox);
    m_dateTimeHBoxLayout->addWidget(m_startDatetimeDTEdit);
    m_dateTimeHBoxLayout->addStretch(0);
    m_dateTimeHBoxLayout->addWidget(m_endDatetimeCheckBox);
    m_dateTimeHBoxLayout->addWidget(m_endDatetimeDTEdit);
    m_dateTimeHBoxLayout->addStretch(0);

    m_mainVBoxLayout->addLayout(m_dateTimeHBoxLayout);

    m_logTypeLabel = new QLabel;
    m_logTypeLabel->setText(tr("Log Type:"));
    m_logTypeLabel->setFixedWidth(120);

    m_logEmergPushButton = new QPushButton;
    m_logEmergPushButton->setText(tr("Emerg"));
    m_logEmergPushButton->setCheckable(true);
    m_logEmergPushButton->setChecked(false);

    m_logAlertPushButton = new QPushButton;
    m_logAlertPushButton->setText(tr("Alert"));
    m_logAlertPushButton->setCheckable(true);
    m_logAlertPushButton->setChecked(false);

    m_logCriticalPushButton = new QPushButton;
    m_logCriticalPushButton->setText(tr("Critical"));
    m_logCriticalPushButton->setCheckable(true);
    m_logCriticalPushButton->setChecked(false);

    m_logErrorPushButton = new QPushButton;
    m_logErrorPushButton->setText(tr("Error"));
    m_logErrorPushButton->setCheckable(true);
    m_logErrorPushButton->setChecked(true);

    m_logWarningPushButton = new QPushButton;
    m_logWarningPushButton->setText(tr("Warning"));
    m_logWarningPushButton->setCheckable(true);
    m_logWarningPushButton->setChecked(false);

    m_logNoticePushButton = new QPushButton;
    m_logNoticePushButton->setText(tr("Notice"));
    m_logNoticePushButton->setCheckable(true);
    m_logNoticePushButton->setChecked(false);

    m_logInfoPushButton = new QPushButton;
    m_logInfoPushButton->setText(tr("Info"));
    m_logInfoPushButton->setCheckable(true);
    m_logInfoPushButton->setChecked(false);

    m_logDebugPushButton = new QPushButton;
    m_logDebugPushButton->setText(tr("Debug"));
    m_logDebugPushButton->setCheckable(true);
    m_logDebugPushButton->setChecked(false);

    m_logButtonGroup = new QButtonGroup(this);
    m_logButtonGroup->addButton(m_logEmergPushButton, LOGLEVEL_EMERG);
    m_logButtonGroup->addButton(m_logAlertPushButton, LOGLEVEL_ALERT);
    m_logButtonGroup->addButton(m_logCriticalPushButton, LOGLEVEL_CRIT);
    m_logButtonGroup->addButton(m_logErrorPushButton, LOGLEVEL_ERR);
    m_logButtonGroup->addButton(m_logWarningPushButton, LOGLEVEL_WARNING);
    m_logButtonGroup->addButton(m_logNoticePushButton, LOGLEVEL_NOTICE);
    m_logButtonGroup->addButton(m_logInfoPushButton, LOGLEVEL_INFO);
    m_logButtonGroup->addButton(m_logDebugPushButton, LOGLEVEL_DEBUG);
    m_logButtonGroup->setExclusive(true);

    m_logButtonGroupHBoxLayout = new QHBoxLayout;
    m_logButtonGroupHBoxLayout->addWidget(m_logTypeLabel);
    m_logButtonGroupHBoxLayout->addWidget(m_logEmergPushButton);
    m_logButtonGroupHBoxLayout->addWidget(m_logAlertPushButton);
    m_logButtonGroupHBoxLayout->addWidget(m_logCriticalPushButton);
    m_logButtonGroupHBoxLayout->addWidget(m_logErrorPushButton);
    m_logButtonGroupHBoxLayout->addWidget(m_logWarningPushButton);
    m_logButtonGroupHBoxLayout->addWidget(m_logNoticePushButton);
    m_logButtonGroupHBoxLayout->addWidget(m_logInfoPushButton);
    m_logButtonGroupHBoxLayout->addWidget(m_logDebugPushButton);
    m_logButtonGroupHBoxLayout->addStretch(0);

    m_mainVBoxLayout->addLayout(m_logButtonGroupHBoxLayout);

    m_showLogPushButton = new QPushButton;
    m_showLogPushButton->setText(tr("Show Log"));
    m_clearLogPushButton = new QPushButton;
    m_clearLogPushButton->setText(tr("Clear Log"));

    m_operateLogHBoxLayout = new QHBoxLayout;
    m_operateLogHBoxLayout->addWidget(m_showLogPushButton);
    m_operateLogHBoxLayout->addWidget(m_clearLogPushButton);
    m_operateLogHBoxLayout->addStretch(1);

    m_mainVBoxLayout->addLayout(m_operateLogHBoxLayout);

    m_journalTextEdit= new QTextEdit();
    m_journalTextEdit->setText(tr("This is display journal log"));
    m_journalTextEdit->setReadOnly(true);
    m_mainVBoxLayout->addWidget(m_journalTextEdit);

    connect(m_startDatetimeCheckBox, &QCheckBox::stateChanged, this, &JournalWindow::slotCheckBoxEnableStartDatatimeEdit);
    connect(m_endDatetimeCheckBox, &QCheckBox::stateChanged, this, &JournalWindow::slotCheckBoxEnableEndDatatimeEdit);
    connect(m_startDatetimeDTEdit, &QDateTimeEdit::dateTimeChanged, this, &JournalWindow::slotStartDatetimeEditChanged);
    connect(m_endDatetimeDTEdit, &QDateTimeEdit::dateTimeChanged, this, &JournalWindow::slotEndDatetimeEditChanged);
    QObject::connect(m_showLogPushButton,&QPushButton::clicked,this,&JournalWindow::slotShowLog);
    QObject::connect(m_clearLogPushButton,&QPushButton::clicked,this,&JournalWindow::slotClearLog);

    connect(m_logButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &JournalWindow::slotButtonGroupChanged);

    slotCheckBoxEnableStartDatatimeEdit(-1);
    slotCheckBoxEnableEndDatatimeEdit(-1);
}

void JournalWindow::slotShowLog()
{
    quint64 startTimeStamp = 0;
    if(m_startFlag){
        startTimeStamp = m_startDatetimeDTEdit->dateTime().toMSecsSinceEpoch()*1000;
    }

    quint64 endTimeStamp = 0;
    if(m_endFlag){
        endTimeStamp = m_endDatetimeDTEdit->dateTime().toMSecsSinceEpoch()*1000;
    }

    SystemdJournalInterface * pJournal = new SystemdJournalInterface;
    QStringList strListAll= pJournal->getJournalEntries(m_logLevelPriority, startTimeStamp,endTimeStamp);
    m_journalTextEdit->clear();
    foreach (const QString str1, strListAll) {
        m_journalTextEdit->append(str1);
    }
}

void JournalWindow::slotClearLog()
{
    m_journalTextEdit->clear();
}

void JournalWindow::slotButtonGroupChanged(int index)
{
    m_logLevelPriority = index;
}

void JournalWindow::slotStartDatetimeEditChanged()
{
    qDebug() << "slotStartDatetimeEditChanged:"<< m_startDatetimeDTEdit->dateTime().toString("yyyy/MM/dd hh:mm:ss");
}

void JournalWindow::slotEndDatetimeEditChanged()
{
    qDebug() << "slotEndDatetimeEditChanged:"<< m_endDatetimeDTEdit->dateTime().toString("yyyy/MM/dd hh:mm:ss");
}

void JournalWindow::slotCheckBoxEnableStartDatatimeEdit(int state)
{
    if ( state == -1  || state == 0 || state ==1  || state ==2)
    {
        if (m_startDatetimeCheckBox->isChecked())
        {
            m_startFlag = true;
            m_startDatetimeDTEdit->setEnabled(true);
        }
        else
        {
            m_startFlag = false;
            m_startDatetimeDTEdit->setEnabled(false);
        }
    }
}

void JournalWindow::slotCheckBoxEnableEndDatatimeEdit(int state)
{
    if ( state == -1  || state == 0 || state ==1  || state ==2)
    {
        if (m_endDatetimeCheckBox->isChecked())
        {
            m_endFlag = true;
            m_endDatetimeDTEdit->setEnabled(true);
        }
        else
        {
            m_endFlag = false;
            m_endDatetimeDTEdit->setEnabled(false);
        }
    }
}

void JournalWindow::resizeEvent(QResizeEvent *event)
{
    if (nullptr != this)
    {
        //m_journalFileTableView->resize(width(), height());
        //update();
    }
}
