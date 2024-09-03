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

#ifndef CUSTOMSORTFILTERPROXYUDEVMODEL_H
#define CUSTOMSORTFILTERPROXYUDEVMODEL_H

#include <QSortFilterProxyModel>

enum filterUdevType
{
    udevType, udevPath
};

class CustomSortFilterProxyUdevModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit CustomSortFilterProxyUdevModel(QObject *parent = nullptr);
    void initFilterMap(const QMap<filterUdevType, QString> &map);
    void addFilterRegExp(filterUdevType type, const QString &pattern);
    virtual ~CustomSortFilterProxyUdevModel();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QMap<filterUdevType, QString> filtersMap;
};

#endif // CUSTOMSORTFILTERPROXYUDEVMODEL_H
