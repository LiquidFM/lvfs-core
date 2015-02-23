/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2015 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#include <efc/List>
#include <efc/Task>
#include <lvfs/Interface>
#include <lvfs/Module>


namespace LVFS {
namespace Core {

class Node;


class PLATFORM_MAKE_PUBLIC INode
{
    DECLARE_INTERFACE(LVFS::Core::INode)

public:
    typedef EFC::List<Interface::Holder> Files;

public:
    virtual ~INode();

    virtual const Interface::Holder &parent() const = 0;
    virtual const Interface::Holder &file() const = 0;

    virtual void refresh(int depth = 0) = 0;
    virtual void opened(const Interface::Holder &view) = 0;
    virtual void closed(const Interface::Holder &view) = 0;

    virtual void accept(const Interface::Holder &view, Files &files) = 0;
    virtual void copy(const Interface::Holder &view, const Interface::Holder &dest, Files &files, bool move = false) = 0;
    virtual void cancel(const Files &files) = 0;

    virtual int refs() const = 0;
    virtual void incRef() = 0;
    virtual int decRef() = 0;
    virtual void clear() = 0;

public:
    static Interface::Holder open(const char *uri, Module::Error &error);
    static void cleanup();

protected:
    virtual Interface::Holder node(const Interface::Holder &file) const = 0;
    virtual void setNode(const Interface::Holder &file, const Interface::Holder &node) = 0;

protected:
    template <typename T, typename Adaptor>
    static inline void clear(T &container, Adaptor adaptor)
    {
        for (typename T::iterator i = container.begin(); i != container.end();)
            if (static_cast<Interface::Holder &>(adaptor(i)).isValid())
            {
                if (static_cast<Interface::Holder &>(adaptor(i))->as<Core::INode>()->refs() == 0)
                {
                    static_cast<Interface::Holder &>(adaptor(i))->as<Core::INode>()->clear();
                    i = container.erase(i);
                }
                else
                    ++i;
            }
            else
                i = container.erase(i);
    }

private:
    friend class Node;
    static void handleTask(EFC::Task::Holder &task);
    static void cancelTask(const EFC::Task *task, bool wait);
};

}}

#endif /* LVFS_CORE_INODE_H_ */
