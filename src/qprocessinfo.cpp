//
// My version is based on this person's code.
//
// Copyright (c) 2016, Baldur Karlsson
// Licensed under BSD 2-Clause License, see LICENSE file.
// Obtained from https://github.com/baldurk/qprocessinfo
//

/********************************************************************************
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
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <QDateTime>
#include <sys/types.h>

QProcessList QProcessInfo::populate()
{
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

            // Get the have running time.
            QFileInfo timeStatusInfo(processDir.absoluteFilePath(QStringLiteral("status")));
            QDateTime startDatetime = timeStatusInfo.lastModified();
            QDateTime now = QDateTime::currentDateTime();
            int runningSecs= startDatetime.secsTo(now);
            QString runningSecsStr=QTime(0, 0, 0).addSecs(int(runningSecs)).toString(QString::fromLatin1("HH:mm:ss"));
            info.setTime(runningSecsStr);

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

const QString &QProcessInfo::getTime() const
{
    return m_time;
}

void QProcessInfo::setTime(const QString &timestr)
{
    m_time = timestr;
}

