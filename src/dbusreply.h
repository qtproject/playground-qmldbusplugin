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

#ifndef DBUSREPLY_H
#define DBUSREPLY_H

#include <QObject>
#include <QVariant>
#include <QDBusPendingCall>

class QuickDBusReply : public QObject
{
    Q_OBJECT

public:
    explicit QuickDBusReply(bool isValid, QList<QVariant> values, QString errorName = QString(), QString errorString = QString(), QObject *parent = 0);
    explicit QuickDBusReply(const QDBusPendingCall& pcall, QObject *parent = 0);

    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)
    Q_PROPERTY(bool waiting READ isWaiting NOTIFY waitingChanged)
    Q_PROPERTY(QList<QVariant> values READ values NOTIFY valuesChanged)
    Q_PROPERTY(QString errorName READ errorName NOTIFY errorNameChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

    bool isWaiting() const;
    bool isValid() const;
    QList<QVariant> values() const;
    QString errorName() const;
    QString errorString() const;

signals:
    void validChanged();
    void waitingChanged();
    void valuesChanged();
    void errorNameChanged();
    void errorStringChanged();

private slots:
    void setValid(bool arg);
    void setWaiting(bool arg);
    void setValues(const QList<QVariant>& values);
    void setErrorName(const QString& errorName);
    void setErrorString(const QString& errorString);

    void callFinishedSlot(QDBusPendingCallWatcher *call);

private:
    bool m_valid;
    bool m_waiting;
    QList<QVariant> m_values;
    QString m_errorName;
    QString m_errorString;
};

#endif // DBUSREPLY_H
