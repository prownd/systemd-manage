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

#include "systemdmanagerinterface.h"
#include <QtDBus>
#include <unistd.h>

SystemdManagerInterface::SystemdManagerInterface()
{
    qDBusRegisterMetaType<SystemdUnit>();
    qDBusRegisterMetaType<SystemdSession>();
    qDBusRegisterMetaType<SystemdUser>();
    setupUserBusPath();
}

SystemdManagerInterface::~SystemdManagerInterface()
{

}

void SystemdManagerInterface::setupUserBusPath()
{
    // Search for user dbus.
    if (QFileInfo::exists(QStringLiteral("/run/user/%1/bus").arg(QString::number(getuid())))) {
        m_userBusPath = QStringLiteral("unix:path=/run/user/%1/bus").arg(QString::number(getuid()));
    } else if (QFileInfo::exists(QStringLiteral("/run/user/%1/dbus/user_bus_socket").arg(QString::number(getuid())))) {
        m_userBusPath = QStringLiteral("unix:path=/run/user/%1/dbus/user_bus_socket").arg(QString::number(getuid()));
    }
}

QDBusMessage SystemdManagerInterface::invokeDbusMethod(QString method, dbusIface ifaceName, dbusBus bus, const QList<QVariant> &args)
{
    //qDebug() << "Calling method" << method << "with iface" << ifaceName << "on bus" << bus;
    QDBusConnection abus(m_systemBus);
    if (bus == user)
        abus = QDBusConnection::connectToBus(m_userBusPath, connSystemd);

    QDBusInterface *iface = NULL;
    if (ifaceName == sysdMgr)
        iface = new QDBusInterface(connSystemd, pathSysdMgr, ifaceMgr, abus, this);
    else if (ifaceName == logdMgr)
        iface = new QDBusInterface(connLogind, pathLogdMgr, ifaceLogdMgr, abus, this);

    QDBusMessage msg;
    if (iface->isValid())
    {
        if (args.isEmpty())
            msg = iface->call(QDBus::AutoDetect, method);
        else
            msg = iface->callWithArgumentList(QDBus::AutoDetect, method, args);
        delete iface;
        if (msg.type() == QDBusMessage::ErrorMessage)
            qDebug() << "DBus method call failed: " << msg.errorMessage();
    }
    else
    {
        qDebug() << "Invalid DBus interface on bus" << bus;
        delete iface;
    }
    return msg;
}

QVariant SystemdManagerInterface::getDbusProperty(QString prop, dbusIface ifaceName, QDBusObjectPath path, dbusBus bus)
{
    // qDebug() << "Fetching property" << prop << ifaceName << path.path() << "on bus" << bus;
    QString conn, ifc;
    QDBusConnection abus(m_systemBus);
    if (bus == user)
        abus = QDBusConnection::connectToBus(m_userBusPath, connSystemd);

    if (ifaceName == sysdMgr)
    {
        conn = connSystemd;
        ifc = ifaceMgr;
    }
    else if (ifaceName == sysdUnit)
    {
        conn = connSystemd;
        ifc = ifaceUnit;
    }
    else if (ifaceName == sysdTimer)
    {
        conn = connSystemd;
        ifc = ifaceTimer;
    }
    else if (ifaceName == logdSession)
    {
        conn = connLogind;
        ifc = ifaceSession;
    }
    else if (ifaceName == logdUser)
    {
        conn = connLogind;
        ifc = ifaceUser;
    }
    QVariant r;
    QDBusInterface *iface = new QDBusInterface (conn, path.path(), ifc, abus, this);
    if (iface->isValid())
    {
        r = iface->property(prop.toLatin1().constData());
        delete iface;
        return r;
    }
    qDebug() << "Interface" << ifc << "invalid for" << path.path();
    return QVariant(QStringLiteral("invalidIface"));
}

QVector<SystemdUnit> SystemdManagerInterface::listUnits(dbusBus bus)
{
    // get an updated list of units via dbus
    QVector<SystemdUnit> list;
    QVector<unitfile> unitfileslist;
    QDBusMessage dbusreply;

    dbusreply = invokeDbusMethod(QStringLiteral("ListUnits"), sysdMgr, bus);

    if (dbusreply.type() == QDBusMessage::ReplyMessage)
    {
        const QDBusArgument argUnits = dbusreply.arguments().at(0).value<QDBusArgument>();
        if (argUnits.currentType() == QDBusArgument::ArrayType)
        {
            argUnits.beginArray();
            while (!argUnits.atEnd())
            {
                SystemdUnit unit;
                argUnits >> unit;
                if (unit.following.isEmpty()) {
                    list.append(unit);
                }
            }
            argUnits.endArray();
        }
        //qDebug() << "Added " << list.size() << " units on bus " << bus;

        // Get a list of unit files
        dbusreply = invokeDbusMethod(QStringLiteral("ListUnitFiles"), sysdMgr, bus);
        const QDBusArgument argUnitFiles = dbusreply.arguments().at(0).value<QDBusArgument>();
        argUnitFiles.beginArray();
        while (!argUnitFiles.atEnd())
        {
            unitfile u;
            argUnitFiles.beginStructure();
            argUnitFiles >> u.name >> u.status;
            argUnitFiles.endStructure();
            unitfileslist.append(u);
        }
        argUnitFiles.endArray();

        // Add unloaded units to the list
        for (const unitfile &f : unitfileslist)
        {
            int index = list.indexOf(SystemdUnit(f.name.section(QLatin1Char('/'), -1)));
            if (index > -1)
            {
                // The unit was already in the list, add unit file and its status
                list[index].unit_file = f.name;
                list[index].unit_file_status = f.status;
            }
            else
            {
                // Unit not in the list, add it.
                QFile unitfile(f.name);
                if (unitfile.symLinkTarget().isEmpty())
                {
                    SystemdUnit unit;
                    unit.id = f.name.section(QLatin1Char('/'), -1);
                    unit.load_state = QStringLiteral("unloaded");
                    unit.active_state = QLatin1Char('-');
                    unit.sub_state = QLatin1Char('-');
                    unit.unit_file = f.name;
                    unit.unit_file_status= f.status;
                    list.append(unit);
                }
            }
        }
        // qDebug() << "Added " << tal << " units from files on bus " << bus;

    }

    return list;
}

QVector<SystemdUnit> SystemdManagerInterface::listSystemUnits()
{
    return listUnits(sys);
}

QVector<SystemdUnit> SystemdManagerInterface::listUserUnits()
{
    return listUnits(user);
}

void SystemdManagerInterface::startSystemUnit(const QString &unitFile)
{
    return executeUnitAction(sys, "StartUnit", unitFile);
}

void SystemdManagerInterface::startUserUnit(const QString &unitFile)
{
    return executeUnitAction(sys, "StartUnit", unitFile);
}

void  SystemdManagerInterface::stopSystemUnit(const QString &unitFile)
{
    return executeUnitAction(sys, "StopUnit", unitFile);
}

void SystemdManagerInterface::stoptUserUnit(const QString &unitFile)
{
    return executeUnitAction(user, "StopUnit", unitFile);
}

void SystemdManagerInterface::restartSystemUnit(const QString &unitFile)
{
    return executeUnitAction(sys, "RestartUnit", unitFile);
}

void SystemdManagerInterface::restartUserUnit(const QString &unitFile)
{
    return executeUnitAction(sys, "RestartUnit", unitFile);
}

void SystemdManagerInterface::enableSystemUnitFiles(const QString &unitFile, const bool runtime, const bool force)
{
    return executeUnitAction(sys, "EnableUnitFiles", unitFile);
}

void SystemdManagerInterface::enableUserUnitFiles(const QString &unitFile, const bool runtime, const bool force)
{
    return executeUnitAction(user, "EnableUnitFiles",unitFile);
}

void SystemdManagerInterface::disableSystemUnitFiles(const QString &unitFile, const bool runtime)
{
    return executeUnitAction(sys, "DisableUnitFiles", unitFile);
}

void SystemdManagerInterface::disableUserUnitFiles(const QString &unitFile, const bool runtime)
{
    return executeUnitAction(user, "DisableUnitFiles", unitFile);
}

void SystemdManagerInterface::executeUnitAction(dbusBus bus, const QString &method, const QString & unitName)
{
    //Q_ASSERT(1);
    QString unit =unitName ;
    QVariantList args;
    if (method == QLatin1String("EnableUnitFiles") ||
        method == QLatin1String("MaskUnitFiles")) {
        args << QVariant(QStringList{unit}) << false << true;
    } else if (method == QLatin1String("DisableUnitFiles") ||
               method == QLatin1String("UnmaskUnitFiles")) {
        args << QVariant(QStringList{unit}) << false;
    } else {
        args = QVariantList{unit, QStringLiteral("replace")};
    }

    invokeDbusMethod(method, sysdMgr, bus, args);
}

void SystemdManagerInterface::lockSessin(const QString &sessionId)
{
    return executeSessionAction("LockSession", sessionId);
}

void SystemdManagerInterface::unlockSessin(const QString &sessionId)
{
    return executeSessionAction("UnlockSession", sessionId);
}

void SystemdManagerInterface::activateSession(const QString &sessionId)
{
    return executeSessionAction("ActivateSession", sessionId);
}

void SystemdManagerInterface::terminateSession(const QString &sessionId)
{
    return executeSessionAction("TerminateSession", sessionId);
}

void SystemdManagerInterface::executeSessionAction(const QString &method, const QString & sessionId)
{
    QString sessionIdStr = sessionId ;
    QVariantList args;

    args = QVariantList{sessionIdStr};

    invokeDbusMethod(method, logdMgr, sys, args);
}

void SystemdManagerInterface::terminateUser(const uint userId)
{
    return executeUserAction("TerminateUser", userId);
}

void SystemdManagerInterface::executeUserAction(const QString &method, const uint  userId)
{
    QVariantList args;

    args = QVariantList{userId};

    invokeDbusMethod(method, logdMgr, sys, args);
}

void SystemdManagerInterface::executeSystemDaemonAction(const QString &method)
{
    // Execute the DBus actions
    //invokeDbusMethod(method, sysdMgr, sys, QVariantList());
    invokeDbusMethod(QStringLiteral("Reload"), sysdMgr, sys, QVariantList());
}

void SystemdManagerInterface::executeUserDaemonAction(const QString &method)
{
    invokeDbusMethod(QStringLiteral("Reload"), sysdMgr, user, QVariantList());
}

QVector<SystemdSession> SystemdManagerInterface::listSessionUnits()
{
    // get an updated list of units via dbus
    QVector<SystemdSession> list;
    QVector<unitfile> unitfileslist;
    QDBusMessage dbusreply;

    dbusreply = invokeDbusMethod(QStringLiteral("ListUnits"), sysdMgr, session);
    if (dbusreply.type() == QDBusMessage::ReplyMessage)
    {
        const QDBusArgument argUnits = dbusreply.arguments().at(0).value<QDBusArgument>();
        if (argUnits.currentType() == QDBusArgument::ArrayType)
        {
            argUnits.beginArray();
            while (!argUnits.atEnd())
            {
                SystemdSession unit;
                argUnits >> unit;
                if (unit.seat_id.isEmpty()) {
                    list.append(unit);
                }
            }
            argUnits.endArray();
        }

        // Get a list of unit files
        dbusreply = invokeDbusMethod(QStringLiteral("ListUnitFiles"), sysdMgr, session);
        const QDBusArgument argUnitFiles = dbusreply.arguments().at(0).value<QDBusArgument>();
        argUnitFiles.beginArray();
        while (!argUnitFiles.atEnd())
        {
            unitfile u;
            argUnitFiles.beginStructure();
            argUnitFiles >> u.name >> u.status;
            argUnitFiles.endStructure();
            unitfileslist.append(u);
        }
        argUnitFiles.endArray();

        // Add unloaded units to the list
        for (const unitfile &f : unitfileslist)
        {
            int index = list.indexOf(SystemdSession(f.name.section(QLatin1Char('/'), -1)));
            if (index > -1)
            {
                // The unit was already in the list, add unit file and its status
                list[index].session_id = f.name;
                list[index].session_state = f.status;
            }
            else
            {
                // Unit not in the list, add it.
                QFile unitfile(f.name);
                if (unitfile.symLinkTarget().isEmpty())
                {
                    SystemdSession unit;
                    unit.session_id = f.name.section(QLatin1Char('/'), -1);
                    unit.session_state= f.status;
                    list.append(unit);
                }
            }
        }
        // qDebug() << "Added " << tal << " units from files on bus " << bus;
    }

    return list;
}
