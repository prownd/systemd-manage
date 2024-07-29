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

#include "unitmodel.h"

#include <QColor>
#include <QIcon>
#include <QtDBus/QtDBus>


UnitModel::UnitModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

UnitModel::UnitModel(QObject *parent, const QVector<SystemdUnit> *list, int count)
    : QAbstractTableModel(parent)
{
    m_unitList = list;
	m_columnCount = count;
}

UnitModel::UnitModel(QObject *parent, const QVector<SystemdUnit> *list, QString userBusPath,int count)
    : QAbstractTableModel(parent)
{
    m_unitList = list;
    m_userBus = userBusPath;
    m_columnCount = count;
}

int UnitModel::rowCount(const QModelIndex &) const
{
    return m_unitList->size();
}

int UnitModel::columnCount(const QModelIndex &) const
{
    return m_columnCount;
}

QVariant UnitModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return tr("Unit");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 1) {
        return tr("Load State");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 2) {
        return tr("Active State");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 3) {
        return tr("Unit State");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 4) {
        return tr("Job");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 5) {
        return tr("Description");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 6) {
        return tr("Unit file");
    }
    return QVariant();
}

QVariant UnitModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return m_unitList->at(index.row()).id;
        else if (index.column() == 1)
            return m_unitList->at(index.row()).load_state;
        else if (index.column() == 2)
            return m_unitList->at(index.row()).active_state;
        else if (index.column() == 3)
            return m_unitList->at(index.row()).sub_state;
        else if (index.column() == 4)
            return m_unitList->at(index.row()).job_id;
        else if (index.column() == 5)
            return m_unitList->at(index.row()).description;
        else if (index.column() == 6)
            return m_unitList->at(index.row()).unit_file;
    }

    else if (role == Qt::DecorationRole)
    {
        if (index.column() == 0) {
            if (m_unitList->at(index.row()).active_state == QLatin1String("active")) {
                //return QIcon::fromTheme(QStringLiteral("emblem-success"));
                return QIcon(QString(":/icons/emblem-success.svg"));
            } else if (m_unitList->at(index.row()).active_state == QLatin1String("inactive")) {
                //return QIcon::fromTheme(QStringLiteral("emblem-pause"));
                return QIcon(QString(":/icons/emblem-pause.svg"));
            } else if (m_unitList->at(index.row()).active_state == QLatin1String("failed")) {
                //return QIcon::fromTheme(QStringLiteral("emblem-error"));
                return QIcon(QString(":/icons/emblem-error.svg"));
            } else if (m_unitList->at(index.row()).active_state == QLatin1String("-")) {
                //return QIcon::fromTheme(QStringLiteral("emblem-unavailable"));
                return QIcon(QString(":/icons/emblem-unavailable.svg"));
            } else {
                return QVariant();
            }
        }
    }

    return QVariant();
}
