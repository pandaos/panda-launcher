#include "calcutil.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>

CalcUtil *CalcUtil::instance()
{
    static CalcUtil calcUtil;
    return &calcUtil;
}

CalcUtil::CalcUtil(QObject *parent)
    : QObject(parent)
{

}

int CalcUtil::calculateBesidePadding(const int screenWidth)
{
    if (screenWidth > 1366)
        return 180;

    return 130;
}

void CalcUtil::calc(const QSize &containerSize)
{
    const QRect pr = qApp->primaryScreen()->availableGeometry();
    const int screenWidth = pr.width();
    int remainWidth = screenWidth - calculateBesidePadding(screenWidth) * 2;

    const int itemWidth = pr.width() <= 1440 ? 170 : 200;
    const int spacing = pr.width() <= 1440 ? 10 : 14;
    const int columns = remainWidth / itemWidth;

    const int calc_item_width = (double(containerSize.width()) - spacing * columns * 2) / columns + 0.5;
    const int calc_spacing = (double(containerSize.width()) - calc_item_width * columns) / (columns * 2) - 1;

    m_itemSpacing = calc_spacing;
    m_itemSize = calc_item_width;
    m_columnCount = columns;

    emit layoutChanged();
}
