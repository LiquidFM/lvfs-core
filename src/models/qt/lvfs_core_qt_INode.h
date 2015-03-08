/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2015 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#include <efc/Pair>
#include <efc/List>
#include <lvfs/Interface>
#include <lvfs-core/INode>
#include <QtCore/QAbstractItemModel>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PUBLIC INode
{
    DECLARE_INTERFACE(LVFS::Core::Qt::INode)

public:
    typedef EFC::List<qint32>                  Geometry;
    typedef EFC::Pair<qint32, ::Qt::SortOrder> Sorting;

public:
    virtual ~INode();

    virtual QAbstractItemModel *model() const = 0;

    virtual const Geometry &geometry() const = 0;
    virtual const Sorting &sorting() const = 0;

    virtual QModelIndex currentIndex() const = 0;
    virtual void setCurrentIndex(const QModelIndex &index) = 0;

    virtual Interface::Holder mapToFile(const QModelIndex &index) const = 0;
    virtual Core::INode::Files mapToFile(const QModelIndexList &indices) const = 0;

    virtual bool isLocked(const QModelIndex &index, quint64 &progress, quint64 &total) const = 0;
    virtual bool compareItems(const QModelIndex &left, const QModelIndex &right, ::Qt::SortOrder sortOrder) const = 0;

    virtual void activated(const Interface::Holder &view, const QModelIndex &index) = 0;
    virtual void rename(const Interface::Holder &view, const QModelIndex &index) = 0;
    virtual void copy(const Interface::Holder &view, const Interface::Holder &dest, Core::INode::Files &files, bool move = false) = 0;
    virtual void copyToClipboard(const Interface::Holder &view, const QModelIndexList &indices, bool move = false) = 0;
    virtual void remove(const Interface::Holder &view, const QModelIndexList &indices) = 0;
    virtual void createDirectory(const Interface::Holder &view, const QModelIndex &index) = 0;
    virtual void cancel(const QModelIndexList &indices) = 0;
};

}}}

#endif /* LVFS_CORE_QT_INODE_H_ */
