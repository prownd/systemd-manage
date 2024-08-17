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

#include <QTreeWidgetItemIterator>
#include <QFont>
#include <QDebug>
#include <QApplication>

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
            << QString(tr("Time"))
            << QString(tr("Command Line"));
    m_processTreeWidget->setHeaderLabels(headers);
    m_processTreeWidget->setMouseTracking(true);
    m_processTreeWidget->resizeColumnToContents(0);
    m_processTreeWidget->resizeColumnToContents(1);
    m_processTreeWidget->resizeColumnToContents(2);
    m_processTreeWidget->resizeColumnToContents(3);
    m_processTreeWidget->setSortingEnabled(true);
    m_processTreeWidget->clear();

    m_mainGridLayout->addWidget(m_processTreeWidget, 1, 0, 1, 6);
    this->setLayout(m_mainGridLayout);

    QObject::connect(m_programNameLineEdit, &QLineEdit::textChanged, this, &QProcessInfoWidget::refreshView);
    QObject::connect(m_userNameLineEdit, &QLineEdit::textChanged, this, &QProcessInfoWidget::refreshView);
    QObject::connect(m_refreshToolButton, &QToolButton::clicked, this, &QProcessInfoWidget::refreshList);
    QObject::connect(m_processTreeWidget, &QTreeWidget::itemDoubleClicked, this, &QProcessInfoWidget::handleDoubleClicked);

    // Load the initial process list.
    refreshList();
}

QProcessInfoWidget::~QProcessInfoWidget()
{
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

QString QProcessInfoWidget::selectedTime() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return "";
    }
    return items[0]->text(4);
}

QString QProcessInfoWidget::selectedCommandLine() const
{
    QList<QTreeWidgetItem*> items = m_processTreeWidget->selectedItems();
    if (items.size() == 0) {
        return "";
    }
    return items[0]->text(5);
}

void QProcessInfoWidget::refreshList()
{
    QApplication::setOverrideCursor(Qt::BusyCursor);

    // Scan the /proc file system.
    QProcessList list = QProcessInfo::populate();

    // Loop through each entry and add it to our view.
    m_processTreeWidget->clear();

    for (const QProcessInfo& info : list) {
        QTreeWidgetItem* item = new QProcessInfoWidgetItem;
        item->setText(0, QString::number(info.getPid()));
        item->setText(1, QString::number(info.getPPid()));
        item->setText(2, info.getUsername());
        item->setText(3, info.getName());
        item->setText(4, info.getTime());
        item->setText(5, info.getCommandLine());
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

    refreshView();

    QApplication::restoreOverrideCursor();
}

void QProcessInfoWidget::refreshView()
{
    // Get this list of program name matches. Or all if there is no program name provided.
    QList<QTreeWidgetItem*> programNameMatches;

    if (m_programNameLineEdit->text() == "") {
        programNameMatches = m_processTreeWidget->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive, 2);
    }else{
        if (m_programNameLineEdit->text().contains('*')) {
            programNameMatches = m_processTreeWidget->findItems(m_programNameLineEdit->text(), Qt::MatchWildcard   | Qt::MatchRecursive, 2);
        }else{
            programNameMatches = m_processTreeWidget->findItems(m_programNameLineEdit->text(), Qt::MatchStartsWith | Qt::MatchRecursive, 2);
        }
    }

    // Get this list of user name matches. Or all if there is no user name provided.
    QList<QTreeWidgetItem*> userNameMatches;
    if (m_userNameLineEdit->text() == "") {
        userNameMatches = m_processTreeWidget->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive, 1);
    }else{
        if (m_userNameLineEdit->text().contains('*')) {
            userNameMatches = m_processTreeWidget->findItems(m_userNameLineEdit->text(), Qt::MatchWildcard   | Qt::MatchRecursive, 1);
        }else{
            userNameMatches = m_processTreeWidget->findItems(m_userNameLineEdit->text(), Qt::MatchStartsWith | Qt::MatchRecursive, 1);
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
}

void QProcessInfoWidget::handleDoubleClicked()
{
    emit pidSelected(selectedPid());
}

QString QProcessInfoWidget::addTextSpaceMargin(const QString strText){
    QString strRet = "  " + strText;
    return strRet;
}
