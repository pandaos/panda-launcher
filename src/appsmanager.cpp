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

#include "appsmanager.h"
#include "desktopproperties.h"
#include "listmodel.h"
#include "utils.h"
#include <QDirIterator>
#include <QSettings>
#include <QProcess>
#include <QDebug>

AppsManager* AppsManager::instance()
{
    static AppsManager manager;

    return &manager;
}

AppsManager::AppsManager(QObject *parent)
    : QObject(parent),
      m_watcher(new QFileSystemWatcher)
{
    m_watcher->addPath(SYSTEMAPPPATH);

    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, [=] {

    });

    initData();
}

AppsManager::~AppsManager()
{
}

void AppsManager::launchApp(const QModelIndex &index)
{
    const QString &appExec = index.data(ListModel::AppExecRole).toString();

    QProcess::startDetached(appExec);
}

void AppsManager::initData()
{
    QDirIterator it(SYSTEMAPPPATH, QDir::Files);

    while (it.hasNext()) {
        it.next();

        QString path = it.filePath();

        if (QFileInfo(path).suffix() != "desktop")
            continue;

        DesktopProperties desktop(path, "Desktop Entry");

        // NoDisplay 与 OnlyShowIn 条件控制 Launcher 中显示
        if (desktop.contains("NoDisplay") &&
            desktop.value("NoDisplay").toBool() ||
            desktop.contains("NotShowIn")) {
            continue;
        }

        QString appName = desktop.value(QString("Name[%1]").arg(QLocale::system().name())).toString();
        QString appExec = desktop.value("Exec").toString();
        QString appIcon = desktop.value("Icon").toString();

        appExec.remove(QRegExp("%."));
        appExec.remove(QRegExp("^\""));
        appExec.remove(QRegExp(" *$"));

        // 如果获取不到与系统语言的名称
        if (appName.isEmpty()) {
            appName = desktop.value("Name").toString();
        }

        DesktopInfo appInfo;
        appInfo.name = appName;
        appInfo.iconName = appIcon;
        appInfo.exec = appExec;
        appInfo.filePath = path;
        m_appList << appInfo;
    }

    emit dataChanged();
}
