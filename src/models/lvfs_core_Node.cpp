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
{}

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

void Node::handleTask(EFC::Task::Holder &task)
{
    tasksPool.handle(task);
}

}}
