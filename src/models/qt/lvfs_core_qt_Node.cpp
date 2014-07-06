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

#include "lvfs_core_qt_Node.h"
#include "../../default/lvfs_core_qt_DefaultNode.h"

#include <lvfs-core/INodeFactory>
#include <lvfs/IDirectory>
#include <efc/TasksPool>
#include <brolly/assert.h>


namespace LVFS {
namespace Core {
namespace Qt {

static ::EFC::TasksPool s_pool(10);


Node::Node(const Holder &parent) :
    m_parent(parent)
{}

Node::~Node()
{}

const Node::Item &Node::parent() const
{
    return m_parent;
}

void Node::doListFile(const Item &file, int depth)
{
    ASSERT(depth >= 0);

    Item node;
    EFC::List<Item> nodes;

    if (IDirectory *dir = file->as<IDirectory>())
        for (auto i : (*dir))
        {
            i = Module::open(i);

            if (INodeFactory *factory = i->as<INodeFactory>())
                node.reset(factory->createNode(i));

            if (!node.isValid())
                node.reset(new (std::nothrow) DefaultNode(i, Item(this)));

            if (node.isValid())
            {
                if (depth > 0)
                    node->as<Core::INode>()->refresh(depth - 1);

                nodes.push_back(std::move(node));
            }
        }

    doListFileDone(nodes);
}

void Node::handleTask(EFC::Task::Holder &task)
{

}

QString Node::toUnicode(const char *string)
{
    return QString::fromLocal8Bit(string);
}

}}}
