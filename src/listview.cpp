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
    setAttribute(Qt::WA_TranslucentBackground);
    setFrameStyle(QFrame::NoFrame);
    setWrapping(true);
    setSpacing(CalcUtil::instance()->itemSpacing());

    setStyleSheet("QListView { background-color: transparent;}");

    connect(CalcUtil::instance(), &CalcUtil::layoutChanged, this, [=] {
        setSpacing(CalcUtil::instance()->itemSpacing());

        QListView::update();
    });
}

ListView::~ListView()
{
}

bool ListView::eventFilter(QObject *object, QEvent *e)
{
    if (object == viewport() && e->type() == QEvent::Paint) {
//         fitToContent();
    }

    if (object == viewport() && e->type() == QEvent::MouseButtonRelease) {
        if (static_cast<QMouseEvent *>(e)->button() == Qt::LeftButton) {
            emit requestHideLauncher();
            return false;
        }
    }

    return false;
}

void ListView::fitToContent()
{
    const QSize size { contentsRect().width(), contentsSize().height() };

    if (size == rect().size()) return;

    setFixedSize(size);
}
