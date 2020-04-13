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

#ifndef FULLSCREENFRAME_H
#define FULLSCREENFRAME_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFileSystemWatcher>
#include <QSettings>

#include "listview.h"
#include "listmodel.h"
#include "itemdelegate.h"
#include "appsmanager.h"
#include "searchedit.h"
#include "calcutil.h"

class FullScreenFrame : public QWidget
{
    Q_OBJECT

public:
    explicit FullScreenFrame(QWidget *parent = nullptr);

public slots:
    void showLauncher();
    void hideLauncher();
    void toggleLauncher();

private:
    void initSize();
    void onSearchTextChanged(const QString &text);
    void onPopupMenu(const QPoint &p, const QModelIndex &idx);
    void onConfigFileChanged(const QString &filePath);

private slots:
    void initContentMargins();
    void initBackground();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    bool eventFilter(QObject *o, QEvent *e) override;
    void showEvent(QShowEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    bool event(QEvent *e) override;

    bool handleKeyEvent(QKeyEvent *e);

private:
    QVBoxLayout *m_mainLayout;
    ListView *m_listView;
    ListModel *m_listModel;
    ItemDelegate *m_itemDelegate;
    AppsManager *m_appsManager;
    SearchEdit *m_searchEdit;
    CalcUtil *m_calcUtil;

    QString m_dockConfigPath;
    QFileSystemWatcher *m_fileWatcher;

    QPixmap m_backgroundPixmap;

    QRect m_frameRect;
};

#endif // FULLSCREENFRAME_H
