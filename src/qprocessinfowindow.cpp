/********************************************************************************
 * Obtained from https://github.com/epasveer/QProcessInfoWidget                 *
 * My version (epasveer) is based on this person's code.                        *
 *                                                                              *
 * Copyright (c) 2016, Baldur Karlsson                                          *
 * Licensed under BSD 2-Clause License, see LICENSE file.                       *
 * Obtained from https://github.com/baldurk/qprocessinfo                        *
 *                                                                              *
 *                                                                              *
 * This version is modify by Han Jinpeng <hanjinpeng127@gmail.com>              *
 * Website: https://github.com/prownd/systemd-manage                            *
 *                                                                              *
 * Licensed under BSD 2-Clause License or GPLv3                                 *
 ********************************************************************************/

#include "qprocessinfowindow.h"
#include "qprocessinfo.h"
#include "qprocessdetail.h"

#include <QTreeWidgetItemIterator>
#include <QFont>
#include <QDebug>
#include <QApplication>
#include <QMenu>
#include <QMessageBox>

QString qprocessWindowToolButtonStyleSheet1(R"(
/* QToolButton size style */
QToolButton{
    border-top: 0px outset transparent;
    border-bottom: 0px outset transparent;
    border-right: 0px outset transparent;
    border-left: 0px outset transparent;
    border-radius:6px;
    background-color: #dedede;
    width: 120px;
    height: 40px;
    margin: 0px 0px;
}

/* hover */
QToolButton:hover{
    background-color: #c7c7c7;
}

/* press */
QToolButton:pressed {
    border-left: 0px outset rgb(93, 95, 97);
    background-color: #c7c7c7;
}

/* checked */
QToolButton:checked {
    background-color: #c7c7c7;
}

/* hover and checked */
QToolButton:hover:checked {
    background-color: #d3d3d3;
}

/* press and checked */
QToolButton:pressed:checked {
    border-left: 0px outset rgb(93, 95, 97);
    background-color: #c7c7c7;
}

)");

QProcessInfoWidget::QProcessInfoWidget(QWidget* parent) :
    QWidget(parent)
{
    //frameless window
    setWindowFlags(Qt::FramelessWindowHint);

    createProcessInfoView();
}

QProcessInfoWidget::~QProcessInfoWidget()
{
}

void QProcessInfoWidget::createProcessInfoView()
{
    m_mainGridLayout = new QGridLayout;

    m_programNameLabel = new QLabel;
    m_programNameLabel->setText(tr("Program Name:"));

    m_programNameLineEdit = new QLineEdit;
    m_programNameLineEdit->setPlaceholderText(tr("Search Program"));

    m_userNameLabel = new QLabel;
    m_userNameLabel->setText(tr("User Name:"));

    m_userNameLineEdit = new QLineEdit;
    m_userNameLineEdit->setPlaceholderText(tr("Search User"));

    m_refreshToolButton = new QToolButton;
    m_refreshToolButton->setText(addTextSpaceMargin(tr("Refresh")));
    m_refreshToolButton->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    m_refreshToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_refreshToolButton->setMinimumWidth(120);
    m_refreshToolButton->setStyleSheet(qprocessWindowToolButtonStyleSheet1);

    m_mainGridLayout->addWidget(m_programNameLabel, 0, 0, 1, 1);
    m_mainGridLayout->addWidget(m_programNameLineEdit, 0, 1, 1, 1);
    m_mainGridLayout->addWidget(m_userNameLabel, 0, 2, 1, 1);
    m_mainGridLayout->addWidget(m_userNameLineEdit, 0, 3, 1, 1);
    m_mainGridLayout->addWidget(m_refreshToolButton, 0, 4, 1, 2);

    m_mainGridLayout->setColumnStretch(0, 0);
    m_mainGridLayout->setColumnStretch(1, 0);
    m_mainGridLayout->setColumnStretch(2, 0);
    m_mainGridLayout->setColumnStretch(3, 0);

    m_processTreeWidget = new QTreeWidget;   
    m_processTreeWidget->setColumnCount(4);
    QStringList headers;
    headers << QString(tr("PID"))
            << QString(tr("PPID"))
            << QString(tr("User Name"))
            << QString(tr("Program Name"))
            << QString(tr("% CPU"))
            << QString(tr("Memory"))
            << QString(tr("State"))
            << QString(tr("Nice"))
            << QString(tr("Command Line"));
    m_processTreeWidget->setHeaderLabels(headers);
    m_processTreeWidget->setMouseTracking(true);
    m_processTreeWidget->resizeColumnToContents(0);
    m_processTreeWidget->resizeColumnToContents(1);
    m_processTreeWidget->resizeColumnToContents(2);
    m_processTreeWidget->resizeColumnToContents(3);
    m_processTreeWidget->resizeColumnToContents(4);
    m_processTreeWidget->resizeColumnToContents(5);
    m_processTreeWidget->resizeColumnToContents(6);
    m_processTreeWidget->resizeColumnToContents(7);
    m_processTreeWidget->setSortingEnabled(true);
    m_processTreeWidget->clear();

    m_mainGridLayout->addWidget(m_processTreeWidget, 1, 0, 1, 6);
    this->setLayout(m_mainGridLayout);

    QObject::connect(m_programNameLineEdit, &QLineEdit::textChanged, this, &QProcessInfoWidget::refreshView);
    QObject::connect(m_userNameLineEdit, &QLineEdit::textChanged, this, &QProcessInfoWidget::refreshView);
    QObject::connect(m_refreshToolButton, &QToolButton::clicked, this, &QProcessInfoWidget::refreshList);
    QObject::connect(m_processTreeWidget, &QTreeWidget::itemDoubleClicked, this, &QProcessInfoWidget::handleDoubleClicked);

    //QTreeWidget custom context policy
    m_processTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_processTreeWidget, &QTreeWidget::customContextMenuRequested, this, &QProcessInfoWidget::QTreeWidgetitemPressedSlot);
    // qtreewidget signal and slot
    //connect(m_processTreeWidget, &QTreeWidget::itemPressed, this, &QProcessInfoWidget::QTreeWidgetitemPressedSlot);

    // double click handle
    connect(m_processTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slotTreeWidgetItemDoubleClicked(QTreeWidgetItem*, int)));

    // Load the initial process list.
    refreshList();
}


void QProcessInfoWidget::slotTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column)
{
    //qDebug() << "Double clicked item: " << item->text(column);
    quint64  pid = item->text(0).toULong();
    QProcessDetail * qprocessDetailWnd = new QProcessDetail(pid, this);
    qprocessDetailWnd->setWindowTitle(QObject::tr("Process Properties Information"));
    qprocessDetailWnd->setWindowModality(Qt::ApplicationModal);
    qprocessDetailWnd->setAttribute(Qt::WA_ShowModal, true);
    qprocessDetailWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
    qprocessDetailWnd->show();
}

//void QProcessInfoWidget::QTreeWidgetitemPressedSlot(QTreeWidgetItem * pressedItem, int column)
void QProcessInfoWidget::QTreeWidgetitemPressedSlot(const QPoint& pos)
{

    if(qApp->mouseButtons() == Qt::RightButton) // button right click
    {
        //qDebug() <<"pressedItem: " << pressedItem->text(column);
        QTreeWidgetItem * selectTreeItem = m_processTreeWidget->itemAt(pos);
        //qDebug()<< "pos:" << pos << " selectTreeItem:" << selectTreeItem->text(0);

        QTreeWidget* qprocessTree = selectTreeItem->treeWidget();
        QMenu* qprocessMenu = new QMenu(qprocessTree);

        QAction* qprocessDetailAction = new QAction(tr("Process Detail Properties"));
        quint64 pid = selectTreeItem->text(0).toULong();
        qprocessDetailAction->setData(QVariant::fromValue(pid));

        qprocessMenu->addAction(qprocessDetailAction);

        qprocessMenu->addSeparator();

        QAction* qprocessKillAction = new QAction(tr("Kill Process"));
        qprocessMenu->addAction(qprocessKillAction);

        connect(qprocessDetailAction, &QAction::triggered, this, &QProcessInfoWidget::handleQProcessDetailAction);
        connect(qprocessKillAction, &QAction::triggered, this, &QProcessInfoWidget::handleQProcessKillAction);

        // popup and display
        //qprocessMenu->exec(QCursor::pos());
        qprocessMenu->popup(m_processTreeWidget->viewport()->mapToGlobal(pos));
    }
}

void QProcessInfoWidget::handleQProcessDetailAction()
{
    QAction* pSendAction= qobject_cast<QAction*>(sender());
    quint64 pSendActionData = pSendAction->data().value<quint64>();
    //qDebug() << "pid:" << pSendActionData;
    //QMessageBox::warning(this, "Action", "Open test");

    QProcessDetail * qprocessDetailWnd = new QProcessDetail(pSendActionData, this);
    qprocessDetailWnd->setWindowTitle(QObject::tr("Process Properties Information"));
    qprocessDetailWnd->setWindowModality(Qt::ApplicationModal);
    qprocessDetailWnd->setAttribute(Qt::WA_ShowModal, true);
    qprocessDetailWnd->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
    qprocessDetailWnd->show();
}

void QProcessInfoWidget::handleQProcessKillAction()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, tr("Warning"), tr("Are you sure to Kill Process?"), QMessageBox::Yes|QMessageBox::No);
    msgBox->button(QMessageBox::Yes)->setText(tr("Yes"));
    msgBox->button(QMessageBox::No)->setText(tr("No"));
    msgBox->setDefaultButton(QMessageBox::No);
    int res = msgBox->exec();
    if(QMessageBox::Yes == res) {
        qDebug()<<"Kill Process";
    }else if(QMessageBox::No == res) {
        return;
    }
}

int QProcessInfoWidget::selectedPid() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return -1;
    }
    return items[0]->text(0).toLong();
}

int QProcessInfoWidget::selectedPPid() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return -1;
    }
    return items[0]->text(1).toLong();
}

QString QProcessInfoWidget::selectedUsername() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return "";
    }
    return items[0]->text(2);
}

QString QProcessInfoWidget::selectedName() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return "";
    }
    return items[0]->text(3);
}

QString QProcessInfoWidget::selectedCPUTime() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return "";
    }
    return items[0]->text(4);
}

quint64 QProcessInfoWidget::selectedMemory() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return 0;
    }
    return items[0]->text(5).toLong();
}

QString QProcessInfoWidget::selectedState() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return "";
    }
    return items[0]->text(6);
}

QString QProcessInfoWidget::selectedNice() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return "";
    }
    return items[0]->text(7);
}

QString QProcessInfoWidget::selectedCommandLine() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return "";
    }
    return items[0]->text(8);
}

void QProcessInfoWidget::refreshList()
{
    QApplication::setOverrideCursor(Qt::BusyCursor);

    // Scan the /proc file system.
    //QProcessList list = QProcessInfo::populate();
    QProcessList list = QProcessInfo::getProcInfoList();

    // Loop through each entry and add it to our view.
    m_processTreeWidget->clear();

    for (const QProcessInfo& info : list) {
        QTreeWidgetItem* item = new QProcessInfoWidgetItem;
        item->setText(0, QString::number(info.getPid()));
        item->setText(1, QString::number(info.getPPid()));
        item->setText(2, info.getUsername());
        item->setText(3, info.getName());
        item->setText(4, info.getCPUTime());
        //item->setText(5, QString::number((info.getMem())));
        item->setText(5, QProcessInfo::format_memsize(info.getMem()));
        item->setText(6, info.getState());
        item->setText(7, info.getNiceLevel(info.getNice()));
        item->setText(8, info.getCommandLine());

        //item add tooltip
        QString toolTipText;
        toolTipText.append(QStringLiteral("<FONT COLOR=DarkCyan>"));
        toolTipText.append(tr("<b>Process Name: %1</b><hr>").arg(info.getUsername()));
        toolTipText.append(tr("<b>Process ID: %1</b><hr>").arg(info.getPid()));
        toolTipText.append(tr("<b>User: %1</b><hr>").arg(info.getUsername()));
        toolTipText.append(tr("<b>Status: %1</b><hr>").arg(info.getState()));
        toolTipText.append(tr("<b>Memory: %1</b><hr>").arg(info.getMem()));
        toolTipText.append(tr("<b>Virtual Memory: %1</b><hr>").arg(info.getVmsize()));
        toolTipText.append(tr("<b>Resident Memory: %1</b><hr>").arg(info.getMemres()));
        toolTipText.append(tr("<b>Writable Memory: %1</b><hr>").arg(info.getMemwritable()));
        toolTipText.append(tr("<b>Shared Memory: %1</b><hr>").arg(info.getMemshared()));
        toolTipText.append(tr("<b>CPU: %1</b><hr>").arg(info.getCPUTime()));
        toolTipText.append(tr("<b>Priority: %1</b><hr>").arg(info.getNiceLevel(info.getNice())));
        toolTipText.append(tr("<b>Command Line: %1</b><hr>").arg(info.getCommandLine()));
        toolTipText.append(QStringLiteral("</FONT"));
        for (int i =0 ;i <9; i++){
            item->setToolTip(i, toolTipText);
        }

        m_processTreeWidget->addTopLevelItem(item);
    }

    // Adjust the column widths.
    m_processTreeWidget->clearSelection();
    m_processTreeWidget->resizeColumnToContents(0);
    m_processTreeWidget->resizeColumnToContents(1);
    m_processTreeWidget->resizeColumnToContents(2);
    m_processTreeWidget->resizeColumnToContents(3);
    m_processTreeWidget->resizeColumnToContents(4);
    m_processTreeWidget->resizeColumnToContents(5);
    m_processTreeWidget->resizeColumnToContents(6);
    m_processTreeWidget->resizeColumnToContents(7);
    refreshView();

    QApplication::restoreOverrideCursor();
}

void QProcessInfoWidget::refreshView()
{
    // Get this list of program name matches. Or all if there is no program name provided.
    QList<QTreeWidgetItem*> programNameMatches;

    if (m_programNameLineEdit->text() == "") {
        programNameMatches = m_processTreeWidget->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive, 3);
    }else{
        if (m_programNameLineEdit->text().contains('*')) {
            programNameMatches = m_processTreeWidget->findItems(m_programNameLineEdit->text(), Qt::MatchWildcard   | Qt::MatchRecursive, 3);
        }else{
            programNameMatches = m_processTreeWidget->findItems(m_programNameLineEdit->text(), Qt::MatchStartsWith | Qt::MatchRecursive, 3);
        }
    }

    // Get this list of user name matches. Or all if there is no user name provided.
    QList<QTreeWidgetItem*> userNameMatches;
    if (m_userNameLineEdit->text() == "") {
        userNameMatches = m_processTreeWidget->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive, 2);
    }else{
        if (m_userNameLineEdit->text().contains('*')) {
            userNameMatches = m_processTreeWidget->findItems(m_userNameLineEdit->text(), Qt::MatchWildcard   | Qt::MatchRecursive, 2);
        }else{
            userNameMatches = m_processTreeWidget->findItems(m_userNameLineEdit->text(), Qt::MatchStartsWith | Qt::MatchRecursive, 2);
        }
    }

    // Go through each item in the tree.
    // If it's in the user, program, show it. Otherwise, hide it.
    QTreeWidgetItemIterator it(m_processTreeWidget);
    while (*it) {
       if (programNameMatches.contains(*it) && userNameMatches.contains(*it)) {
            (*it)->setHidden(false);
        }else{
            (*it)->setHidden(true);
        }
        ++it;
    }

    // Resize the columns.
    m_processTreeWidget->resizeColumnToContents(0);
    m_processTreeWidget->resizeColumnToContents(1);
    m_processTreeWidget->resizeColumnToContents(2);
    m_processTreeWidget->resizeColumnToContents(3);
    m_processTreeWidget->resizeColumnToContents(4);
    m_processTreeWidget->resizeColumnToContents(5);
    m_processTreeWidget->resizeColumnToContents(6);
    m_processTreeWidget->resizeColumnToContents(7);
    m_processTreeWidget->resizeColumnToContents(8);
}

void QProcessInfoWidget::handleDoubleClicked()
{
    emit pidSelected(selectedPid());
}

QString QProcessInfoWidget::addTextSpaceMargin(const QString strText){
    QString strRet = "  " + strText;
    return strRet;
}
