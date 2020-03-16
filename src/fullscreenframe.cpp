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

#include "fullscreenframe.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QMouseEvent>

#include <KF5/KWindowSystem/KWindowEffects>
#include <KF5/KWindowSystem/KWindowSystem>

#define PADDING 200

const QPoint widgetRelativeOffset(const QWidget *const self, const QWidget *w)
{
    QPoint offset;
    while (w && w != self) {
        offset += w->pos();
        w = qobject_cast<QWidget *>(w->parent());
    }

    return offset;
}

FullScreenFrame::FullScreenFrame(QWidget *parent)
    : QFrame(parent),
      m_listView(new ListView),
      m_listModel(new ListModel),
      m_itemDelegate(new ItemDelegate),
      m_appsManager(AppsManager::instance()),
      m_searchEdit(new SearchEdit),
      m_calcUtil(CalcUtil::instance())
{
    setAttribute(Qt::WA_NoSystemBackground, false);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::ClickFocus);

    QPalette pal = this->palette();
    QColor windowColor("#EEEEEE");
    windowColor.setAlpha(140);
    pal.setColor(QPalette::Window, windowColor);
    setPalette(pal);

    KWindowEffects::enableBlurBehind(winId(), true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(200, 10, 200, 10);
    layout->addWidget(m_searchEdit, 0, Qt::AlignHCenter);
    layout->addWidget(m_listView);

    m_listView->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_listView->setItemDelegate(m_itemDelegate);
    m_listView->setModel(m_listModel);

//    setAttribute(Qt::WA_InputMethodEnabled, true);
//    installEventFilter(this);
//    m_searchEdit->installEventFilter(this);

    initSize();

    connect(m_listView, &QListView::clicked, m_appsManager, &AppsManager::launchApp);
    connect(m_listView, &QListView::clicked, this, &FullScreenFrame::hideLauncher);
    connect(qApp->primaryScreen(), &QScreen::geometryChanged, this, &FullScreenFrame::initSize, Qt::QueuedConnection);
}

void FullScreenFrame::initSize()
{
    QRect geometry = qApp->primaryScreen()->availableGeometry();
    setGeometry(geometry);

    m_calcUtil->calc(rect().size()- QSize(PADDING + PADDING, 0));
}

void FullScreenFrame::hideLauncher()
{
    QWidget::hide();
}

void FullScreenFrame::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        hideLauncher();
    }
}

void FullScreenFrame::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        return;

    hideLauncher();
}

bool FullScreenFrame::eventFilter(QObject *o, QEvent *e)
{
    if (o == m_searchEdit && e->type() == QEvent::KeyPress) {
        QKeyEvent *keyPress = static_cast<QKeyEvent *>(e);
        if (keyPress->key() == Qt::Key_Left || keyPress->key() == Qt::Key_Right) {
            QKeyEvent *event = new QKeyEvent(keyPress->type(), keyPress->key(), keyPress->modifiers());
            qApp->postEvent(this, event);
            return true;
        }
    } else if (o == m_listView->viewport() && e->type() == QEvent::Resize) {
        m_calcUtil->calc(static_cast<QResizeEvent *>(e)->size() - QSize(PADDING + PADDING, 0));
    }

    return false;
}

void FullScreenFrame::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    KWindowSystem::setState(winId(), NET::SkipTaskbar);

    initSize();
}
