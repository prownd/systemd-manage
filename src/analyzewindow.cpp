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

#include "analyzewindow.h"

#include <QProcess>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>


AnalyzeWindow::AnalyzeWindow(QWidget *parent) :
    QWidget(parent)
{
    //frameless window
    setWindowFlags(Qt::FramelessWindowHint);
    createMainWidgetView();
}

AnalyzeWindow::~AnalyzeWindow()
{
}

void AnalyzeWindow::createMainWidgetView()
{
    m_pMainWidget = new QWidget(this);
    m_vBoxLayout = new QVBoxLayout(this);
    m_vBoxLayout->addWidget(m_pMainWidget);

    m_systemdBootTimeLabelDesc = new QLabel(this);
    m_systemdBootTimeLabelDesc->setText(tr("systemd boot time is:"));
    m_systemdBootTimeLabelDesc->setStyleSheet("QLabel {"
                         "background-color: transparent;"
                         "min-height: 40px;"
                         "max-height: 40px;"
                         "font-size: 17px;"
                         "padding-left: 0px;"
                         "border-width: 0 0 0 0;"
                         "}");

    m_vBoxLayout->addWidget(m_systemdBootTimeLabelDesc);

    // run /usr/bin/busctl  call org.freedesktop.systemd1 /org/freedesktop/systemd1 org.freedesktop.DBus.Properties  GetAll s "" --json=short
    QString systemdBusctlGetPropertiesCmdStr = QStringLiteral("/usr/bin/busctl");
    QStringList systemdBusctlGetPropertiesArguments;
    systemdBusctlGetPropertiesArguments.append(QStringLiteral("call"));
    systemdBusctlGetPropertiesArguments.append(QStringLiteral("org.freedesktop.systemd1"));
    systemdBusctlGetPropertiesArguments.append(QStringLiteral("/org/freedesktop/systemd1"));
    systemdBusctlGetPropertiesArguments.append(QStringLiteral("org.freedesktop.DBus.Properties"));
    systemdBusctlGetPropertiesArguments.append(QStringLiteral("GetAll"));
    systemdBusctlGetPropertiesArguments.append(QStringLiteral("s"));
    systemdBusctlGetPropertiesArguments.append(QStringLiteral(""""));
    systemdBusctlGetPropertiesArguments.append(QStringLiteral("--json=short"));

    QString busctlCommandOutputStr = invokeCommand(systemdBusctlGetPropertiesCmdStr, systemdBusctlGetPropertiesArguments);
    int ret = parseBusctlCallGetPropertiesJsonData(busctlCommandOutputStr);
    if (ret != -1 ){
        QStringList allStageList;
        allStageList.append(QStringLiteral("firmware"));
        allStageList.append(QStringLiteral("loader"));
        allStageList.append(QStringLiteral("kernel"));
        allStageList.append(QStringLiteral("initrd"));
        allStageList.append(QStringLiteral("userspace"));
        allStageList.append(QStringLiteral("total"));

        canttTimeView = new CustomCanttChartView(this);
        canttTimeView->setAxisRange(allStageList,
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getFirmwareTimestampMilliSecond()),
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getFirmwareTimestampMilliSecond() + m_bootTime.getTotal_realtime()/1000)
                        );

        canttTimeView->addElapseTimeBar(allStageList[0],
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getFirmwareTimestampMilliSecond()),
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getFirmwareTimestampMilliSecond() + m_bootTime.getFirmware_realtime()/1000)
                        );
        canttTimeView->addElapseTimeBar(allStageList[1],
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getLoaderTimestampMilliSecond()),
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getLoaderTimestampMilliSecond() + m_bootTime.getLoader_realtime()/1000)
                        );

        canttTimeView->addElapseTimeBar(allStageList[2],
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getKernelTimestampMilliSecond()),
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getKernelTimestampMilliSecond() + m_bootTime.getKernel_realtime()/1000)
                        );

        canttTimeView->addElapseTimeBar(allStageList[3],
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getInitRDTimestampMilliSecond()),
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getInitRDTimestampMilliSecond() + m_bootTime.getInitrd_realtime()/1000)
                        );

        canttTimeView->addElapseTimeBar(allStageList[4],
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getUserspaceTimestampMilliSecond()),
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getUserspaceTimestampMilliSecond() + m_bootTime.getUserspace_realtime()/1000)
                        );

        canttTimeView->addElapseTimeBar(allStageList[5],
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getFirmwareTimestampMilliSecond()),
                        QDateTime::fromMSecsSinceEpoch(m_bootTime.getFirmwareTimestampMilliSecond() + m_bootTime.getTotal_realtime()/1000)
                        );
        m_vBoxLayout->addWidget(canttTimeView);
        m_vBoxLayout->addStretch(0);
    }else{
        QString systemdAnalyzeBootTimeCmdStr = QStringLiteral("/usr/bin/systemd-analyze");
        QStringList systemdAnalyzeBootTimeArguments;
        systemdAnalyzeBootTimeArguments.append(QStringLiteral("time"));
        QString bootTimeCommandOutputStr = invokeCommand(systemdAnalyzeBootTimeCmdStr, systemdAnalyzeBootTimeArguments);
        //qDebug() << "bootTimeCommandOutputStr:" << bootTimeCommandOutputStr;

        QStringList bootTimeCommandOutputStrList = bootTimeCommandOutputStr.split("\n");
        QStringList tmpbootTimeList1 = bootTimeCommandOutputStrList[0].trimmed().split("Startup finished in");
        QStringList tmpbootTimeListSplit1 = tmpbootTimeList1[1].trimmed().split("=");

        QString tmpbootTimeTotalStr=tmpbootTimeListSplit1[1].trimmed().remove('s');
        QStringList tmpbootTimeAllStageList = tmpbootTimeListSplit1[0].trimmed().split("+");
        if (tmpbootTimeAllStageList.length() >=3 ){
            QString tmpbootTimeKernelStr = (tmpbootTimeAllStageList[0].trimmed().split(" "))[0].remove('s');
            QString tmpbootTimeInitrdStr = (tmpbootTimeAllStageList[1].trimmed().split(" "))[0].remove('s');;
            QString tmpbootTimeUserspaceStr = (tmpbootTimeAllStageList[2].trimmed().split(" "))[0].remove('s');

            /*
            qDebug() << "tmpbootTimeTotalStr:" << tmpbootTimeTotalStr;
            qDebug() << "tmpbootTimeKernelStr:" <<   tmpbootTimeKernelStr;
            qDebug() << "tmpbootTimeInitrdStr:" <<   tmpbootTimeInitrdStr;
            qDebug() <<"tmpbootTimeUserspaceStr" << tmpbootTimeUserspaceStr;
            */

            qint64 tmpbootTimeTotalMilli = static_cast<qint64>(tmpbootTimeTotalStr.toDouble()*1000);
            qint64 tmpbootTimeKernelMilli = static_cast<qint64>(tmpbootTimeKernelStr.toDouble()*1000);
            qint64 tmpbootTimeInitrdMilli = static_cast<qint64>(tmpbootTimeInitrdStr.toDouble()*1000);
            qint64 tmpbootTimeUserspaceMilli = static_cast<qint64>(tmpbootTimeUserspaceStr.toDouble()*1000);

            qint64 dtcurrentMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();

            QTime current_time = QTime::currentTime();
            int curhour = current_time.hour();        // current hour
            int curminute = current_time.minute();    // current min
            int cursecond = current_time.second();    // current sec
            int curmsec = current_time.msec();       //  current msec
            qint64 tmpcurrentMSecsSinceEpoch = dtcurrentMSecsSinceEpoch - curhour*3600*1000 - curminute*60*1000 - cursecond*1000 - curmsec;

            /*
            qDebug() << "tmpbootTimeTotalMilli:" << tmpbootTimeTotalMilli;
            qDebug() << "tmpbootTimeKernelMilli:" <<   tmpbootTimeKernelMilli;
            qDebug() << "tmpbootTimeInitrdMilli:" <<   tmpbootTimeInitrdMilli;
            qDebug() <<"tmpbootTimeUserspaceMilli" << tmpbootTimeUserspaceMilli;
            qDebug() << "curhour:" << curhour;
            qDebug() << "curminute:" <<   curminute;
            qDebug() << "cursecond:" <<   cursecond;
            qDebug() <<"curmsec" << curmsec;
            qDebug() <<"tmpcurrentMSecsSinceEpoch" << tmpcurrentMSecsSinceEpoch;
            */

            if ( tmpcurrentMSecsSinceEpoch > 0 ) {
                QStringList allStageList;
                allStageList.append(QStringLiteral("kernel"));
                allStageList.append(QStringLiteral("initrd"));
                allStageList.append(QStringLiteral("userspace"));
                allStageList.append(QStringLiteral("total"));

                canttTimeView = new CustomCanttChartView(this);
                canttTimeView->setAxisRange(allStageList,
                                            QDateTime::fromMSecsSinceEpoch(tmpcurrentMSecsSinceEpoch ),
                                            QDateTime::fromMSecsSinceEpoch(tmpcurrentMSecsSinceEpoch + tmpbootTimeTotalMilli)
                                            );

                canttTimeView->addElapseTimeBar(allStageList[0],
                                                QDateTime::fromMSecsSinceEpoch(tmpcurrentMSecsSinceEpoch  ),
                                                QDateTime::fromMSecsSinceEpoch(tmpcurrentMSecsSinceEpoch + tmpbootTimeKernelMilli)
                                                );

                canttTimeView->addElapseTimeBar(allStageList[1],
                                                QDateTime::fromMSecsSinceEpoch(tmpcurrentMSecsSinceEpoch + tmpbootTimeKernelMilli ),
                                                QDateTime::fromMSecsSinceEpoch(tmpcurrentMSecsSinceEpoch + tmpbootTimeKernelMilli + tmpbootTimeInitrdMilli)
                                                );

                canttTimeView->addElapseTimeBar(allStageList[2],
                                                QDateTime::fromMSecsSinceEpoch(tmpcurrentMSecsSinceEpoch + tmpbootTimeKernelMilli + tmpbootTimeInitrdMilli ),
                                                QDateTime::fromMSecsSinceEpoch(tmpcurrentMSecsSinceEpoch + tmpbootTimeKernelMilli + tmpbootTimeInitrdMilli + tmpbootTimeUserspaceMilli)
                                                );

                m_vBoxLayout->addWidget(canttTimeView);
                m_vBoxLayout->addStretch(0);
            }
        }
    }

    m_systemdAnalyzePlotLabelDesc = new QLabel(this);
    m_systemdAnalyzePlotLabelDesc->setText(tr("systemd boot flow plot chart is:"));
    m_systemdAnalyzePlotLabelDesc->setStyleSheet("QLabel {"
                                              "background-color: transparent;"
                                              "min-height: 40px;"
                                              "max-height: 40px;"
                                              "font-size: 17px;"
                                              "padding-left: 0px;"
                                              "border-width: 0 0 0 0;"
                                              "}");

    m_vBoxLayout->addWidget(m_systemdAnalyzePlotLabelDesc);

    QString systemdAnalyzePlotCmdStr = QStringLiteral("/usr/bin/systemd-analyze");
    QStringList systemdAnalyzePlotArguments;
    systemdAnalyzePlotArguments.append(QStringLiteral("plot"));
    QString plotCommandOutputStr = invokeCommand(systemdAnalyzePlotCmdStr, systemdAnalyzePlotArguments);

    myGraphicScene = new QGraphicsScene(this);

    myRenderer = new QSvgRenderer();
    myRenderer->load(plotCommandOutputStr.toUtf8());

    mySvgItem = new QGraphicsSvgItem();
    mySvgItem->setSharedRenderer(myRenderer);

    myGraphicScene->addItem(mySvgItem);

    myCustomView = new CustomSvgGraphicsView(myGraphicScene);
    myCustomView->setWindowTitle(tr("SVG Viewer with Zoom"));
    myCustomView->resize(1000, 800);

    m_vBoxLayout->addWidget(myCustomView);
}


QString  AnalyzeWindow::invokeCommand(QString cmdstr, QStringList arguments)
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


int AnalyzeWindow::parseBusctlCallGetPropertiesJsonData(const QString jsonstr)
{
    QJsonParseError jsonError;
    QJsonDocument  jsonRootDoc = QJsonDocument::fromJson(jsonstr.toUtf8(), &jsonError);     //format JSON

    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "JSON format invalid";
        return -1;
    }
    else    //JSON format valid
    {
        // qDebug() << "JSON format valid:" << rootDoc;
        QJsonObject rootObj = jsonRootDoc.object();
        QJsonValue dataValue = rootObj.value("data");    //data is array
        if(dataValue.isArray())
        {
            //data array only have 1 object
            QJsonObject dataObj = dataValue.toArray().at(0).toObject();

            QJsonObject FirmwareTimestampMonotonic_Obj = dataObj.value("FirmwareTimestampMonotonic").toObject();
            qint64  firmware_time = FirmwareTimestampMonotonic_Obj.value("data").toInt();
            m_bootTime.setFirmware_time(firmware_time);

            QJsonObject LoaderTimestampMonotonic_Obj = dataObj.value("LoaderTimestampMonotonic").toObject();
            qint64 loader_time = LoaderTimestampMonotonic_Obj.value("data").toInt();
            m_bootTime.setLoader_time(loader_time);

            QJsonObject KernelTimestamp_Obj = dataObj.value("KernelTimestamp").toObject();
            qint64 kernelTimeStamp = static_cast<qint64>(KernelTimestamp_Obj.value("data").toDouble());
            m_bootTime.setKernelTimestamp(kernelTimeStamp);

            QJsonObject InitRDTimestampMonotonic_Obj = dataObj.value("InitRDTimestampMonotonic").toObject();
            qint64 initrd_time = InitRDTimestampMonotonic_Obj.value("data").toInt();
            m_bootTime.setInitrd_time(initrd_time);

            QJsonObject InitRDTimestamp_Obj = dataObj.value("InitRDTimestamp").toObject();
            qint64 InitRDTimestamp = static_cast<qint64>(InitRDTimestamp_Obj.value("data").toDouble());
            m_bootTime.setInitRDTimestamp(InitRDTimestamp);

            qint64 kernel_done_time = InitRDTimestampMonotonic_Obj.value("data").toInt();
            m_bootTime.setKernel_done_time(kernel_done_time);

            QJsonObject UserspaceTimestampMonotonic_Obj = dataObj.value("UserspaceTimestampMonotonic").toObject();
            qint64 userspace_time = UserspaceTimestampMonotonic_Obj.value("data").toInt();
            m_bootTime.setUserspace_time(userspace_time);

            QJsonObject UserspaceTimestamp_Obj = dataObj.value("UserspaceTimestamp").toObject();
            qint64 UserspaceTimestamp = static_cast<qint64>(UserspaceTimestamp_Obj.value("data").toDouble());
            m_bootTime.setUserspaceTimestamp(UserspaceTimestamp);

            QJsonObject FinishTimestampMonotonic_Obj = dataObj.value("FinishTimestampMonotonic").toObject();
            qint64 finish_time = FinishTimestampMonotonic_Obj.value("data").toInt();
            m_bootTime.setFinish_time(finish_time);

            /*
            qDebug() << "firmware_time:" << m_bootTime.getFinish_time();
            qDebug() << "loader_time:" <<   m_bootTime.getLoader_time();
            qDebug() << "initrd_time:" <<   m_bootTime.getInitrd_time();
            qDebug() <<"kernel_done_time" << m_bootTime.getKernel_done_time();
            qDebug() << "userspace_time:" << m_bootTime.getUserspace_time();
            qDebug() << "finish_time:" <<   m_bootTime.getFinish_time();
            */

            m_bootTime.setFirmware_realtime(m_bootTime.getFirmware_time() - m_bootTime.getLoader_time());
            m_bootTime.setLoader_realtime(m_bootTime.getLoader_time());
            m_bootTime.setKernel_realtime(m_bootTime.getKernel_done_time());
            m_bootTime.setInitrd_realtime(m_bootTime.getUserspace_time() - m_bootTime.getInitrd_time());
            m_bootTime.setUserspace_realtime(m_bootTime.getFinish_time() - m_bootTime.getUserspace_time());
            m_bootTime.setTotal_realtime(m_bootTime.getFirmware_time() + m_bootTime.getFinish_time());

            m_bootTime.setFirmwareTimestamp(m_bootTime.getKernelTimestamp()  - m_bootTime.getLoader_realtime() - m_bootTime.getFirmware_realtime());
            m_bootTime.setLoaderTimestamp(m_bootTime.getKernelTimestamp() - m_bootTime.getLoader_realtime());
            m_bootTime.setFinishTimestamp(m_bootTime.getFirmwareTimestamp() + m_bootTime.getTotal_realtime());

            m_bootTime.setFirmwareTimestampMilliSecond(m_bootTime.getFirmwareTimestamp()/1000);
            m_bootTime.setLoaderTimestampMilliSecond(m_bootTime.getLoaderTimestamp()/1000);
            m_bootTime.setKernelTimestampMilliSecond(m_bootTime.getKernelTimestamp()/1000);
            m_bootTime.setInitRDTimestampMilliSecond(m_bootTime.getInitRDTimestamp()/1000);
            m_bootTime.setUserspaceTimestampMilliSecond(m_bootTime.getUserspaceTimestamp()/1000);
            m_bootTime.setFinishTimestampMilliSecond(m_bootTime.getFinishTimestamp()/1000);

            /*
            qDebug() << "------------------------------------------------";
            qDebug() << "FirmwareTimeStamp:" << m_bootTime.getFirmwareTimestamp();
            qDebug() << "LoaderTimestamp:" <<   m_bootTime.getLoaderTimestamp();
            qDebug() << "kernelTimeStamp:" << m_bootTime.getKernelTimestamp();
            qDebug() << "InitRDTimestamp:" <<   m_bootTime.getInitRDTimestamp();
            qDebug() << "UserspaceTimestamp:" <<   m_bootTime.getUserspaceTimestamp();
            qDebug() <<"FinishTimestamp" << m_bootTime.getFinishTimestamp();
            qDebug() << "------------------------------------------------";

            qDebug() << "------------------------------------------------";
            qDebug() << "getFirmwareTimestampMilliSecond:" << m_bootTime.getFirmwareTimestampMilliSecond();
            qDebug() << "getLoaderTimestampMilliSecond:" <<   m_bootTime.getLoaderTimestampMilliSecond();
            qDebug() << "getKernelTimestampMilliSecond:" << m_bootTime.getKernelTimestampMilliSecond();
            qDebug() << "getInitRDTimestampMilliSecond:" <<   m_bootTime.getInitRDTimestampMilliSecond();
            qDebug() << "getUserspaceTimestampMilliSecond:" <<   m_bootTime.getUserspaceTimestampMilliSecond();
            qDebug() <<"getFinishTimestampMilliSecond" << m_bootTime.getFinishTimestampMilliSecond();
            qDebug() << "------------------------------------------------";

            qDebug() << "each stage start time:" ;
            qDebug() << "firmware_realtime:" << m_bootTime.getFirmware_realtime();
            qDebug() << "loader_realtime:" <<   m_bootTime.getLoader_realtime();
            qDebug() << "kernel_realtime:" <<   m_bootTime.getKernel_realtime();
            qDebug() << "initrd_realtime:" <<   m_bootTime.getInitrd_realtime();
            qDebug() <<"userspace_realtime" << m_bootTime.getUserspace_realtime();
            qDebug() << "total_realtime:" << m_bootTime.getTotal_realtime();
            */
        }
        //qDebug() << "parse finish";
    }
    return 0;
}

void AnalyzeWindow::resizeEvent(QResizeEvent *event)
{
    if (nullptr != m_pMainWidget)
    {
        m_pMainWidget->resize(width(), height());
        update();
    }
}
