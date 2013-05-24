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
#include "dbus.h"

/*!
    \qmltype DBus
    \instantiates QuickDBus
    \inmodule qmldbusplugin
    \brief A QML representation of a Connection to specific DBus

    The DBus object should only be used if you want to connect to
    a specific non-standard Bus.

    For the most usecases SessionBus and SystemBus are the suitable
    Elements

    \sa SessionBus, SystemBus
*/


/*!
    \qmltype SessionBus
    \instantiates QuickSessionBus
    \inmodule qmldbusplugin
    \brief A QML representation of a Connection to the SessionBus

    This Bus should be used for all services you want to call on
    the SessionBus

    \sa SystemBus, DBus
*/

/*!
    \qmltype SystemBus
    \instantiates QuickSystemBus
    \inmodule qmldbusplugin
    \brief A QML representation of a Connection to the SystemBus

    This Bus should be used for all services you want to call on
    the SystemBus

    \sa SessionBus, DBus
*/

/*!
    \qmlproperty string DBus::address

    The address where this DBus runs on
*/

QuickDBus::QuickDBus(QObject *parent)
    : QObject(parent)
    , m_connection(QDBusConnection("tmpDummy"))
{
}

QuickDBus::QuickDBus(const QDBusConnection &connection, QObject *parent)
    : QObject(parent)
    , m_connection(connection)
{
}

QuickDBus::~QuickDBus()
{
}

QString QuickDBus::address() const
{
    return m_address;
}

QDBusConnection QuickDBus::connection() const
{
    return m_connection;
}

void QuickDBus::setAddress(const QString &address)
{
    if (m_address != address) {
        m_address = address;

        if (m_connection.isConnected())
            m_connection.disconnectFromBus(m_connection.name());

        m_connection = QDBusConnection::connectToBus(m_address, m_address);

        emit addressChanged(address);
    }
}
