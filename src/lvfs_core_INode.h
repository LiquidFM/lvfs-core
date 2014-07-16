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

#ifndef LVFS_CORE_INODE_H_
#define LVFS_CORE_INODE_H_

#include <cstdlib>
#include <efc/Pair>
#include <efc/List>
#include <lvfs/IEntry>
#include <lvfs/Module>
#include <QtCore/QString>


namespace LVFS {
namespace Core {

class PLATFORM_MAKE_PUBLIC INode
{
    DECLARE_INTERFACE(LVFS::Core::INode)

public:
    typedef EFC::List<qint32>                  Geometry;
    typedef EFC::Pair<qint32, ::Qt::SortOrder> Sorting;

public:
    virtual ~INode();

    virtual const QString &title() const = 0;
    virtual const QString &schema() const = 0;
    virtual const QString &location() const = 0;
    virtual const Interface::Holder &file() const = 0;
    virtual const Interface::Holder &parent() const = 0;
    virtual const Geometry &geometry() const = 0;
    virtual const Sorting &sorting() const = 0;

    virtual void refresh(int depth = 0) = 0;
    virtual void opened(const Interface::Holder &view) = 0;
    virtual void closed(const Interface::Holder &view) = 0;

public:
    static Interface::Holder open(const char *uri, Module::Error &error);
    static Interface::Holder view(const Interface::Holder &node);
};

}}

#endif /* LVFS_CORE_INODE_H_ */
