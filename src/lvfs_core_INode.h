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

    virtual int refs() const = 0;
    virtual void incRef() = 0;
    virtual int decRef() = 0;
    virtual void clear() = 0;

    virtual Interface::Holder node(const Interface::Holder &file) const = 0;
    virtual void setNode(const Interface::Holder &file, const Interface::Holder &node) = 0;

public:
    static Interface::Holder open(const char *uri, Module::Error &error);
    static void cleanup();
    static void lastCheck();

protected:
    template<typename T>
    struct ClearDeleter
    {
        inline typename T::iterator operator()(T &container, typename T::iterator &iterator) const
        { return container.erase(iterator); }
    };

    template <typename T, typename Adaptor, typename Deleter = ClearDeleter<T>>
    static inline void clear(T &container, Adaptor adaptor, Deleter deleter = Deleter())
    {
        for (typename T::iterator i = container.begin(); i != container.end();)
        {
            const Interface::Holder &item = static_cast<const Interface::Holder &>(adaptor(i));

            if (item.isValid())
            {
                if (item->as<Core::INode>()->refs() == 0)
                {
                    item->as<Core::INode>()->clear();
                    i = deleter(container, i);
                }
                else
                    ++i;
            }
            else
                i = deleter(container, i);
        }
    }

private:
    friend class Node;
    static bool handleTask(EFC::Task::Holder &task);
    static void cancelTask(const EFC::Task *task, bool wait);
};

}}

#endif /* LVFS_CORE_INODE_H_ */
