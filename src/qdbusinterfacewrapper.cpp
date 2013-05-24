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
#include "qdbusinterfacewrapper.h"
#include <QDebug>
#include <QMetaProperty>

QDBusInterfaceWrapper::QDBusInterfaceWrapper(const QString &service, const QString &path, const QString &interface, const QDBusConnection &connection, QObject *parent) :
    QDBusInterface(service, path, interface, connection, parent)
{
    QDBusInterface::connection().connect(service,
                       path,
                       "org.freedesktop.DBus.Properties",
                       "PropertiesChanged",
                       this,
                       SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

int QDBusInterfaceWrapper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty) {
        QMetaProperty mp = metaObject()->property(_id);

        QVariant var;
        if (_c == QMetaObject::ReadProperty) {
            var = QVariant(mp.type());
        } else {
            var = *reinterpret_cast<QVariant*>(_a[0]);
        }

        int status = -1;
        void *args[] = { 0, &var, &status};

        QDBusInterface::qt_metacall(_c, _id, args);

        QVariant &returnValue = *reinterpret_cast<QVariant*>(_a[0]);
        returnValue = var;

        return status;
    } else {
        return QDBusInterface::qt_metacall(_c, _id, _a);
    }
}

void QDBusInterfaceWrapper::onPropertiesChanged(const QString &interfaceName, const QVariantMap &changed_properties, const QStringList &invalidated_properties)
{
    if (interfaceName == interfaceName) {
        foreach (const QString& key, changed_properties.keys()) {
            int idx = metaObject()->indexOfProperty(qPrintable(key));
            if (idx == -1)
                continue;

            QMetaProperty prop = metaObject()->property(idx);
            QMetaMethod signal = prop.notifySignal();

            signal.invoke(this);
        }
    }
}
