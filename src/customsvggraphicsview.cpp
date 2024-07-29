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

#include "customsvggraphicsview.h"

CustomSvgGraphicsView::CustomSvgGraphicsView(QGraphicsScene *scene) : QGraphicsView(scene)
{
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setRenderHint(QPainter::TextAntialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //setInteractive(true);
    //setBackgroundBrush(QBrush(QColor(232,232,232), Qt::DiagCrossPattern));
    //setBackgroundBrush(Qt::NoBrush);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setOptimizationFlag(QGraphicsView::DontSavePainterState);
}

CustomSvgGraphicsView::~CustomSvgGraphicsView()
{

}

void CustomSvgGraphicsView::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.1;
    if (event->angleDelta().y() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        // Zoom out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void CustomSvgGraphicsView::resizeEvent(QResizeEvent *event)
{
    //fitInView(0, 0, width(), height(), Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
}

