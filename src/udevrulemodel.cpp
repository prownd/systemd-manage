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

#include "udevrulemodel.h"

#include <QColor>
#include <QIcon>


UdevRuleModel::UdevRuleModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

UdevRuleModel::UdevRuleModel(QObject *parent, const QList<UdevRuleFile> *list, int count)
    : QAbstractTableModel(parent)
{
    m_udevRuleFileList = list;
	m_columnCount = count;
}

int UdevRuleModel::rowCount(const QModelIndex &) const
{
    return m_udevRuleFileList->size();
}

int UdevRuleModel::columnCount(const QModelIndex &) const
{
    return m_columnCount;
}

QVariant UdevRuleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return tr("File Name");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 1) {
        return tr("File Path");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 2) {
        return tr("Modified");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 3) {
        return tr("Created Time");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 4) {
        return tr("Size (Byte)");
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 5) {
        return tr("Description");
    }
    return QVariant();
}

QVariant UdevRuleModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return m_udevRuleFileList->at(index.row()).fileName;
        else if (index.column() == 1)
            return m_udevRuleFileList->at(index.row()).filePath;
        else if (index.column() == 2)
            return m_udevRuleFileList->at(index.row()).lastModified;
        else if (index.column() == 3)
            return m_udevRuleFileList->at(index.row()).birthTime;
        else if (index.column() == 4)
            return m_udevRuleFileList->at(index.row()).fileSize;
        else if (index.column() == 4)
            return m_udevRuleFileList->at(index.row()).description;
    }
    else if (role == Qt::DecorationRole)
    {
        if (index.column() == 0) {
            if (m_udevRuleFileList->at(index.row()).fileName == QLatin1String("rule")) {
                return QIcon(QString(":/icons/emblem-success.svg"));
            } else if (m_udevRuleFileList->at(index.row()).fileName == QLatin1String("link")) {
                return QIcon(QString(":/icons/emblem-pause.svg"));
            } else {
                return QVariant();
            }
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        QString selFileName = m_udevRuleFileList->at(index.row()).fileName;
        QString selFilePath = m_udevRuleFileList->at(index.row()).filePath;
        QString selLastModified = m_udevRuleFileList->at(index.row()).lastModified;
        QString selBirthTime = m_udevRuleFileList->at(index.row()).birthTime;
        QString selFileSize = m_udevRuleFileList->at(index.row()).fileSize;
        QString selDescription = m_udevRuleFileList->at(index.row()).description;

        QString toolTipText;
        toolTipText.append("<FONT COLOR=DarkCyan>");

        toolTipText.append(tr("<b>File Name: </b>"));
        toolTipText.append(selFileName);
        toolTipText.append(tr("<br><b>File Path: </b>"));
        toolTipText.append(selFilePath);
        toolTipText.append(tr("<br><b>Modified: </b>"));
        toolTipText.append(selLastModified);
        toolTipText.append(tr("<br><b>Created Time: </b>"));
        toolTipText.append(selBirthTime);
        toolTipText.append(tr("<br><b>Size (Byte): </b>"));
        toolTipText.append(selFileSize);
        toolTipText.append(tr("<br><b>Description: </b>"));
        toolTipText.append(selDescription);

        toolTipText.append("</FONT");
        return toolTipText;
    }
    return QVariant();
}
