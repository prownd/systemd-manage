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

#ifndef CUSTOMCANTTCHARTVIEW_H
#define CUSTOMCANTTCHARTVIEW_H

#include <QGraphicsView>
#include <QDateTime>
#include "customcanttchartscene.h"
#include "customcantttimebaritem.h"

class CustomCanttChartView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CustomCanttChartView(QWidget *parent = 0);

    void setAxisRange(const QStringList &tasks, const QDateTime &start, const QDateTime &end);
    CustomCanttTimeBarItem *addElapseTimeBar(const QString &task, const QDateTime &start, const QDateTime &end);
protected:
    virtual void wheelEvent(QWheelEvent *) override;

    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
private slots:
    void zoomIn();
    void zoomOut();
private:
    void scaleBy(double factor);
private:
    CustomCanttChartScene *m_pScene;
};

#endif // CUSTOMCANTTCHARTVIEW_H
