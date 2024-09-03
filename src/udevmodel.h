/*******************************************************************************
 * Copyright (C) 2016 Ragnar Thomsen <rthomsen6@gmail.com>                     *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU General Public License as published by the Free  *
 * Software Foundation, either version 2 of the License, or (at your option)   *
 * any later version.                                                          *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
 * more details.                                                               *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program. If not, see <http://www.gnu.org/licenses/>.              *
 *******************************************************************************/

#ifndef UDEVMODEL_H
#define UDEVMODEL_H

#include <QAbstractTableModel>

#include "qudev.h"

class UdevModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit UdevModel(QObject *parent = nullptr);
    explicit UdevModel(QObject *parent = nullptr, const QList<UdevDevice> *list = NULL, int  count = 1);
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

private:
    const QList<UdevDevice> *m_udevDeviceList;
    int m_columnCount;

    const QString m_connSystemd = QStringLiteral("org.freedesktop.systemd1");

};

#endif // UNITMODEL_H

