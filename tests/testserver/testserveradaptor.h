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

#ifndef TESTSERVERADAPTOR_H
#define TESTSERVERADAPTOR_H

#include <QtDBus>
#include <QDBusAbstractAdaptor>

class TestServerAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.pelagicore.TestServer")
    Q_PROPERTY(bool testProperty READ testProperty WRITE setTestProperty NOTIFY testPropertyChanged)
public:
    explicit TestServerAdaptor(QObject *parent = 0);

    bool testProperty() const;
    void setTestProperty(bool arg);

public slots:
    void userError();
    void systemError();
    void testMethod();
    void testMethodWithArguments(const QString& argument);
    bool testMethodWithReturnValue();

    void asyncUserError(const QDBusMessage &message);
    void asyncSystemError(const QDBusMessage &message);
    void asyncTestMethod(const QDBusMessage &message);
    void asyncTestMethodWithArguments(const QString& argument, const QDBusMessage &message);
    bool asyncTestMethodWithReturnValue(const QDBusMessage &message);

signals:
    void testPropertyChanged();
    void testMethodCalled();
    void testMethodWithArgumentsCalled(const QString& argument);
    void asyncTestMethodCalled();
    void asyncTestMethodWithArgumentsCalled(const QString& argument);
};

#endif // TESTSERVERADAPTOR_H
