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

#include <QtQuickTest/quicktest.h>
#include <QApplication>
#include <QProcess>

int main(int argc, char **argv)
{
    QProcess* testServer = new QProcess();
    testServer->start("../testserver/testserver");

    if (!testServer->waitForStarted())
        qFatal("failed to start TestServer: %s", qPrintable(testServer->errorString()));

    int retVal;
#ifdef QUICK_TEST_SOURCE_DIR
    retVal = quick_test_main(argc, argv, "dbusplugin", QUICK_TEST_SOURCE_DIR);
#else
    retVal = quick_test_main(argc, argv, "dbusplugin", 0);
#endif

    testServer->kill();

    return retVal;
}
