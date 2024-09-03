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

#include "udevmodel.h"

#include <QColor>
#include <QIcon>


UdevModel::UdevModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

UdevModel::UdevModel(QObject *parent, const QList<UdevDevice> *list, int count)
    : QAbstractTableModel(parent)
{
    m_udevDeviceList = list;
	m_columnCount = count;
}

int UdevModel::rowCount(const QModelIndex &) const
{
    return m_udevDeviceList->size();
}

int UdevModel::columnCount(const QModelIndex &) const
{
    return m_columnCount;
}

QVariant UdevModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return tr("Device Node");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 1) {
        return tr("Device Subsystem");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 2) {
        return tr("Device Type");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 3) {
        return tr("Device Path");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 4) {
        return tr("Device Action");
    }
    return QVariant();
}

QVariant UdevModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return m_udevDeviceList->at(index.row()).deviceNode();
        else if (index.column() == 1)
            return m_udevDeviceList->at(index.row()).deviceSubsystem();
        else if (index.column() == 2)
            return m_udevDeviceList->at(index.row()).deviceType();
        else if (index.column() == 3)
            return m_udevDeviceList->at(index.row()).devicePath();
        else if (index.column() == 4)
            return m_udevDeviceList->at(index.row()).deviceAction();
    }
    else if (role == Qt::DecorationRole)
    {
        if (index.column() == 0) {
            if (m_udevDeviceList->at(index.row()).deviceType() == QLatin1String("disk")) {
                return QIcon(QString(":/icons/emblem-success.svg"));
            } else if (m_udevDeviceList->at(index.row()).deviceType() == QLatin1String("net")) {
                return QIcon(QString(":/icons/emblem-pause.svg"));
            } else {
                return QVariant();
            }
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        QString selDeviceNode = m_udevDeviceList->at(index.row()).deviceNode();
        QString selDeviceSubsystem = m_udevDeviceList->at(index.row()).deviceSubsystem();
        QString selDeviceType = m_udevDeviceList->at(index.row()).deviceType();
        QString selDevicePath = m_udevDeviceList->at(index.row()).devicePath();
        QString selDeviceAction = m_udevDeviceList->at(index.row()).deviceAction();

        QString toolTipText;
        toolTipText.append("<FONT COLOR=DarkCyan>");

        toolTipText.append(tr("<b>Device Node: </b>"));
        toolTipText.append(selDeviceNode);
        toolTipText.append(tr("<br><b>Device Subsystem: </b>"));
        toolTipText.append(selDeviceSubsystem);
        toolTipText.append(tr("<br><b>Device Type: </b>"));
        toolTipText.append(selDeviceType);
        toolTipText.append(tr("<br><b>Device Path: </b>"));
        toolTipText.append(selDevicePath);
        toolTipText.append(tr("<br><b>Device Action: </b>"));
        toolTipText.append(selDeviceAction);

        toolTipText.append("</FONT");
        return toolTipText;
    }
    return QVariant();
}
