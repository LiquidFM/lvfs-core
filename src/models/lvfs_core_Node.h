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

#ifndef LVFS_CORE_NODE_H_
#define LVFS_CORE_NODE_H_

#include <efc/Map>
#include <efc/List>
#include <lvfs-core/INode>


namespace LVFS {
namespace Core {

/**
 * Implementation of links counting for Node objects
 */
class PLATFORM_MAKE_PUBLIC Node : public Implements<Core::INode>
{
public:
    Node(const Interface::Holder &file, const Interface::Holder &parent);
    virtual ~Node();

public: /* Core::INode */
    virtual const Interface::Holder &parent() const;
    virtual const Interface::Holder &file() const;

    virtual void cancel(const Files &files);

    virtual int refs() const;
    virtual void incRef();
    virtual int decRef();

protected:
    typedef EFC::Map<EFC::Task *, Files>             Tasks;
    typedef EFC::Map<Interface::Holder, EFC::Task *> Items;

    void handleTask(EFC::Task::Holder &task, const Interface::Holder &file);
    void handleTask(EFC::Task::Holder &task, const Files &files);
    void cancelTask(const Interface::Holder &file);
    void doneTask(EFC::Task *task);
    void cancelTasks();

private:
    int m_ref;
    Tasks m_tasks;
    Items m_items;
    Interface::Holder m_file;
    Interface::Holder m_parent;
};

}}

#endif /* LVFS_CORE_NODE_H_ */
