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

#ifndef UDEVRULEMODEL_H
#define UDEVRULEMODEL_H

#include <QAbstractTableModel>
#include "udevrulefile.h"

class UdevRuleModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit UdevRuleModel(QObject *parent = nullptr);
    explicit UdevRuleModel(QObject *parent = nullptr, const QList<UdevRuleFile> *list = NULL, int  count = 1);
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

private:
    const QList<UdevRuleFile> *m_udevRuleFileList;
    int m_columnCount;
};

#endif // UDEVRULEMODEL_H

