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

#ifndef SYSTEMDMANAGERINTERFACE_H
#define SYSTEMDMANAGERINTERFACE_H

#include "systemdgeneric.h"

#include <QString>
#include <QDBusConnection>

#include <QDBusMetaType>
#include <QDBusMessage>

struct conffile
{
    QString filePath;
    QString manPage;
    QString description;

    conffile(QString f, QString m, QString d)
    {
        filePath = f;
        manPage = m;
        description = d;
    }

    conffile(){}

    conffile(QString f)
    {
        filePath = f;
    }

    bool operator==(const QString& right) const
    {
        if (filePath == right)
            return true;
        else
            return false;
    }

    bool operator==(const conffile& right) const
    {
        if (filePath == right.filePath)
            return true;
        else
            return false;
    }
};


struct unitfile
{
    QString name;
    QString status;

    bool operator==(const unitfile& right) const
    {
        if (name.section(QLatin1Char('/'), -1) == right.name)
            return true;
        else
            return false;
    }
};

enum dbusConn
{
    systemd, logind
};

enum dbusIface
{
    sysdMgr, sysdUnit, sysdTimer, logdMgr, logdSession, logdUser
};

class SystemdManagerInterface:public QObject
{
public:
    SystemdManagerInterface();
    ~SystemdManagerInterface();

    void setupUserBusPath();

    QVector<SystemdUnit> listUnits(dbusBus bus);
    QVector<SystemdUnit> listSystemUnits();
    QVector<SystemdUnit> listUserUnits();
    QVector<SystemdSession> listSessionUnits();
    QDBusMessage invokeDbusMethod(QString method, dbusIface ifaceName, dbusBus bus = sys, const QList<QVariant> &args = QList<QVariant> ());
    //QVariant getDbusProperty(QString prop, dbusIface ifaceName, QDBusObjectPath path, dbusBus bus);
    QVariant getDbusProperty(QString prop, dbusIface ifaceName, QDBusObjectPath path = QDBusObjectPath("/org/freedesktop/systemd1"), dbusBus bus = sys);

    void startSystemUnit(const QString &unitFile);
    void startUserUnit(const QString &unitFile);
    void stopSystemUnit(const QString &unitFile);
    void stoptUserUnit(const QString &unitFile);
    void restartSystemUnit(const QString &unitFile);
    void restartUserUnit(const QString &unitFile);
    void enableSystemUnitFiles(const QString &unitFile, const bool runtime = true, const bool force = true);
    void enableUserUnitFiles(const QString &unitFile, const bool runtime = true, const bool force = true);
    void disableSystemUnitFiles(const QString &unitFile, const bool runtime =true);
    void disableUserUnitFiles(const QString &unitFile, const bool runtime = true);
    void executeUnitAction(dbusBus bus, const QString &method, const QString & unitName);

    void lockSessin(const QString &sessionId);
    void unlockSessin(const QString &sessionId);
    void activateSession(const QString &sessionId);
    void terminateSession(const QString &sessionId);
    void executeSessionAction(const QString &method, const QString & sessionId);

    void terminateUser(const uint sessionId);
    void executeUserAction(const QString &method, const uint  userId);

    void executeSystemDaemonAction(const QString &method ="");
    void executeUserDaemonAction(const QString &method = "");

private:
    const QString connSystemd = QStringLiteral("org.freedesktop.systemd1");
    const QString connLogind = QStringLiteral("org.freedesktop.login1");
    const QString pathSysdMgr = QStringLiteral("/org/freedesktop/systemd1");
    const QString pathLogdMgr = QStringLiteral("/org/freedesktop/login1");
    const QString ifaceMgr = QStringLiteral("org.freedesktop.systemd1.Manager");
    const QString ifaceLogdMgr = QStringLiteral("org.freedesktop.login1.Manager");
    const QString ifaceUnit = QStringLiteral("org.freedesktop.systemd1.Unit");
    const QString ifaceTimer = QStringLiteral("org.freedesktop.systemd1.Timer");
    const QString ifaceSession = QStringLiteral("org.freedesktop.login1.Session");
    const QString ifaceUser = QStringLiteral("org.freedesktop.login1.User");
    const QString ifaceDbusProp = QStringLiteral("org.freedesktop.DBus.Properties");
    QDBusConnection m_systemBus = QDBusConnection::systemBus();
    QString m_userBusPath;
};

#endif // SYSTEMDMANAGERINTERFACE_H
