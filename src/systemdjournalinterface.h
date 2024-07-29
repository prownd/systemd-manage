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

#ifndef SYSTEMDJOURNALINTERFACE_H
#define SYSTEMDJOURNALINTERFACE_H

#include <QMap>
#include <QObject>
#include <QString>

// LOG_EMERG	0	/* system is unusable */
// LOG_ALERT	1	/* action must be taken immediately */
// LOG_CRIT	2	/* critical conditions */
// LOG_ERR		3	/* error conditions */
// LOG_WARNING	4	/* warning conditions */
// LOG_NOTICE	5	/* normal but significant condition */
// LOG_INFO	6	/* informational */
// LOG_DEBUG	7	/* debug-level messages */

enum LogLevelType
{
    LOGLEVEL_EMERG, LOGLEVEL_ALERT, LOGLEVEL_CRIT,
    LOGLEVEL_ERR, LOGLEVEL_WARNING, LOGLEVEL_NOTICE,
    LOGLEVEL_INFO, LOGLEVEL_DEBUG,
};

class SystemdJournalInterface:public QObject
{
public:
    SystemdJournalInterface();
    ~SystemdJournalInterface();

    QStringList getJournalEntries(int logLevel=3, quint64 startTimestamp=0, quint64 endTimestamp=0) const;
    QStringList getUnitJournalEntries(QString unit) const;
private:
    QString m_userBus;

    const QStringList m_logLevelNames = QStringList{ QStringLiteral("Emerg"),
                                                    QStringLiteral("Alert"),
                                                    QStringLiteral("Crit"),
                                                    QStringLiteral("Error"),
                                                    QStringLiteral("Warning"),
                                                    QStringLiteral("Notice"),
                                                    QStringLiteral("Info"),
                                                    QStringLiteral("Debug")};

    QMap<int, QString> m_logLevelNameMap;
};

#endif // SYSTEMDJOURNALINTERFACE_H
