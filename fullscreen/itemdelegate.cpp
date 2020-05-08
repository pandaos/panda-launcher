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

#include "itemdelegate.h"
#include "listmodel.h"
#include <QApplication>

const QRect itemBoundingRect(const QRect &itemRect)
{
    const int w = itemRect.width();
    const int h = itemRect.height();
    const int sub = qAbs((w - h) / 2);

    if (w == h)
        return itemRect;
    else if (w > h)
        return itemRect - QMargins(sub, 0, sub, 0);
    else
        return itemRect - QMargins(0, 0, 0, sub * 2);
}

const QPixmap getThemeIcon(const QString &iconName, const int size)
{
    const auto ratio = qApp->devicePixelRatio();
    QPixmap pixmap;

    const QIcon icon = QIcon::fromTheme(iconName, QIcon::fromTheme("application-x-desktop"));
    pixmap = icon.pixmap(QSize(size, size));

    if (qFuzzyCompare(pixmap.devicePixelRatioF(), 1.0)) {
        pixmap = pixmap.scaled(QSize(size, size) * ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pixmap.setDevicePixelRatio(ratio);
    }

    return pixmap;
}

ItemDelegate::ItemDelegate(QObject *parent) :
    QAbstractItemDelegate(parent),
    m_currentIndex(QModelIndex())
{

}

void ItemDelegate::setCurrentIndex(const QModelIndex &idx)
{
    if (m_currentIndex == idx)
        return;

    QModelIndex previousIdx = m_currentIndex;
    m_currentIndex = idx;

    emit requestUpdate(previousIdx);
    emit requestUpdate(m_currentIndex);
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = itemBoundingRect(option.rect);

    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setPen(Qt::transparent);

    const bool isCurrent = index == m_currentIndex;
    if (isCurrent) {
        int radius = rect.width() * 0.1;
        painter->setBrush(QColor(0, 0, 0, 50));
        painter->drawRoundedRect(rect, radius, radius);
    }

    int iconSize = rect.width() * 0.55;
    QPixmap iconPixmap = getThemeIcon(index.data(ListModel::AppIconRole).toString(), iconSize);
    int iconLeftMargin = (rect.width() - iconPixmap.width()) / 2;
    int topMargin = rect.height() * 0.1;
    const QRect iconRect(rect.x() + iconLeftMargin,
                         rect.y() + topMargin,
                         iconPixmap.width(),
                         iconPixmap.height());
    painter->drawPixmap(iconRect, iconPixmap);

    QTextOption appNameOption;
    appNameOption.setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    appNameOption.setWrapMode(QTextOption::WordWrap);

    painter->setBrush(Qt::transparent);
    painter->setPen(Qt::black);

    const QRect textRect(rect.x(),
                         iconRect.bottom() + topMargin,
                         rect.width(),rect.height());

    QString displayName = index.data(ListModel::AppNameRole).toString();
    QFontMetrics fm(painter->fontMetrics());
    displayName = fm.elidedText(displayName, Qt::ElideRight, textRect.width() - 20);
    painter->setPen(QColor(0, 0, 0, 100));
    painter->drawText(textRect.adjusted(0.5, 1.5, 0.5, 1.5), Qt::AlignHCenter | Qt::AlignTop, displayName);
    painter->drawText(textRect.adjusted(0.5, -0.5, 0.5, -0.5), Qt::AlignHCenter | Qt::AlignTop, displayName);
    painter->drawText(textRect.adjusted(1, -0.6, 1, -0.6), Qt::AlignHCenter | Qt::AlignTop, displayName);
    painter->setPen(Qt::white);
    painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, displayName);

//    QString appName = index.data(ListModel::AppNameRole).toString();
//    const QPair<QString, bool> appTextResolvedInfo = holdTextInRect(painter->fontMetrics(), appName, textRect);
//    painter->setPen(QColor(0, 0, 0, 100));
//    painter->drawText(textRect.adjusted(0.7, 1, 0.7, 1), appTextResolvedInfo.first, appNameOption);
//    painter->drawText(textRect.adjusted(-0.7, 1, -0.7, 1), appTextResolvedInfo.first, appNameOption);
//    painter->setPen(Qt::white);
//    painter->drawText(textRect, appTextResolvedInfo.first, appNameOption);
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    return index.data(ListModel::ItemSizeHintRole).toSize();
}
