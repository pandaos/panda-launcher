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
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QMouseEvent>

FullScreenFrame::FullScreenFrame(QWidget *parent)
    : QWidget(parent),
      m_listView(new ListView),
      m_listModel(new ListModel),
      m_itemDelegate(new ItemDelegate),
      m_appsManager(AppsManager::instance())
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(50, 10, 50, 10);
    layout->addWidget(m_listView);

    m_listView->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_listView->setItemDelegate(m_itemDelegate);
    m_listView->setModel(m_listModel);

    initSize();

    connect(m_listView, &QListView::clicked, m_appsManager, &AppsManager::launchApp);
    connect(m_listView, &QListView::clicked, this, &FullScreenFrame::hideLauncher);
    connect(qApp->primaryScreen(), &QScreen::geometryChanged, this, &FullScreenFrame::initSize, Qt::QueuedConnection);
}

void FullScreenFrame::initSize()
{
    QRect geometry = qApp->primaryScreen()->availableGeometry();
    setGeometry(geometry);
}

void FullScreenFrame::hideLauncher()
{
    QWidget::hide();
}

void FullScreenFrame::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        hideLauncher();
    }
}

void FullScreenFrame::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        return;

    hideLauncher();
}
