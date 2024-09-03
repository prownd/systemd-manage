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

#include "customsortfilterproxyudevmodel.h"

#include <QDebug>
#include <QRegularExpression>

CustomSortFilterProxyUdevModel::CustomSortFilterProxyUdevModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

CustomSortFilterProxyUdevModel::~CustomSortFilterProxyUdevModel()
{
}

void CustomSortFilterProxyUdevModel::initFilterMap(const QMap<filterUdevType, QString> &map)
{
    filtersMap.clear();

    for(QMap<filterUdevType, QString>::const_iterator iter = map.constBegin(); iter != map.constEnd(); ++iter)
    {
        filtersMap[iter.key()] = iter.value();
    }

}

void CustomSortFilterProxyUdevModel::addFilterRegExp(filterUdevType type, const QString &pattern)
{
    if (!filtersMap.contains(type)) {
        return;
    }

    filtersMap[type] = pattern;
}

bool CustomSortFilterProxyUdevModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if(filtersMap.isEmpty()) {
        return true;
    }

    bool ret = false;

    for(QMap<filterUdevType, QString>::const_iterator iter = filtersMap.constBegin(); iter != filtersMap.constEnd(); ++iter)
    {
        QModelIndex indexUdevType = sourceModel()->index(sourceRow, 2, sourceParent);
        QModelIndex indexUdevPath = sourceModel()->index(sourceRow, 3, sourceParent);

        if (iter.key() == udevType) {
            ret = (indexUdevType.data().toString().contains(QRegularExpression(iter.value())));
        } else if (iter.key() == udevPath) {
            ret = (indexUdevPath.data().toString().contains(QRegularExpression(iter.value())));
        }

        if(!ret) {
            return ret;
        }
    }

    return ret;
}

