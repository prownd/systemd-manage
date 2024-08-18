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

#include "qprocessinfo.h"
#include <QDir>
#include <QProcess>
#include <QRegExp>
#include <QStandardPaths>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>

#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <QDateTime>
#include <sys/types.h>
#include <glibtop.h>
#include <glibtop/proclist.h>
#include <glibtop/procstate.h>
#include <glibtop/procio.h>
#include <glibtop/proctime.h>
#include <glibtop/procuid.h>
#include <glibtop/procargs.h>
#include <glibtop/procmem.h>
#include <glibtop/prockernel.h>
#include <glib.h>

QProcessList QProcessInfo::getProcInfoList()
{
    QProcessList processListRet;
    pid_t*pid_list;
    glibtop_proclist proclist;
    int which = GLIBTOP_KERN_PROC_ALL;
    int arg = 0;

    pid_list = glibtop_get_proclist (&proclist, which, arg);
    unsigned i = 0;
    for (i = 0; i < proclist.number; ++i) {
        glibtop_proc_time proctime;
        glibtop_proc_uid procuid;
        glibtop_proc_state procstate;
        glibtop_proc_args procargs;
        glibtop_cpu proccpu;
        glibtop_proc_mem procmem;
        glibtop_proc_kernel prockernel;
        gchar**arguments;

        pid_t pid = pid_list[i];

        glibtop_get_proc_time(&proctime, pid);
        glibtop_get_proc_uid(&procuid, pid);
        glibtop_get_proc_state(&procstate, pid);
        glibtop_get_cpu (&proccpu);
        glibtop_get_proc_mem (&procmem, pid);
        glibtop_get_proc_kernel (&prockernel, pid);

        glibtop_get_proc_state (&procstate, pid);
        arguments = glibtop_get_proc_argv (&procargs, pid, 0);

        QString cmdline = getProcessNname(procstate.cmd, static_cast<const GStrv>(arguments));

        /* almost equivalent to ps -x -o user,pid,time,usertime,systime,start,command */
        /*
        printf("%u pid=%u real=%.2f user=%.2f sys=%.2f start=%lu %s (%lx)\n",
               puid.uid,
               pid,
               (double)ptime.rtime / ptime.frequency,
               (double)ptime.utime / ptime.frequency,
               (double)ptime.stime / ptime.frequency,
               (unsigned long)ptime.start_time,
               pstate.cmd,
               (long)ptime.flags);
        */

        QProcessInfo info;
        info.setPid(pid);
        info.setPPid(procuid.ppid);
        info.setNice(procuid.nice);
        info.setUsername(lookupUser(procuid.uid));
        info.setName(procstate.cmd);

        guint cpu_scale = 100;
        gdouble pcputime = (gdouble)proctime.rtime * cpu_scale / proccpu.total;
        pcputime = qMin(pcputime, (gdouble)cpu_scale);
        info.setCPUTime(QString::number(pcputime, 'f', 2));

        info.setCommandLine(cmdline);
        info.setState(formatProcessState(procstate.state));
        info.setStarttime(proctime.start_time);
        info.setWchan(getProcKernelWchan(prockernel));

        info.setVmsize(procmem.vsize);
        info.setMemres(procmem.resident);
        info.setMemshared(procmem.share);
        info.setMem(info.getMemres() - info.getMemshared());

        processListRet.push_back(info);
    }

    if(false){
        for (const QProcessInfo & procesinfoStr : processListRet)
        {
            qDebug() << "---------------------------------------------------";
            qDebug() << "ProcessInfo pid:" <<procesinfoStr.getPid();
            qDebug() << "ProcessInfo ppid:" <<procesinfoStr.getPPid();
            qDebug() << "ProcessInfo getNice:" <<procesinfoStr.getNice();
            qDebug() << "ProcessInfo getUsername:" <<procesinfoStr.getUsername();
            qDebug() << "ProcessInfo getName:" <<procesinfoStr.getName();
            qDebug() << "ProcessInfo getCPUTime:" <<procesinfoStr.getCPUTime();
            qDebug() << "ProcessInfo getCommandLine:" <<procesinfoStr.getCommandLine();
            qDebug() << "ProcessInfo getState:" <<procesinfoStr.getState();
            qDebug() << "ProcessInfo getWchan:" <<procesinfoStr.getWchan();

            qDebug() << "ProcessInfo getMem:" <<procesinfoStr.getMem();
            qDebug() << "ProcessInfo getVmsize:" <<procesinfoStr.getVmsize();
            qDebug() << "ProcessInfo getMemres:" <<procesinfoStr.getMemres();
            qDebug() << "ProcessInfo getMemshared:" <<procesinfoStr.getMemshared();
            qDebug() << "---------------------------------------------------";
        }
    }

    return processListRet;
}

QString QProcessInfo::getProcessNname(const gchar *cmd, const GStrv args)
{
    if (args)
    {
        for (int i = 0; i != 2 && args[i]; ++i)
        {
            char*basename;
            basename = g_path_get_basename (args[i]);

            if (g_str_has_prefix (basename, cmd))
            {
                return QString(basename);
            }

            g_free (basename);
        }
    }
    return QString(cmd);
}

QString QProcessInfo::lookupUser(guint uid)
{
    QString usernameStr;
    struct passwd*pwd;
    pwd = getpwuid (uid);
    if (pwd && pwd->pw_name)
    {
        usernameStr = pwd->pw_name;
    }else
    {
        usernameStr = QString::number(uid);
    }
    return usernameStr;
}

QString QProcessInfo::formatProcessState (guint state)
{
    QString status;
    switch (state)
    {
        case GLIBTOP_PROCESS_RUNNING:
            status = QObject::tr("Running");
            break;
        case GLIBTOP_PROCESS_STOPPED:
            status = QObject::tr("Stopped");
            break;
        case GLIBTOP_PROCESS_ZOMBIE:
            status = QObject::tr("Zombie");
            break;
        case GLIBTOP_PROCESS_UNINTERRUPTIBLE:
            status = QObject::tr("Uninterruptible");
            break;
        default:
            status = QObject::tr("Sleeping");
            break;
    }
    return status;
}

QString  QProcessInfo::getNiceLevel (gint nice)
{
    if (nice < -7)
        return QObject::tr("Very High");
    else if (nice < -2)
        return QObject::tr("High");
    else if (nice < 3)
        return QObject::tr("Normal");
    else if (nice < 7)
        return QObject::tr("Low");
    else
        return QObject::tr("Very Low");
}

QString  QProcessInfo::getNiceLevelWithPriority(gint nice)
{
    if (nice < -7)
        return QObject::tr("Very High Priority");
    else if (nice < -2)
        return QObject::tr("High Priority");
    else if (nice < 3)
        return QObject::tr("Normal Priority");
    else if (nice < 7)
        return QObject::tr("Low Priority");
    else
        return QObject::tr("Very Low Priority");
}

QString QProcessInfo::getProcKernelWchan (glibtop_proc_kernel&obj)
{
    QString wchanStr(obj.wchan);
    return wchanStr;
}

QString QProcessInfo::format_memsize (guint64 size)
{
    QString formatMemStr;
    if (size == 0)
        formatMemStr = QObject::tr("N/A");
    else
        formatMemStr = g_format_size_full (size, G_FORMAT_SIZE_IEC_UNITS);
    return  formatMemStr;
}

QProcessList QProcessInfo::populate()
{
    getProcInfoList();
    QProcessList ret;
    QDir proc(QStringLiteral("/proc"));
    QStringList files = proc.entryList();
    for (const QString& f : files) {
        bool ok = false;
        uint32_t pid = f.toUInt(&ok);

        if (ok) {
            QProcessInfo info;
            info.setPid(pid);
            QDir processDir(QStringLiteral("/proc/") + f);
            QFileInfo exe(processDir.absoluteFilePath(QStringLiteral("exe")));
            exe = QFileInfo(exe.symLinkTarget());
            info.setName(exe.completeBaseName());

            // if we didn't get a name from the symlink, check in the status file
            if (info.getName().isEmpty()) {
                QFile status(processDir.absoluteFilePath(QStringLiteral("status")));
                if (status.open(QIODevice::ReadOnly)) {
                    QByteArray contents = status.readAll();
                    QTextStream in(&contents);
                    while(!in.atEnd()) {
                        QString line = in.readLine();
                        if (line.startsWith(QStringLiteral("Name:"))) {
                            line.remove(0, 5);
                            // if we're using this name, surround with []s to indicate it's not a file
                            info.setName(QStringLiteral("[%1]").arg(line.trimmed()));
                            break;
                        }
                    }
                    status.close();
                }
            }

            // Get the username.
            QFile status(processDir.absoluteFilePath(QStringLiteral("status")));
            if (status.open(QIODevice::ReadOnly)) {
                QByteArray contents = status.readAll();
                QTextStream in(&contents);
                while(!in.atEnd()) {
                    QString line = in.readLine();
                    if (line.startsWith(QStringLiteral("Uid:"))) {
                        //qDebug() << line;
                        //qDebug() << line.split(QRegExp("\\s+")).at(1);
                        info.setUsername(line.split(QRegExp("\\s+")).at(1));
                        break;
                    }
                }
                status.close();

                struct passwd* pw = getpwuid(info.getUsername().toULong());
                if (pw) {
                    info.setUsername(pw->pw_name);
                }
            }

            // Get the ppid.
            QFile ppidStatus(processDir.absoluteFilePath(QStringLiteral("status")));
            if (ppidStatus.open(QIODevice::ReadOnly)) {
                QByteArray contents = ppidStatus.readAll();
                QTextStream in(&contents);
                while(!in.atEnd()) {
                    QString line = in.readLine();
                    if (line.startsWith(QStringLiteral("PPid:"))) {
                        //qDebug() << "ppid:" <<line.split(QRegExp("\\s+")).at(1).toInt();
                        info.setPPid(line.split(QRegExp("\\s+")).at(1).toInt());
                        break;
                    }
                }
                ppidStatus.close();
            }

            // Get the state
            QFile processStatusFile(processDir.absoluteFilePath(QStringLiteral("status")));
            if (processStatusFile.open(QIODevice::ReadOnly)) {
                QByteArray contents = processStatusFile.readAll();
                QTextStream in(&contents);
                while(!in.atEnd()) {
                    QString line = in.readLine();
                    if (line.startsWith(QStringLiteral("State:"))) {
                        info.setState(line.split(QRegExp("\\s+")).at(1));
                        break;
                    }
                }
                processStatusFile.close();
            }

            // Get the have running time.
            QFileInfo timeStatusInfo(processDir.absoluteFilePath(QStringLiteral("status")));
            QDateTime startDatetime = timeStatusInfo.lastModified();
            QDateTime now = QDateTime::currentDateTime();
            int runningSecs= startDatetime.secsTo(now);
            QString runningSecsStr=QTime(0, 0, 0).addSecs(int(runningSecs)).toString(QString::fromLatin1("HH:mm:ss"));
            info.setCPUTime(runningSecsStr);

            // Get the command line
            QFile cmdline(processDir.absoluteFilePath(QStringLiteral("cmdline")));
            if (cmdline.open(QIODevice::ReadOnly)) {
                QByteArray contents = cmdline.readAll();
                int nullIdx = contents.indexOf('\0');
                if (nullIdx > 0) {
                    QString firstparam = QString::fromUtf8(contents.data(), nullIdx);

                    // if name is a truncated form of a filename, replace it
                    if (firstparam.endsWith(info.getName()) && QFileInfo::exists(firstparam)) {
                        info.setName(QFileInfo(firstparam).completeBaseName());
                    }

                    // if we don't have a name, replace it but with []s
                    if (info.getName().isEmpty()) {
                        info.setName(QStringLiteral("[%1]").arg(firstparam));
                    }

                    contents.replace('\0', ' ');
                }

                info.setCommandLine(QString::fromUtf8(contents).trimmed());
                cmdline.close();
            }

            // Add the process to the list.
            ret.push_back(info);
        }
    }

    return ret;
}

QProcessInfo::QProcessInfo()
{
    m_pid = 0;
}

uint32_t QProcessInfo::getPid() const
{
    return m_pid;
}

void QProcessInfo::setPid(uint32_t pid)
{
    m_pid = pid;
}

uint32_t QProcessInfo::getPPid() const
{
    return m_ppid;
}

void QProcessInfo::setPPid(uint32_t ppid)
{
    m_ppid = ppid;
}

const QString &QProcessInfo::getUsername() const
{
    return m_username;
}

void QProcessInfo::setUsername(const QString &username)
{
    m_username = username;
}

const QString &QProcessInfo::getName() const
{
    return m_name;
}

void QProcessInfo::setName(const QString &name)
{
    m_name = name;
}

const QString &QProcessInfo::getCommandLine() const
{
    return m_cmdLine;
}

void QProcessInfo::setCommandLine(const QString &cmd)
{
    m_cmdLine = cmd;
}

const QString &QProcessInfo::getCPUTime() const
{
    return m_cputime;
}

void QProcessInfo::setCPUTime(const QString &timestr)
{
    m_cputime = timestr;
}

QString QProcessInfo::getState() const
{
    return m_state;
}

void QProcessInfo::setState(const QString &newState)
{
    m_state = newState;
}

uint32_t QProcessInfo::getNice() const
{
    return m_nice;
}

void QProcessInfo::setNice(uint32_t newNice)
{
    m_nice = newNice;
}

uint64_t QProcessInfo::getMem() const
{
    return m_mem;
}

void QProcessInfo::setMem(uint64_t newMem)
{
    m_mem = newMem;
}

QString QProcessInfo::getWchan() const
{
    return m_wchan;
}

void QProcessInfo::setWchan(const QString &newWchan)
{
    m_wchan = newWchan;
}

uint64_t QProcessInfo::getStarttime() const
{
    return m_starttime;
}

void QProcessInfo::setStarttime(uint64_t newStarttime)
{
    m_starttime = newStarttime;
}

uint64_t QProcessInfo::getVmsize() const
{
    return m_vmsize;
}

void QProcessInfo::setVmsize(uint64_t newVmsize)
{
    m_vmsize = newVmsize;
}

uint64_t QProcessInfo::getMemres() const
{
    return m_memres;
}

void QProcessInfo::setMemres(uint64_t newMemres)
{
    m_memres = newMemres;
}

uint64_t QProcessInfo::getMemshared() const
{
    return m_memshared;
}

void QProcessInfo::setMemshared(uint64_t newMemshared)
{
    m_memshared = newMemshared;
}

uint64_t QProcessInfo::getMemwritable() const
{
    return m_memwritable;
}

void QProcessInfo::setMemwritable(uint64_t newMemwritable)
{
    m_memwritable = newMemwritable;
}

