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

#include "configfile.h"
#include <QDebug>
#include <QDir>

ConfigSetting::ConfigSetting() {
    m_settings = new QSettings;
}

ConfigSetting::ConfigSetting(const QString &organization, const QString &application) {
    m_settings = new QSettings(organization, application);
}

ConfigSetting::~ConfigSetting() {
    delete m_settings;
}

void ConfigSetting::setValue(const QString &key, const QVariant &value) {
    m_settings->setValue(key, value);
}

QVariant ConfigSetting::getValue(const QString &key, const QVariant &defaultValue) const {
    return m_settings->value(key, defaultValue);
}

QString ConfigSetting::getFileName() const {
    return m_settings->fileName();
}
