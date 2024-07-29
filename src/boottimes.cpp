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

#include "boottimes.h"

BootTimes::BootTimes() {}

qint64 BootTimes::getFirmware_time() const
{
    return firmware_time;
}

void BootTimes::setFirmware_time(qint64 newFirmware_time)
{
    firmware_time = newFirmware_time;
}

qint64 BootTimes::getLoader_time() const
{
    return loader_time;
}

void BootTimes::setLoader_time(qint64 newLoader_time)
{
    loader_time = newLoader_time;
}

qint64 BootTimes::getInitrd_time() const
{
    return initrd_time;
}

void BootTimes::setInitrd_time(qint64 newInitrd_time)
{
    initrd_time = newInitrd_time;
}

qint64 BootTimes::getKernel_done_time() const
{
    return kernel_done_time;
}

void BootTimes::setKernel_done_time(qint64 newKernel_done_time)
{
    kernel_done_time = newKernel_done_time;
}

qint64 BootTimes::getUserspace_time() const
{
    return userspace_time;
}

void BootTimes::setUserspace_time(qint64 newUserspace_time)
{
    userspace_time = newUserspace_time;
}

qint64 BootTimes::getFinish_time() const
{
    return finish_time;
}

void BootTimes::setFinish_time(qint64 newFinish_time)
{
    finish_time = newFinish_time;
}

qint64 BootTimes::getFirmware_realtime() const
{
    return firmware_realtime;
}

void BootTimes::setFirmware_realtime(qint64 newFirmware_realtime)
{
    firmware_realtime = newFirmware_realtime;
}

qint64 BootTimes::getLoader_realtime() const
{
    return loader_realtime;
}

void BootTimes::setLoader_realtime(qint64 newLoader_realtime)
{
    loader_realtime = newLoader_realtime;
}

qint64 BootTimes::getKernel_realtime() const
{
    return kernel_realtime;
}

void BootTimes::setKernel_realtime(qint64 newKernel_realtime)
{
    kernel_realtime = newKernel_realtime;
}

qint64 BootTimes::getInitrd_realtime() const
{
    return initrd_realtime;
}

void BootTimes::setInitrd_realtime(qint64 newInitrd_realtime)
{
    initrd_realtime = newInitrd_realtime;
}

qint64 BootTimes::getUserspace_realtime() const
{
    return userspace_realtime;
}

void BootTimes::setUserspace_realtime(qint64 newUserspace_realtime)
{
    userspace_realtime = newUserspace_realtime;
}

qint64 BootTimes::getTotal_realtime() const
{
    return total_realtime;
}

void BootTimes::setTotal_realtime(qint64 newTotal_realtime)
{
    total_realtime = newTotal_realtime;
}

qint64 BootTimes::getKernelTimestamp() const
{
    return KernelTimestamp;
}

void BootTimes::setKernelTimestamp(qint64 newKernelTimestamp)
{
    KernelTimestamp = newKernelTimestamp;
}

qint64 BootTimes::getInitRDTimestamp() const
{
    return InitRDTimestamp;
}

void BootTimes::setInitRDTimestamp(qint64 newInitRDTimestamp)
{
    InitRDTimestamp = newInitRDTimestamp;
}

qint64 BootTimes::getUserspaceTimestamp() const
{
    return UserspaceTimestamp;
}

void BootTimes::setUserspaceTimestamp(qint64 newUserspaceTimestamp)
{
    UserspaceTimestamp = newUserspaceTimestamp;
}

qint64 BootTimes::getFinishTimestamp() const
{
    return FinishTimestamp;
}

void BootTimes::setFinishTimestamp(qint64 newFinishTimestamp)
{
    FinishTimestamp = newFinishTimestamp;
}

qint64 BootTimes::getFirmwareTimestamp() const
{
    return FirmwareTimestamp;
}

void BootTimes::setFirmwareTimestamp(qint64 newFirmwareTimestamp)
{
    FirmwareTimestamp = newFirmwareTimestamp;
}

qint64 BootTimes::getLoaderTimestamp() const
{
    return LoaderTimestamp;
}

void BootTimes::setLoaderTimestamp(qint64 newLoaderTimestamp)
{
    LoaderTimestamp = newLoaderTimestamp;
}

qint64 BootTimes::getLoaderTimestampMilliSecond() const
{
    return LoaderTimestampMilliSecond;
}

void BootTimes::setLoaderTimestampMilliSecond(qint64 newLoaderTimestampMilliSecond)
{
    LoaderTimestampMilliSecond = newLoaderTimestampMilliSecond;
}

qint64 BootTimes::getKernelTimestampMilliSecond() const
{
    return KernelTimestampMilliSecond;
}

void BootTimes::setKernelTimestampMilliSecond(qint64 newKernelTimestampMilliSecond)
{
    KernelTimestampMilliSecond = newKernelTimestampMilliSecond;
}

qint64 BootTimes::getInitRDTimestampMilliSecond() const
{
    return InitRDTimestampMilliSecond;
}

void BootTimes::setInitRDTimestampMilliSecond(qint64 newInitRDTimestampMilliSecond)
{
    InitRDTimestampMilliSecond = newInitRDTimestampMilliSecond;
}

qint64 BootTimes::getUserspaceTimestampMilliSecond() const
{
    return UserspaceTimestampMilliSecond;
}

void BootTimes::setUserspaceTimestampMilliSecond(qint64 newUserspaceTimestampMilliSecond)
{
    UserspaceTimestampMilliSecond = newUserspaceTimestampMilliSecond;
}

qint64 BootTimes::getFinishTimestampMilliSecond() const
{
    return FinishTimestampMilliSecond;
}

void BootTimes::setFinishTimestampMilliSecond(qint64 newFinishTimestampMilliSecond)
{
    FinishTimestampMilliSecond = newFinishTimestampMilliSecond;
}

qint64 BootTimes::getFirmwareTimestampMilliSecond() const
{
    return FirmwareTimestampMilliSecond;
}

void BootTimes::setFirmwareTimestampMilliSecond(qint64 newFirmwareTimestampMilliSecond)
{
    FirmwareTimestampMilliSecond = newFirmwareTimestampMilliSecond;
}
