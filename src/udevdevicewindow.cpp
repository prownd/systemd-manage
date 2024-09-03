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

#include "udevdevicewindow.h"
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
#include <QFileInfo>
#include <QDateTime>
#include <QFileSystemWatcher>
#include <QDir>
#include "udevdevicedetail.h"
#include "udevrulefileinformation.h"

UdevDeviceWindow::UdevDeviceWindow(QWidget *parent) :
    QWidget(parent)
{
    //frameless window
    setWindowFlags(Qt::FramelessWindowHint);

    initData();
    slotRefreshUdevDeviceList(true);
    createTabWidgetView();
    createUdevDeviceTableView();
    createUdevRuleFileTableView();
    show();
}

UdevDeviceWindow::~UdevDeviceWindow()
{
}

void UdevDeviceWindow::initData()
{
    m_udevDeviceColumnCount = 5;
    m_udevRuleFileColumnCount = 6;
}

void UdevDeviceWindow::createTabWidgetView()
{
    // init tabwidget
    m_pTabWidget = new QTabWidget(this);
    m_pTabWidget->setMinimumWidth(width());
    m_pTabWidget->setMinimumHeight(height());
    m_pTabWidget->adjustSize();

    // init udev device subwidget
    m_pUdevDeviceWidget = new QWidget();
    m_subUdevDeviceLayout = new QVBoxLayout(m_pUdevDeviceWidget);
    m_pUdevDeviceWidget->setLayout(m_subUdevDeviceLayout);

    // init udev rule files subwidget
    m_pUdevRuleFileWidget = new QWidget();
    m_subUdevRuleFileLayout = new QVBoxLayout(m_pUdevRuleFileWidget);
    m_pUdevRuleFileWidget->setLayout(m_subUdevRuleFileLayout);

    // add all sub widget
    m_pTabWidget->addTab(m_pUdevDeviceWidget, tr("Udev Device"));
    m_pTabWidget->addTab(m_pUdevRuleFileWidget, tr("Udev Rule File"));
}

void UdevDeviceWindow::createUdevDeviceTableView()
{
    m_filterUdevDeviceHLayout = new  QHBoxLayout;

    QStringList p_filterUdevDeviceTypes = QStringList{tr("All"), tr("disk"), tr("partition"),
                                                      tr("usb_device"), tr("usb_interface"), tr("port"),
                                                      tr("wlan")};
    m_udevDeviceTypeComboBox = new QComboBox;
    m_udevDeviceTypeComboBox->addItems(p_filterUdevDeviceTypes);


    m_searchUdevDeviceLineEdit = new QLineEdit;

    m_filterUdevDeviceHLayout->addWidget(m_udevDeviceTypeComboBox);
    //m_filterUdevDeviceHLayout->addStretch(0);
    m_filterUdevDeviceHLayout->addWidget(m_searchUdevDeviceLineEdit);
    m_filterUdevDeviceHLayout->addStretch(0);

    m_subUdevDeviceLayout->addLayout(m_filterUdevDeviceHLayout);

    m_udevDeviceTableView = new QTableView(m_pUdevDeviceWidget);
    m_udevDeviceStandItemModel = new QStandardItemModel();

    m_udevDevice = new UdevDevice;
    m_udevDeviceList = m_udevDevice->getAllDevicesList();
    m_udevDeviceModel = new UdevModel(this, &m_udevDeviceList, m_udevDeviceColumnCount);

    QMap<filterUdevType, QString> filters;
    filters[udevType] = QString();
    filters[udevPath] = QString();
    m_udevDeviceFilterModel = new CustomSortFilterProxyUdevModel(this);
    m_udevDeviceFilterModel->setDynamicSortFilter(false);
    m_udevDeviceFilterModel->initFilterMap(filters);
    m_udevDeviceFilterModel->setSourceModel(m_udevDeviceModel);

    //m_udevDeviceTableView->setModel(m_udevDeviceModel);
    m_udevDeviceTableView->setModel(m_udevDeviceFilterModel);
    m_udevDeviceTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //m_udevDeviceTableView->horizontalHeader()->setDefaultSectionSize(80);
    m_udevDeviceTableView->verticalHeader()->setDefaultSectionSize(40);
    m_udevDeviceTableView->resizeColumnsToContents();
    m_udevDeviceTableView->horizontalHeader()->setStretchLastSection(true);

    m_udevDeviceTableView->verticalHeader()->hide();
    m_udevDeviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_udevDeviceTableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_udevDeviceTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //m_udevDeviceTableView->setColumnHidden(m_udevDeviceColumnCount-1, true);

    m_udevDeviceTableView->setMinimumWidth(width());
    m_udevDeviceTableView->setMinimumHeight(height());
    m_udevDeviceTableView->adjustSize();
    m_udevDeviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_udevDeviceTableView->setMouseTracking(true);

    m_subUdevDeviceLayout->addWidget(m_udevDeviceTableView);

    //create contextMenu
    m_udevDeviceTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_udevDeviceTableView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(udevDeviceCustomMenuRequested(QPoint)));

    m_udevDeviceCountLabel = new QLabel;
    m_udevDeviceCountLabel->setText(tr("total: 1 udev device, display: 1 udev device"));
    m_subUdevDeviceLayout->addWidget(m_udevDeviceCountLabel);

    connect(m_udevDeviceTypeComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &UdevDeviceWindow::slotComboBoxShowUdevDevice);
    connect(m_searchUdevDeviceLineEdit, &QLineEdit::textChanged, this, &UdevDeviceWindow::slotLineEditSearchUdevDeviceChanged);

    connect(m_udevDeviceTableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotUdevDeviceTableRowDoubleClicked(const QModelIndex &)));
    connect(m_udevDeviceTableView, SIGNAL(entered(QModelIndex)), this, SLOT(slotUdevDeviceTableRowTooltip(QModelIndex)));

    updateUdevDeviceCount();
}


void UdevDeviceWindow::slotUdevDeviceTableRowTooltip(const QModelIndex index)
{
    /*
    int curRow=index.row();
    const QModelIndex& curCellIndex = m_systemUnitFilterModel->index(curRow,0);
    QString systemUnitName = m_systemUnitFilterModel->data(curCellIndex).toString();

    // QVariantList list = unitPropertyRequiresStr.toList();
    // foreach(QVariant e, list ){  qDebug()<< e.toString(); }

    QString toolTipText;
    toolTipText.append(QStringLiteral("<FONT COLOR=DarkCyan>"));
    toolTipText.append(QStringLiteral("<b>Unit Name:%1</b><hr>").arg(systemUnitName));
    toolTipText.append(QStringLiteral("<b>Unit ID:%1</b><hr>").arg(unitPropertyIDStr));
    toolTipText.append(QStringLiteral("<b>Unit ID:%1</b><hr>").arg(unitPropertyActiveStateStr));
    toolTipText.append(QStringLiteral("</FONT"));
    */

    //m_systemUnitFilterModel->itemFromIndex(index)->setToolTip(toolTipText);
    //QToolTip::showText(QCursor::pos(), toolTipText, this, QRect(), -1);

    return;
}

void UdevDeviceWindow::slotUdevDeviceTableRowDoubleClicked(const QModelIndex index)
{
    QModelIndexList selectedIndexes = m_udevDeviceTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        //int curRow = index.row();
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_udevDeviceFilterModel->index(curRow, 3);
        QString udevDevicePath = m_udevDeviceFilterModel->data(curCellIndex).toString();
        //qDebug()<<"cell data 1:"<< udevDevicePath;

        UdevDevice selectUdevDevice;
        bool foundSelected = false;
        for (const UdevDevice &udevItem : m_udevDeviceList)
        {
            if (udevItem.devicePath() == udevDevicePath)
            {
                selectUdevDevice = udevItem;
                foundSelected = true;
                break;
            }
        }
        if(foundSelected){
            //QMessageBox::warning(this, "Action", "found selected");
            UdevDeviceDetail * udevDeviceDetailWnd = new UdevDeviceDetail(selectUdevDevice, this);
            udevDeviceDetailWnd->setWindowTitle(QObject::tr("Udev Device Information"));
            udevDeviceDetailWnd->setWindowModality(Qt::ApplicationModal);
            udevDeviceDetailWnd->setAttribute(Qt::WA_ShowModal, true);
            udevDeviceDetailWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
            udevDeviceDetailWnd->show();
        }else{
            //QMessageBox::warning(this, "Action", "not found selected");
            QMessageBox messageBox;
            messageBox.setWindowTitle(tr("Warning"));
            messageBox.setText(tr("Not found Udev Device, please refresh!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.button(QMessageBox::Ok)->setText(tr("Ok"));
            messageBox.exec();
        }

    }
}

void UdevDeviceWindow::udevDeviceCustomMenuRequested(QPoint pos)
{
    QModelIndexList selectedIndexes = m_udevDeviceTableView->selectionModel()->selectedRows();

    if (!selectedIndexes.isEmpty())
    {
        QModelIndex clickedIndex = m_udevDeviceTableView->indexAt(pos);   // get index when right click
        if (clickedIndex.isValid()){ // if index valid, continue
            QMenu * udevDeviceMenu = new QMenu(this);

            QAction*  udevDeviceInfoStatusAction = new QAction(tr("Udev Device info"), this);
            udevDeviceMenu->addAction(udevDeviceInfoStatusAction);

            udevDeviceMenu->addSeparator();

            QAction* udevDeviceRefreshAction = new QAction(tr("Refresh"), this);
            udevDeviceMenu->addAction(udevDeviceRefreshAction);

            // unit status info
            connect(udevDeviceInfoStatusAction, &QAction::triggered, this, &UdevDeviceWindow::handleUdevDeviceInfoAction);
            connect(udevDeviceRefreshAction, &QAction::triggered, this, &UdevDeviceWindow::handleUdevDeviceRefreshAction);

            udevDeviceMenu->popup(m_udevDeviceTableView->viewport()->mapToGlobal(pos));
        }
    }
}

void UdevDeviceWindow::handleUdevDeviceInfoAction()
{
    QModelIndexList selectedIndexes = m_udevDeviceTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_udevDeviceFilterModel->index(curRow,3);
        QString udevDevicePath = m_udevDeviceFilterModel->data(curCellIndex).toString();
        //qDebug()<<"cell data:"<< udevDevicePath;

        UdevDevice selectUdevDevice;
        bool foundSelected = false;
        for (const UdevDevice &udevItem : m_udevDeviceList)
        {
            if (udevItem.devicePath() == udevDevicePath)
            {
                selectUdevDevice = udevItem;
                foundSelected = true;
                break;
            }
        }

        if(foundSelected){
            UdevDeviceDetail * udevDeviceDetailWnd = new UdevDeviceDetail(selectUdevDevice, this);
            udevDeviceDetailWnd->setWindowTitle(QObject::tr("Udev Device Information"));
            udevDeviceDetailWnd->setWindowModality(Qt::ApplicationModal);
            udevDeviceDetailWnd->setAttribute(Qt::WA_ShowModal, true);
            udevDeviceDetailWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
            udevDeviceDetailWnd->show();
        }else{
            QMessageBox messageBox;
            messageBox.setWindowTitle(tr("Warning"));
            messageBox.setText(tr("Not found Udev Device, please refresh!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.button(QMessageBox::Ok)->setText(tr("Ok"));
            messageBox.exec();
        }
    }
}

void UdevDeviceWindow::handleUdevDeviceRefreshAction()
{
    slotRefreshUdevDeviceList(false);
}

void UdevDeviceWindow::setupUdevDevicelist()
{
    // Sets up the units list initially
    m_udevDevice = new UdevDevice;
    m_udevDeviceList = m_udevDevice->getAllDevicesList();
    m_udevDeviceModel = new UdevModel(this, &m_udevDeviceList, m_udevDeviceColumnCount);
}

void UdevDeviceWindow::slotLineEditSearchUdevDeviceChanged(QString term)
{
    m_udevDeviceFilterModel->addFilterRegExp(udevPath, term);
    m_udevDeviceFilterModel->invalidate();
    m_udevDeviceTableView->sortByColumn(m_udevDeviceTableView->horizontalHeader()->sortIndicatorSection(),
                              m_udevDeviceTableView->horizontalHeader()->sortIndicatorOrder());
    updateUdevDeviceCount();
}

void UdevDeviceWindow::slotComboBoxShowUdevDevice(int index)
{
    //qDebug()<<"slotComboBoxShowSystemUnits: "<< index;
    m_udevDeviceFilterModel->addFilterRegExp(udevType,  QStringLiteral("(%1)$").arg(m_udevTypeSufix.at(index)));
    m_udevDeviceFilterModel->invalidate();
    m_udevDeviceTableView->sortByColumn(m_udevDeviceTableView->horizontalHeader()->sortIndicatorSection(),
                              m_udevDeviceTableView->horizontalHeader()->sortIndicatorOrder());
    updateUdevDeviceCount();
}

void UdevDeviceWindow::updateUdevDeviceCount()
{
    QString totalUdevDeviceShowText = tr("Total: %1 udev device,").arg(m_udevDeviceModel->rowCount());
    QString displayUdevDeviceShowText = tr("Display: %1 udev device").arg(m_udevDeviceFilterModel->rowCount());
    QString udevDeviceShowText = tr("%1 %2").arg(totalUdevDeviceShowText).arg(displayUdevDeviceShowText);
    m_udevDeviceCountLabel->setText(udevDeviceShowText);
}

void UdevDeviceWindow::slotRefreshUdevDeviceList(bool initial)
{
    // Updates the unit lists
    if (!initial) {
        //qDebug() << "not initial udev device ...";
        m_udevDeviceModel->dataChanged(m_udevDeviceModel->index(0, 0), m_udevDeviceModel->index(m_udevDeviceModel->rowCount(), 0));
        m_udevDeviceFilterModel->invalidate();
        updateUdevDeviceCount();
    }
}


// udev rule file
void UdevDeviceWindow::createUdevRuleFileTableView()
{
    m_filterUdevRuleFileHLayout = new  QHBoxLayout;

    m_searchUdevRuleFileLabel = new QLabel;
    m_searchUdevRuleFileLabel->setText(tr("Search Udev Rule:"));
    m_searchUdevRuleFileLineEdit = new QLineEdit;

    m_filterUdevRuleFileHLayout->addWidget(m_searchUdevRuleFileLabel);
    m_filterUdevRuleFileHLayout->addWidget(m_searchUdevRuleFileLineEdit);
    m_filterUdevRuleFileHLayout->addStretch(0);

    m_subUdevRuleFileLayout->addLayout(m_filterUdevRuleFileHLayout);

    m_udevRuleFileTableView= new QTableView();


    m_subUdevRuleFileLayout->addWidget(m_udevRuleFileTableView);

    m_udevRuleFileItemModel = new QStandardItemModel(this);
    m_udevRuleFileItemModel->setHorizontalHeaderItem(0, new QStandardItem(tr("File Name")));
    m_udevRuleFileItemModel->setHorizontalHeaderItem(1, new QStandardItem(tr("File Path")));
    m_udevRuleFileItemModel->setHorizontalHeaderItem(2, new QStandardItem(tr("Modified")));
    m_udevRuleFileItemModel->setHorizontalHeaderItem(3, new QStandardItem(tr("Created Time")));
    m_udevRuleFileItemModel->setHorizontalHeaderItem(4, new QStandardItem(tr("Size (Byte)")));
    m_udevRuleFileItemModel->setHorizontalHeaderItem(5, new QStandardItem(tr("Description")));

    //m_configFileTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_udevRuleFileTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_udevRuleFileTableView->verticalHeader()->setDefaultSectionSize(40);
    m_udevRuleFileTableView->verticalHeader()->hide();

    m_udevRuleFileTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_udevRuleFileTableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_udevRuleFileTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_udevRuleFileTableView->resizeColumnsToContents();
    m_udevRuleFileTableView->horizontalHeader()->setStretchLastSection(true);

    m_udevRuleFileTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_udevRuleFileTableView->setColumnWidth(0,500);

    // Set model for QTableView (should be called after headers are set)
    getAllUdevRuleFiles();
    m_udevRuleFileModel = new UdevRuleModel(this, &m_udevRuleFileList, m_udevRuleFileColumnCount);
    QMap<filterUdevRuleFileType, QString> filters;
    filters[udevRuleFileName] = QString();
    filters[udevRuleFilePath] = QString();
    m_udevRuleFileFilterModel = new CustomSortFilterProxyUdevRuleFileModel(this);
    m_udevRuleFileFilterModel->setDynamicSortFilter(false);
    m_udevRuleFileFilterModel->initFilterMap(filters);
    m_udevRuleFileFilterModel->setSourceModel(m_udevRuleFileModel);
    m_udevRuleFileTableView->setModel(m_udevRuleFileFilterModel);

    m_udevRuleFileTableView->sortByColumn(1, Qt::AscendingOrder);
    m_udevRuleFileTableView->setMouseTracking(true);

    /*
    m_udevRuleFileList.append(UdevRuleFile(QStringLiteral("/usr/lib/udev/rules.d/60-net.rules"),
                                   QStringLiteral("60-net.rules"),
                                   tr("udev net rule")));
    */



    //create contextMenu
    m_udevRuleFileTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_udevRuleFileTableView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(udevRuleFileCustomMenuRequested(QPoint)));

    m_udevRuleFileCountLabel = new QLabel;
    m_udevRuleFileCountLabel->setText(tr("total: 1 udev rule, display: 1 udev rule"));
    m_subUdevRuleFileLayout->addWidget(m_udevRuleFileCountLabel);

    connect(m_searchUdevRuleFileLineEdit, &QLineEdit::textChanged, this, &UdevDeviceWindow::slotLineEditSearchUdevRuleFileChanged);

    connect(m_udevRuleFileTableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotUdevRuleFileTableRowDoubleClicked(const QModelIndex &)));
    //connect(m_udevRuleFileTableView, SIGNAL(entered(QModelIndex)), this, SLOT(slotUdevRuleFileTableRowTooltip(QModelIndex)));

    slotRefreshUdevRuleFileList();

    QFileSystemWatcher *m_fileWatcher = new QFileSystemWatcher;
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &UdevDeviceWindow::slotRefreshUdevRuleFileList);
    for (const UdevRuleFile &f : m_udevRuleFileList) {
        m_fileWatcher->addPath(f.filePath);
    }

    updateUdevRuleFileCount();
}

void UdevDeviceWindow::listFilesInDirectory(const QString &path, const QString globstr) {
    QDir dir(path);

    if (!dir.exists()) {
        qCritical() << "dir not exist:" << path;
        return;
    }

    // set filter, list file (no contain subdir)
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    QStringList globfilename ;
    globfilename << globstr;

    // get all file of dir
    QStringList files = dir.entryList(globfilename);

    // print filelist
    foreach (QString file, files) {
        //qDebug() << "filepath:" << dir.absoluteFilePath(file);
        QString filePath = dir.absoluteFilePath(file);
        QString lastModified = QFileInfo(filePath).lastModified().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
        QString birthTime = QFileInfo(filePath).birthTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
        QString fileSize = QString::number(QFileInfo(filePath).size());
        QString describe = " ";
        m_udevRuleFileList.append(UdevRuleFile(filePath, file, lastModified, birthTime, fileSize, describe));
    }
}

void UdevDeviceWindow::getAllUdevRuleFiles()
{
    m_udevRuleFileList.clear();
    QStringList dirList;
    dirList.append("/etc/udev/rules.d/");
    dirList.append("/usr/lib/udev/rules.d/");
    foreach(QString dirPath , dirList)
    {
        listFilesInDirectory(dirPath);
    }

    QString networkLinkDir("/usr/lib/systemd/network/");
    listFilesInDirectory(networkLinkDir, "*.link");
}

void UdevDeviceWindow::slotLineEditSearchUdevRuleFileChanged(QString term)
{
    m_udevRuleFileFilterModel->addFilterRegExp(udevRuleFileName, term);
    m_udevRuleFileFilterModel->invalidate();
    m_udevRuleFileTableView->sortByColumn(m_udevRuleFileTableView->horizontalHeader()->sortIndicatorSection(),
                                   m_udevRuleFileTableView->horizontalHeader()->sortIndicatorOrder());
    updateUdevRuleFileCount();
}

void UdevDeviceWindow::slotUdevRuleFileTableRowTooltip(const QModelIndex index)
{
    bool is_show = false;
    if (is_show)
    {
        int curRow=index.row();
        const QModelIndex& curCellIndex1 = m_udevRuleFileFilterModel->index(curRow,0);
        QString curUdevRuleFileName = m_udevRuleFileFilterModel->data(curCellIndex1).toString();

        const QModelIndex& curCellIndex2 = m_udevRuleFileFilterModel->index(curRow, 1);
        QString curUdevRuleFilePath =m_udevRuleFileFilterModel->data(curCellIndex2).toString();

        const QModelIndex& curCellIndex3 = m_udevRuleFileFilterModel->index(curRow, 3);
        QString curUdevRuleFileCreateTime =m_udevRuleFileFilterModel->data(curCellIndex3).toString();

        const QModelIndex& curCellIndex4 = m_udevRuleFileFilterModel->index(curRow, 4);
        QString curUdevRuleFileSize = m_udevRuleFileFilterModel->data(curCellIndex4).toString();

        const QModelIndex& curCellIndex5 = m_udevRuleFileFilterModel->index(curRow, 5);
        QString curUdevRuleFileDescription = m_udevRuleFileFilterModel->data(curCellIndex5).toString();

        QString toolTipText;
        toolTipText.append(QStringLiteral("<FONT COLOR=DarkCyan>"));
        toolTipText.append(tr("<b>Config File Name: %1</b><hr>").arg(curUdevRuleFileName));
        toolTipText.append(tr("<b>Config File Path: %1</b><hr>").arg(curUdevRuleFilePath));
        toolTipText.append(tr("<b>Config File Create Time: %1</b><hr>").arg(curUdevRuleFileCreateTime));
        toolTipText.append(tr("<b>Config File Size: %1</b><hr>").arg(curUdevRuleFileSize));
        toolTipText.append(tr("<b>Config File Description: %1</b><hr>").arg(curUdevRuleFileDescription));
        toolTipText.append(QStringLiteral("</FONT"));

        //m_udevRuleFileModel->itemFromIndex(index)->setToolTip(toolTipText);
        QToolTip::showText(QCursor::pos(), toolTipText, this, QRect(), -1);
    }
}

void UdevDeviceWindow::slotUdevRuleFileTableRowDoubleClicked(const QModelIndex index)
{
    QModelIndexList selectedIndexes = m_udevRuleFileTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        const QModelIndex& curCellIndex = m_udevRuleFileFilterModel->index(curRow, 1);
        QString udevRuleFilePath = m_udevRuleFileFilterModel->data(curCellIndex).toString();

        UdevRuleFileInformation * udevRuleFileInformationWnd = new UdevRuleFileInformation(udevRuleFilePath, this);
        udevRuleFileInformationWnd->setWindowTitle(QObject::tr("Show Config File Info"));
        udevRuleFileInformationWnd->setWindowModality(Qt::ApplicationModal); //setting block model
        udevRuleFileInformationWnd->setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
        udevRuleFileInformationWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
        udevRuleFileInformationWnd->show();
    }
}

void UdevDeviceWindow::udevRuleFileCustomMenuRequested(QPoint pos){

    // get select row index
    QModelIndexList selectedIndexes = m_udevRuleFileTableView->selectionModel()->selectedRows();
    QModelIndex clickedIndex = m_udevRuleFileTableView->indexAt(pos);   // get right click row index
    if (clickedIndex.isValid()){// if index is valid， continue
        QMenu * udevRuleFileMenu = new QMenu(this);

        QAction*  udevRuleFileStatusAction = new QAction(tr("View Edit File"), this);
        udevRuleFileMenu->addAction(udevRuleFileStatusAction);

        udevRuleFileMenu->addSeparator();

        QAction* udevRuleFileRefreshtAction = new QAction(tr("Refresh File List"), this);
        udevRuleFileMenu->addAction(udevRuleFileRefreshtAction);

        connect(udevRuleFileStatusAction, &QAction::triggered, this, &UdevDeviceWindow::handleUdevRuleFileStatusAction);
        connect(udevRuleFileRefreshtAction, &QAction::triggered, this, &UdevDeviceWindow::handleUdevRuleFileRefreshAction);

        udevRuleFileMenu->popup(m_udevRuleFileTableView->viewport()->mapToGlobal(pos));
    }
}

void UdevDeviceWindow::handleUdevRuleFileStatusAction()
{
    QModelIndexList selectedIndexes = m_udevRuleFileTableView->selectionModel()->selectedRows();
    if (selectedIndexes.count() >=1)
    {
        const QModelIndex& curRowIndex = selectedIndexes.at(0);
        int curRow = curRowIndex.row();
        //file path
        const QModelIndex& curCellIndex = m_udevRuleFileFilterModel->index(curRow,1);
        QString configFilePath = m_udevRuleFileFilterModel->data(curCellIndex).toString();

        UdevRuleFileInformation * configFileInformationWnd = new UdevRuleFileInformation(configFilePath, this);
        configFileInformationWnd->setWindowTitle(QObject::tr("Show Config File Info"));
        configFileInformationWnd->setWindowModality(Qt::ApplicationModal); //setting block model
        configFileInformationWnd->setAttribute(Qt::WA_ShowModal, true);    //property, true:model false:non model
        configFileInformationWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
        configFileInformationWnd->show();
    }
}

void UdevDeviceWindow::updateUdevRuleFileCount()
{
    QString totalUdevRuleFileShowText = tr("Total: %1 udev rule,").arg(m_udevRuleFileModel->rowCount());
    QString displayUdevRuleFileShowText = tr("Display: %1 udev rule").arg(m_udevRuleFileFilterModel->rowCount());
    QString udevRuleFileShowText = tr("%1 %2").arg(totalUdevRuleFileShowText).arg(displayUdevRuleFileShowText);
    m_udevRuleFileCountLabel->setText(udevRuleFileShowText);
}

void UdevDeviceWindow::handleUdevRuleFileRefreshAction()
{
    getAllUdevRuleFiles();
}

void UdevDeviceWindow::slotRefreshUdevRuleFileList()
{
    int index=0;
    for (const UdevRuleFile &f : m_udevRuleFileList) {
        if (!QFileInfo::exists(f.filePath)) {
            continue;
        }
        QStandardItem *item1 = new QStandardItem(QIcon::fromTheme(QStringLiteral("text-plain")), QStringLiteral("%1").arg(f.fileName));
        QStandardItem *item2 = new QStandardItem(QIcon::fromTheme(QStringLiteral("text-plain")), QStringLiteral("%1").arg(f.filePath));
        QStandardItem *item3 = new QStandardItem(QFileInfo(f.filePath).lastModified().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
        QStandardItem *item4 = new QStandardItem(QFileInfo(f.filePath).birthTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
        QStandardItem *item5 = new QStandardItem( QString::number(QFileInfo(f.filePath).size()));
        QStandardItem *item6 = new QStandardItem(f.description);
        m_udevRuleFileItemModel->setItem(index, 0, item1);
        m_udevRuleFileItemModel->setItem(index, 1, item2);
        m_udevRuleFileItemModel->setItem(index, 2, item3);
        m_udevRuleFileItemModel->setItem(index, 3, item4);
        m_udevRuleFileItemModel->setItem(index, 4, item5);
        m_udevRuleFileItemModel->setItem(index, 5, item6);
        index++;
    }
    //updateUdevRuleFileCount();
}



void UdevDeviceWindow::resizeEvent(QResizeEvent *event)
{
    if (nullptr != m_pTabWidget)
    {
       m_pTabWidget->resize(width(), height());
        update();
    }
}
