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

void CalcUtil::calc()
{
    const QRect pr = qApp->primaryScreen()->geometry();
    const int screenWidth = pr.width();
    int remainWidth = screenWidth - calculateBesidePadding(screenWidth) * 2;
    QSize frameSize = pr.size() - QSize(m_margins.left() + m_margins.right(), 0);

    remainWidth = screenWidth - m_margins.left() - m_margins.right();

    const int itemWidth = pr.width() <= 1440 ? 160 : 190;
    const int spacing = pr.width() <= 1440 ? 15 : 20;
    const int columns = remainWidth / itemWidth;

    const int calcItemWidth = (double(frameSize.width()) - spacing * columns * 2) / columns + 0.5;
    const int calcSpacing = (double(frameSize.width()) - calcItemWidth * columns) / (columns * 2) - 1;

    m_itemSpacing = calcSpacing;
    m_itemSize = calcItemWidth;
    m_columnCount = columns;

    emit layoutChanged();
}
