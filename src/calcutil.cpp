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

void CalcUtil::calc()
{
    const QRect pr = qApp->primaryScreen()->geometry();
    const int screenWidth = pr.width();
    int remainWidth = screenWidth - m_margins.left() - m_margins.right();
    QSize frameSize = pr.size() - QSize(m_margins.left() + m_margins.right(), 0);

    const int itemWidth = pr.width() <= 1440 ? 160 : 210;
    const int spacing = pr.width() <= 1440 ? 20 : 30;
    const int columns = remainWidth / itemWidth;

    m_itemSize = (double(frameSize.width()) - spacing * columns * 2) / columns + 0.5;
    m_itemSpacing = (double(frameSize.width()) - m_itemSize * columns) / (columns * 2) - 1;
    m_columnCount = columns;

    emit layoutChanged();
}
