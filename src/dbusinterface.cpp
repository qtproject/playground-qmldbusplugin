/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the qmldpusplugin of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/
#include "dbusinterface.h"
#include "dbus.h"
#include "qdbusinterfacewrapper.h"

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusError>
#include <QMetaMethod>

#include <QDebug>


/*!
    \qmltype DBusInterface
    \instantiates QuickDBusInterface
    \inmodule qmldbusplugin
    \brief A QML representation of a DBus Interface

    The DBusInterface can be used to interact with any service running
    on a DBus.

    Per Default the DBusInterface is not connected to any Bus and is
    not ready to be used.

    To be usable you have to set the following four properties:

    \list
        \li \l {bus}
        \li \l {service}
        \li \l {path}
        \li \l {interfaceName}
    \endlist


    In the following example we connect to a Interface and make a call to
    that Interface:

    \code
    DBusInterface {
        id: dbusInterface

        interfaceName: "org.kde.KMix.Control"
        service: "org.kde.kded"
        path: "/Mixers/0/alsa_output_usb_Logitech_Logitech_USB_Headset_00_Headset_analog_stereo"

        bus: SessionBus {}

        Component.onCompleted: {
            if (dbusInterface.interfaceObject) {
                dbusInterface.interfaceObject.toggleMute()
            }
        }
    }
    \endcode


    For calling the method the interfaceObject property is used. This property can also be used to
    read DBus Properties or connect to DBus signals:

    \code
    DBusInterface {
        id: dbusInterface

        interfaceName: "com.pelagicore.test"
        service: "com.pelagicore.test"
        path: "/"

        bus: SessionBus {}
    }

    Rectangle {
        id: rect
        width: dbusInterface.interfaceObject.width
        height: dbusInterface.interfaceObject.height
    }

    //Connecting to a DBus signal
    Connections {
        ignoreUnknownSignals: true
        target: dbusInterface.interfaceObject
        onWidthChanged: {
            print("muteChanged")
        }
    }
    \endcode

    The Rectangle with the id rect uses the width and height property of the DBus interface. When the
    DBus interface changes the properties the Rectangle will also be resized.
*/


QuickDBusInterface::QuickDBusInterface(QObject *parent)
    : QObject(parent)
    , m_bus(0)
    , m_dbusInterface(0)
{
}

/*! \qmlproperty string DBusInterface::service
 *
 *  The DBus Service you want to connect to
 */
QString QuickDBusInterface::service() const
{
    return m_service;
}

/*! \qmlproperty string DBusInterface::path
 *
 *  The DBus Path you want to connect to
 */
QString QuickDBusInterface::path() const
{
    return m_path;
}

/*! \qmlproperty string DBusInterface::interfaceName
 *
 *  The DBus interface you want to connect to
 */
QString QuickDBusInterface::interfaceName() const
{
    return m_interface;
}

/*! \qmlproperty bool DBusInterface::ready
 *
 *  Returns true when the DBusInterface is ready to be used
 */
bool QuickDBusInterface::isReady() const
{
    return m_ready;
}

/*! \qmlproperty DBus DBusInterface::bus
 *
 *  The D-Bus which should to used for the connection
 *
 *  \sa DBus, SessionBus, SystemBus
 */
QuickDBus *QuickDBusInterface::bus() const
{
    return m_bus;
}

/*! \qmlmethod bool DBusReply QuickDBusInterface::call(string name, variant arg1, variant arg2,
                                                 variant arg3, variant arg4, variant arg5,
                                                 variant arg6, variant arg7, variant arg8)

    Calls the function with the Name \a name on the DBus Interface and uses the arg1 - arg8
    as arguments for this call.

    The return value is a DBusReply which contains the value or an errorName and errorString
    if the call was not successful.

    This method can be used to call functions on Interfaces which you doesn't know at the time
    of writing and instead used the DBus introspection.

    The following example calls a function using this function and prints the return value

    \code
    DBusInterface {
        id: dbusInterface

        interfaceName: "org.kde.KMix.Control"
        service: "org.kde.kded"
        path: "/Mixers/0/alsa_output_usb_Logitech_Logitech_USB_Headset_00_Headset_analog_stereo"

        bus: SessionBus {}

        Component.onCompleted: {
            //Call DBus method using a generic function
            //With this you could also use functions you got using the DBus Introspection
            var id= dbusInterface.call("getClipboardHistoryItem", 1)

            print(id)
            print(id.valid)
            print(id.errorString)
            print(id.values)
        }
    }
    \endcode
 */


QuickDBusReply* QuickDBusInterface::call(const QString &name, const QVariant &arg1, const QVariant &arg2,
                         const QVariant &arg3, const QVariant &arg4, const QVariant &arg5,
                         const QVariant &arg6, const QVariant &arg7, const QVariant &arg8) const
{
    if (!m_dbusInterface)
        return new QuickDBusReply(false, QList<QVariant>(), "No Interface", "The Interface is not initialized");

    if (!m_bus)
        return new QuickDBusReply(false, QList<QVariant>(), "No Bus", "The Bus is not initialized");

    QDBusMessage reply = m_dbusInterface->call(name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);

    if (reply.type() == QDBusMessage::ReplyMessage)
        return new QuickDBusReply(true, reply.arguments());
    else
        return new QuickDBusReply(false, QList<QVariant>(), reply.errorName(), reply.errorMessage());
}

QuickDBusReply *QuickDBusInterface::asyncCall(const QString &name, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3, const QVariant &arg4, const QVariant &arg5, const QVariant &arg6, const QVariant &arg7, const QVariant &arg8) const
{
    if (!m_dbusInterface)
        return new QuickDBusReply(false, QList<QVariant>(), "No Interface", "The Interface is not initialized");

    if (!m_bus)
        return new QuickDBusReply(false, QList<QVariant>(), "No Bus", "The Bus is not initialized");

    QDBusPendingCall pcall = m_dbusInterface->asyncCall(name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);

    return new QuickDBusReply(pcall);
}

QObject *QuickDBusInterface::interfaceObject()
{
    return m_dbusInterface;
}

void QuickDBusInterface::setService(const QString &arg)
{
    if (m_service != arg) {
        m_service = arg;
        emit serviceChanged(arg);
        createDBusInterface();
    }
}
void QuickDBusInterface::setPath(const QString &arg)
{
    if (m_path != arg) {
        m_path = arg;
        emit pathChanged(arg);
        createDBusInterface();
    }
}
void QuickDBusInterface::setInterfaceName(const QString &arg)
{
    if (m_interface != arg) {
        m_interface = arg;
        emit interfaceNameChanged(arg);
        createDBusInterface();
    }
}

void QuickDBusInterface::setBus(QuickDBus *bus)
{
    if (m_bus != bus) {

        if (m_bus) {
            disconnect(m_bus);
        }

        m_bus = bus;
        emit busChanged(bus);
        createDBusInterface();
    }
}

void QuickDBusInterface::createDBusInterface()
{
    setReady(false);

    if (m_dbusInterface) {
        delete m_dbusInterface;
        m_dbusInterface = 0;
        emit interfaceObjectChanged(0);
    }

    if (!m_bus)
        return;

    if (m_service.isEmpty() || m_interface.isEmpty() || m_path.isEmpty())
        return;

    connect(m_bus, SIGNAL(connectionChanged(QDBusConnection)), this, SLOT(createDBusInterface()));
    m_dbusInterface = new QDBusInterfaceWrapper(m_service, m_path, m_interface, m_bus->connection());
    emit interfaceObjectChanged(m_dbusInterface);

    if (!m_dbusInterface->isValid())
        qWarning() << m_dbusInterface->lastError().message();

    setReady(m_dbusInterface->isValid());
}

void QuickDBusInterface::setReady(bool ready)
{
    if (m_ready != ready) {
        m_ready = ready;
        emit readyChanged(ready);
    }
}
