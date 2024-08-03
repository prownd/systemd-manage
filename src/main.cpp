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

#include "mainwidget.h"
#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QIcon>

#include "globalvar.h"

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName(globalOrganizationName);
    QApplication::setApplicationName(globalApplicationName);

    QApplication a(argc, argv);
    a.setStyleSheet(globalApplicationWindowtStyleSheet1);
    QSettings cfgSetting;
    // i18n traslate and locale
    // setting zh
    QTranslator translator;
    QString lang = cfgSetting.value("Language").toString();
    if(!lang.isEmpty()){
        translator.load(QString(":/translators/app_") + lang);
    }else{
        QString locale = QLocale::system().name();
        translator.load(QString(":/translators/app_") + locale);
    }
    a.installTranslator(&translator);

    QApplication::setWindowIcon(QIcon(QString(":/icons/systemd-manage.png")));

    MainWidget w;
    w.show();
    return a.exec();
}
