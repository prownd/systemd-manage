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

#include "configfilewindow.h"
#include "systemdgeneric.h"

#include <QMenu>
#include <QMessageBox>

#include "configfileinformation.h"

ConfigFileWindow::ConfigFileWindow(QWidget *parent) :
    QWidget(parent)
{
    //frameless window
    setWindowFlags(Qt::FramelessWindowHint);

    m_systemdManagerInterface=new SystemdManagerInterface;
    qDBusRegisterMetaType<SystemdUnit>();
    createConfigFileTableView();
}

ConfigFileWindow::~ConfigFileWindow()
{
}

void ConfigFileWindow::createConfigFileTableView()
{
    m_hBoxLayout = new QHBoxLayout(this);

    m_configFileTableView= new QTableView();


    m_hBoxLayout->addWidget(m_configFileTableView);

    m_confFileModel = new QStandardItemModel(this);
    m_confFileModel->setHorizontalHeaderItem(0, new QStandardItem(tr("File")));
    m_confFileModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Modified")));
    m_confFileModel->setHorizontalHeaderItem(2, new QStandardItem(tr("Created Time")));
    m_confFileModel->setHorizontalHeaderItem(3, new QStandardItem(tr("Size (Byte)")));
    m_confFileModel->setHorizontalHeaderItem(4, new QStandardItem(tr("Description")));

    //m_configFileTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_configFileTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_configFileTableView->verticalHeader()->setDefaultSectionSize(40);
    m_configFileTableView->verticalHeader()->hide();

    m_configFileTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_configFileTableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_configFileTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_configFileTableView->resizeColumnsToContents();
    m_configFileTableView->horizontalHeader()->setStretchLastSection(true);

    m_configFileTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_configFileTableView->setColumnWidth(0,400);
    m_configFileTableView->setModel(m_confFileModel);
    m_configFileTableView->sortByColumn(1, Qt::AscendingOrder);
    m_configFileTableView->setMouseTracking(true);

    m_confFileList.append(conffile(QStringLiteral("/etc/systemd/coredump.conf"),
                                   QStringLiteral("coredump.conf"),
                                   tr("Coredump generation and storage")));
    m_confFileList.append(conffile(QStringLiteral("/etc/systemd/networkd.conf"),
                                   QStringLiteral("networkd.conf"),
                                   tr("Network manager configuration")));
    m_confFileList.append(conffile(QStringLiteral("/etc/systemd/journald.conf"),
                                   QStringLiteral("journald.conf"),
                                   tr("Journal manager settings")));
    m_confFileList.append(conffile(QStringLiteral("/etc/systemd/logind.conf"),
                                   QStringLiteral("logind.conf"),
                                   tr("Login manager configuration")));
    m_confFileList.append(conffile(QStringLiteral("/etc/systemd/resolved.conf"),
                                   QStringLiteral("resolved.conf"),
                                   tr("Network name resolution configuration")));
    m_confFileList.append(conffile(QStringLiteral("/etc/systemd/system.conf"),
                                   QStringLiteral("systemd-system.conf"),
                                   tr("Systemd daemon configuration")));
    m_confFileList.append(conffile(QStringLiteral("/etc/systemd/timesyncd.conf"),
                                   QStringLiteral("timesyncd.conf"),
                                   tr("Time synchronization settings")));
    m_confFileList.append(conffile(QStringLiteral("/etc/systemd/user.conf"),
                                   QStringLiteral("systemd-system.conf"),
                                   tr("Systemd user daemon configuration")));

    //create contextMenu
    m_configFileTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_configFileTableView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(configFileCustomMenuRequested(QPoint)));

    connect(m_configFileTableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotConfigFileTableRowDoubleClicked(const QModelIndex &)));
    connect(m_configFileTableView, SIGNAL(entered(QModelIndex)), this, SLOT(slotConfigFileTableRowTooltip(QModelIndex)));

    slotRefreshConfFileList();

    QFileSystemWatcher *m_fileWatcher = new QFileSystemWatcher;
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &ConfigFileWindow::slotRefreshConfFileList);
    for (const conffile &f : m_confFileList) {
        m_fileWatcher->addPath(f.filePath);
    }
}

void ConfigFileWindow::slotConfigFileTableRowTooltip(const QModelIndex index)
{
    int curRow=index.row();
    const QModelIndex& curCellIndex1 = m_confFileModel->index(curRow,0);
    QString configFileName = m_confFileModel->data(curCellIndex1).toString();

    const QModelIndex& curCellIndex2 = m_confFileModel->index(curRow, 2);
    QString configFileCreateTime =m_confFileModel->data(curCellIndex2).toString();

    const QModelIndex& curCellIndex3 = m_confFileModel->index(curRow, 3);
    QString configFileSize = m_confFileModel->data(curCellIndex3).toString();

    const QModelIndex& curCellIndex4 = m_confFileModel->index(curRow, 4);
    QString configFileDescription = m_confFileModel->data(curCellIndex4).toString();

    QString toolTipText;
    toolTipText.append(QStringLiteral("<FONT COLOR=DarkCyan>"));
    toolTipText.append(tr("<b>Config File Name: %1</b><hr>").arg(configFileName));
    toolTipText.append(tr("<b>Config File Create Time: %1</b><hr>").arg(configFileCreateTime));
    toolTipText.append(tr("<b>Config File Size: %1</b><hr>").arg(configFileSize));
    toolTipText.append(tr("<b>Config File Description: %1</b><hr>").arg(configFileDescription));
    toolTipText.append(QStringLiteral("</FONT"));

    m_confFileModel->itemFromIndex(index)->setToolTip(toolTipText);
    //QToolTip::showText(QCursor::pos(), toolTipText, this, QRect(), -1);
}

void ConfigFileWindow::slotConfigFileTableRowDoubleClicked(const QModelIndex index)
{
    QModelIndexList selectedIndexes = m_configFileTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_confFileModel->index(curRow,0);
        QString configFilePath = m_confFileModel->data(curCellIndex).toString();

        ConfigFileInformation * configFileInformationWnd = new ConfigFileInformation(configFilePath, this);
        configFileInformationWnd->setWindowTitle(QObject::tr("Show Config File Info"));
        configFileInformationWnd->setWindowModality(Qt::ApplicationModal); //setting block model
        configFileInformationWnd->setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
        configFileInformationWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
        configFileInformationWnd->show();
    }
}

void ConfigFileWindow::configFileCustomMenuRequested(QPoint pos){

    // get select row index
    QModelIndexList selectedIndexes = m_configFileTableView->selectionModel()->selectedRows();
    QModelIndex clickedIndex = m_configFileTableView->indexAt(pos);   // get right click row index
    if (clickedIndex.isValid()){// if index is valid， continue
        QMenu * configFileMenu = new QMenu(this);

        QAction*  configFileStatusAction = new QAction(tr("View Edit File"), this);
        configFileMenu->addAction(configFileStatusAction);

        configFileMenu->addSeparator();

        QAction* configFileRefreshtAction = new QAction(tr("Refresh File List"), this);
        configFileMenu->addAction(configFileRefreshtAction);

        connect(configFileStatusAction, &QAction::triggered, this, &ConfigFileWindow::handleConfigFileStatusAction); // 连接槽函数处理操作
        connect(configFileRefreshtAction, &QAction::triggered, this, &ConfigFileWindow::handleConfigFileRefreshAction);

        configFileMenu->popup(m_configFileTableView->viewport()->mapToGlobal(pos));
    }
}

void ConfigFileWindow::handleConfigFileStatusAction()
{
    QModelIndexList selectedIndexes = m_configFileTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_confFileModel->index(curRow,0);
        QString configFilePath = m_confFileModel->data(curCellIndex).toString();

        ConfigFileInformation * configFileInformationWnd = new ConfigFileInformation(configFilePath, this);
        configFileInformationWnd->setWindowTitle(QObject::tr("Show Config File Info"));
        configFileInformationWnd->setWindowModality(Qt::ApplicationModal); //setting block model
        configFileInformationWnd->setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
        //configFileInformationWnd->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        configFileInformationWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
        configFileInformationWnd->show();
    }
}

void ConfigFileWindow::handleConfigFileRefreshAction()
{
    slotRefreshConfFileList();
}

void ConfigFileWindow::slotRefreshConfFileList()
{
    for (const conffile &f : m_confFileList) {
        if (!QFileInfo::exists(f.filePath)) {
            continue;
        }
        QStandardItem *item = new QStandardItem(QIcon::fromTheme(QStringLiteral("text-plain")), QStringLiteral("%1").arg(f.filePath));
        QStandardItem *item2 = new QStandardItem(QFileInfo(f.filePath).lastModified().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
        QStandardItem *item3 = new QStandardItem(QFileInfo(f.filePath).birthTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
        QStandardItem *item4 = new QStandardItem( QString::number(QFileInfo(f.filePath).size()));
        QStandardItem *item5 = new QStandardItem(f.description);
        m_confFileModel->setItem(m_confFileList.indexOf(f), 0, item);
        m_confFileModel->setItem(m_confFileList.indexOf(f), 1, item2);
        m_confFileModel->setItem(m_confFileList.indexOf(f), 2, item3);
        m_confFileModel->setItem(m_confFileList.indexOf(f), 3, item4);
        m_confFileModel->setItem(m_confFileList.indexOf(f), 4, item5);
    }
}


void ConfigFileWindow::resizeEvent(QResizeEvent *event)
{
    if (nullptr != m_configFileTableView)
    {
        m_configFileTableView->resize(width(), height());
        update();
    }
}
