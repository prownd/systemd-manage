#ifndef QUDEV_H
#define QUDEV_H

#include <QObject>
#include <QDebug>
#include <libudev.h>

class UdevDevice;
typedef QList<UdevDevice> UdeDeviceList;

enum UDEV_DEVICE_ACTION_TYPE
{
    UDEV_DEVICE_ACTION_ADD,
    UDEV_DEVICE_ACTION_REMOVE,
    UDEV_DEVICE_ACTION_CHANGE,
    UDEV_DEVICE_ACTION_ONLINE,
    UDEV_DEVICE_ACTION_OFFLINE,
    UDEV_DEVICE_ACTION_UNKNOWN
};

//class UdevDevice : public QObject
class UdevDevice
{
public:
    explicit UdevDevice();
    virtual ~UdevDevice();

    UdeDeviceList getAllDevicesList(const QString parentSubsystem ="");

    UDEV_DEVICE_ACTION_TYPE getUdevDeviceAction(udev_device* ptrUdevDevice = nullptr);

    QString deviceNode() const;
    void setDeviceNode(const QString &newDeviceNode);

    QString deviceSubsystem() const;
    void setDeviceSubsystem(const QString &newDeviceSubsystem);

    QString deviceType() const;
    void setDeviceType(const QString &newDeviceType);

    QString devicePath() const;
    void setDevicePath(const QString &newDevicePath);

    QString deviceAction() const;
    void setDeviceAction(const QString &newDeviceAction);

private:
    QString  m_deviceNode;
    QString  m_deviceSubsystem;
    QString  m_deviceType;
    QString  m_devicePath;
    QString  m_deviceAction;

};

#endif  // QUDEV_H
