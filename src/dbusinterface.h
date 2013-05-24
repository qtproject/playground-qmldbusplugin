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

#ifndef DBUSINTERFACE_H
#define DBUSINTERFACE_H

#include <QObject>
#include <dbusreply.h>

class QDBusInterface;
class QDBusInterfaceWrapper;
class QuickDBus;

class QuickDBusInterface : public QObject
{
    Q_OBJECT

public:
    explicit QuickDBusInterface(QObject *parent = 0);

    Q_PROPERTY(QString service READ service WRITE setService NOTIFY serviceChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString interfaceName READ interfaceName WRITE setInterfaceName NOTIFY interfaceNameChanged)
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
    Q_PROPERTY(QObject* interfaceObject READ interfaceObject() NOTIFY interfaceObjectChanged)
    Q_PROPERTY(QuickDBus* bus READ bus WRITE setBus NOTIFY busChanged)

    QString service() const;
    QString path() const;
    QString interfaceName() const;
    bool isReady() const;
    QuickDBus* bus() const;

    Q_INVOKABLE QuickDBusReply* call(const QString &name, const QVariant & arg1 = QVariant(), const QVariant & arg2 = QVariant(),
                          const QVariant & arg3 = QVariant(), const QVariant & arg4 = QVariant(),
                          const QVariant & arg5 = QVariant(), const QVariant & arg6 = QVariant(),
                          const QVariant & arg7 = QVariant(), const QVariant & arg8 = QVariant()) const;

    Q_INVOKABLE QuickDBusReply* asyncCall(const QString &name, const QVariant & arg1 = QVariant(), const QVariant & arg2 = QVariant(),
                          const QVariant & arg3 = QVariant(), const QVariant & arg4 = QVariant(),
                          const QVariant & arg5 = QVariant(), const QVariant & arg6 = QVariant(),
                          const QVariant & arg7 = QVariant(), const QVariant & arg8 = QVariant()) const;

    QObject *interfaceObject();

    //TODO add lastError call

signals:
    void serviceChanged(const QString &arg);
    void pathChanged(const QString &arg);
    void interfaceNameChanged(const QString &arg);
    void readyChanged(bool arg);
    void interfaceObjectChanged(QObject* arg);
    void busChanged(QuickDBus *bus);

public slots:
    void setService(const QString &arg);
    void setPath(const QString &arg);
    void setInterfaceName(const QString &arg);
    void setBus(QuickDBus* bus);

private slots:
    void createDBusInterface();
    void setReady(bool ready);

private:
    QString m_service;
    QString m_path;
    QString m_interface;
    bool m_ready;
    QuickDBus* m_bus;

    QDBusInterfaceWrapper* m_dbusInterface;
};

#endif // DBUSINTERFACE_H
