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

#include "listview.h"
#include <QEvent>
#include <QApplication>
#include <QScreen>

ListView::ListView(QWidget *parent)
    : QListView(parent)
{
    viewport()->installEventFilter(this);

    setFlow(QListView::LeftToRight);
    setLayoutMode(QListView::Batched);
    setResizeMode(QListView::Adjust);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setFrameStyle(QFrame::NoFrame);
    setWrapping(true);
}

ListView::~ListView()
{
}

bool ListView::eventFilter(QObject *object, QEvent *e)
{
    if (object == viewport() && e->type() == QEvent::Paint) {
        // fitToContent();
    }

    return false;
}

void ListView::fitToContent()
{
    if (width() == contentsRect().width() &&
        height() == contentsSize().height()) {
        return;
    }

    const int h = contentsSize().height();

    setFixedHeight(h < 0 ? 0 : h);
    setFixedWidth(contentsRect().width());
}
