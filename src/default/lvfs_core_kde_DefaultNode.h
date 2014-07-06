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

#ifndef LVFS_CORE_KDE_DEFAULTNODE_H_
#define LVFS_CORE_KDE_DEFAULTNODE_H_

#include <efc/Vector>
#include <lvfs-core/INode>
#include <lvfs-core/models/Kde/INode>
#include <kitemviews/kstandarditemmodel.h>


namespace LVFS {
namespace Core {
namespace Kde {

class PLATFORM_MAKE_PRIVATE DefaultNode : public Implements<INode, Kde::INode>, public KStandardItemModel
{
    typedef int               size_type;

public:
    DefaultNode(const Interface::Holder &file);
    virtual ~DefaultNode();

    virtual void refresh();
    virtual const IEntry *entry() const;

    virtual size_type size() const;
    virtual Interface::Holder at(size_type index) const;
    virtual size_type indexOf(const Interface::Holder &node) const;

    virtual KItemModelBase *model() const;

    /* KItemModelBase */
    virtual bool setExpanded(int index, bool expanded);
    virtual bool isExpanded(int index) const;
    virtual bool isExpandable(int index) const;
    virtual int expandedParentsCount(int index) const;

protected:
//    virtual QVariant data(int column, int role) const;

protected:
//    virtual void processListFile(EFC::List<Node::Holder> &files);
//    virtual void doListFileDone(EFC::List<Node::Holder> &files);

private:
    Interface::Holder m_file;
    EFC::Vector<Interface::Holder> m_files;
};

}}}

#endif /* LVFS_CORE_KDE_DEFAULTNODE_H_ */
