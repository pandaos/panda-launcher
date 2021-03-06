/*
 * Copyright (C) 2018 ~ 2019 PandaOS.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fullscreenframe.h"
#include <QApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("panda-launcher");
    app.setQuitOnLastWindowClosed(false);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.registerService(QLatin1String("org.panda.launcher"))) {
        QDBusInterface iface("org.panda.launcher", "/Launcher", "org.panda.Launcher", dbus, &app);
        iface.call("toggleLauncher");
        return -1;
    }

    FullScreenFrame w;
    return app.exec();
}
