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
#include <QtSingleApplication>
#include <QApplication>

int main(int argc, char *argv[])
{
    QtSingleApplication app("flyma-launcher", argc, argv);
    app.setQuitOnLastWindowClosed(false);

    if (app.isRunning()) {
        app.sendMessage("raise_window_noop");
        return 0;
    }

    FullScreenFrame w;
    w.show();

    app.setActivationWindow(&w);

    return app.exec();
}
