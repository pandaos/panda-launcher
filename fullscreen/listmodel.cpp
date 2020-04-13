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

#include "listmodel.h"
#include "desktopinfo.h"
#include "calcutil.h"
#include <QSize>

ListModel::ListModel(QObject *parent)
    : QAbstractListModel(parent),
      m_appsManager(AppsManager::instance())
{
    connect(m_appsManager, &AppsManager::dataChanged, this, [=] {
        emit QAbstractListModel::layoutChanged();
    });
}

ListModel::~ListModel()
{
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    if (m_appsManager->mode() == AppsManager::Normal) {
        return m_appsManager->appList().size();
    } else {
        return m_appsManager->searchList().size();
    }
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int r = index.row();
    DesktopInfo info;

    if (m_appsManager->mode() == AppsManager::Normal) {
        info = m_appsManager->appList().at(r);
    } else {
        info = m_appsManager->searchList().at(r);
    }

    switch (role) {
    case AppNameRole:
        return info.name;
    case AppIconRole:
        return info.iconName;
    case AppExecRole:
        return info.exec;
    case ItemSizeHintRole:
        return CalcUtil::instance()->itemSize();
    default:
        return QVariant();
    }

    return QVariant();
}
