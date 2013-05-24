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
#include "dbusreply.h"
#include <QDebug>

QuickDBusReply::QuickDBusReply(bool isValid, QList<QVariant> values, QString errorName, QString errorString, QObject *parent)
    : QObject(parent)
    , m_valid(isValid)
    , m_waiting(false)
    , m_values(values)
    , m_errorName(errorName)
    , m_errorString(errorString)
{
}

QuickDBusReply::QuickDBusReply(const QDBusPendingCall &pcall, QObject *parent)
    : QObject(parent)
    , m_valid(true)
    , m_waiting(true)
{
    QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher(pcall, this);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(callFinishedSlot(QDBusPendingCallWatcher*)));
}

bool QuickDBusReply::isWaiting() const
{
    return m_waiting;
}

bool QuickDBusReply::isValid() const
{
    return m_valid;
}

QList<QVariant> QuickDBusReply::values() const
{
    return m_values;
}

QString QuickDBusReply::errorName() const
{
    return m_errorName;
}

QString QuickDBusReply::errorString() const
{
    return m_errorString;
}

void QuickDBusReply::callFinishedSlot(QDBusPendingCallWatcher *call)
{
    Q_ASSERT(call);

    setWaiting(false);

    QDBusMessage msg = call->reply();

    if (msg.type() == QDBusMessage::ReplyMessage) {
        setValues(msg.arguments());
    } else {
        setValid(false);
        setErrorName(msg.errorName());
        setErrorString(msg.errorMessage());
    }

    call->deleteLater();
}


void QuickDBusReply::setValid(bool arg)
{
    if (m_valid != arg) {
        m_valid = arg;
        emit validChanged();
    }
}

void QuickDBusReply::setWaiting(bool arg)
{
    if (m_waiting != arg) {
        m_waiting = arg;
        emit waitingChanged();
    }
}

void QuickDBusReply::setValues(const QList<QVariant> &values)
{
    if (m_values != values) {
        m_values = values;
        emit valuesChanged();
    }
}

void QuickDBusReply::setErrorName(const QString &errorName)
{
    if (m_errorName != errorName) {
        m_errorName = errorName;
        emit errorNameChanged();
    }
}

void QuickDBusReply::setErrorString(const QString &errorString)
{
    if (m_errorString != errorString) {
        m_errorString = errorString;
        emit errorStringChanged();
    }
}
