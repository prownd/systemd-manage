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

#include "customcanttchartview.h"
#include "customcanttchartscene.h"

#include <QWheelEvent>

CustomCanttChartView::CustomCanttChartView(QWidget *parent) : QGraphicsView(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    m_pScene = new CustomCanttChartScene(this);
    setScene(m_pScene);

    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    centerOn(0, 0);
}

void CustomCanttChartView::setAxisRange(const QStringList & tasks, const QDateTime &start, const QDateTime &end)
{
    m_pScene->setAxisRange(tasks, start, end);
}

CustomCanttTimeBarItem *CustomCanttChartView::addElapseTimeBar(const QString &task, const QDateTime &start, const QDateTime &end)
{
    return m_pScene->addElapseTimeBar(task, start, end);
}

void CustomCanttChartView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
    {
        zoomOut();
    }
    else
    {
        zoomIn();
    };
}

void CustomCanttChartView::zoomIn()
{
    scaleBy(1.0 / 1.1);
}

void CustomCanttChartView::zoomOut()
{
    scaleBy(1.1);
}

void CustomCanttChartView::scaleBy(double factor)
{
    scale(factor, factor);
}

void CustomCanttChartView::resizeEvent(QResizeEvent *event)
{
    //fitInView(0, 0, width(), height(), Qt::KeepAspectRatio);
    //QGraphicsView::resizeEvent(event);
}
