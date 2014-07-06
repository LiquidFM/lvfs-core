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

#ifndef LVFS_CORE_QT_DEFAULTNODE_H_
#define LVFS_CORE_QT_DEFAULTNODE_H_

#include <efc/Vector>
#include <lvfs-core/models/Qt/ModelNode>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE DefaultNode : public ModelNode
{
public:
    DefaultNode(const Interface::Holder &file, const Item &parent = Item());
    virtual ~DefaultNode();

    /* Core::INode */
    virtual void refresh(int depth);
    virtual const IEntry *entry() const;

    /* Qt::INode */
    virtual size_type size() const;
    virtual const Item &at(size_type index) const;
    virtual size_type indexOf(const Item &node) const;
    virtual QVariant data(int column, int role) const;

protected:
    virtual void processListFile(EFC::List<Item> &files);
    virtual void doListFileDone(EFC::List<Item> &files);

private:
    Interface::Holder m_file;
    EFC::Vector<Item> m_files;
};

}}}

#endif /* LVFS_CORE_QT_DEFAULTNODE_H_ */
