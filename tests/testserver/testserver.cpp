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

#include "testserver.h"

TestServer::TestServer(QObject *parent) :
    QObject(parent) ,
    m_testProperty(false)
{
}

bool TestServer::testProperty() const
{
    qDebug() << __PRETTY_FUNCTION__;
    return m_testProperty;
}

void TestServer::setTestProperty(bool arg)
{
    qDebug() << __PRETTY_FUNCTION__ << arg;
    m_testProperty = arg;
    emit testPropertyChanged();
    notifyPropertyChanged("testProperty");
}

void TestServer::userError()
{
    qDebug() << __PRETTY_FUNCTION__;
    sendErrorReply("com.userError", "This is a user error");
}

void TestServer::systemError()
{
    qDebug() << __PRETTY_FUNCTION__;
    sendErrorReply(QDBusError::InternalError, "Internal error");
}

void TestServer::testMethod()
{
    qDebug() << __PRETTY_FUNCTION__;
    emit testMethodCalled();
}

void TestServer::testMethodWithArguments(const QString &argument)
{
    qDebug() << __PRETTY_FUNCTION__;
    emit testMethodWithArgumentsCalled(argument);
}

bool TestServer::testMethodWithReturnValue()
{
    qDebug() << __PRETTY_FUNCTION__;
    return true;
}

void TestServer::asyncUserError(const QDBusMessage &message)
{
    qDebug() << __PRETTY_FUNCTION__;
    createReply(message);
    m_pendingReply = message.createErrorReply("com.userError", "This is a user error");

    QTimer::singleShot(2000, this, SLOT(sendAsyncResult()));
}

void TestServer::asyncSystemError(const QDBusMessage &message)
{
    qDebug() << __PRETTY_FUNCTION__;
    createReply(message);
    m_pendingReply = message.createErrorReply(QDBusError::InternalError, "Internal error");

    QTimer::singleShot(2000, this, SLOT(sendAsyncResult()));
}

void TestServer::asyncTestMethod(const QDBusMessage &message)
{
    qDebug() << __PRETTY_FUNCTION__;
    createReply(message);

    emit asyncTestMethodCalled();

    QTimer::singleShot(2000, this, SLOT(sendAsyncResult()));
}

void TestServer::asyncTestMethodWithArguments(const QString &argument, const QDBusMessage &message)
{
    qDebug() << __PRETTY_FUNCTION__;
    createReply(message);

    emit asyncTestMethodWithArgumentsCalled(argument);

    QTimer::singleShot(2000, this, SLOT(sendAsyncResult()));
}

bool TestServer::asyncTestMethodWithReturnValue(const QDBusMessage &message)
{
    qDebug() << __PRETTY_FUNCTION__;
    createReply(message);
    m_pendingReply << true;

    QTimer::singleShot(2000, this, SLOT(sendAsyncResult()));

    return false;
}

void TestServer::sendAsyncResult()
{
    qDebug() << "send Async Result" << m_pendingReply;

    QDBusConnection::sessionBus().send(m_pendingReply);
}

void TestServer::createReply(const QDBusMessage &message)
{
    message.setDelayedReply(true);
    m_pendingReply = message.createReply();
}

void TestServer::notifyPropertyChanged(const QString& propertyName )
{
    qDebug() << "notify Property changes" << propertyName;

    QDBusMessage signal = QDBusMessage::createSignal(
        "/object/path",
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged");
    signal << "org.pelagicore.TestServer";
    QVariantMap changedProps;
    changedProps.insert(propertyName, property(qPrintable(propertyName)));
    signal << changedProps;
    signal << QStringList();
    QDBusConnection::sessionBus().send(signal);
}
