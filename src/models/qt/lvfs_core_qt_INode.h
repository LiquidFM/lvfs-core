/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#ifndef LVFS_CORE_QT_INODE_H_
#define LVFS_CORE_QT_INODE_H_

#include <QtCore/QVariant>
#include <QtCore/QAbstractItemModel>
#include <lvfs/Interface>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PUBLIC INode
{
    DECLARE_INTERFACE(LVFS::Core::Qt::INode)

public:
    typedef int               size_type;
    typedef Interface::Holder Item;

public:
    virtual ~INode();

    virtual const Item &parent() const = 0;
    virtual size_type size() const = 0;
    virtual const Item &at(size_type index) const = 0;
    virtual size_type indexOf(const Item &node) const = 0;
    virtual QVariant data(int column, int role) const = 0;

    virtual QAbstractItemModel *model() const = 0;

    virtual QModelIndex parentIndex() const = 0;
    virtual void setParentIndex(const QModelIndex &index) = 0;
};

}}}

#endif /* LVFS_CORE_QT_INODE_H_ */
