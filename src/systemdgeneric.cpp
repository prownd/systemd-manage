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

#include "systemdgeneric.h"

QDBusArgument &operator<<(QDBusArgument &argument, const SystemdUnit &unit)
{
    argument.beginStructure();
    argument << unit.id
             << unit.description
             << unit.load_state
             << unit.active_state
             << unit.sub_state
             << unit.following
             << unit.unit_path
             << unit.job_id
             << unit.job_type
             << unit.job_path;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, SystemdUnit &unit)
{
    argument.beginStructure();
    argument >> unit.id
        >> unit.description
        >> unit.load_state
        >> unit.active_state
        >> unit.sub_state
        >> unit.following
        >> unit.unit_path
        >> unit.job_id
        >> unit.job_type
        >> unit.job_path;
    argument.endStructure();
    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const SystemdSession &session)
{
    argument.beginStructure();
    argument << session.session_id
             << session.user_id
             << session.user_name
             << session.seat_id
             << session.session_path
             << session.session_state;

    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, SystemdSession &session)
{
    argument.beginStructure();
    argument >> session.session_id
        >> session.user_id
        >> session.user_name
        >> session.seat_id
        >> session.session_path
        >> session.session_state;

    argument.endStructure();
    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const SystemdUser &user)
{
    argument.beginStructure();
    argument << user.user_id
             << user.user_name
             << user.user_path
             << user.user_state;

    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, SystemdUser &user)
{
    argument.beginStructure();
    argument >> user.user_id
        >> user.user_name
        >> user.user_path
        >> user.user_state;

    argument.endStructure();
    return argument;
}
