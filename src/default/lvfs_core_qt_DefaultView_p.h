/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2016 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * lvfs-core is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lvfs-core is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lvfs-core. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LVFS_CORE_QT_DEFAULTVIEW_P_H_
#define LVFS_CORE_QT_DEFAULTVIEW_P_H_

#include <platform/utils.h>
#include <QtCore/QObject>
#include <QtGui/QTreeView>


namespace LVFS {
namespace Core {
namespace Qt {

class DefaultView;

}}}


class PLATFORM_MAKE_PRIVATE DefaultViewListener : public QObject
{
    Q_OBJECT

public:
    typedef void (LVFS::Core::Qt::DefaultView::*Method)(const QModelIndex &index);

public:
    DefaultViewListener(LVFS::Core::Qt::DefaultView *listener, Method collapsed, Method expanded) :
        m_listener(listener),
        m_collapsed(collapsed),
        m_expanded(expanded)
    {}
    virtual ~DefaultViewListener();

public slots:
    void collapsed(const QModelIndex &index);
    void expanded(const QModelIndex &index);

private:
    LVFS::Core::Qt::DefaultView *m_listener;
    Method m_collapsed;
    Method m_expanded;
};

#endif /* LVFS_CORE_QT_DEFAULTVIEW_P_H_ */
