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

#ifndef MYSTYLEDITEMDELEGATE_H
#define MYSTYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>
#include<map>
using std::map;

class MyStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit MyStyledItemDelegate(QObject *parent = nullptr);

public:

    /* Set column width for QTableView, no effective for QTreeView、QTableWidget
      */
    void setColumnWidth(int columnIndex, int columnWidth);

protected:

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    map<int, int> m_mpColumnWidth;

};

#endif // MYSTYLEDITEMDELEGATE_H
