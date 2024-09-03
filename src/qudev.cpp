#include "qudev.h"

//UdevDevice::UdevDevice(QObject *parent) :QObject(parent)
UdevDevice::UdevDevice()
{

}

UdevDevice::~UdevDevice()
{

}

UdeDeviceList UdevDevice::getAllDevicesList(const QString parentSubsystem) {
    UdeDeviceList udevDeviceListRet;

    struct udev *udev;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *entry;

    // create udev object
    udev = udev_new();
    if (!udev) {
        qCritical() << "can not create udev context";
        return udevDeviceListRet;
    }

    // create subsystem enumerator
    enumerate = udev_enumerate_new(udev);

    if(!parentSubsystem.isEmpty())
    {
        udev_enumerate_add_match_subsystem(enumerate, parentSubsystem.toLatin1().constData());
    }

    // perform sysfs scanning
    udev_enumerate_scan_devices(enumerate);

    // get udev device list
    devices = udev_enumerate_get_list_entry(enumerate);

    // iterate over all devices in the enumeration list
    udev_list_entry_foreach(entry, devices) {
        const char *path;
        struct udev_device *dev;

        // get device sys path
        path = udev_list_entry_get_name(entry);
        // device path, create udev device object
        dev = udev_device_new_from_syspath(udev, path);

        UdevDevice udevInfo;
        udevInfo.setDeviceAction(QString(udev_device_get_action(dev)).trimmed().toUpper());
        udevInfo.setDeviceNode(udev_device_get_devnode(dev) ? udev_device_get_devnode(dev) : "N/A");
        udevInfo.setDeviceSubsystem(udev_device_get_subsystem(dev));
        udevInfo.setDeviceType(udev_device_get_devtype(dev));
        udevInfo.setDevicePath(udev_device_get_syspath(dev));

        udevDeviceListRet.push_back(udevInfo);

        // display device information
        /*
        qDebug() << "-----------------------------";
        //qDebug() << "device action: " << getUdevDeviceAction(dev);
        qDebug() << "device action: " << QString(udev_device_get_action(dev)).trimmed().toUpper();
        qDebug() << "device node path: " << (udev_device_get_devnode(dev) ? udev_device_get_devnode(dev) : "N/A");
        qDebug() << "subsystem: " << udev_device_get_subsystem(dev);
        qDebug() << "device type: " << udev_device_get_devtype(dev);
        qDebug() << "device path: " << udev_device_get_syspath(dev);
        qDebug() << "idVendor: " << udev_device_get_sysattr_value(dev, "idVendor");
        qDebug() << "idProduct: " << udev_device_get_sysattr_value(dev, "idProduct");
        qDebug() << "product: " << udev_device_get_sysattr_value(dev, "product");
        qDebug() << "manufacturer: " << udev_device_get_sysattr_value(dev, "manufacturer");
        qDebug() << "serial: " << udev_device_get_sysattr_value(dev, "serial");
        qDebug() << "-----------------------------";
        */

        // release udev device object
        udev_device_unref(dev);
    }

    // release udev enumerate object
    udev_enumerate_unref(enumerate);

    // release udev object
    udev_unref(udev);

    return udevDeviceListRet;
}


UDEV_DEVICE_ACTION_TYPE UdevDevice::getUdevDeviceAction(udev_device* ptrUdevDevice)
{
    UDEV_DEVICE_ACTION_TYPE actionType = UDEV_DEVICE_ACTION_UNKNOWN;

    if (nullptr != ptrUdevDevice)
    {
        QString actionString = QString(udev_device_get_action(ptrUdevDevice)).trimmed().toUpper();
        if (!actionString.isEmpty())
        {
            if ("ADD" == actionString)
                actionType = UDEV_DEVICE_ACTION_ADD;
            else if ("REMOVE" == actionString)
                actionType = UDEV_DEVICE_ACTION_REMOVE;
            else if ("CHANGE" == actionString)
                actionType = UDEV_DEVICE_ACTION_CHANGE;
            else if ("ONLINE" == actionString)
                actionType = UDEV_DEVICE_ACTION_ONLINE;
            else if ("OFFLINE" == actionString)
                actionType = UDEV_DEVICE_ACTION_OFFLINE;
        }
    }

    return actionType;
}

QString UdevDevice::deviceNode() const
{
    return m_deviceNode;
}

void UdevDevice::setDeviceNode(const QString &newDeviceNode)
{
    m_deviceNode = newDeviceNode;
}

QString UdevDevice::deviceSubsystem() const
{
    return m_deviceSubsystem;
}

void UdevDevice::setDeviceSubsystem(const QString &newDeviceSubsystem)
{
    m_deviceSubsystem = newDeviceSubsystem;
}

QString UdevDevice::deviceType() const
{
    return m_deviceType;
}

void UdevDevice::setDeviceType(const QString &newDeviceType)
{
    m_deviceType = newDeviceType;
}

QString UdevDevice::devicePath() const
{
    return m_devicePath;
}

void UdevDevice::setDevicePath(const QString &newDevicePath)
{
    m_devicePath = newDevicePath;
}

QString UdevDevice::deviceAction() const
{
    return m_deviceAction;
}

void UdevDevice::setDeviceAction(const QString &newDeviceAction)
{
    m_deviceAction = newDeviceAction;
}
