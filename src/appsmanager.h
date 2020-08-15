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

#ifndef APPSMANAGER_H
#define APPSMANAGER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QMenu>
#include "desktopinfo.h"

#define SYSTEMAPPPATH "/usr/share/applications"

class AppsManager : public QObject
{
    Q_OBJECT

public:
    enum Mode {
        Normal = 0, Search
    };

    static AppsManager* instance();

    AppsManager(QObject *parent = nullptr);
    ~AppsManager();

    const QList<DesktopInfo> &appList() const { return m_appList; }
    const QList<DesktopInfo> &searchList() const { return m_searchList; }
    void launchApp(const QModelIndex &index);
    void searchApp(const QString &keyword);

    void switchToNormalMode();
    void switchToSearchMode();

    inline Mode mode() { return m_mode; };

    void showRightMenu(const QPoint &p, const QModelIndex &idx);

    void sendToDock(const QModelIndex &idx);

signals:
    void dataChanged();
    void modeChanged();
    void requestHideLauncher();

private:
    void initData();
    void onDirectoryChanged();

private:
    QList<DesktopInfo> m_appList;
    QList<DesktopInfo> m_searchList;
    QFileSystemWatcher *m_watcher;
    Mode m_mode;
};

#endif
