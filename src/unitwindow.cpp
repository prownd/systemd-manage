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

#include "unitwindow.h"
#include <QDBusMetaType>
#include <QVBoxLayout>
#include <QProcess>
#include <unistd.h>
#include <QMenu>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QApplication>
#include <QToolTip>

#include "unitinformation.h"

UnitWindow::UnitWindow(QWidget *parent) :
    QWidget(parent),
    m_systemdManagerInterface(new SystemdManagerInterface)
{
    //frameless window
    setWindowFlags(Qt::FramelessWindowHint);

    qDBusRegisterMetaType<SystemdUnit>();
    initData();
    slotRefreshUnitsList(true, sys);
    slotRefreshUnitsList(true, user);
    createTabWidgetView();
    createSystemUnitTableView();
    createUserUnitTableView();
    createTimerTableView();
    show();
}

UnitWindow::~UnitWindow()
{
}

void UnitWindow::initData()
{
    m_systemUnitColumnCount = 8;
    m_userUnitColumnCount = 7;
    m_timerColumnCount = 7;
}

void UnitWindow::createTabWidgetView()
{
    // init tabwidget
    m_pTabWidget = new QTabWidget(this);
    m_pTabWidget->setMinimumWidth(width());
    m_pTabWidget->setMinimumHeight(height());
    m_pTabWidget->adjustSize();

    // init unit subwidget
    m_pSystemUnitWidget = new QWidget();
    m_subSystemUnitLayout = new QVBoxLayout(m_pSystemUnitWidget);
    m_pSystemUnitWidget->setLayout(m_subSystemUnitLayout);

    // init session subwidget
    m_pUserUnitWidget = new QWidget();
    m_subUserUnitLayout = new QVBoxLayout(m_pUserUnitWidget);
    m_pUserUnitWidget->setLayout(m_subUserUnitLayout);

    // init timer subwidget
    m_pTimerWidget = new QWidget();
    m_subTimerLayout = new QVBoxLayout(m_pTimerWidget);
    m_pTimerWidget->setLayout(m_subTimerLayout);

    // add all sub widget
    m_pTabWidget->addTab(m_pSystemUnitWidget, tr("System Unit"));
    m_pTabWidget->addTab(m_pUserUnitWidget, tr("User Unit"));
    m_pTabWidget->addTab(m_pTimerWidget, tr("Timer"));
}

void UnitWindow::createSystemUnitTableView()
{
    m_filterSystemUnitHLayout = new  QHBoxLayout;

    QStringList p_filterSystemUnitTypes = QStringList{tr("All"), tr("Services"),tr("Devices"),
                                                      tr("Automounts"),tr("Mounts"), tr("Paths"),
                                                      tr("Scopes"),tr("Slices"),tr("Sockets"),
                                                      tr("Swaps"), tr("Targets"), tr("Timers")};
    m_systemUnitTypeComboBox = new QComboBox;
    m_systemUnitTypeComboBox->addItems(p_filterSystemUnitTypes);

    m_inactiveSystemUnitCheckBox = new QCheckBox(this);
    //m_inactiveSystemUnitCheckBox->setTristate(true);
    m_inactiveSystemUnitCheckBox->setCheckState(Qt::Checked);
    m_inactiveSystemUnitCheckBox->setCheckable(true);
    m_inactiveSystemUnitCheckBox->setText(tr("Show inactive"));

    m_unloadSystemUnitCheckBox = new QCheckBox(this);
    //m_unloadSystemUnitCheckBox->setTristate(true);
    m_unloadSystemUnitCheckBox->setCheckState(Qt::Checked);
    m_unloadSystemUnitCheckBox->setCheckable(true);
    m_unloadSystemUnitCheckBox->setText(tr("Show unload"));

    m_searchSystemUnitLineEdit = new QLineEdit;

    m_filterSystemUnitHLayout->addWidget(m_systemUnitTypeComboBox);
    m_filterSystemUnitHLayout->addWidget(m_inactiveSystemUnitCheckBox);
    m_filterSystemUnitHLayout->addWidget(m_unloadSystemUnitCheckBox);
    m_filterSystemUnitHLayout->addStretch(0);
    m_filterSystemUnitHLayout->addWidget(m_searchSystemUnitLineEdit);

    m_subSystemUnitLayout->addLayout(m_filterSystemUnitHLayout);

    m_systemUnitTableView= new QTableView(m_pSystemUnitWidget);
    m_systemUnitStandItemModel = new QStandardItemModel();

    qDBusRegisterMetaType<SystemdUnit>();
    m_systemUnitModel = new UnitModel(this, &m_systemUnitsList, m_systemUnitColumnCount);

    QMap<filterType, QString> filters;
    filters[activeState] = QString();
    filters[unitType] = QString();
    filters[unitName] = QString();
    m_systemUnitFilterModel = new CustomSortFilterProxyModel(this);
    m_systemUnitFilterModel->setDynamicSortFilter(false);
    m_systemUnitFilterModel->initFilterMap(filters);
    m_systemUnitFilterModel->setSourceModel(m_systemUnitModel);

    //m_systemUnitTableView->setModel(m_systemUnitModel);
    m_systemUnitTableView->setModel(m_systemUnitFilterModel);
    //tableView->setModel(standItemModel);
    m_systemUnitTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //m_systemUnitTableView->horizontalHeader()->setDefaultSectionSize(80);
    m_systemUnitTableView->verticalHeader()->setDefaultSectionSize(40);
    m_systemUnitTableView->resizeColumnsToContents();
    m_systemUnitTableView->horizontalHeader()->setStretchLastSection(true);
    m_systemUnitTableView->setColumnWidth(0,600);
    //tableView->setColumnWidth(1,200);
    m_systemUnitTableView->verticalHeader()->hide();
    m_systemUnitTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_systemUnitTableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_systemUnitTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
       m_systemUnitTableView->setColumnHidden(m_systemUnitColumnCount-2, true);
    m_systemUnitTableView->setColumnHidden(m_systemUnitColumnCount-1, true);
    m_systemUnitTableView->setMinimumWidth(width());
    m_systemUnitTableView->setMinimumHeight(height());
    m_systemUnitTableView->adjustSize();
    m_systemUnitTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_systemUnitTableView->setMouseTracking(true);

    m_subSystemUnitLayout->addWidget(m_systemUnitTableView);

    //create contextMenu
    m_systemUnitTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_systemUnitTableView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(systemUnitCustomMenuRequested(QPoint)));

    m_systemUnitCountLabel = new QLabel;
    m_systemUnitCountLabel->setText(tr("total unit : 18, active unit:10, inactive unit:10 "));
    m_subSystemUnitLayout->addWidget(m_systemUnitCountLabel);

    connect(m_systemUnitTypeComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &UnitWindow::slotComboBoxShowSystemUnits);

    connect(m_inactiveSystemUnitCheckBox, &QCheckBox::stateChanged, this, &UnitWindow::slotCheckBoxShowSystemUnits);
    connect(m_unloadSystemUnitCheckBox, &QCheckBox::stateChanged, this, &UnitWindow::slotCheckBoxShowSystemUnits);

    connect(m_searchSystemUnitLineEdit, &QLineEdit::textChanged, this, &UnitWindow::slotLineEditSearchSystemUnitChanged);

    connect(m_systemUnitTableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotSystemUnitTableRowDoubleClicked(const QModelIndex &)));
    connect(m_systemUnitTableView, SIGNAL(entered(QModelIndex)), this, SLOT(slotSystemUnitTableRowTooltip(QModelIndex)));

    slotCheckBoxShowSystemUnits(-1);
}

void UnitWindow::slotSystemUnitTableRowTooltip(const QModelIndex index)
{
    /*
    int curRow=index.row();
    const QModelIndex& curCellIndex = m_systemUnitFilterModel->index(curRow,0);
    QString systemUnitName = m_systemUnitFilterModel->data(curCellIndex).toString();
    const QModelIndex& curCellIndex2 = m_systemUnitFilterModel->index(curRow, m_systemUnitColumnCount-2);
    QString systemUnitFile = m_systemUnitFilterModel->data(curCellIndex2).toString();
    const QModelIndex& curCellIndex3 = m_systemUnitFilterModel->index(curRow, m_systemUnitColumnCount-1);
    QString systemUnitPath = m_systemUnitFilterModel->data(curCellIndex3).toString();

    QDBusObjectPath unit_path(systemUnitPath);
    QString  unitPropertyDescriptionStr =
        m_systemdManagerInterface->getDbusProperty(QStringLiteral("Description"), sysdUnit, unit_path, sys).toString();
    QString  unitPropertyIDStr =
        m_systemdManagerInterface->getDbusProperty(QStringLiteral("ID"), sysdUnit, unit_path, sys).toString();
    QString  unitPropertyActiveStateStr =
        m_systemdManagerInterface->getDbusProperty(QStringLiteral("ActiveState"), sysdUnit, unit_path, sys).toString();

    QVariant  unitPropertyRequiresStr =
        m_systemdManagerInterface->getDbusProperty(QStringLiteral("Requires"), sysdUnit, unit_path, sys).toString();
    qDebug()<<"unitPropertyRequiresStr:"<< unitPropertyRequiresStr;

    // QVariantList list = unitPropertyRequiresStr.toList();
    // foreach(QVariant e, list ){  qDebug()<< e.toString(); }

    QString toolTipText;
    toolTipText.append(QStringLiteral("<FONT COLOR=DarkCyan>"));
    toolTipText.append(QStringLiteral("<b>Unit Name:%1</b><hr>").arg(systemUnitName));
    toolTipText.append(QStringLiteral("<b>Unit File:%1</b><hr>").arg(systemUnitFile));
    toolTipText.append(QStringLiteral("<b>Unit Description:%1</b><hr>").arg(unitPropertyDescriptionStr));
    toolTipText.append(QStringLiteral("<b>Unit ID:%1</b><hr>").arg(unitPropertyIDStr));
    toolTipText.append(QStringLiteral("<b>Unit ID:%1</b><hr>").arg(unitPropertyActiveStateStr));
    toolTipText.append(QStringLiteral("</FONT"));
    */

    //m_systemUnitFilterModel->itemFromIndex(index)->setToolTip(toolTipText);
    //QToolTip::showText(QCursor::pos(), toolTipText, this, QRect(), -1);

    return;
}

void UnitWindow::slotSystemUnitTableRowDoubleClicked(const QModelIndex index)
{
    QModelIndexList selectedIndexes = m_systemUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        //int curRow = index.row();
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_systemUnitFilterModel->index(curRow,0);
        QString systemUnitName = m_systemUnitFilterModel->data(curCellIndex).toString();
        //qDebug()<<"cell data 1:"<< systemUnitName;

        const QModelIndex& curCellIndex2 = m_systemUnitFilterModel->index(curRow, m_systemUnitColumnCount-2);
        QString systemUnitFile = m_systemUnitFilterModel->data(curCellIndex2).toString();

        UnitInformation * unitInformationWnd = new UnitInformation(systemUnitName, systemUnitFile, this);//only pointer ,not object instance
        unitInformationWnd->setWindowTitle(QObject::tr("Show system unit config info"));
        unitInformationWnd->setWindowModality(Qt::ApplicationModal);
        unitInformationWnd->setAttribute(Qt::WA_ShowModal, true);
        unitInformationWnd->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        unitInformationWnd->show();
    }
}

void UnitWindow::systemUnitCustomMenuRequested(QPoint pos)
{
    QModelIndexList selectedIndexes = m_systemUnitTableView->selectionModel()->selectedRows();

    if (!selectedIndexes.isEmpty())
    {
        QModelIndex clickedIndex = m_systemUnitTableView->indexAt(pos);   // get index when right click
        if (clickedIndex.isValid()){ // if index valid, continue
            QMenu *systemUnitMenu = new QMenu(this);

            QAction*  systemUnitStatusAction = new QAction(tr("System Unit Status"), this);
            systemUnitMenu->addAction(systemUnitStatusAction);

            systemUnitMenu->addSeparator();

            QAction* systemUnitStartAction = new QAction(tr("Run System Unit"), this);
            systemUnitMenu->addAction(systemUnitStartAction);

            QAction* systemUnitStopAction = new QAction(tr("Stop System Unit"), this);
            systemUnitMenu->addAction(systemUnitStopAction);

            QAction* systemUnitRestartAction = new QAction(tr("Restart System Unit"), this);
            systemUnitMenu->addAction(systemUnitRestartAction);

            systemUnitMenu->addSeparator();

            QAction* systemUnitEnableAction = new QAction(tr("Enable System Unit"), this);
            systemUnitMenu->addAction(systemUnitEnableAction);

            QAction* systemUnitDisableAction = new QAction(tr("Disable System Unit"), this);
            systemUnitMenu->addAction(systemUnitDisableAction);

            systemUnitMenu->addSeparator();

            QAction* systemUnitRefreshAction = new QAction(tr("Refresh System Unit"), this);
            systemUnitMenu->addAction(systemUnitRefreshAction);

            // unit status info
            connect(systemUnitStatusAction, &QAction::triggered, this, &UnitWindow::handleSystemUnitStatusAction);
            // start server unit
            connect(systemUnitStartAction, &QAction::triggered, this, &UnitWindow::handleSystemUnitStartAction);
            // stop server unit
            connect(systemUnitStopAction, &QAction::triggered, this, &UnitWindow::handleSystemUnitStopAction);
            // restart server unit
            connect(systemUnitRestartAction, &QAction::triggered, this, &UnitWindow::handleSystemUnitRestartAction);
            // enable server unit
            connect(systemUnitEnableAction, &QAction::triggered, this, &UnitWindow::handleSystemUnitEnableAction);
            // disable server unit
            connect(systemUnitDisableAction, &QAction::triggered, this, &UnitWindow::handleSystemUnitDisableAction);
            // refresh server unit
            connect(systemUnitRefreshAction, &QAction::triggered, this, &UnitWindow::handleSystemUnitRefreshAction);
            systemUnitMenu->popup(m_systemUnitTableView->viewport()->mapToGlobal(pos));
        }
    }
}

void UnitWindow::handleSystemUnitStatusAction()
{
    QModelIndexList selectedIndexes = m_systemUnitTableView->selectionModel()->selectedRows();
    //foreach(const QModelIndex& index, selectedIndexes)
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        //const QModelIndex& curCellIndex = m_systemUnitModel->index(curRow,0);
        //QString systemUnitName = m_systemUnitModel->data(curCellIndex).toString();
        const QModelIndex& curCellIndex = m_systemUnitFilterModel->index(curRow,0);
        QString systemUnitName = m_systemUnitFilterModel->data(curCellIndex).toString();
        //qDebug()<<"cell data:"<< systemUnitName;

        //const QModelIndex& curCellIndex2 = m_systemUnitModel->index(curRow, m_systemUnitColumnCount-1);
        //QString systemUnitFile = m_systemUnitModel->data(curCellIndex2).toString();
        const QModelIndex& curCellIndex2 = m_systemUnitFilterModel->index(curRow, m_systemUnitColumnCount-2);
        QString systemUnitFile = m_systemUnitFilterModel->data(curCellIndex2).toString();

        UnitInformation * unitInformationWnd = new UnitInformation(systemUnitName, systemUnitFile, this);//only pointer ,not object instance
        unitInformationWnd->setWindowTitle(QObject::tr("Show system unit config info"));
        unitInformationWnd->setWindowModality(Qt::ApplicationModal); //setting block
        unitInformationWnd->setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
        unitInformationWnd->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        unitInformationWnd->show();
    }
}

void UnitWindow::handleSystemUnitStartAction()
{
    QModelIndexList selectedIndexes = m_systemUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_systemUnitFilterModel->index(curRow,0);
        QString systemUnitName = m_systemUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->startSystemUnit(systemUnitName);
    }
}

void UnitWindow::handleSystemUnitStopAction()
{
    QModelIndexList selectedIndexes = m_systemUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_systemUnitFilterModel->index(curRow,0);
        QString systemUnitName = m_systemUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->stopSystemUnit(systemUnitName);
    }
}

void UnitWindow::handleSystemUnitRestartAction()
{
    QModelIndexList selectedIndexes = m_systemUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_systemUnitFilterModel->index(curRow,0);
        QString systemUnitName = m_systemUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->restartSystemUnit(systemUnitName);
    }
}

void UnitWindow::handleSystemUnitEnableAction()
{
    QModelIndexList selectedIndexes = m_systemUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_systemUnitFilterModel->index(curRow,0);
        QString systemUnitName=m_systemUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->enableSystemUnitFiles(systemUnitName);
    }
}

void UnitWindow::handleSystemUnitDisableAction()
{
    QModelIndexList selectedIndexes = m_systemUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_systemUnitFilterModel->index(curRow,0);
        QString systemUnitName = m_systemUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->disableSystemUnitFiles(systemUnitName);
    }
}

void UnitWindow::handleSystemUnitRefreshAction()
{
    slotRefreshUnitsList(false, sys);
}

void UnitWindow::createUserUnitTableView()
{
    m_filterUserUnitHLayout = new  QHBoxLayout;

    QStringList p_filterSystemUnitTypes = QStringList{tr("All"), tr("Services"),tr("Devices"),
                                                      tr("Automounts"),tr("Mounts"), tr("Paths"),
                                                      tr("Scopes"),tr("Slices"),tr("Sockets"),
                                                      tr("Swaps"), tr("Targets"), tr("Timers")};
    m_userUnitTypeComboBox = new QComboBox;
    m_userUnitTypeComboBox->addItems(p_filterSystemUnitTypes);

    m_inactiveUserUnitCheckBox = new QCheckBox;
    m_inactiveUserUnitCheckBox->setCheckState(Qt::Checked);
    m_inactiveUserUnitCheckBox->setCheckable(true);
    m_inactiveUserUnitCheckBox->setText(tr("Show inactive"));

    m_unloadUserUnitCheckBox = new QCheckBox;
    m_unloadUserUnitCheckBox->setCheckState(Qt::Checked);
    m_unloadUserUnitCheckBox->setCheckable(true);
    m_unloadUserUnitCheckBox->setText(tr("Show unload"));

    m_searchUserUnitLineEdit = new QLineEdit;

    m_filterUserUnitHLayout->addWidget(m_userUnitTypeComboBox);
    m_filterUserUnitHLayout->addWidget(m_inactiveUserUnitCheckBox);
    m_filterUserUnitHLayout->addWidget(m_unloadUserUnitCheckBox);
    m_filterUserUnitHLayout->addStretch(0);
    m_filterUserUnitHLayout->addWidget(m_searchUserUnitLineEdit);

    m_subUserUnitLayout->addLayout(m_filterUserUnitHLayout);

    m_userUnitTableView= new QTableView(m_pUserUnitWidget);
    m_userUnitStandItemModel = new QStandardItemModel();

    qDBusRegisterMetaType<SystemdUnit>();
    m_userUnitModel = new UnitModel(this, &m_userUnitsList, m_userUnitColumnCount);

    QMap<filterType, QString> filters;
    filters[activeState] = QString();
    filters[unitType] = QString();
    filters[unitName] = QString();
    m_userUnitFilterModel = new CustomSortFilterProxyModel(this);
    m_userUnitFilterModel->setDynamicSortFilter(false);
    m_userUnitFilterModel->initFilterMap(filters);
    m_userUnitFilterModel->setSourceModel(m_userUnitModel);

    //m_userUnitTableView->setModel(m_userUnitModel);
    m_userUnitTableView->setModel(m_userUnitFilterModel);

    //tableView->setModel(standItemModel);
    m_userUnitTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_userUnitTableView->verticalHeader()->setDefaultSectionSize(40);
    m_userUnitTableView->resizeColumnsToContents();
    m_userUnitTableView->setColumnWidth(0,600);
    m_userUnitTableView->verticalHeader()->hide();
    m_userUnitTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_userUnitTableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_userUnitTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //auto fill columns width
    m_userUnitTableView->horizontalHeader()->setStretchLastSection(true);
    m_userUnitTableView->setColumnHidden(m_userUnitColumnCount-1, true);
    m_userUnitTableView->setMinimumWidth(width());
    m_userUnitTableView->setMinimumHeight(height());
    m_userUnitTableView->adjustSize();

    m_subUserUnitLayout->addWidget(m_userUnitTableView);

    //create contextMenu
    m_userUnitTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_userUnitTableView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(userUnitCustomMenuRequested(QPoint)));

    m_userUnitCountLabel = new QLabel;
    m_userUnitCountLabel->setText(tr("total unit : 18, active unit:10, inactive unit:10 "));
    m_subUserUnitLayout->addWidget(m_userUnitCountLabel);

    connect(m_userUnitTypeComboBox,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &UnitWindow::slotComboBoxShowUserUnits);

    connect(m_inactiveUserUnitCheckBox, &QCheckBox::stateChanged, this, &UnitWindow::slotCheckBoxShowUserUnits);
    connect(m_unloadUserUnitCheckBox, &QCheckBox::stateChanged, this, &UnitWindow::slotCheckBoxShowUserUnits);
    connect(m_searchUserUnitLineEdit, &QLineEdit::textChanged, this, &UnitWindow::slotLineEditSearchUserUnitChanged);

    connect(m_userUnitTableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotUserUnitTableRowDoubleClicked(const QModelIndex &)));

    slotCheckBoxShowUserUnits(-1);
}

void UnitWindow::slotUserUnitTableRowDoubleClicked(const QModelIndex index)
{
    QModelIndexList selectedIndexes = m_userUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_userUnitFilterModel->index(curRow,0);
        QString userUnitName = m_userUnitFilterModel->data(curCellIndex).toString();

        const QModelIndex& curCellIndex2 = m_userUnitFilterModel->index(curRow, m_userUnitColumnCount-1);
        QString userUnitFile = m_userUnitFilterModel->data(curCellIndex2).toString();

        UnitInformation * unitInformationWnd = new UnitInformation(userUnitName, userUnitFile, this);
        unitInformationWnd->setWindowTitle(QObject::tr("Show User unit information"));
        unitInformationWnd->setWindowModality(Qt::ApplicationModal); //setting block
        unitInformationWnd->setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
        unitInformationWnd->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        unitInformationWnd->show();
    }
}

void UnitWindow::userUnitCustomMenuRequested(QPoint pos)
{
    // select row index
    QModelIndexList selectedIndexes = m_userUnitTableView->selectionModel()->selectedRows();

    if (!selectedIndexes.isEmpty())
    {
        QModelIndex clickedIndex = m_userUnitTableView->indexAt(pos);
        if (clickedIndex.isValid()){  // if index valid ,continue
            QMenu * userUnitMenu = new QMenu(this);

            QAction*  userUnitStatusAction = new QAction(tr("User Unit Status"), this);
            userUnitMenu->addAction(userUnitStatusAction);

            userUnitMenu->addSeparator();

            QAction* userUnitStartAction = new QAction(tr("Run User Unit"), this);
            userUnitMenu->addAction(userUnitStartAction);

            QAction* userUnitStopAction = new QAction(tr("Stop User Unit"), this);
            userUnitMenu->addAction(userUnitStopAction);

            QAction* userUnitRestartAction = new QAction(tr("Restart User Unit"), this);
            userUnitMenu->addAction(userUnitRestartAction);

            userUnitMenu->addSeparator();

            QAction* userUnitEnableAction = new QAction(tr("Enable User Unit"), this);
            userUnitMenu->addAction(userUnitEnableAction);

            QAction* userUnitDisableAction = new QAction(tr("Disable User Unit"), this);
            userUnitMenu->addAction(userUnitDisableAction);

            userUnitMenu->addSeparator();

            QAction* userUnitRefreshAction = new QAction(tr("Refresh User Unit"), this);
            userUnitMenu->addAction(userUnitRefreshAction);

            // user status info
            connect(userUnitStatusAction, &QAction::triggered, this, &UnitWindow::handleUserUnitStatusAction);
            // start user unit
            connect(userUnitStartAction, &QAction::triggered, this, &UnitWindow::handleUserUnitStartAction);
            // stop user unit
            connect(userUnitStopAction, &QAction::triggered, this, &UnitWindow::handleUserUnitStopAction);
            // restart user unit
            connect(userUnitRestartAction, &QAction::triggered, this, &UnitWindow::handleUserUnitRestartAction);
            // enable user unit
            connect(userUnitEnableAction, &QAction::triggered, this, &UnitWindow::handleUserUnitEnableAction);
            // disable user unit
            connect(userUnitDisableAction, &QAction::triggered, this, &UnitWindow::handleUserUnitDisableAction);
            // refresh user unit
            connect(userUnitRefreshAction, &QAction::triggered, this, &UnitWindow::handleUserUnitRefreshAction);

            userUnitMenu->popup(m_userUnitTableView->viewport()->mapToGlobal(pos));
        }
    }
}

void UnitWindow::handleUserUnitStatusAction()
{
    QModelIndexList selectedIndexes = m_userUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_userUnitFilterModel->index(curRow,0);
        QString userUnitName = m_userUnitFilterModel->data(curCellIndex).toString();

        const QModelIndex& curCellIndex2 = m_userUnitFilterModel->index(curRow, m_userUnitColumnCount-1);
        QString userUnitFile = m_userUnitFilterModel->data(curCellIndex2).toString();

        UnitInformation * unitInformationWnd = new UnitInformation(userUnitName, userUnitFile, this);
        unitInformationWnd->setWindowTitle(QObject::tr("Show User unit information"));
        unitInformationWnd->setWindowModality(Qt::ApplicationModal); //setting block
        unitInformationWnd->setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
        unitInformationWnd->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        unitInformationWnd->show();
    }
}

void UnitWindow::handleUserUnitStartAction()
{
    QModelIndexList selectedIndexes = m_userUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_userUnitFilterModel->index(curRow,0);
        QString userUnitName = m_userUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->startUserUnit(userUnitName);
    }
}

void UnitWindow::handleUserUnitStopAction()
{
    QModelIndexList selectedIndexes = m_userUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_userUnitFilterModel->index(curRow,0);
        QString userUnitName = m_userUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->stoptUserUnit(userUnitName);
    }
}

void UnitWindow::handleUserUnitRestartAction()
{
    QModelIndexList selectedIndexes = m_userUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_userUnitFilterModel->index(curRow,0);
        QString userUnitName = m_userUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->restartUserUnit(userUnitName);
    }
}

void UnitWindow::handleUserUnitEnableAction()
{
    QModelIndexList selectedIndexes = m_userUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_userUnitFilterModel->index(curRow,0);
        QString userUnitName = m_userUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->enableUserUnitFiles(userUnitName);
    }
}

void UnitWindow::handleUserUnitDisableAction()
{
    QModelIndexList selectedIndexes = m_userUnitTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_userUnitFilterModel->index(curRow,0);
        QString userUnitName = m_userUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->disableUserUnitFiles(userUnitName);
    }
}

void UnitWindow::handleUserUnitRefreshAction()
{
    slotRefreshUnitsList(false, user);
}

void UnitWindow::createTimerTableView()
{
    m_filterTimerUnitHLayout = new  QHBoxLayout;

    m_searchTimerUnitLabel = new QLabel;
    m_searchTimerUnitLabel->setText(tr("Search Timer:"));
    m_searchTimerUnitLineEdit = new QLineEdit;

    m_filterTimerUnitHLayout->addWidget(m_searchTimerUnitLabel);
    m_filterTimerUnitHLayout->addWidget(m_searchTimerUnitLineEdit);
    m_filterTimerUnitHLayout->addStretch(0);

    m_subTimerLayout->addLayout(m_filterTimerUnitHLayout);

    m_timerTableView= new QTableView(m_pTimerWidget);
    m_timerStandItemModel = new QStandardItemModel(this);

    m_timerStandItemModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Timer")));
    m_timerStandItemModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Next")));
    m_timerStandItemModel->setHorizontalHeaderItem(2, new QStandardItem(tr("Left")));
    m_timerStandItemModel->setHorizontalHeaderItem(3, new QStandardItem(tr("Last")));
    m_timerStandItemModel->setHorizontalHeaderItem(4, new QStandardItem(tr("Passed")));
    m_timerStandItemModel->setHorizontalHeaderItem(5, new QStandardItem(tr("Activates")));
    m_timerStandItemModel->setHorizontalHeaderItem(6, new QStandardItem(tr("Timer File")));
    m_timerTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_timerTableView->verticalHeader()->setDefaultSectionSize(40);
    m_timerTableView->resizeColumnsToContents();
    m_timerTableView->setColumnHidden(m_timerColumnCount-1, true);

    m_timerTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_timerTableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_timerTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //use delegate to set column
    m_pTableViewItemDelegate =new MyStyledItemDelegate(this);
    m_timerTableView->setItemDelegate(m_pTableViewItemDelegate);
    m_pTableViewItemDelegate->setColumnWidth(2,300);
    m_pTableViewItemDelegate->setColumnWidth(4,300);
    m_timerTableView->verticalHeader()->hide();

    //auto fill columns width
    m_timerTableView->horizontalHeader()->setStretchLastSection(true);

    // Set model for QTableView (should be called after headers are set)
    QMap<filterType, QString> filters;
    filters[activeState] = QString();
    filters[unitType] = QString();
    filters[unitName] = QString();
    m_timerUnitFilterModel = new CustomSortFilterProxyModel(this);
    m_timerUnitFilterModel->setDynamicSortFilter(false);
    m_timerUnitFilterModel->initFilterMap(filters);
    m_timerUnitFilterModel->setSourceModel(m_timerStandItemModel);
    m_timerTableView->setModel(m_timerUnitFilterModel);

    m_timerTableView->sortByColumn(1, Qt::AscendingOrder);
    m_timerTableView->setMinimumWidth(this->width());
    m_timerTableView->setMinimumHeight(height());
    m_timerTableView->adjustSize();
    m_timerTableView->setMouseTracking(true);

    m_subTimerLayout->addWidget(m_timerTableView);

    //create contextMenu
    m_timerTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_timerTableView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(timerUnitCustomMenuRequested(QPoint)));

    // Setup a timer that updates the left and passed columns every 5secs
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTimers()));
    timer->start(1000);

    connect(m_searchTimerUnitLineEdit, &QLineEdit::textChanged, this, &UnitWindow::slotLineEditSearchTimerUnitChanged);

    connect(m_timerTableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotTimerUnitTableRowDoubleClicked(const QModelIndex &)));

    slotRefreshTimerList();

    connect(m_timerTableView, SIGNAL(entered(QModelIndex)), this, SLOT(slotTimerUnitTableRowTooltip(QModelIndex)));
}

void UnitWindow::slotTimerUnitTableRowTooltip(const QModelIndex index)
{
    int curRow=index.row();
    const QModelIndex& curCellIndex1 = m_timerUnitFilterModel->index(curRow,0);
    QString timerUnitName = m_timerUnitFilterModel->data(curCellIndex1).toString();

    const QModelIndex& curCellIndex2 = m_timerUnitFilterModel->index(curRow, 4);
    QString timerUnitPassed = m_timerUnitFilterModel->data(curCellIndex2).toString();

    const QModelIndex& curCellIndex3 = m_timerUnitFilterModel->index(curRow, 5);
    QString timerUnitActivates = m_timerUnitFilterModel->data(curCellIndex3).toString();

    QString toolTipText;
    toolTipText.append(QStringLiteral("<FONT COLOR=DarkCyan>"));
    toolTipText.append(QStringLiteral("<b>Timer: %1</b><hr>").arg(timerUnitName));
    toolTipText.append(QStringLiteral("<b>Timer Passed: %1</b><hr>").arg(timerUnitPassed));
    toolTipText.append(QStringLiteral("<b>Timer Activates: %1</b><hr>").arg(timerUnitActivates));
    toolTipText.append(QStringLiteral("</FONT"));

    //m_timerStandItemModel->itemFromIndex(index)->setToolTip(toolTipText);
    QToolTip::showText(QCursor::pos(), toolTipText, this, QRect(), -1);
}

void UnitWindow::slotTimerUnitTableRowDoubleClicked(const QModelIndex index)
{
    QModelIndexList selectedIndexes = m_timerTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_timerUnitFilterModel->index(curRow,0);
        QString timerUnitName = m_timerUnitFilterModel->data(curCellIndex).toString();

        const QModelIndex& curCellIndex2 = m_timerUnitFilterModel->index(curRow, m_timerColumnCount-1);
        QString timerUnitFile = m_timerUnitFilterModel->data(curCellIndex2).toString();

        UnitInformation * unitInformationWnd = new UnitInformation(timerUnitName, timerUnitFile, this);
        unitInformationWnd->setWindowTitle(QObject::tr("Show Timer information"));
        unitInformationWnd->setWindowModality(Qt::ApplicationModal); //setting block
        unitInformationWnd->setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
        unitInformationWnd->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        unitInformationWnd->show();
    }
}
void UnitWindow::timerUnitCustomMenuRequested(QPoint pos)
{
    QModelIndex clickedIndex = m_timerTableView->indexAt(pos);
    if (clickedIndex.isValid()){
        QMenu * timerUnitMenu = new QMenu(this);

        QAction*  timerUnitStatusAction = new QAction(tr("Timer Unit Status"), this);
        timerUnitMenu->addAction(timerUnitStatusAction);

        timerUnitMenu->addSeparator();

        QAction* timerUnitStartAction = new QAction(tr("Start Timer Unit"), this);
        timerUnitMenu->addAction(timerUnitStartAction);

        QAction* timerUnitStopAction = new QAction(tr("Stop Timer Unit"), this);
        timerUnitMenu->addAction(timerUnitStopAction);

        QAction* timerUnitRestartAction = new QAction(tr("Restart Timer Unit"), this);
        timerUnitMenu->addAction(timerUnitRestartAction);

        timerUnitMenu->addSeparator();

        QAction* timerUnitEnableAction = new QAction(tr("Enable Timer Unit"), this);
        timerUnitMenu->addAction(timerUnitEnableAction);

        QAction* timerUnitDisableAction = new QAction(tr("Disable Timer Unit"), this);
        timerUnitMenu->addAction(timerUnitDisableAction);

        timerUnitMenu->addSeparator();

        QAction* timerUnitRefreshAction = new QAction(tr("Refresh Timer Unit"), this);
        timerUnitMenu->addAction(timerUnitRefreshAction);

        //Timer unit status
        connect(timerUnitStatusAction, &QAction::triggered, this, &UnitWindow::handleTimerUnitStatusAction);

        // Start Timer unit
        connect(timerUnitStartAction, &QAction::triggered, this, &UnitWindow::handleTimerUnitStartAction);
        // Stop Timer unit
        connect(timerUnitStopAction, &QAction::triggered, this, &UnitWindow::handleTimerUnitStopAction);
        // Restart Timer unit
        connect(timerUnitRestartAction, &QAction::triggered, this, &UnitWindow::handleTimerUnitRestartAction);

        // Enable Timer unit
        connect(timerUnitEnableAction, &QAction::triggered, this, &UnitWindow::handleTimerUnitEnableAction);
        // Disable Timer unit
        connect(timerUnitDisableAction, &QAction::triggered, this, &UnitWindow::handleTimerUnitDisableAction);
        // Refresh Timer unit
        connect(timerUnitRefreshAction, &QAction::triggered, this, &UnitWindow::handleTimerUnitRefreshAction);

        timerUnitMenu->popup(m_timerTableView->viewport()->mapToGlobal(pos));
    }
}

void UnitWindow::handleTimerUnitStatusAction()
{
    QModelIndexList selectedIndexes = m_timerTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_timerUnitFilterModel->index(curRow,0);
        QString timerUnitName = m_timerUnitFilterModel->data(curCellIndex).toString();

        const QModelIndex& curCellIndex2 = m_timerUnitFilterModel->index(curRow, m_timerColumnCount-1);
        QString timerUnitFile = m_timerUnitFilterModel->data(curCellIndex2).toString();

        UnitInformation * unitInformationWnd = new UnitInformation(timerUnitName, timerUnitFile, this);
        unitInformationWnd->setWindowTitle(QObject::tr("Show Timer information"));
        unitInformationWnd->setWindowModality(Qt::ApplicationModal); //setting block
        unitInformationWnd->setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
        unitInformationWnd->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        unitInformationWnd->show();
    }
}

void UnitWindow::handleTimerUnitStartAction()
{
    QModelIndexList selectedIndexes = m_timerTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_timerUnitFilterModel->index(curRow,0);
        QString timerUnitName = m_timerUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->startSystemUnit(timerUnitName);
    }
}

void UnitWindow::handleTimerUnitStopAction()
{
    QModelIndexList selectedIndexes = m_timerTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_timerUnitFilterModel->index(curRow,0);
        QString timerUnitName = m_timerUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->stopSystemUnit(timerUnitName);
    }
}

void UnitWindow::handleTimerUnitRestartAction()
{
    QModelIndexList selectedIndexes = m_timerTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_timerUnitFilterModel->index(curRow,0);
        QString timerUnitName = m_timerUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->restartSystemUnit(timerUnitName);
    }
}

void UnitWindow::handleTimerUnitEnableAction()
{
    QModelIndexList selectedIndexes = m_timerTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_timerUnitFilterModel->index(curRow,0);
        QString timerUnitName = m_timerUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->enableSystemUnitFiles(timerUnitName);
    }
}

void UnitWindow::handleTimerUnitDisableAction()
{
    QModelIndexList selectedIndexes = m_timerTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_timerUnitFilterModel->index(curRow,0);
        QString timerUnitName = m_timerUnitFilterModel->data(curCellIndex).toString();
        m_systemdManagerInterface->disableSystemUnitFiles(timerUnitName);
    }
}

void UnitWindow::handleTimerUnitRefreshAction()
{
    slotRefreshTimerList();
}

void UnitWindow::setupSystemUnitslist()
{
    // Sets up the units list initially
    // Register the meta type for storing units
    qDBusRegisterMetaType<SystemdUnit>();
    m_systemUnitModel = new UnitModel(this, &m_systemUnitsList, m_systemUnitColumnCount);
}

void UnitWindow::setupUserUnitslist()
{
    // Sets up the units list initially
    // Register the meta type for storing units
    qDBusRegisterMetaType<SystemdUnit>();
    m_userUnitModel = new UnitModel(this, &m_userUnitsList, m_userUnitColumnCount);
}

void UnitWindow::setupTimerslist()
{

}

void UnitWindow::slotLineEditSearchSystemUnitChanged(QString term)
{
    m_systemUnitFilterModel->addFilterRegExp(unitName, term);
    m_systemUnitFilterModel->invalidate();
    m_systemUnitTableView->sortByColumn(m_systemUnitTableView->horizontalHeader()->sortIndicatorSection(),
                              m_systemUnitTableView->horizontalHeader()->sortIndicatorOrder());
    updateSystemUnitCount();
}

void UnitWindow::slotLineEditSearchUserUnitChanged(QString term)
{
    m_userUnitFilterModel->addFilterRegExp(unitName, term);
    m_userUnitFilterModel->invalidate();
    m_userUnitTableView->sortByColumn(m_userUnitTableView->horizontalHeader()->sortIndicatorSection(),
                              m_userUnitTableView->horizontalHeader()->sortIndicatorOrder());
    updateUserUnitCount();
}

void UnitWindow::slotLineEditSearchTimerUnitChanged(QString term)
{
    m_timerUnitFilterModel->addFilterRegExp(unitName, term);
    m_timerUnitFilterModel->invalidate();
    m_timerTableView->sortByColumn(m_timerTableView->horizontalHeader()->sortIndicatorSection(),
                                        m_timerTableView->horizontalHeader()->sortIndicatorOrder());
}

void UnitWindow::slotComboBoxShowSystemUnits(int index)
{
    //qDebug()<<"slotComboBoxShowSystemUnits: "<< index;
    m_systemUnitFilterModel->addFilterRegExp(unitType,  QStringLiteral("(%1)$").arg(unitTypeSufx.at(index)));
    m_systemUnitFilterModel->invalidate();
    m_systemUnitTableView->sortByColumn(m_systemUnitTableView->horizontalHeader()->sortIndicatorSection(),
                              m_systemUnitTableView->horizontalHeader()->sortIndicatorOrder());
    updateSystemUnitCount();
}

void UnitWindow::slotComboBoxShowUserUnits(int index)
{
    m_userUnitFilterModel->addFilterRegExp(unitType, QStringLiteral("(%1)$").arg(unitTypeSufx.at(index)));
    m_userUnitFilterModel->invalidate();
    m_userUnitTableView->sortByColumn(m_userUnitTableView->horizontalHeader()->sortIndicatorSection(),
                                  m_userUnitTableView->horizontalHeader()->sortIndicatorOrder());
    updateUserUnitCount();
}

void UnitWindow::slotCheckBoxShowSystemUnits(int state)
{
    //qDebug()<<"QObject::sender()->objectName(): "<< state;
    if ( state == -1  || state == 0 || state ==1  || state ==2)
    {
        // System units
        if (m_inactiveSystemUnitCheckBox->isChecked())
        {
            m_unloadSystemUnitCheckBox->setEnabled(true);
            if (m_unloadSystemUnitCheckBox->isChecked())
                m_systemUnitFilterModel->addFilterRegExp(activeState, QString());
            else
                m_systemUnitFilterModel->addFilterRegExp(activeState, QStringLiteral("active"));
        }
        else
        {
            m_unloadSystemUnitCheckBox->setEnabled(false);
            m_systemUnitFilterModel->addFilterRegExp(activeState, QStringLiteral("^(active)"));
        }
        m_systemUnitFilterModel->invalidate();
        m_systemUnitTableView->sortByColumn(m_systemUnitTableView->horizontalHeader()->sortIndicatorSection(),
                                  m_systemUnitTableView->horizontalHeader()->sortIndicatorOrder());
    }
    updateSystemUnitCount();
}

void UnitWindow::slotCheckBoxShowUserUnits(int state)
{
    if ( state == -1  || state == 0 || state ==1  || state ==2)
    {
        // User units
        if (m_inactiveUserUnitCheckBox->isChecked())
        {
            m_unloadUserUnitCheckBox->setEnabled(true);
            if (m_unloadUserUnitCheckBox->isChecked())
                m_userUnitFilterModel->addFilterRegExp(activeState, QString());
            else
                m_userUnitFilterModel->addFilterRegExp(activeState, QStringLiteral("active"));
        }
        else
        {
            m_unloadUserUnitCheckBox->setEnabled(false);
            m_userUnitFilterModel->addFilterRegExp(activeState, QStringLiteral("^(active)"));
        }
        m_userUnitFilterModel->invalidate();
        m_userUnitTableView->sortByColumn(m_userUnitTableView->horizontalHeader()->sortIndicatorSection(),
                                      m_userUnitTableView->horizontalHeader()->sortIndicatorOrder());
    }
    updateUserUnitCount();
}

void UnitWindow::updateSystemUnitCount()
{
    QString totalSystemUnitShowText = tr("Total: %1 unit,").arg(m_systemUnitModel->rowCount());
    QString activeSystemUnitShowText = tr("%2 active unit,").arg(m_noActSystemUnits);
    QString displaySystemUnitShowText = tr("%3 display unit").arg(m_systemUnitFilterModel->rowCount());
    //QString systemdUnitCountShowText = tr("Total: %1 unit, %2 active unit, %3 display unit").arg(m_systemUnitModel->rowCount()).arg(m_noActSystemUnits).arg(m_systemUnitFilterModel->rowCount());
    QString systemUnitShowText = tr("%1 %2 %3").arg(totalSystemUnitShowText).arg(activeSystemUnitShowText).arg(displaySystemUnitShowText);
    m_systemUnitCountLabel->setText(systemUnitShowText);
    //m_systemUnitCountLabel->setText(tr("Total: %1 unit, %2 active unit, %3 display unit").arg(m_systemUnitModel->rowCount()).arg(m_noActSystemUnits).arg(m_systemUnitFilterModel->rowCount()));
}

void UnitWindow::updateUserUnitCount()
{
    QString totalUserUnitShowText = tr("Total: %1 unit,").arg(m_userUnitModel->rowCount());
    QString activeUserUnitShowText = tr("%2 active unit,").arg(m_noActUserUnits);
    QString displayUserUnitShowText = tr("%3 display unit").arg(m_userUnitFilterModel->rowCount());
    QString userUnitShowText = tr("%1 %2 %3").arg(totalUserUnitShowText).arg(activeUserUnitShowText).arg(displayUserUnitShowText);
    m_userUnitCountLabel->setText(userUnitShowText);
    //m_userUnitCountLabel->setText(tr("Total: %1 unit, %2 active unit, %3 display unit").arg(m_userUnitModel->rowCount()).arg(m_noActUserUnits).arg(m_userUnitFilterModel->rowCount()));
}

void UnitWindow::slotRefreshUnitsList(bool initial, dbusBus bus)
{
    // Updates the unit lists
    if (bus == sys) {
        // get an updated list of system units via dbus
        m_systemUnitsList.clear();
        m_systemUnitsList = m_systemdManagerInterface->listSystemUnits();
        m_noActSystemUnits = 0;
        int i=1;
        // qDebug() << "get unit total number : "<< m_systemUnitsList.count();
        for (const SystemdUnit &unit : m_systemUnitsList)
        {
            if (unit.active_state == QLatin1String("active"))
                m_noActSystemUnits++;
        }
        if (!initial) {
            qDebug() << "not initial system units...";
            m_systemUnitModel->dataChanged(m_systemUnitModel->index(0, 0), m_systemUnitModel->index(m_systemUnitModel->rowCount(), 0));
            m_systemUnitFilterModel->invalidate();
            updateSystemUnitCount();
            //slotRefreshTimerList();
            //updateActions();
        }
    } else if (enableUserUnits && bus == user) {
        // get an updated list of user units via dbus
        m_userUnitsList.clear();
        m_userUnitsList = m_systemdManagerInterface->listUserUnits();
        m_noActUserUnits = 0;
        for (const SystemdUnit &unit : m_userUnitsList)
        {
            if (unit.active_state == QLatin1String("active"))
                m_noActUserUnits++;
        }
        if (!initial) {
            m_userUnitModel->dataChanged(m_userUnitModel->index(0, 0), m_userUnitModel->index(m_userUnitModel->rowCount(), 0));
            m_userUnitFilterModel->invalidate();
            updateUserUnitCount();
            //slotRefreshTimerList();
            //updateActions();
        }
    }
}

void UnitWindow::slotRefreshTimerList()
{
    // Updates the timer list
    m_timerStandItemModel->removeRows(0, m_timerStandItemModel->rowCount());

    // Iterate through system unitlist and add timers to the model
    for (const SystemdUnit &unit : m_systemUnitsList)
    {
        if (unit.id.endsWith(QLatin1String(".timer")) &&
            unit.load_state != QLatin1String("unloaded")) {
            m_timerStandItemModel->appendRow(buildTimerListRow(unit, m_systemUnitsList, sys));
        }
    }

    // Iterate through user unitlist and add timers to the model
    for (const SystemdUnit &unit : m_userUnitsList)
    {
        if (unit.id.endsWith(QLatin1String(".timer")) &&
            unit.load_state != QLatin1String("unloaded")) {
            m_timerStandItemModel->appendRow(buildTimerListRow(unit, m_userUnitsList, user));
        }
    }

    // Update the left and passed columns
    slotUpdateTimers();

    m_timerTableView->resizeColumnsToContents();

    m_timerColumnCount = m_timerTableView->model()->columnCount();
    m_timerTableView->setColumnHidden(m_timerColumnCount-1, true);
    m_timerTableView->sortByColumn(m_timerTableView->horizontalHeader()->sortIndicatorSection(),
                               m_timerTableView->horizontalHeader()->sortIndicatorOrder());
}

void UnitWindow::slotUpdateTimers()
{
    // Updates the left and passed columns in the timers list
    for (int row = 0; row < m_timerStandItemModel->rowCount(); ++row)
    {
        QDateTime next = m_timerStandItemModel->index(row, 1).data().toDateTime();
        QDateTime last = m_timerStandItemModel->index(row, 3).data().toDateTime();
        QDateTime current = QDateTime().currentDateTime();
        qlonglong leftSecs = current.secsTo(next);
        qlonglong passedSecs = last.secsTo(current);

        QString left;
        if (leftSecs >= 31536000)
            left = QStringLiteral("%1 years").arg(leftSecs / 31536000);
        else if (leftSecs >= 604800)
            left = QStringLiteral("%1 weeks").arg(leftSecs / 604800);
        else if (leftSecs >= 86400)
            left = QStringLiteral("%1 days").arg(leftSecs / 86400);
        else if (leftSecs >= 3600)
            left = QStringLiteral("%1 hr").arg(leftSecs / 3600);
        else if (leftSecs >= 60)
            left = QStringLiteral("%1 min").arg(leftSecs / 60);
        else if (leftSecs < 0)
            left = QStringLiteral("0 s");
        else
            left = QStringLiteral("%1 s").arg(leftSecs);
        m_timerStandItemModel->setData(m_timerStandItemModel->index(row, 2), left);

        QString passed;
        if (m_timerStandItemModel->index(row, 3).data() == QStringLiteral("n/a"))
            passed = QStringLiteral("n/a");
        else if (passedSecs >= 31536000)
            passed = QStringLiteral("%1 years").arg(passedSecs / 31536000);
        else if (passedSecs >= 604800)
            passed = QStringLiteral("%1 weeks").arg(passedSecs / 604800);
        else if (passedSecs >= 86400)
            passed = QStringLiteral("%1 days").arg(passedSecs / 86400);
        else if (passedSecs >= 3600)
            passed = QStringLiteral("%1 hr").arg(passedSecs / 3600);
        else if (passedSecs >= 60)
            passed = QStringLiteral("%1 min").arg(passedSecs / 60);
        else if (passedSecs < 0)
            passed = QStringLiteral("0 s");
        else
            passed = QStringLiteral("%1 s").arg(passedSecs);
        m_timerStandItemModel->setData(m_timerStandItemModel->index(row, 4), passed);
    }
}

QList<QStandardItem *> UnitWindow::buildTimerListRow(const SystemdUnit &unit, const QVector<SystemdUnit> &list, dbusBus bus)
{
    // Builds a row for the timers list
    QDBusObjectPath path = unit.unit_path;
    QString unitToActivate = m_systemdManagerInterface->getDbusProperty(QStringLiteral("Unit"), sysdTimer, path, bus).toString();

    QDateTime time;
    QIcon icon;
    if (bus == sys)
        icon = QIcon::fromTheme(QStringLiteral("applications-system"));
    else
        icon = QIcon::fromTheme(QStringLiteral("user-identity"));

    // Add the next elapsation point
    qlonglong nextElapseMonotonicMsec = m_systemdManagerInterface->getDbusProperty(QStringLiteral("NextElapseUSecMonotonic"), sysdTimer, path, bus).toULongLong() / 1000;
    qlonglong nextElapseRealtimeMsec = m_systemdManagerInterface->getDbusProperty(QStringLiteral("NextElapseUSecRealtime"), sysdTimer, path, bus).toULongLong() / 1000;
    qlonglong lastTriggerMSec = m_systemdManagerInterface->getDbusProperty(QStringLiteral("LastTriggerUSec"), sysdTimer, path, bus).toULongLong() / 1000;

    if (nextElapseMonotonicMsec == 0)
    {
        // Timer is calendar-based
        time.setMSecsSinceEpoch(nextElapseRealtimeMsec);
    }
    else
    {
        // Timer is monotonic
        time = QDateTime().currentDateTime();
        time = time.addMSecs(nextElapseMonotonicMsec);

        // Get the monotonic system clock
        struct timespec ts;
        if (clock_gettime( CLOCK_MONOTONIC, &ts ) != 0)
            qDebug() << "Failed to get the monotonic system clock!";

        // Convert the monotonic system clock to microseconds
        qlonglong now_mono_usec = ts.tv_sec * 1000000 + ts.tv_nsec / 1000;

        // And subtract it.
        time = time.addMSecs(-now_mono_usec/1000);
    }

    QString next = time.toString(QStringLiteral("yyyy.MM.dd hh:mm:ss"));

    QString last;

    // use unit object to get last time for activated service
    int index = list.indexOf(SystemdUnit(unitToActivate));
    if (index != -1)
    {
        qlonglong inactivateExitTimestampMsec =
            m_systemdManagerInterface->getDbusProperty(QStringLiteral("InactiveExitTimestamp"), sysdUnit, list.at(index).unit_path, bus).toULongLong() / 1000;

        if (inactivateExitTimestampMsec == 0)
        {
            // The unit has not run in this boot
            // Use LastTrigger to see if the timer is persistent
            if (lastTriggerMSec == 0)
                last = QStringLiteral("n/a");
            else
            {
                time.setMSecsSinceEpoch(lastTriggerMSec);
                last = time.toString(QStringLiteral("yyyy.MM.dd hh:mm:ss"));
            }
        }
        else
        {
            QDateTime time;
            time.setMSecsSinceEpoch(inactivateExitTimestampMsec);
            last = time.toString(QStringLiteral("yyyy.MM.dd hh:mm:ss"));
        }
    }

    // Set icon for id column
    QStandardItem *id = new QStandardItem(unit.id);
    id->setData(icon, Qt::DecorationRole);

    QString unit_file=unit.unit_file;

    // Build a row from QStandardItems
    QList<QStandardItem *> row;
    row << id <<
        new QStandardItem(next) <<
        new QStandardItem() <<
        new QStandardItem(last) <<
        new QStandardItem() <<
        new QStandardItem(unitToActivate) <<
        new QStandardItem(unit_file);

    return row;
}

void UnitWindow::resizeEvent(QResizeEvent *event)
{
    if (nullptr != m_pTabWidget)
    {
       m_pTabWidget->resize(width(), height());
        update();
    }
    if (nullptr != m_pSystemUnitWidget)
    {
        m_pSystemUnitWidget->resize(width(), height()-m_systemUnitCountLabel->height()-60);
        update();
    }

    if (nullptr != m_pUserUnitWidget)
    {
        m_pUserUnitWidget->resize(width(), height()-80);
        update();
    }
}
