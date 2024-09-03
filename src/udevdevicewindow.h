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

#ifndef UDEVDEVICEWINDOW_H
#define UDEVDEVICEWINDOW_H

#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>

#include "customsortfilterproxyudevmodel.h"
#include "customsortfilterproxyudevrulefilemodel.h"

#include "qudev.h"
#include "udevmodel.h"
#include "mystyleditemdelegate.h"
#include "udevrulefile.h"
#include "udevrulemodel.h"


class UdevDeviceWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UdevDeviceWindow(QWidget *parent = nullptr);
    ~UdevDeviceWindow();

    void initData();
    void createTabWidgetView();
    void createUdevDeviceTableView();
    void createUdevRuleFileTableView();

private:
    QTabWidget * m_pTabWidget;
    QWidget * m_pUdevDeviceWidget;
    QVBoxLayout *m_subUdevDeviceLayout;

    QWidget * m_pUdevRuleFileWidget;
    QVBoxLayout *m_subUdevRuleFileLayout;
    QLabel* m_udevRuleFileCountLabel;

    QBoxLayout* m_mainLayout1;

    QStandardItemModel *m_udevDeviceStandItemModel;
    QTableView *m_udevDeviceTableView;

    QComboBox *     m_udevDeviceTypeComboBox;
    QLineEdit *     m_searchUdevDeviceLineEdit;
    QHBoxLayout *   m_filterUdevDeviceHLayout;

    QLabel *        m_udevDeviceCountLabel;
    QHBoxLayout *   m_udevDeviceCountHLayout;


    CustomSortFilterProxyUdevModel * m_udevDeviceFilterModel;

    UdevDevice * m_udevDevice;
    QList<UdevDevice> m_udevDeviceList;
    UdevModel * m_udevDeviceModel;

    void setupUdevDevicelist();

    MyStyledItemDelegate * m_pTableViewItemDelegate;

    int m_udevDeviceColumnCount;

    QStringList m_udevTypeSufix = QStringList{  QString(), QStringLiteral("disk"),
                                                QStringLiteral("partition"),
                                                QStringLiteral("usb_device"), QStringLiteral("usb_interface"),
                                                QStringLiteral("port"),QStringLiteral("wlan")};

private Q_SLOTS:
    void slotRefreshUdevDeviceList(bool inital);

    void handleUdevDeviceInfoAction();
    void handleUdevDeviceRefreshAction();

    void updateUdevDeviceCount();

    void slotComboBoxShowUdevDevice(int index);

    void slotLineEditSearchUdevDeviceChanged(QString term);

public Q_SLOTS:
    void slotUdevDeviceTableRowDoubleClicked(const QModelIndex);
    void slotUdevDeviceTableRowTooltip(const QModelIndex);
    void udevDeviceCustomMenuRequested(QPoint pos);


public:
    void listFilesInDirectory(const QString &path, const QString globstr = "*.rules");
    void getAllUdevRuleFiles();
    void updateUdevRuleFileCount();

private:
    QStandardItemModel *m_udevRuleFileItemModel;
    UdevRuleModel *m_udevRuleFileModel;

    int m_udevRuleFileColumnCount;

    QTableView * m_udevRuleFileTableView;
    QList<UdevRuleFile> m_udevRuleFileList;

    QHBoxLayout * m_hUdevRuleBoxLayout ;
    QHBoxLayout *m_filterUdevRuleFileHLayout;

    QLabel *  m_searchUdevRuleFileLabel;
    QLineEdit * m_searchUdevRuleFileLineEdit;
    CustomSortFilterProxyUdevRuleFileModel * m_udevRuleFileFilterModel;

protected Q_SLOTS:
    void handleUdevRuleFileStatusAction();
    void handleUdevRuleFileRefreshAction();
    void udevRuleFileCustomMenuRequested(QPoint pos);
    void slotUdevRuleFileTableRowDoubleClicked(const QModelIndex);
    void slotUdevRuleFileTableRowTooltip(const QModelIndex index);
    void slotRefreshUdevRuleFileList();
    void slotLineEditSearchUdevRuleFileChanged(QString term);

protected:
    //auto adjust window size
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // UDEVDEVICEWINDOW_H
