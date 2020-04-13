#ifndef CALCUTIL_H
#define CALCUTIL_H

#include <QObject>
#include <QMargins>
#include <QSize>

class CalcUtil : public QObject
{
    Q_OBJECT

public:
    static CalcUtil *instance();
    explicit CalcUtil(QObject *parent = nullptr);

    int calculateBesidePadding(const int screenWidth);

    void calc();

    inline int itemSpacing() const { return m_itemSpacing; };
    inline QSize itemSize() const { return QSize(m_itemSize, m_itemSize); };
    inline int columnCount() const { return m_columnCount; };

    void setMargins(const QMargins &margins) { m_margins = margins; };

signals:
    void layoutChanged();

private:
    int m_itemSpacing = 10;
    int m_itemSize = 130;
    int m_columnCount = 7;
    QMargins m_margins;
};

#endif // CALCUTIL_H
