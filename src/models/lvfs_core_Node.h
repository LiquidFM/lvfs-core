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

#include <platform/utils.h>
#include <efc/Set>
#include <efc/Task>
#include <lvfs/Interface>


namespace LVFS {
namespace Core {

/**
 * Implementation of links counting for Node objects
 */
class PLATFORM_MAKE_PUBLIC Node
{
public:
    inline Node(const Interface::Holder &parent) :
        m_links(0),
        m_parent(parent)
    {}
    virtual ~Node();

    inline const Interface::Holder &parent() const { return m_parent; }
    inline void opened(const Interface::Holder &view) { m_views.insert(view); ++m_links; }
    inline void closed(const Interface::Holder &view) { m_views.erase(view); if (--m_links == 0) removeChildren(); }

protected:
    virtual void removeChildren() = 0;

protected:
    void handleTask(EFC::Task::Holder &task);

protected:
    const EFC::Set<Interface::Holder> &views() const { return m_views; }

private:
    int m_links;
    Interface::Holder m_parent;
    EFC::Set<Interface::Holder> m_views;
};

}}

#endif /* LVFS_CORE_NODE_H_ */
