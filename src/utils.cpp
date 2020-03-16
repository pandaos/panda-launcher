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

#include "utils.h"
#include <QApplication>
#include <QImageReader>
#include <QPixmap>
#include <QPainter>

QT_BEGIN_NAMESPACE
extern void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

Utils::Utils(QObject *parent)
    : QObject(parent)
{
}

Utils::~Utils()
{

}

QByteArray Utils::detectDesktopEnvironment()
{
    const QByteArray desktop = qgetenv("XDG_CURRENT_DESKTOP");

    if (!desktop.isEmpty()) {
        return desktop.toUpper();
    }

    return QByteArray("UNKNOWN");
}

QPixmap Utils::renderSVG(const QString &path, const QSize &size)
{
    const qreal devicePixelRatio = qApp->devicePixelRatio();
    QImageReader reader;
    QPixmap pixmap;
    reader.setFileName(path);
    if (reader.canRead()) {
        reader.setScaledSize(size * devicePixelRatio);
        pixmap = QPixmap::fromImage(reader.read());
        pixmap.setDevicePixelRatio(devicePixelRatio);
    }
    else {
        pixmap.load(path);
    }

    return pixmap;
}

void Utils::blurImage(const QString &path)
{
    QPixmap pixmap(path);
    QPixmap target(pixmap.size());
    QPainter painter(&target);
    QImage img = pixmap.toImage();
    qt_blurImage(&painter, img, 100, true, false);
    painter.end();
    target.save(path);
}
