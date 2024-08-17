#ifndef QPROCESSINFO_H
#define QPROCESSINFO_H

/********************************************************************************
 * Obtained from https://github.com/epasveer/QProcessInfoWidget                 *
 * My version (epasveer) is based on this person's code.                        *
 *                                                                              *
 * Copyright (c) 2016, Baldur Karlsson                                          *
 * Licensed under BSD 2-Clause License, see LICENSE file.                       *
 * Obtained from https://github.com/baldurk/qprocessinfo                        *
 *                                                                              *
 *                                                                              *
 * This version is modify by Han Jinpeng <hanjinpeng127@gmail.com>              *
 * Website: https://github.com/prownd/systemd-manage                            *
 *                                                                              *
 * Licensed under BSD 2-Clause License or GPLv3                                 *
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
