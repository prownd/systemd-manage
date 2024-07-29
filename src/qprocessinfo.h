#ifndef QPROCESSINFO_H
#define QPROCESSINFO_H

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

#include <QList>

class QProcessInfo;
typedef QList<QProcessInfo> QProcessList;

class QProcessInfo
{
    public:
        QProcessInfo();
        static QProcessList populate();
        uint32_t getPid() const;
        void setPid(uint32_t pid);
        uint32_t getPPid() const;
        void setPPid(uint32_t ppid);
        const QString& getUsername() const;
        void setUsername(const QString& username);
        const QString& getName() const;
        void setName(const QString& name);
        const QString& getCommandLine() const;
        void  setCommandLine(const QString& cmd);
        const QString& getTime() const;
        void  setTime(const QString& timestr);
    private:
        uint32_t m_pid;
        uint32_t m_ppid;
        QString  m_username;
        QString  m_name;
        QString  m_cmdLine;
        QString  m_time;
};

#endif // QPROCESSINFO_H
