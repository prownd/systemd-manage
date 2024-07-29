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

#include "customcanttchartscene.h"
#include "customcantttimebaritem.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QCheckBox>
#include <QGraphicsProxyWidget>
#include <QCursor>

const int firstHorizantalGridWidth = 0;
const int horizontalGridWidth = 80;
const int verticalGridHeight = 40;
const int horizontalAxisTextHeight = 40;
const int horizontalAxisTextOffset = 5;
const QPoint axisStartPoint = QPoint(20, 40);
const QPoint taskHeaderOffset = QPoint(6, 10);
const QColor gridLineColor = QColor(48, 85, 93);
const QColor scaleDateColor = QColor(20, 21, 21);
const QColor scaleTimeColor = QColor(20, 21, 21);

CustomCanttChartScene::CustomCanttChartScene(QObject *parent) : QGraphicsScene(parent),
    m_rowCount(0), m_columnCount(0), m_stepTimeValue(0, 0, 1)
{
    setBackgroundBrush(QBrush(QColor(248, 248, 248)));
    m_perPixelHMsecs =  m_stepTimeValue.msecsSinceStartOfDay() / (double)horizontalGridWidth;
}

void CustomCanttChartScene::setAxisRange(const QStringList &tasks, const QDateTime &start, const QDateTime &end)
{
    if (start >= end || 0 == tasks.count())
    {
        return;
    }

    m_rowCount = tasks.count();
    m_startDateTime = start;
    m_endDateTime = end;
    m_tasks = tasks;
    m_firstTimeBarStartX = axisStartPoint.x() + firstHorizantalGridWidth;
    m_firstTimeBarStartY = axisStartPoint.y();

    //clear current
    clear();

    //reserve before paint
    //double sceneMiniWidth = m_firstTimeBarStartX + horizontalGridWidth
    //        + (end.toMSecsSinceEpoch() - start.toMSecsSinceEpoch()) / m_perPixelHMsecs;

    double sceneMiniWidth = m_firstTimeBarStartX + (end.toMSecsSinceEpoch() - start.toMSecsSinceEpoch()) / m_perPixelHMsecs;
    double sceneMiniHeight = m_firstTimeBarStartY + tasks.count() * verticalGridHeight;

    setSceneRect(0, 0, sceneMiniWidth, sceneMiniHeight + 800);

    drawVerticalAxis(tasks);

    QDateTime startDateTime = start;
    QDate startDate = start.date();

    double x = m_firstTimeBarStartX;
    for (; x <= sceneMiniWidth; x += horizontalGridWidth)
    {
        QGraphicsTextItem *timeItem = new QGraphicsTextItem(startDateTime.toString("hh:mm:ss"));
        timeItem->setDefaultTextColor(scaleTimeColor);
        timeItem->setZValue(std::numeric_limits<int>::min());
        timeItem->setPos(x - horizontalAxisTextOffset, axisStartPoint.y() - horizontalAxisTextHeight);
        addItem(timeItem);

        if (x == axisStartPoint.x() + firstHorizantalGridWidth)
        {
            QGraphicsTextItem *dateItem = new QGraphicsTextItem(startDateTime.date().toString("yyyy-MM-dd"));
            dateItem->setDefaultTextColor(scaleDateColor);
            dateItem->setZValue(std::numeric_limits<int>::min());
            addItem(dateItem);
            dateItem->setPos(x - horizontalAxisTextOffset, axisStartPoint.y() - horizontalAxisTextHeight*2);
        }
        else
        {
            if (startDateTime.date() > startDate)
            {
                QGraphicsTextItem *dateItem = new QGraphicsTextItem(startDateTime.date().toString("yyyy-MM-dd"));
                dateItem->setDefaultTextColor(scaleDateColor);
                dateItem->setZValue(std::numeric_limits<int>::min());
                addItem(dateItem);
                dateItem->setPos(x - horizontalAxisTextOffset, axisStartPoint.y() - horizontalAxisTextHeight*2);
                startDate = startDateTime.date();
            }
        }

        startDateTime = startDateTime.addMSecs(m_stepTimeValue.msecsSinceStartOfDay());

        m_columnCount++;

        if (startDateTime > QDateTime::currentDateTime())
        {
            // no break;
        }
    }

    drawGridLines();

    QRectF rect = this->sceneRect();
    setSceneRect(0, 0, rect.width() + 200, rect.height() + 200);
}

void CustomCanttChartScene::drawVerticalAxis(const QStringList &tasks, bool checked)
{
    bool needCheckBox = false;

    if (tasks.count() == 0)
    {
        return;
    }

    //const double maxY = this->height();
    const double maxY = (m_rowCount + 1 )  * verticalGridHeight;

    //paint table vertical header
    int index = 0;
    for (double y = axisStartPoint.y(); y <= maxY; y += verticalGridHeight)
    {
        if (index > tasks.count() - 1)
        {
            break;
        }

        if (needCheckBox)
        {
            QCheckBox *box = new QCheckBox;
            box->setObjectName("taskCheckBox");
            box->setStyleSheet("#taskCheckBox {"
                               "color: rgb(25, 28, 25);"
                               "background-color: rgb(248, 248, 248);"
                               "}"
                              );
            if (checked){
                connect(box, &QCheckBox::clicked, [=](bool checked) {
                    auto list = m_plaformTimeBarHash.values(box->text());

                    if (checked)
                    {
                        foreach (QGraphicsItem *item, list)
                        {
                            item->hide();
                        }
                    }
                    else
                    {
                        foreach (QGraphicsItem *item, list)
                        {
                            item->show();
                        }
                    }
                });
            }

            box->setText(tasks.at(index));
            box->setEnabled(checked);
            box->hide();
            QGraphicsProxyWidget *proxy = addWidget(box);
            proxy->setCursor(QCursor(Qt::PointingHandCursor));
            proxy->setPos(QPoint(axisStartPoint.x(), y) + taskHeaderOffset);
        }
        m_taskStartYHash.insert(tasks.at(index), y);
        index++;
    }
}

CustomCanttTimeBarItem *CustomCanttChartScene::addElapseTimeBar(const QString &task, const QDateTime &start, const QDateTime &end)
{
    if (!m_taskStartYHash.keys().contains(task))
    {
        return nullptr;
    }

    //add cache
    auto pair = qMakePair(start, end);
    m_planTimeBarTemp.insert(task, pair);

    //paint timebar item
    CustomCanttTimeBarItem *item = new CustomCanttTimeBarItem(task, start, end, m_perPixelHMsecs);

    double x = m_firstTimeBarStartX + (start.toMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch()) / m_perPixelHMsecs;
    double y = m_taskStartYHash.value(task) + 3;

    addItem(item);
    item->setPos(x, y);

    m_plaformTimeBarHash.insert(task, item);

    return item;
}

void CustomCanttChartScene::drawGridLines()
{
    //const double maxY = this->height();
    const double maxY = (m_rowCount + 1 )  * verticalGridHeight;
    const double maxX = m_firstTimeBarStartX + m_columnCount * horizontalGridWidth;

    //paint first x grid line
    QGraphicsLineItem *item = new QGraphicsLineItem(axisStartPoint.x(), axisStartPoint.y(), axisStartPoint.x(), maxY);
    item->setPen(QPen(gridLineColor));
    item->setZValue(std::numeric_limits<int>::min());
    addItem(item);

    //paint x grid line
    for (double x = axisStartPoint.x() + firstHorizantalGridWidth; x <= maxX; x += horizontalGridWidth)
    {
        QGraphicsLineItem *item = new QGraphicsLineItem(x, axisStartPoint.y(), x, maxY);
        item->setPen(QPen(gridLineColor));
        item->setZValue(std::numeric_limits<int>::min());
        addItem(item);
    }

    //paint y line
    for (double y = axisStartPoint.y(); y <= maxY; y += verticalGridHeight)
    {
        QGraphicsLineItem *item = new QGraphicsLineItem(axisStartPoint.x(), y, maxX, y);
        item->setPen(QPen(gridLineColor));
        item->setZValue(std::numeric_limits<int>::min());
        addItem(item);
    }
}

