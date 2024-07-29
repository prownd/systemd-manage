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

#include "unitinformation.h"
#include "systemdjournalinterface.h"

#include <QDBusMetaType>
#include <QVBoxLayout>
#include <QProcess>
#include <unistd.h>
#include <QMenu>
#include <QMessageBox>

QString unitinformationTabWidgetStyleSheet1(R"(
QTabBar::tab {
    /* color: rgb(84,2,119);    */
    /* background-image: url(:/pics/wood.jpg); */
    /* border: 1px solid rgb(68,66,64);     */
    /* border-bottom-color: rgb(68,66,64); */
    border-top: 2px outset transparent;
    border-bottom: 6px outset transparent;
    border-right: 6px outset transparent;
    border-left: 6px outset transparent;
    background-color: #ebebeb;

    border-top-left-radius: 4px;
    border-top-right-radius: 4px;

    margin-top: 2px;

    height: 40px;
    width: 140px;
}
QTabWidget::tab-bar {
    alignment: center;
}

QWidget {
    alignment: center;
}

QTabBar::tab:hover {
    margin-top: 5px;
    background-color: #dedede;
}

QTabBar::tab:!selected {
    margin-top: 5px;
}
QTabBar::tab:selected {
    color: rgb(255,0,128);
    background-color: #c7c7c7;
}


/* QPushButton */
QPushButton {
    border-top: 0px outset transparent;
    border-bottom: 0px outset transparent;
    border-right: 0px outset transparent;
    border-left: 0px outset transparent;
    border-radius:6px;
    background-color: #dedede;
    width: 160px;
    height: 40px;
    margin: 4px 4px;
}

QPushButton:checked {
    background-color: #00ff7e;
}

/* press and checked */
QPushButton:pressed:checked {
    border-left: 0px outset rgb(93, 95, 97);
    background-color: #00ff7e;
}

QPushButton:hover:!checked{
    background-color: #c7c7c7;
}

/* QTextEdit */
QTextEdit {
    background: #fcfcfc;
    border-radius:4px;
    border:1px solid lightgray;
    /* font-size:20px; */
    font: 14pt;
    margin: 4px 4px 4px 4px;
}

QLabel {
    font: 14pt;
    margin: 4px 4px 4px 4px;
}

)");


UnitInformation::UnitInformation(QWidget *parent) :
    QWidget(parent)
{
    init();
    show();
}

UnitInformation::UnitInformation(QString unitName, QString unitFile, QWidget *parent) :
    QWidget(parent)
{
    m_unitName = unitName;
    m_unitFile = unitFile;
    init();
    show();
}

UnitInformation::~UnitInformation()
{

}

void UnitInformation::init(){
    setWindowTitle(QObject::tr("Hint model window"));
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_ShowModal, true);
    //setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::Dialog);

    createMainWindowView();
    createTabWidgetView();
    createUnitConfigView();
    createUnitLogView();
    createUnitDependencyView();
    //this->setFixedSize(1200,800);
}

void UnitInformation::createMainWindowView()
{
    m_pMainWidget = new QWidget(this);

    m_mainLayout1=new QVBoxLayout(this);
    this->setLayout(m_mainLayout1);
}

void UnitInformation::createTabWidgetView()
{
    // init tabwidget
    m_pUnitInfoTabWidget = new QTabWidget(m_pMainWidget);
    m_pUnitInfoTabWidget->setMinimumWidth(width());
    m_pUnitInfoTabWidget->setMinimumHeight(height());
    m_pUnitInfoTabWidget->adjustSize();

    // init unit subwidget
    m_pUnitConfigureWidget = new QWidget(m_pMainWidget);
    m_subUnitConfigureVLayout = new QVBoxLayout(m_pUnitConfigureWidget);
    m_pUnitConfigureWidget->setLayout(m_subUnitConfigureVLayout);

    // init unit log subwidget
    m_pUnitLogWidget = new QWidget(m_pMainWidget);
    m_subUnitLogVLayout = new QVBoxLayout(m_pUnitLogWidget);
    m_pUnitLogWidget->setLayout(m_subUnitLogVLayout);

    // init unit dependency subwidget
    m_pUnitDependencyWidget = new QWidget(m_pMainWidget);
    m_subUnitDependencyVLayout = new QVBoxLayout(m_pUnitDependencyWidget);
    m_pUnitDependencyWidget->setLayout(m_subUnitDependencyVLayout);

    // add all sub widget
    m_pUnitInfoTabWidget->addTab(m_pUnitConfigureWidget, tr("Unit config"));
    m_pUnitInfoTabWidget->addTab(m_pUnitLogWidget, tr("Unit log"));
    m_pUnitInfoTabWidget->addTab(m_pUnitDependencyWidget, tr("Unit depency"));
    m_pUnitInfoTabWidget->setStyleSheet(unitinformationTabWidgetStyleSheet1);

    m_mainLayout1->addWidget(m_pUnitInfoTabWidget);
    m_mainLayout1->setSizeConstraint(QLayout::SetFixedSize);
}

void UnitInformation::createUnitConfigView()
{
    m_unitConfigLabel = new QLabel;
    m_unitConfigLabel->setText(tr("Config file: %1").arg(m_unitFile));

    m_subUnitConfigureVLayout->addWidget(m_unitConfigLabel);
    m_subUnitConfigureVLayout->addStretch();

    m_unitConfigureTextEdit= new QTextEdit(m_pUnitConfigureWidget);
    m_unitConfigureTextEdit->setText(tr("This is unit configure content item"));
    m_unitConfigureTextEdit->setText(tr("Unit config item , unit name: ") + m_unitName);
    m_unitConfigureTextEdit->setFixedSize(900,700);
    m_unitConfigureTextEdit->setReadOnly(true);

    m_subUnitConfigureVLayout->addWidget(m_unitConfigureTextEdit);
    m_subUnitConfigureVLayout->setMargin(0);
    m_subUnitConfigureVLayout->setSpacing(0);
    m_subUnitConfigureVLayout->setContentsMargins(0, 0, 0, 0);
    m_subUnitConfigureVLayout->addStretch();

    QFont font;
    //font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(14);
    m_unitConfigureTextEdit->setFont(font);

    highlighter = new Highlighter(m_unitConfigureTextEdit->document());
    if (!m_unitFile.isEmpty())
    {
        QFile file(m_unitFile);
        if (file.open(QFile::ReadOnly | QFile::Text)){
            m_unitConfigContentByteArray = file.readAll();
            m_unitConfigureTextEdit->setPlainText(m_unitConfigContentByteArray);
        }
        file.close();
    }

    m_unitConfigBtnHLayout = new QHBoxLayout(this);
    m_unitConfigEditBtn = new QPushButton(m_pUnitConfigureWidget);
    m_unitConfigEditBtn->setText(tr("Edit"));
    m_unitConfigEditBtn->setFixedSize(140,40);

    m_unitConfigResetBtn = new QPushButton(m_pUnitConfigureWidget);
    m_unitConfigResetBtn->setText(tr("Reset"));
    m_unitConfigResetBtn->setFixedSize(140,40);
    m_unitConfigResetBtn->setEnabled(false);

    m_unitConfigApplyBtn = new QPushButton(m_pUnitConfigureWidget);
    m_unitConfigApplyBtn->setText(tr("Apply"));
    m_unitConfigApplyBtn->setFixedSize(140,40);

    m_unitConfigCancelBtn = new QPushButton(m_pUnitConfigureWidget);
    m_unitConfigCancelBtn->setText(tr("Cancel"));
    m_unitConfigCancelBtn->setFixedSize(140,40);

    m_unitConfigBtnHLayout->addWidget(m_unitConfigEditBtn);
    m_unitConfigBtnHLayout->addWidget(m_unitConfigResetBtn);
    m_unitConfigBtnHLayout->addStretch(0);
    m_unitConfigBtnHLayout->addWidget(m_unitConfigApplyBtn);
    m_unitConfigBtnHLayout->addWidget(m_unitConfigCancelBtn);

    m_subUnitConfigureVLayout->addLayout(m_unitConfigBtnHLayout);

    connect(m_unitConfigEditBtn, &QPushButton::clicked, this, &UnitInformation::handleUnitConfigEditBtnClick);
    connect(m_unitConfigResetBtn, &QPushButton::clicked, this, &UnitInformation::handleUnitConfigResetBtnClick);
    connect(m_unitConfigApplyBtn, &QPushButton::clicked, this, &UnitInformation::handleUnitConfigApplyBtnClick);
    connect(m_unitConfigCancelBtn, &QPushButton::clicked, this, &UnitInformation::handleUnitConfigCancelBtnClick);
}

void UnitInformation::handleUnitConfigEditBtnClick()
{
    m_unitConfigureTextEdit->setReadOnly(false);
    m_unitConfigResetBtn->setEnabled(true);
}

void UnitInformation::handleUnitConfigResetBtnClick()
{
    m_unitConfigResetBtn->setEnabled(true);
    m_unitConfigureTextEdit->setPlainText(m_unitConfigContentByteArray);
}

void UnitInformation::handleUnitConfigApplyBtnClick()
{
    if (!m_unitFile.isEmpty())
    {
        QFile file(m_unitFile);
        if (! file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::critical(this, tr("Error"), tr("Can't save file"));
            return;
        }
        QTextStream out(&file);
        out << m_unitConfigureTextEdit->toPlainText();
        file.close();
    }
}

void UnitInformation::handleUnitConfigCancelBtnClick()
{
    this->close();
}

void UnitInformation::createUnitLogView()
{
    m_unitLogTextEdit= new QTextEdit(m_pUnitLogWidget);
    m_unitLogTextEdit->setText(tr("This is display unit log"));
    m_unitLogTextEdit->setText(tr("Unit log, unit name: ") + m_unitName);
    m_unitLogTextEdit->setFixedSize(900,700);
    m_unitLogTextEdit->setReadOnly(true);
    m_subUnitLogVLayout->addWidget(m_unitLogTextEdit);

    SystemdJournalInterface * pJournal = new SystemdJournalInterface;
    QStringList strList= pJournal->getUnitJournalEntries(m_unitName);
    m_unitLogTextEdit->clear();
    foreach (const QString str1, strList) {
        m_unitLogTextEdit->append(str1);
    }
}

void UnitInformation::createUnitDependencyView()
{
    m_unitDependencyTextEdit= new QTextEdit(m_pUnitDependencyWidget);
    m_unitDependencyTextEdit->setText(tr("Display unit dependency"));
    m_unitDependencyTextEdit->setText(tr("Unit dependency, unit name:") + m_unitName);
    m_unitDependencyTextEdit->setFixedSize(900,700);
    m_unitDependencyTextEdit->setReadOnly(true);
    m_subUnitDependencyVLayout->addWidget(m_unitDependencyTextEdit);

    QString systemdDependencyCmdStr = QStringLiteral("/usr/bin/systemctl");
    QStringList systemdDependencyArguments;
    systemdDependencyArguments.append(QStringLiteral("list-dependencies"));
    systemdDependencyArguments.append(QStringLiteral("--no-pager"));
    systemdDependencyArguments.append(m_unitName);
    m_unitDependencyTextEdit->setText(invokeCommand(systemdDependencyCmdStr,systemdDependencyArguments));
}

QString  UnitInformation::invokeCommand(QString cmdstr, QStringList arguments)
{
    QProcess process;
    //process.start(cmdstr, QStringList());
    process.start(cmdstr, arguments);

    if (!process.waitForFinished())
        qDebug() << "Error: " << process.errorString();

    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();

    //qDebug() << "Standard Output:\n" << output;
    //qDebug() << "Standard Error:\n" << errorOutput;
    return output;
}

void UnitInformation::resizeEvent(QResizeEvent *event)
{
    if (nullptr != m_pUnitInfoTabWidget)
    {
        m_pUnitInfoTabWidget->resize(width(), height());
        update();
    }

    if (nullptr != m_pUnitConfigureWidget)
    {
        m_pUnitConfigureWidget->resize(width(), height());
        update();
    }
}


