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
#include <glib.h>
#include <glibtop/prockernel.h>

class QProcessInfo;
typedef QList<QProcessInfo> QProcessList;

class QProcessInfo
{
    public:
        QProcessInfo();
        static QProcessList getProcInfoList();
        static QProcessList populate();
        static QString getProcessNname(const gchar *cmd, const GStrv args);
        static QString lookupUser(guint uid);
        static QString formatProcessState(guint state);
        static QString getNiceLevel(gint nice);
        static QString getNiceLevelWithPriority(gint nice);
        static QString getProcKernelWchan (glibtop_proc_kernel&obj);
        static QString format_memsize (guint64 size);

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

        const QString& getCPUTime() const;
        void  setCPUTime(const QString& timestr);

        QString getState() const;
        void setState(const QString &newState);

        uint32_t getNice() const;
        void setNice(uint32_t newNice);

        uint64_t getMem() const;
        void setMem(uint64_t newMem);

        uint64_t getVmsize() const;
        void setVmsize(uint64_t newVmsize);

        uint64_t getMemres() const;
        void setMemres(uint64_t newMemres);

        uint64_t getMemshared() const;
        void setMemshared(uint64_t newMemshared);

        uint64_t getMemwritable() const;
        void setMemwritable(uint64_t newMemwritable);

        QString getWchan() const;
        void setWchan(const QString &newWchan);

        uint64_t getStarttime() const;
        void setStarttime(uint64_t newStarttime);

    private:
        uint32_t m_pid;
        uint32_t m_ppid;
        QString  m_username;
        QString  m_name;
        QString  m_cmdLine;
        QString  m_cputime;
        QString  m_state;
        QString  m_gid;
        uint32_t m_nice;

        uint64_t m_starttime;

        uint64_t m_vmsize;
        uint64_t m_memres;
        uint64_t m_memshared;
        uint64_t m_memwritable;
        uint64_t m_mem;
        QString m_wchan;
};

#endif // QPROCESSINFO_H
