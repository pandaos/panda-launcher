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
#include "calcutil.h"
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>
#include <QScreen>
#include <QDebug>
#include <QScroller>

ListView::ListView(QWidget *parent)
    : QListView(parent)
{
    setFlow(QListView::LeftToRight);
    setLayoutMode(QListView::Batched);
    setResizeMode(QListView::Adjust);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setAttribute(Qt::WA_TranslucentBackground);
    setFrameStyle(QFrame::NoFrame);
    setMouseTracking(true);
    setWrapping(true);
    setSpacing(CalcUtil::instance()->itemSpacing());

    QScroller::grabGesture(this, QScroller::TouchGesture);
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);

    setStyleSheet("QListView { background-color: transparent;}");

    connect(CalcUtil::instance(), &CalcUtil::layoutChanged, this, [=] {
        setSpacing(CalcUtil::instance()->itemSpacing());

        QListView::update();
    });
}

ListView::~ListView()
{
}

void ListView::enterEvent(QEvent *e)
{
    QListView::enterEvent(e);

    emit entered(indexAt(mapFromGlobal(QCursor::pos())));
}

void ListView::mouseMoveEvent(QMouseEvent *e)
{
    const QModelIndex &idx = indexAt(e->pos());

    emit entered(idx);
}

void ListView::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton) {
        QPoint p = mapToGlobal(e->pos());
        const QModelIndex &idx = QListView::indexAt(e->pos());

        if (idx.isValid())
            emit requestPopupMenu(p, idx);
    }

    QListView::mousePressEvent(e);
}

void ListView::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
//        emit requestHideLauncher();
    }

    QListView::mouseReleaseEvent(e);
}

void ListView::fitToContent()
{
    const QSize size { contentsRect().width(), contentsSize().height() };

    if (size == rect().size()) return;

    setFixedSize(size);
}
