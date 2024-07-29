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

#ifndef BOOTTIMES_H
#define BOOTTIMES_H
#include <QObject>

class BootTimes:public QObject
{
public:
    BootTimes();
    qint64 getFirmware_time() const;
    void setFirmware_time(qint64 newFirmware_time);

    qint64 getLoader_time() const;
    void setLoader_time(qint64 newLoader_time);

    qint64 getInitrd_time() const;
    void setInitrd_time(qint64 newInitrd_time);

    qint64 getKernel_done_time() const;
    void setKernel_done_time(qint64 newKernel_done_time);

    qint64 getUserspace_time() const;
    void setUserspace_time(qint64 newUserspace_time);

    qint64 getFinish_time() const;
    void setFinish_time(qint64 newFinish_time);

    qint64 getFirmware_realtime() const;
    void setFirmware_realtime(qint64 newFirmware_realtime);

    qint64 getLoader_realtime() const;
    void setLoader_realtime(qint64 newLoader_realtime);

    qint64 getKernel_realtime() const;
    void setKernel_realtime(qint64 newKernel_realtime);

    qint64 getInitrd_realtime() const;
    void setInitrd_realtime(qint64 newInitrd_realtime);

    qint64 getUserspace_realtime() const;
    void setUserspace_realtime(qint64 newUserspace_realtime);

    qint64 getTotal_realtime() const;
    void setTotal_realtime(qint64 newTotal_realtime);

    qint64 getKernelTimestamp() const;
    void setKernelTimestamp(qint64 newKernelTimestamp);

    qint64 getInitRDTimestamp() const;
    void setInitRDTimestamp(qint64 newInitRDTimestamp);

    qint64 getUserspaceTimestamp() const;
    void setUserspaceTimestamp(qint64 newUserspaceTimestamp);

    qint64 getFinishTimestamp() const;
    void setFinishTimestamp(qint64 newFinishTimestamp);

    qint64 getFirmwareTimestamp() const;
    void setFirmwareTimestamp(qint64 newFirmwareTimestamp);

    qint64 getLoaderTimestamp() const;
    void setLoaderTimestamp(qint64 newLoaderTimestamp);

    qint64 getLoaderTimestampMilliSecond() const;
    void setLoaderTimestampMilliSecond(qint64 newLoaderTimestampMilliSecond);

    qint64 getKernelTimestampMilliSecond() const;
    void setKernelTimestampMilliSecond(qint64 newKernelTimestampMilliSecond);

    qint64 getInitRDTimestampMilliSecond() const;
    void setInitRDTimestampMilliSecond(qint64 newInitRDTimestampMilliSecond);

    qint64 getUserspaceTimestampMilliSecond() const;
    void setUserspaceTimestampMilliSecond(qint64 newUserspaceTimestampMilliSecond);

    qint64 getFinishTimestampMilliSecond() const;
    void setFinishTimestampMilliSecond(qint64 newFinishTimestampMilliSecond);

    qint64 getFirmwareTimestampMilliSecond() const;
    void setFirmwareTimestampMilliSecond(qint64 newFirmwareTimestampMilliSecond);

private:
    qint64 firmware_time;
    qint64 loader_time;
    qint64 initrd_time;
    qint64 kernel_done_time;
    qint64 userspace_time;
    qint64 finish_time;


    qint64 FirmwareTimestamp;
    qint64 LoaderTimestamp;
    qint64 KernelTimestamp;
    qint64 InitRDTimestamp;
    qint64 UserspaceTimestamp;
    qint64 FinishTimestamp;

    qint64 FirmwareTimestampMilliSecond;
    qint64 LoaderTimestampMilliSecond;
    qint64 KernelTimestampMilliSecond;
    qint64 InitRDTimestampMilliSecond;
    qint64 UserspaceTimestampMilliSecond;
    qint64 FinishTimestampMilliSecond;

    //realtime of start action
    qint64 firmware_realtime;
    qint64 loader_realtime;
    qint64 kernel_realtime;
    qint64 initrd_realtime;
    qint64 userspace_realtime;
    qint64 total_realtime;

};

#endif // BOOTTIMES_H
