/********************************************************************************
 *                                                                              *
 * Obtained origin from https://www.cnblogs.com/luoxiang/p/17521164.html        *
 * My version is based on this person's code.                                   *
 *                                                                              *
 * This version is modify by Han Jinpeng <hanjinpeng127@gmail.com>              *
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

#include "customcantttimebaritem.h"

#include <QBrush>
#include <QPen>
#include <QCursor>
#include <QPoint>
#include <QLabel>
#include <QGraphicsProxyWidget>

const int eventItemYOffset = 1;

CustomCanttTimeBarItem::CustomCanttTimeBarItem(const QString textstr, const QDateTime &start, const QDateTime &end, double factor)
    : QGraphicsRectItem(nullptr),
      m_pFactor(factor),
      m_startDateTime(start),
      m_endDateTime(end)
{
    double width = (end.toMSecsSinceEpoch() - start.toMSecsSinceEpoch()) / m_pFactor;

    setRect(0, 0, width, customCanttTimeBarHeight);
    setCursor(QCursor(Qt::PointingHandCursor));
    setBrush(QBrush(QColor(92, 201, 221)));
    QPen pen;
    pen.setStyle(Qt::NoPen);
    setPen(pen);

    qint64 timeMillisecondsDiff = start.msecsTo(end);
    double timeSecondDiff = timeMillisecondsDiff/(double)1000;

    QLabel *label = new QLabel;
    label->setStyleSheet("QLabel {"
                         "background-color: transparent;"
                         "qproperty-alignment: AlignCenter;"
                         "min-height: 40px;"
                         "max-height: 40px;"
                         "font-size: 17px;"
                         "padding-left: 0px;"
                         "border-width: 0 0 0 0;"
                         "}");
    label->setText(QString("%1  %2s").arg(textstr).arg(QString::number(timeSecondDiff)));

    qint64 lablePosX = (width/2-40)>0?(width/2-40):0;

    QGraphicsProxyWidget *proxyItem = new QGraphicsProxyWidget(this);
    proxyItem->setWidget(label);
    proxyItem->setPos(lablePosX, eventItemYOffset);
}

