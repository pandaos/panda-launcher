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

#include <QFrame>
#include "listview.h"
#include "listmodel.h"
#include "itemdelegate.h"
#include "appsmanager.h"
#include "searchedit.h"

class FullScreenFrame : public QFrame
{
    Q_OBJECT

public:
    explicit FullScreenFrame(QWidget *parent = nullptr);

private:
    void initSize();
    void hideLauncher();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    bool eventFilter(QObject *o, QEvent *e) override;
    void showEvent(QShowEvent *e) override;

private:
    ListView *m_listView;
    ListModel *m_listModel;
    ItemDelegate *m_itemDelegate;
    AppsManager *m_appsManager;
    SearchEdit *m_searchEdit;
};

#endif // FULLSCREENFRAME_H
