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

#ifndef CUSTOMCANTTCHARTSCENE_H
#define CUSTOMCANTTCHARTSCENE_H

#include <QGraphicsScene>
#include <QDateTime>
#include <QTime>
#include <QHash>
#include <QPair>
#include "customcantttimebaritem.h"

class CustomCanttChartScene : public QGraphicsScene
{
    Q_OBJECT
public:    
    explicit CustomCanttChartScene(QObject *parent = 0);
    void setAxisRange(const QStringList &tasks, const QDateTime &start, const QDateTime &end);
    CustomCanttTimeBarItem *addElapseTimeBar(const QString &task, const QDateTime &start, const QDateTime &end);
private:
    void drawGridLines();
    void drawVerticalAxis(const QStringList &tasks, bool checked=false);

    int m_rowCount;
    int m_columnCount;

    QDateTime m_startDateTime;
    QDateTime m_endDateTime;
    QTime m_stepTimeValue;
    QStringList m_tasks;

    int m_firstTimeBarStartX;
    int m_firstTimeBarStartY;
    double m_perPixelHMsecs;

    QHash<QString, double> m_taskStartYHash;
    QHash<QString, QPair<QDateTime, QDateTime>> m_planTimeBarTemp;
    QMultiHash<QString, QGraphicsItem*> m_plaformTimeBarHash;
};

#endif // CUSTOMCANTTCHARTSCENE_H
