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

#include <efc/Pair>
#include <efc/List>
#include <QtGui/QIcon>
#include <QtCore/QString>
#include <QtCore/QDateTime>
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
    typedef int size_type;

    struct Item
    {
        bool isDir;
        QString title;
        QString schema;
        QString location;
        QIcon icon;
        QString size;
        QDateTime modified;
        Interface::Holder node;
    };

    typedef EFC::List<qint32>                  Geometry;
    typedef EFC::Pair<qint32, ::Qt::SortOrder> Sorting;

public:
    virtual ~INode();

    virtual size_type size() const = 0;
    virtual const Item &at(size_type index) const = 0;
    virtual size_type indexOf(const Item &node) const = 0;

    virtual QAbstractItemModel *model() const = 0;

    virtual const Geometry &geometry() const = 0;
    virtual const Sorting &sorting() const = 0;

    virtual QModelIndex currentIndex() const = 0;
    virtual void setCurrentIndex(const QModelIndex &index) = 0;

    virtual void copyToClipboard(const QModelIndexList &files, bool move) = 0;
};

}}}

#endif /* LVFS_CORE_QT_INODE_H_ */
