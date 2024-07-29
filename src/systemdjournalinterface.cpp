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

#include "systemdjournalinterface.h"
#include <systemd/sd-journal.h>

#include <QDateTime>
#include <QDebug>

SystemdJournalInterface::SystemdJournalInterface()
{
    //foreach (const QString &str, m_logLevelNames)
    for(int i=0; i<m_logLevelNames.length(); ++i)
    {
        m_logLevelNameMap[i] = m_logLevelNames[i];
    }

    for(QMap<int, QString>::const_iterator iter = m_logLevelNameMap.constBegin(); iter != m_logLevelNameMap.constEnd(); ++iter)
    {
        //qDebug()<< "logLevelNameMap key,value:" << iter.key() << iter.value();
    }
}

SystemdJournalInterface::~SystemdJournalInterface()
{

}

QStringList SystemdJournalInterface::getJournalEntries(int logLevel, quint64 startTimestamp, quint64 endTimestamp) const
{
    QString match1, match2;
    int r, jflags;
    QStringList reply;
    const void *data;
    size_t length;
    uint64_t time;
    sd_journal *journal;

    jflags = (SD_JOURNAL_LOCAL_ONLY);

    r = sd_journal_open(&journal, jflags);
    if (r != 0)
    {
        qDebug()<< "Failed to open journal";
        return reply;
    }

    QString logLevelPriorityString;
    logLevelPriorityString = QString("PRIORITY=%1").arg(logLevel);

    //sd_journal_add_match(journal, "PRIORITY=0", 0);
    sd_journal_add_match(journal, logLevelPriorityString.toLatin1().data(), 0);

    //r = sd_journal_seek_tail(journal);
    //if (r != 0)
    //    return reply;

    QDateTime currentDateTime = QDateTime::currentDateTime();
    //QString beforeDayTime = currentDateTime.addDays(-1).toString("yyyy/MM/dd hh:mm");
    quint64 curMSecTimestamp = currentDateTime.addDays(-1).toMSecsSinceEpoch();
    quint64 curUSecTimestamp = curMSecTimestamp*1000;

    //qDebug()<< QStringLiteral("get journal loglevel:%1, start: %2, end:%3").arg(logLevel).arg(startTimestamp).arg(endTimestamp);
    if (startTimestamp > 0)
    {
        r = sd_journal_seek_realtime_usec(journal, startTimestamp);
        if (r <0 )
        {
            qDebug()<<QStringLiteral("Failed to seek to date: %1").arg(startTimestamp);
        }
    }
    else
    {
        r = sd_journal_seek_realtime_usec(journal, curUSecTimestamp);
        if (r <0 )
        {
            qDebug()<<QStringLiteral("Failed to seek to date: %1").arg(curUSecTimestamp);
        }
    }

    for (;;)
    {
        r = sd_journal_next(journal);
        if ( r <= 0 ){
            break;
        }

        QString line;

        // Get the date and time
        r = sd_journal_get_realtime_usec(journal, &time);
        if (r >= 0)
        {
            if (endTimestamp > 0)
            {
                if ( time > endTimestamp)
                    continue;
            }
            QDateTime date;
            date.setMSecsSinceEpoch(time/1000);
            line.append(date.toString(QStringLiteral("yyyy.MM.dd hh:mm:ss")));
        }

        // get hostname
        r = sd_journal_get_data(journal, "_HOSTNAME", &data, &length);
        if (r == 0)
        {
            line.append(QStringLiteral(" %1 ").arg(QString::fromUtf8((const char *)data, length).section(QLatin1Char('='),1)));
        }

        r = sd_journal_get_data(journal, "_COMM", &data, &length);
        if (r == 0)
        {
            line.append(QStringLiteral(" %1").arg(QString::fromUtf8((const char *)data, length).section(QLatin1Char('='),1)));
        }

        r = sd_journal_get_data(journal, "_PID", &data, &length);
        if (r == 0)
        {
            line.append(QStringLiteral("[%1]").arg(QString::fromUtf8((const char *)data, length).section(QLatin1Char('='),1)));
        }

        // Color messages according to priority
        r = sd_journal_get_data(journal, "PRIORITY", &data, &length);
        if (r == 0)
        {
            int prio = QString::fromUtf8((const char *)data, length).section(QLatin1Char('='),1).toInt();
            if (prio <= 3)
                line.append(QStringLiteral("<span style='color:tomato;'> [%1] ").arg(m_logLevelNameMap[prio]));
            else if (prio == 4)
                line.append(QStringLiteral("<span style='color:khaki;'> [%1] ").arg(m_logLevelNameMap[prio]));
            else
                line.append(QStringLiteral("<span style='color:palegreen;'> [%1] ").arg(m_logLevelNameMap[prio]));
        }

        // Get the message itself
        r = sd_journal_get_data(journal, "MESSAGE", &data, &length);
        if (r == 0)
        {
            line.append(QStringLiteral(": %1</span>").arg(QString::fromUtf8((const char *)data, length).section(QLatin1Char('='),1)));
            if (line.length() > 195)
                line = QStringLiteral("%1...</span>").arg(line.left(195));
            reply << line;
        }else if(r <0)
        {
            qDebug()<< "Failed to get MESSAGE";
            return reply;
        }
    }

    sd_journal_close(journal);

    return reply;
}

QStringList SystemdJournalInterface::getUnitJournalEntries(QString unit) const
{
    QString match1, match2;
    int r, jflags;
    QStringList reply;
    const void *data;
    size_t length;
    uint64_t time;
    sd_journal *journal;

    if (!m_userBus.isEmpty())
    {
        match1 = QStringLiteral("USER_UNIT=%1").arg(unit);
        jflags = (SD_JOURNAL_LOCAL_ONLY | SD_JOURNAL_CURRENT_USER);
    }
    else
    {
        match1 = QStringLiteral("_SYSTEMD_UNIT=%1").arg(unit);
        match2 = QStringLiteral("UNIT=%1").arg(unit);
        //jflags = (SD_JOURNAL_LOCAL_ONLY | SD_JOURNAL_SYSTEM);
        jflags = (SD_JOURNAL_LOCAL_ONLY);
    }

    r = sd_journal_open(&journal, jflags);
    if (r != 0)
    {
        qDebug()<< "Failed to open journal";
        return reply;
    }

    sd_journal_flush_matches(journal);

    r = sd_journal_add_match(journal, match1.toUtf8().constData(), 0);
    if (r != 0)
        return reply;

    if (!match2.isEmpty())
    {
        sd_journal_add_disjunction(journal);
        r = sd_journal_add_match(journal, match2.toUtf8().constData(), 0);
        if (r != 0)
            return reply;
    }

    r = sd_journal_seek_tail(journal);
    if (r != 0)
        return reply;

    SD_JOURNAL_FOREACH(journal)
    {
        //r = sd_journal_previous(journal);
        //if (r == 1)
        if ( true )
        {
            QString line;

            // Get the date and time
            r = sd_journal_get_realtime_usec(journal, &time);
            if (r == 0)
            {
                QDateTime date;
                date.setMSecsSinceEpoch(time/1000);
                line.append(date.toString(QStringLiteral("yyyy.MM.dd hh:mm:ss")));
            }

            // get hostname
            r = sd_journal_get_data(journal, "_HOSTNAME", &data, &length);
            if (r == 0)
            {
                line.append(QStringLiteral(" %1 ").arg(QString::fromUtf8((const char *)data, length).section(QLatin1Char('='),1)));
            }

            r = sd_journal_get_data(journal, "_COMM", &data, &length);
            if (r == 0)
            {
                line.append(QStringLiteral(" %1").arg(QString::fromUtf8((const char *)data, length).section(QLatin1Char('='),1)));
            }

            r = sd_journal_get_data(journal, "_PID", &data, &length);
            if (r == 0)
            {
                line.append(QStringLiteral("[%1]").arg(QString::fromUtf8((const char *)data, length).section(QLatin1Char('='),1)));
            }

            // Color messages according to priority
            r = sd_journal_get_data(journal, "PRIORITY", &data, &length);
            if (r == 0)
            {
                int prio = QString::fromUtf8((const char *)data, length).section(QLatin1Char('='),1).toInt();
                if (prio <= 3)
                    line.append(QStringLiteral("<span style='color:tomato;'> [%1] ").arg(m_logLevelNameMap[prio]));
                else if (prio == 4)
                    line.append(QStringLiteral("<span style='color:khaki;'> [%1] ").arg(m_logLevelNameMap[prio]));
                else
                    line.append(QStringLiteral("<span style='color:palegreen;'> [%1] ").arg(m_logLevelNameMap[prio]));
            }

            // Get the message itself
            r = sd_journal_get_data(journal, "MESSAGE", &data, &length);
            if (r == 0)
            {
                line.append(QStringLiteral(": %1</span>").arg(QString::fromUtf8((const char *)data, length).section(QLatin1Char('='),1)));
                if (line.length() > 195)
                    line = QStringLiteral("%1...</span>").arg(line.left(195));
                reply << line;
            }else if(r <0)
            {
                qDebug()<< "Failed to get MESSAGE";
                return reply;
            }
        }
        else // previous failed, no more entries
            return reply;
    }

    sd_journal_close(journal);

    return reply;
}
