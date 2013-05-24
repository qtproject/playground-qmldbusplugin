/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the qmldpusplugin of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "testserveradaptor.h"

TestServerAdaptor::TestServerAdaptor(QObject *parent) :
    QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

bool TestServerAdaptor::testProperty() const
{
    return parent()->property("testProperty").toBool();
}

void TestServerAdaptor::setTestProperty(bool arg)
{
    parent()->setProperty("testProperty", arg);
}

void TestServerAdaptor::userError()
{
    QMetaObject::invokeMethod(parent(), "userError");
}

void TestServerAdaptor::systemError()
{
    QMetaObject::invokeMethod(parent(), "systemError");
}

void TestServerAdaptor::testMethod()
{
    QMetaObject::invokeMethod(parent(), "testMethod");
}

void TestServerAdaptor::testMethodWithArguments(const QString &argument)
{
    QMetaObject::invokeMethod(parent(), "testMethodWithArguments", Q_ARG(QString, argument));
}

bool TestServerAdaptor::testMethodWithReturnValue()
{
    bool retVal;
    QMetaObject::invokeMethod(parent(), "testMethodWithReturnValue", Q_RETURN_ARG(bool, retVal));

    return retVal;
}

void TestServerAdaptor::asyncUserError(const QDBusMessage &message)
{
    QMetaObject::invokeMethod(parent(), "asyncUserError", Q_ARG(QDBusMessage, message));
}

void TestServerAdaptor::asyncSystemError(const QDBusMessage &message)
{
    QMetaObject::invokeMethod(parent(), "asyncSystemError", Q_ARG(QDBusMessage, message));
}

void TestServerAdaptor::asyncTestMethod(const QDBusMessage &message)
{
    QMetaObject::invokeMethod(parent(), "asyncTestMethod", Q_ARG(QDBusMessage, message));
}

void TestServerAdaptor::asyncTestMethodWithArguments(const QString &argument, const QDBusMessage &message)
{
    QMetaObject::invokeMethod(parent(), "asyncTestMethodWithArguments", Q_ARG(QString, argument), Q_ARG(QDBusMessage, message));
}

bool TestServerAdaptor::asyncTestMethodWithReturnValue(const QDBusMessage &message)
{
    bool retVal;
    QMetaObject::invokeMethod(parent(), "asyncTestMethodWithReturnValue", Q_RETURN_ARG(bool, retVal), Q_ARG(QDBusMessage, message));

    return retVal;
}
