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

#ifndef CUSTOMSVGGRAPHICSVIEW_H
#define CUSTOMSVGGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class CustomSvgGraphicsView : public QGraphicsView {
public:
    CustomSvgGraphicsView(QGraphicsScene *scene);
    ~CustomSvgGraphicsView();

protected:
    void wheelEvent(QWheelEvent *event) override;

    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // CUSTOMSVGGRAPHICSVIEW_H
