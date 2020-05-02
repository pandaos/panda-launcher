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
#include <QStandardPaths>
#include <QSettings>
#include <QProcess>
#include <QDebug>

#include "apprunthread.h"

AppsManager* AppsManager::instance()
{
    static AppsManager manager;

    return &manager;
}

AppsManager::AppsManager(QObject *parent)
    : QObject(parent),
      m_watcher(new QFileSystemWatcher),
      m_mode(Normal)
{
    m_watcher->addPath(SYSTEMAPPPATH);

    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &AppsManager::onDirectoryChanged, Qt::QueuedConnection);

    initData();
}

AppsManager::~AppsManager()
{
}

void AppsManager::launchApp(const QModelIndex &index)
{
    const QString &appExec = index.data(ListModel::AppExecRole).toString();

    AppRunThread *thread = new AppRunThread(appExec);
    thread->start();

    connect(thread, &AppRunThread::finished, thread, &AppRunThread::deleteLater);

    emit requestHideLauncher();

    //    QProcess::startDetached(appExec);
}

void AppsManager::searchApp(const QString &keyword)
{
    m_searchList.clear();

    if (!keyword.isEmpty()) {
        for (const DesktopInfo &info : m_appList) {
            const QString &name = info.name;
            const QString &fileName = info.fileName;

            if (name.contains(keyword, Qt::CaseInsensitive) ||
                fileName.contains(keyword, Qt::CaseInsensitive)) {
                m_searchList.append(info);
            }

            for (const QString &comment : info.comments) {
                if (comment.contains(keyword, Qt::CaseInsensitive)) {
                    m_searchList.append(info);
                }
            }
        }
    }

    emit dataChanged();
}

void AppsManager::switchToNormalMode()
{
    m_mode = Normal;

    emit dataChanged();
}

void AppsManager::switchToSearchMode()
{
    m_mode = Search;

    emit dataChanged();
}

void AppsManager::showRightMenu(const QPoint &p, const QModelIndex &idx)
{
    QMenu *menu = new QMenu;
    QAction *openAction = new QAction(tr("Open"), this);
    QAction *uninstallAction = new QAction(tr("Uninstall"), this);
    QAction *dockAction = new QAction(tr("Send to dock"), this);

    connect(openAction, &QAction::triggered, this, [=] { launchApp(idx); });

    connect(dockAction, &QAction::triggered, this, [=] { sendToDock(idx); });

    // todo: auto start, uninstall

    menu->addAction(openAction);
    menu->addAction(uninstallAction);
    menu->addSeparator();
    menu->addAction(dockAction);

    menu->move(p);
    menu->exec();
}

void AppsManager::sendToDock(const QModelIndex &idx)
{
    QSettings settings(QString("%1/%2/dock_list.conf")
                       .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
                       .arg("panda-dock"),
                       QSettings::IniFormat);

    QStringList list = settings.value("appname").value<QStringList>();
    QString appExec = idx.data(ListModel::AppExecRole).toString();

    if (!list.contains(appExec)) {
        list.append(appExec);
        settings.setValue("appname", QVariant::fromValue(list));
    }
}

void AppsManager::initData()
{
    const QString env = Utils::detectDesktopEnvironment();

    QDir dir(SYSTEMAPPPATH);
    for (const QFileInfo &info : dir.entryInfoList(QDir::Files)) {
        if (info.suffix() != "desktop")
            continue;

        DesktopProperties desktop(info.filePath(), "Desktop Entry");

        // NoDisplay 与 OnlyShowIn 条件控制 Launcher 中显示
        if (desktop.contains("OnlyShowIn")) {
            const QString &value = desktop.value("OnlyShowIn").toString();
            if (!value.contains(env, Qt::CaseInsensitive)) {
                continue;
            }
        }

        if (desktop.value("NoDisplay").toBool() || desktop.value("Hidden").toBool())
            continue;

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
        appInfo.filePath = info.filePath();
        appInfo.fileName = info.fileName();
        m_appList << appInfo;

        for (const QString &key : desktop.allKeys()) {
            if (key.startsWith("Comment")) {
                appInfo.comments.insert(key, desktop.value(key).toString());
            }
        }
    }

    emit dataChanged();
}

void AppsManager::onDirectoryChanged()
{

}
