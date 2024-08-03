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

#ifndef JOURNALWINDOW_H
#define JOURNALWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QDateTimeEdit>
#include <QCheckBox>

class JournalWindow : public QWidget
{
    Q_OBJECT

public:
    explicit JournalWindow(QWidget *parent = nullptr);
    ~JournalWindow();  
    void createJournalViewWindow();

private:
    QTextEdit *m_journalTextEdit;
    QVBoxLayout * m_mainVBoxLayout ;

    QHBoxLayout * m_dateTimeHBoxLayout;
    QLabel * m_datetimeLabel;
    QCheckBox * m_startDatetimeCheckBox;
    QCheckBox * m_endDatetimeCheckBox;
    QDateTimeEdit * m_startDatetimeDTEdit;
    QDateTimeEdit * m_endDatetimeDTEdit;

    QHBoxLayout * m_logButtonGroupHBoxLayout ;
    QLabel * m_logTypeLabel;
    QPushButton * m_logEmergPushButton;
    QPushButton * m_logAlertPushButton;
    QPushButton * m_logCriticalPushButton;
    QPushButton * m_logErrorPushButton;
    QPushButton * m_logWarningPushButton;
    QPushButton * m_logNoticePushButton;
    QPushButton * m_logInfoPushButton;
    QPushButton * m_logDebugPushButton;
    QButtonGroup *    m_logButtonGroup;

    QPushButton * m_showLogPushButton;
    QPushButton * m_clearLogPushButton;
    QHBoxLayout * m_operateLogHBoxLayout;

    bool m_startFlag=false, m_endFlag=false;
    int m_logLevelPriority = 3;

protected Q_SLOTS:
    void slotCheckBoxEnableStartDatatimeEdit(int state);
    void slotCheckBoxEnableEndDatatimeEdit(int state);
    void slotStartDatetimeEditChanged();
    void slotEndDatetimeEditChanged();

    void slotShowLog();
    void slotClearLog();

    void slotButtonGroupChanged(int index);
protected:
    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // JOURNALWINDOW_H
