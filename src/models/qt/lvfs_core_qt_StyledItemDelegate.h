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

#ifndef LVFS_CORE_QT_STYLEDITEMDELEGATE_H_
#define LVFS_CORE_QT_STYLEDITEMDELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include <QtGui/QStyleOptionViewItem>
#include <QtGui/QSortFilterProxyModel>
#include <platform/utils.h>
#include <lvfs/Interface>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PUBLIC StyledItemDelegate : public QStyledItemDelegate
{
    PLATFORM_MAKE_NONCOPYABLE(StyledItemDelegate)

public:
    StyledItemDelegate(const Interface::Holder &node, QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    void paintBackgroundLines(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintProgressInMb(QPainter *painter, const QStyleOptionViewItem &option, int progress) const;

private:
    const Interface::Holder &m_node;
};

}}}

#endif /* LVFS_CORE_QT_STYLEDITEMDELEGATE_H_ */
