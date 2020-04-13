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

    const int itemWidth = pr.width() <= 1440 ? 160 : 200;
    const int spacing = pr.width() <= 1440 ? 20 : 25;
    const int columns = remainWidth / itemWidth;

    m_itemSize = (double(frameSize.width()) - spacing * columns * 2) / columns + 0.5;
    m_itemSpacing = (double(frameSize.width()) - m_itemSize * columns) / (columns * 2) - 1;
    m_columnCount = columns;

    emit layoutChanged();
}
