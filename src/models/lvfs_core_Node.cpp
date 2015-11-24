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

#include "lvfs_core_Node.h"

#include <brolly/assert.h>


namespace LVFS {
namespace Core {

Node::Node(const Interface::Holder &file, const Interface::Holder &parent) :
    m_ref(0),
    m_file(file),
    m_parent(parent)
{
    ASSERT(m_file.isValid());
}

Node::~Node()
{
    ASSERT(m_tasks.empty());
}

const Interface::Holder &Node::parent() const
{
    return m_parent;
}

const Interface::Holder &Node::file() const
{
    return m_file;
}

int Node::refs() const
{
    return m_ref;
}

void Node::incRef()
{
    ++m_ref;
}

int Node::decRef()
{
    return --m_ref;
}

void Node::handleTask(EFC::Task::Holder &task, const Interface::Holder &file)
{
    TaskId taskId = task.get();

    if (Core::INode::handleTask(task))
    {
        m_tasks[taskId].push_back(file);
        m_items[file] = taskId;

        for (Interface::Holder n = Interface::self(); n.isValid(); n = n->as<Core::INode>()->parent())
            n->as<Core::INode>()->incRef();
    }
}

void Node::handleTask(EFC::Task::Holder &task, const Files &files)
{
    TaskId taskId = task.get();

    if (Core::INode::handleTask(task))
    {
        m_tasks[taskId] = files;

        for (auto &i : files)
            m_items[i] = taskId;

        for (Interface::Holder n = Interface::self(); n.isValid(); n = n->as<Core::INode>()->parent())
            n->as<Core::INode>()->incRef();
    }
}

void Node::cancelTask(const Interface::Holder &file)
{
    auto i = m_items.find(file);

    if (i != m_items.end())
        Core::INode::cancelTask(i->second, false);
}

void Node::doneTask(TaskId task)
{
    auto i = m_tasks.find(task);

    if (i != m_tasks.end())
    {
        for (auto &q : i->second)
            m_items.erase(q);

        m_tasks.erase(i);

        for (Interface::Holder n = Interface::self(); n.isValid(); n = n->as<Core::INode>()->parent())
            if (n->as<Core::INode>()->decRef() == 0)
                n->as<Core::INode>()->clear();
    }
}

void Node::cancelTasks(const Files &files)
{
    for (auto &i : files)
        cancelTask(i);
}

void Node::cancelTasks()
{
    m_items.clear();

    for (auto i = m_tasks.begin(); i != m_tasks.end(); i = m_tasks.erase(i))
    {
        Core::INode::cancelTask(i->first, true);

        for (Interface::Holder n = Interface::self(); n.isValid(); n = n->as<Core::INode>()->parent())
            if (n->as<Core::INode>()->decRef() == 0)
                n->as<Core::INode>()->clear();
    }
}

}}
