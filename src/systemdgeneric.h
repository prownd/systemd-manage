/*******************************************************************************
 * Copyright (C) 2016 Ragnar Thomsen <rthomsen6@gmail.com>                     *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU General Public License as published by the Free  *
 * Software Foundation, either version 2 of the License, or (at your option)   *
 * any later version.                                                          *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
 * more details.                                                               *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program. If not, see <http://www.gnu.org/licenses/>.              *
 *******************************************************************************/

#ifndef SYSTEMDGENERIC_H
#define SYSTEMDGENERIC_H

#include <QString>
#include <QDBusObjectPath>
#include <QtDBus>
#include <QDBusMetaType>

enum dbusBus
{
  sys, session, user
};

// struct for storing units retrieved from systemd via DBus
struct SystemdUnit
{
  QString id, description, load_state, active_state, sub_state, following, job_type, unit_file, unit_file_status;
  QDBusObjectPath unit_path, job_path;
  uint job_id;
  
  // The == operator must be provided to use contains() and indexOf()
  // on QLists of this struct
  bool operator==(const SystemdUnit& right) const
  {
    if (id == right.id)
      return true;
    else
      return false;
  }
  SystemdUnit(){}

  SystemdUnit(QString newId)
  {
    id = newId;
  }
};
Q_DECLARE_METATYPE(SystemdUnit)
typedef QList<SystemdUnit> SystemdUnitList;
Q_DECLARE_METATYPE(SystemdUnitList)

QDBusArgument &operator<<(QDBusArgument &argument, const SystemdUnit &unit);
const QDBusArgument &operator>>(const QDBusArgument &argument, SystemdUnit &unit);


// struct for storing sessions retrieved from logind via DBus
struct SystemdSession
{
  QString session_id, user_name, seat_id, session_state;
  QDBusObjectPath session_path;
  uint user_id;

  // The == operator must be provided to use contains() and indexOf()
  // on QLists of this struct
  bool operator==(const SystemdSession& right) const
  {
    if (session_id == right.session_id)
      return true;
    else
      return false;
  }

  SystemdSession(){}

  SystemdSession(QString newId)
  {
      session_id = newId;
  }
};
Q_DECLARE_METATYPE(SystemdSession)
typedef QList<SystemdSession> SystemdSessionList;
Q_DECLARE_METATYPE(SystemdSessionList)

QDBusArgument &operator<<(QDBusArgument &argument, const SystemdSession &session);
const QDBusArgument &operator>>(const QDBusArgument &argument, SystemdSession &session);


// struct for storing sessions retrieved from logind via DBus
struct SystemdUser
{
    uint user_id;
    QString group_id, user_name, seat_id, user_state, user_slice, user_runtimepath, user_service;
    QDBusObjectPath user_path;
    QVector<QString> user_sessions;

    // The == operator must be provided to use contains() and indexOf()
    // on QLists of this struct
    bool operator==(const SystemdUser& right) const
    {
        if (user_id == right.user_id)
            return true;
        else
            return false;
    }

    SystemdUser(){}

    SystemdUser(uint newId)
    {
        user_id = newId;
    }
};
Q_DECLARE_METATYPE(SystemdUser)
typedef QList<SystemdUser> SystemdUserList;
Q_DECLARE_METATYPE(SystemdUserList)

QDBusArgument &operator<<(QDBusArgument &argument, const SystemdUser &user);
const QDBusArgument &operator>>(const QDBusArgument &argument, SystemdUser &user);

#endif // SYSTEMDGENERIC_H
