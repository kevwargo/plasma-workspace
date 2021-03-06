/*****************************************************************
ksmserver - the KDE session management server

Copyright 2016 Martin Graesslin <mgraesslin@kde.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/
#include <QApplication>
#include <QQuickWindow>
#include <QCommandLineParser>
#include <QLibraryInfo>

#include <KQuickAddons/QtQuickSettings>

#include <unistd.h>

#include "greeter.h"

int main(int argc, char *argv[])
{
    if (QLibraryInfo::version() < QVersionNumber(5, 12)) {
        // Before Qt 5.12, the xdg-shell v6 integration does not support fullscreen.
        qputenv("QT_WAYLAND_SHELL_INTEGRATION", "wl-shell");
    }

    KWorkSpace::detectPlatform(argc, argv);
    QQuickWindow::setDefaultAlphaBuffer(true);
    QApplication app(argc, argv);

    KQuickAddons::QtQuickSettings::init();

    QCommandLineParser parser;
    parser.addHelpOption();

    // TODO: should these things be translated? It's internal after all...
    QCommandLineOption shutdownAllowedOption(QStringLiteral("shutdown-allowed"),
                                             QStringLiteral("Whether the user is allowed to shut down the system."));
    parser.addOption(shutdownAllowedOption);

    QCommandLineOption chooseOption(QStringLiteral("choose"),
                                    QStringLiteral("Whether the user is offered the choices between logout, shutdown, etc."));
    parser.addOption(chooseOption);

    QCommandLineOption modeOption(QStringLiteral("mode"),
                                  QStringLiteral("The initial exit mode to offer to the user."),
                                  QStringLiteral("logout|shutdown|reboot"),
                                  QStringLiteral("logout"));
    parser.addOption(modeOption);

    QCommandLineOption fdOption(QStringLiteral("mode-fd"),
                                QStringLiteral("An optional file descriptor the selected mode is written to on accepted"),
                                QStringLiteral("fd"), QString::number(-1));
    parser.addOption(fdOption);

    parser.process(app);

    KWorkSpace::ShutdownType type = KWorkSpace::ShutdownTypeDefault;
    if (parser.isSet(modeOption)) {
        const QString modeValue = parser.value(modeOption);
        if (QString::compare(QLatin1String("logout"), modeValue, Qt::CaseInsensitive) == 0) {
            type = KWorkSpace::ShutdownTypeNone;
        } else if (QString::compare(QLatin1String("shutdown"), modeValue, Qt::CaseInsensitive) == 0) {
            type = KWorkSpace::ShutdownTypeHalt;
        } else if (QString::compare(QLatin1String("reboot"), modeValue, Qt::CaseInsensitive) == 0) {
            type = KWorkSpace::ShutdownTypeReboot;
        } else {
            return 1;
        }
    }

    int fd = -1;
    if (parser.isSet(fdOption)) {
        bool ok = false;
        const int passedFd = parser.value(fdOption).toInt(&ok);
        if (ok) {
            fd = dup(passedFd);
        }
    }
    Greeter greeter(fd, parser.isSet(shutdownAllowedOption), parser.isSet(chooseOption), type);
    greeter.init();

    return app.exec();
}

#include "main.moc"
