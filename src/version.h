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

#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QStringList>
#include <QObject>

QString APPLICATION_NAME = QObject::tr("Systemd Manage");

QString APPLICATION_VERSION = QObject::tr("1.0");

QString EMAIL = QObject::tr("hanjinpeng127@gmail.com");
QString AUTHOR = QObject::tr("Han Jinpeng");
QStringList CREDITS={(QObject::tr("Han Jinpeng"))};

QString LICENSE = QObject::tr("GPLv3");
QString DATE = QObject::tr("2024-07-28");

QString WEBSITE = QObject::tr("<a href=https://github.com/prownd/systemd-manage>https://github.com/prownd/systemd-manage</a>");
QString LEGAL= QObject::tr("© 2024–2024 Han Jinpeng "
                      "This application comes with absolutely no warranty.<br/>"
                      "See the <a href=https://www.gnu.org/licenses/gpl-3.0.html>GNU General Public License, version 3 or later</a> for details."
                      );

#endif // VERSION_H
