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

#ifndef CUSTOMCANTTTIMEBARITEM_H
#define CUSTOMCANTTTIMEBARITEM_H

#include <QGraphicsRectItem>
#include <QDateTime>

const int customCanttTimeBarType = QGraphicsItem::UserType + 1;
const int customCanttTimeBarHeight = 38;

class CustomCanttTimeBarItem : public QGraphicsRectItem
{
public:
    enum {Type = customCanttTimeBarType};
    enum TimeType {
        ElapseTime,
    };

    explicit CustomCanttTimeBarItem(const QString textstr, const QDateTime &start, const QDateTime &end, double factor);
private:
    double m_pFactor;
    QDateTime m_startDateTime;
    QDateTime m_endDateTime;
};

#endif // CUSTOMCANTTTIMEBARITEM_H
