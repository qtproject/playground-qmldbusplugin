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

import QtQuick 2.0
import QtTest 1.0
import com.pelagicore.dbus 1.0

TestCase {
    name: "InterfaceTests"

    DBusInterface {
        id: dbusInterface

        bus: SessionBus{}

        interfaceName: "org.pelagicore.TestServer"
        path: "/"
        service: "org.pelagicore.TestServer"
    }

    SignalSpy {
        id: interfaceSpy
        target: dbusInterface.interfaceObject
    }

    SignalSpy {
        id: spy
    }

    function test_errors_sync() {
        var callReply = dbusInterface.call("userError");

        compare(callReply.valid, false)
        compare(callReply.errorName, "com.userError")
        compare(callReply.errorString, "This is a user error")

        var interfaceReply = dbusInterface.interfaceObject.userError();

        //TODO add lastError to Interface and check this against it.

        callReply = dbusInterface.call("systemError");

        compare(callReply.valid, false)
        compare(callReply.errorName, "org.qtproject.QtDBus.Error.InternalError")
        compare(callReply.errorString, "Internal error")

        interfaceReply = dbusInterface.interfaceObject.systemError();
    }

    function test_errors_async() {
        spy.clear()
        var callReply = dbusInterface.asyncCall("asyncUserError");
        spy.signalName = ""
        spy.target = callReply
        spy.signalName = "waitingChanged"

        compare(spy.count, 0)

        compare(callReply.valid, true)
        compare(callReply.waiting, true)

        spy.wait()

        compare(spy.count, 1)

        compare(callReply.waiting, false)
        compare(callReply.valid, false)
        compare(callReply.errorName, "com.userError")
        compare(callReply.errorString, "This is a user error")


        spy.clear()
        callReply = dbusInterface.asyncCall("asyncSystemError");
        spy.target = callReply

        compare(spy.count, 0)

        compare(callReply.valid, true)
        compare(callReply.waiting, true)

        spy.wait()

        compare(spy.count, 1)

        compare(callReply.waiting, false)
        compare(callReply.valid, false)
        compare(callReply.errorName, "org.qtproject.QtDBus.Error.InternalError")
        compare(callReply.errorString, "Internal error")
    }

    function test_methods_sync() {
        interfaceSpy.clear()
        interfaceSpy.signalName = "testMethodCalled"
        var callReply = dbusInterface.call("testMethod");

        compare(callReply.valid, true)
        compare(callReply.errorName, "")
        compare(callReply.errorString, "")
        interfaceSpy.wait()
        compare(interfaceSpy.count, 1)


        interfaceSpy.clear()
        interfaceSpy.signalName = "testMethodWithArgumentsCalled"
        callReply = dbusInterface.call("testMethodWithArguments", "hello");

        compare(callReply.valid, true)
        compare(callReply.errorName, "")
        compare(callReply.errorString, "")
        interfaceSpy.wait()
        compare(interfaceSpy.signalArguments[0][0], "hello")
        compare(interfaceSpy.count, 1)


        callReply = dbusInterface.call("testMethodWithReturnValue");

        compare(callReply.valid, true)
        compare(callReply.errorName, "")
        compare(callReply.errorString, "")
        compare(callReply.values.length, 1)
        compare(callReply.values[0], true)
    }

    function test_methods_async() {

        //asyncTestMethod

        interfaceSpy.clear()
        interfaceSpy.signalName = "asyncTestMethodCalled"
        spy.clear();

        var callReply = dbusInterface.asyncCall("asyncTestMethod");
        spy.signalName = ""
        spy.target = callReply
        spy.signalName = "waitingChanged"

        compare(spy.count, 0)
        compare(interfaceSpy.count, 0)

        compare(callReply.valid, true)
        compare(callReply.waiting, true)

        spy.wait()
        compare(spy.count, 1)

        if (interfaceSpy.count == 0)
            interfaceSpy.wait()

        compare(interfaceSpy.count, 1)

        compare(callReply.waiting, false)
        compare(callReply.valid, true)
        compare(callReply.errorName, "")
        compare(callReply.errorString, "")


        //asyncTestMethodWithArguments

        interfaceSpy.clear()
        interfaceSpy.signalName = "asyncTestMethodWithArgumentsCalled"
        spy.clear();

        callReply = dbusInterface.asyncCall("asyncTestMethodWithArguments", "hello");
        spy.signalName = ""
        spy.target = callReply
        spy.signalName = "waitingChanged"

        compare(spy.count, 0)
        compare(interfaceSpy.count, 0)

        compare(callReply.valid, true)
        compare(callReply.waiting, true)

        spy.wait()
        compare(spy.count, 1)

        if (interfaceSpy.count == 0)
            interfaceSpy.wait()

        compare(interfaceSpy.signalArguments[0][0], "hello")
        compare(interfaceSpy.count, 1)

        compare(callReply.waiting, false)
        compare(callReply.valid, true)
        compare(callReply.errorName, "")
        compare(callReply.errorString, "")

        //asyncTestMethodWithReturnValue

        spy.clear();

        callReply = dbusInterface.asyncCall("asyncTestMethodWithReturnValue");
        spy.signalName = ""
        spy.target = callReply
        spy.signalName = "waitingChanged"

        compare(spy.count, 0)

        compare(callReply.valid, true)
        compare(callReply.waiting, true)

        spy.wait()
        compare(spy.count, 1)

        compare(callReply.waiting, false)
        compare(callReply.valid, true)
        compare(callReply.errorName, "")
        compare(callReply.errorString, "")
        compare(callReply.values[0], true)
    }

    function test_properties() {
        compare(dbusInterface.interfaceObject.testProperty, false)
        spy.clear()
        spy.signalName = ""
        spy.target = dbusInterface.interfaceObject
        spy.signalName = "testPropertyChanged"
        dbusInterface.interfaceObject.testProperty = true
        spy.wait()
        compare(spy.count, 1)
        compare(dbusInterface.interfaceObject.testProperty, true)
    }
}
