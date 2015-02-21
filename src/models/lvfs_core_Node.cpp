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

#include <efc/TasksPool>
#include <brolly/assert.h>


namespace LVFS {
namespace Core {

static EFC::TasksPool tasksPool(10);

Node::Node(const Interface::Holder &file, const Interface::Holder &parent) :
    m_ref(0),
    m_file(file),
    m_parent(parent)
{
    ASSERT(m_file.isValid());
}

Node::~Node()
{
    /* All tasks should be canceled before derived class destroyed */
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

void Node::cancel(const Files &files)
{
    for (auto &i : files)
        cancelTask(i);
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
    m_tasks[task.get()].push_back(file);
    m_items[file] = task.get();
    tasksPool.handle(task);
}

void Node::handleTask(EFC::Task::Holder &task, const Files &files)
{
    m_tasks[task.get()] = files;

    for (auto &i : files)
        m_items[i] = task.get();

    tasksPool.handle(task);
}

void Node::cancelTask(const Interface::Holder &file)
{
    auto i = m_items.find(file);

    if (i != m_items.end())
        tasksPool.cancel(i->second, false);
}

void Node::doneTask(EFC::Task *task)
{
    auto i = m_tasks.find(task);

    if (i != m_tasks.end())
    {
        for (auto &q : i->second)
            m_items.erase(q);

        m_tasks.erase(i);
    }
}

void Node::cancelTasks()
{
    m_items.clear();

    for (auto i = m_tasks.begin(); i != m_tasks.end(); i = m_tasks.erase(i))
        tasksPool.cancel(i->first, true);
}

}}
