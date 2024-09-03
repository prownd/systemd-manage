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

#include "customsortfilterproxyudevrulefilemodel.h"

#include <QDebug>
#include <QRegularExpression>

CustomSortFilterProxyUdevRuleFileModel::CustomSortFilterProxyUdevRuleFileModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

CustomSortFilterProxyUdevRuleFileModel::~CustomSortFilterProxyUdevRuleFileModel()
{
}

void CustomSortFilterProxyUdevRuleFileModel::initFilterMap(const QMap<filterUdevRuleFileType, QString> &map)
{
    filtersMap.clear();

    for(QMap<filterUdevRuleFileType, QString>::const_iterator iter = map.constBegin(); iter != map.constEnd(); ++iter)
    {
        filtersMap[iter.key()] = iter.value();
    }

}

void CustomSortFilterProxyUdevRuleFileModel::addFilterRegExp(filterUdevRuleFileType type, const QString &pattern)
{
    if (!filtersMap.contains(type)) {
        return;
    }

    filtersMap[type] = pattern;
}

bool CustomSortFilterProxyUdevRuleFileModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if(filtersMap.isEmpty()) {
        return true;
    }

    bool ret = false;

    for(QMap<filterUdevRuleFileType, QString>::const_iterator iter = filtersMap.constBegin(); iter != filtersMap.constEnd(); ++iter)
    {
        QModelIndex indexUdevRuleFileName = sourceModel()->index(sourceRow, 0, sourceParent);
        QModelIndex indexUdevRuleFilePath = sourceModel()->index(sourceRow, 1, sourceParent);

        if (iter.key() == udevRuleFileName) {
            ret = (indexUdevRuleFileName.data().toString().contains(QRegularExpression(iter.value())));
        } else if (iter.key() == udevRuleFilePath) {
            ret = (indexUdevRuleFilePath.data().toString().contains(QRegularExpression(iter.value())));
        }

        if(!ret) {
            return ret;
        }
    }

    return ret;
}

