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
#include "applicationadaptor.h"
#include "utils.h"

#include <QApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStandardPaths>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QtConcurrent>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusServiceWatcher>
#include <QImageReader>

#include <KF5/KWindowSystem/KWindowEffects>
#include <KF5/KWindowSystem/KWindowSystem>

FullScreenFrame::FullScreenFrame(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(new QVBoxLayout),
      m_listView(new ListView),
      m_listModel(new ListModel),
      m_itemDelegate(new ItemDelegate),
      m_appsManager(AppsManager::instance()),
      m_searchEdit(new QLineEdit),
      m_calcUtil(CalcUtil::instance()),
      m_fileWatcher(new QFileSystemWatcher(this))
{
    // init dock settings.
    m_dockConfigPath = QString("%1/panda-dock/config.conf")
            .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));

    m_fileWatcher->addPath(m_dockConfigPath);
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &FullScreenFrame::onConfigFileChanged);

    // init dbus
    {
        QDBusConnection dbus = QDBusConnection::sessionBus();
        if (dbus.registerService(QLatin1String("org.panda.launcher"))) {
            new ApplicationAdaptor(this);
            dbus.registerObject("/Launcher", this);
        }
    }

    QDBusInterface *interface = new QDBusInterface("org.panda.files", "/Files",
                                                   "org.panda.Files", QDBusConnection::sessionBus());
    if (interface->isValid())
        connect(interface, SIGNAL(wallpaperChanged()), this, SLOT(delayUpdateBackground()));

    QDBusServiceWatcher *serviceWatcher = new QDBusServiceWatcher("org.panda.files", QDBusConnection::sessionBus(),
                                                                  QDBusServiceWatcher::WatchForRegistration);
    connect(serviceWatcher, &QDBusServiceWatcher::serviceRegistered, this, [=] {
        QDBusInterface *iface = new QDBusInterface("org.panda.files", "/Files",
                                                   "org.panda.Files", QDBusConnection::sessionBus());
        connect(iface, SIGNAL(wallpaperChanged()), this, SLOT(delayUpdateBackground()));

        delayUpdateBackground();
    });

    // Init attributes.
    setAttribute(Qt::WA_TranslucentBackground);
    // setAttribute(Qt::WA_NoSystemBackground, false);
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::ClickFocus);

    KWindowEffects::enableBlurBehind(winId(), true);

    m_mainLayout->setSpacing(0);
    m_mainLayout->addWidget(m_searchEdit, 0, Qt::AlignHCenter);
    m_mainLayout->addSpacing(10);
    m_mainLayout->addWidget(m_listView);
    setLayout(m_mainLayout);

    initSize();
    delayUpdateBackground();

    m_itemDelegate->setCurrentIndex(QModelIndex());
    m_listView->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_listView->setItemDelegate(m_itemDelegate);
    m_listView->setModel(m_listModel);

    installEventFilter(this);
    m_listView->installEventFilter(this);
    m_listView->viewport()->installEventFilter(this);
    m_searchEdit->installEventFilter(this);
    m_searchEdit->setPlaceholderText(tr("Search App"));
    m_searchEdit->setFixedHeight(30);
    m_searchEdit->setFixedWidth(300);
    m_searchEdit->setFocusPolicy(Qt::ClickFocus);
    hideLauncher();

    connect(qApp->primaryScreen(), &QScreen::geometryChanged, this, &FullScreenFrame::initSize, Qt::QueuedConnection);
    connect(m_listView, &QListView::entered, m_itemDelegate, &ItemDelegate::setCurrentIndex);
    connect(m_listView, &QListView::clicked, m_appsManager, &AppsManager::launchApp);
    connect(m_listView, &QListView::clicked, this, &FullScreenFrame::hideLauncher);
    connect(m_listView, &ListView::requestHideLauncher, this, &FullScreenFrame::hideLauncher);
    connect(m_listView, &ListView::requestPopupMenu, this, &FullScreenFrame::onPopupMenu);

    connect(m_itemDelegate, &ItemDelegate::requestUpdate, m_listView, static_cast<void (ListView::*)(const QModelIndex&)>(&ListView::update));

    connect(m_appsManager, &AppsManager::requestHideLauncher, this, &FullScreenFrame::hideLauncher);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &FullScreenFrame::onSearchTextChanged);
}

void FullScreenFrame::showLauncher()
{
    QWidget::setVisible(true);
    QWidget::activateWindow();
}

void FullScreenFrame::hideLauncher()
{
    QWidget::setVisible(false);
}

void FullScreenFrame::toggleLauncher()
{
    isVisible() ? hideLauncher() : showLauncher();
}

void FullScreenFrame::delayUpdateBackground()
{
    QTimer::singleShot(0, this, &FullScreenFrame::initBackground);
}

void FullScreenFrame::initSize()
{
    QRect geometry = qApp->primaryScreen()->geometry();
    QWidget::setGeometry(geometry);
    QWidget::setFixedSize(geometry.size());

    initContentMargins();

    if (m_frameRect != geometry) {
        delayUpdateBackground();
    }

    m_frameRect = geometry;
}

void FullScreenFrame::onSearchTextChanged(const QString &text)
{
    m_appsManager->searchApp(text);

    if (text.isEmpty())
        m_appsManager->switchToNormalMode();
    else
        m_appsManager->switchToSearchMode();
}

void FullScreenFrame::onPopupMenu(const QPoint &p, const QModelIndex &idx)
{
    m_appsManager->showRightMenu(p, idx);
}

void FullScreenFrame::initContentMargins()
{
    // 计算边缘 padding 值
    QMargins margins;
    const QRect screenRect = qApp->primaryScreen()->geometry();
    const QRect availableRect = qApp->primaryScreen()->availableGeometry();
    const int edgePadding = screenRect.size().width() * 0.05;
    const int padding = 20;
    int topMargin = availableRect.y() + padding;
    int bottomMargin = screenRect.height() - availableRect.height() - availableRect.y() + padding;
    int edgeMargin = availableRect.x() + edgePadding;

    margins.setLeft(edgeMargin);
    margins.setRight(edgeMargin);
    margins.setTop(topMargin);
    margins.setBottom(bottomMargin);

    m_mainLayout->setContentsMargins(margins);
    m_calcUtil->setMargins(margins);
    m_calcUtil->calc();
}

void FullScreenFrame::initBackground()
{
    return;
    QDBusConnection dbus = QDBusConnection::sessionBus();
    QDBusInterface iface(QLatin1String("org.panda.files"), QStringLiteral("/Files"), QLatin1String("org.panda.Files"), dbus, this);

    if (!iface.isValid())
        return;

    QRect screenRect = qApp->primaryScreen()->geometry();
    QString fileName = iface.property("wallpaper").toString();
    QImage image;
    QImageReader reader(fileName);
    QSize origSize = reader.size();

    if (origSize.isValid()) {
        QSize desiredSize = origSize;
        Qt::AspectRatioMode mode = Qt::KeepAspectRatioByExpanding;
        desiredSize.scale(screenRect.width(), screenRect.height(), mode);
        QImage image(fileName);
        image = image.scaled(desiredSize.width(), desiredSize.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        if (!image.isNull()) {
            QPixmap pixmap(screenRect.size());
            QPainter painter(&pixmap);
            int x = (screenRect.width() - image.width()) / 2;
            int y = (screenRect.height() - image.height()) / 2;
            painter.drawImage(x, y, image);
            m_backgroundPixmap = pixmap;

            image = QImage();
        }
    }

    // const QSize &size = qApp->primaryScreen()->size() * qApp->primaryScreen()->devicePixelRatio();
    // m_backgroundPixmap.load(iface.property("wallpaper").toString());
    // m_backgroundPixmap = m_backgroundPixmap.scaled(size,
    //                                                Qt::IgnoreAspectRatio,
    //                                                Qt::SmoothTransformation);
    // m_backgroundPixmap.setDevicePixelRatio(devicePixelRatioF());
    // m_backgroundPixmap = Utils::blurPixmap(m_backgroundPixmap, 200);

    FullScreenFrame::update();
}

void FullScreenFrame::onConfigFileChanged(const QString &filePath)
{
    // Since Qt 5.4, QSettings uses QSaveFile to save the config files.
    // https://github.com/qtproject/qtbase/commit/8d15068911d7c0ba05732e2796aaa7a90e34a6a1#diff-e691c0405f02f3478f4f50a27bdaecde
    // QSaveFile will save the content to a new temp file, and replace the old file later.
    bool fileDeleted = !m_fileWatcher->files().contains(m_dockConfigPath);
    if (fileDeleted)
        m_fileWatcher->addPath(m_dockConfigPath);

    QtConcurrent::run(this, &FullScreenFrame::initSize);
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
//        m_calcUtil->calc(static_cast<QResizeEvent *>(e)->size() - QSize(PADDING + PADDING, 0));
    }

    if (e->type() == QEvent::KeyPress)
        return handleKeyEvent(static_cast<QKeyEvent *>(e));

    return false;
}

void FullScreenFrame::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    KWindowSystem::setState(winId(), NET::SkipTaskbar);
    KWindowSystem::setState(winId(), NET::SkipSwitcher);
    m_searchEdit->clearFocus();
    m_searchEdit->clear();

    initSize();

}

void FullScreenFrame::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QColor color("#000000");
    color.setAlpha(80);
    painter.fillRect(rect(), color);

    // QScreen const *s = qApp->primaryScreen();
    // const QRect &geom = s->geometry();
    // QRect tr(QPoint(0, 0), geom.size());
    // painter.drawPixmap(tr, m_backgroundPixmap, QRect(tr.topLeft(),
    //                                                  tr.size() * m_backgroundPixmap.devicePixelRatioF()));
}

bool FullScreenFrame::event(QEvent *e)
{
    if (e->type() == QEvent::ActivationChange) {
        if (qApp->activeWindow() != this)
            hideLauncher();
    }

    return QWidget::event(e);
}

bool FullScreenFrame::handleKeyEvent(QKeyEvent *e)
{
    bool ctrlPressed = e->modifiers() & Qt::ControlModifier;

    if (e->key() == Qt::Key_Escape) {
        hideLauncher();
        return true;
    }

    if (e->key() == Qt::Key_Backspace) {
        m_searchEdit->backspace();
        return true;
    }

    if ((e->key() <= Qt::Key_Z && e->key() >= Qt::Key_A) ||
        (e->key() <= Qt::Key_9 && e->key() >= Qt::Key_0)) {

        if (ctrlPressed && e->key() == Qt::Key_A) {
            m_searchEdit->selectAll();
            return true;
        }

        if (ctrlPressed && e->key() == Qt::Key_X) {
            m_searchEdit->cut();
            return true;
        }

        if (ctrlPressed && e->key() == Qt::Key_C) {
            m_searchEdit->copy();
            return true;
        }

        if (ctrlPressed && e->key() == Qt::Key_V) {
            m_searchEdit->paste();
            return true;
        }

        const char ch = e->text()[0].toLatin1();
        if (m_searchEdit->hasSelectedText())
            m_searchEdit->backspace();

        m_searchEdit->setText(m_searchEdit->text() + ch);

        return true;
    }

    return false;
}
