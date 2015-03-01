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

#include "lvfs_core_INode.h"
#include "default/lvfs_core_qt_DefaultNode.h"
#include "default/lvfs_core_qt_DefaultView.h"

#include <lvfs/IEntry>
#include <lvfs/IDirectory>
#include <lvfs-core/INodeFactory>
#include <lvfs-core/IViewFactory>

#include <efc/Map>
#include <efc/String>
#include <efc/TasksPool>
#include <brolly/assert.h>

#include <cstring>
#include <cstdio>

#include <linux/limits.h>


int DefaultNode_count;
static EFC::TasksPool tasksPool(10);


namespace LVFS {
namespace Core {

typedef EFC::Map<EFC::String, Interface::Holder> Container;

namespace {

struct RootNode
{
    ~RootNode()
    {
        ASSERT(items.empty());
    }

    Container items;
};

static RootNode root;

}


INode::~INode()
{}

Interface::Holder INode::open(const char *uri, Module::Error &error)
{
    char buffer[Module::MaxUriLength];
    Interface::Holder current;
    char *p;

    if (p = const_cast<char *>(strstr(uri, Module::SchemaDelimiter)))
    {
        p += Module::SchemaDelimiterLength;

        if (p - uri >= Module::MaxSchemaLength + Module::SchemaDelimiterLength)
            return current;

        buffer[sizeof(buffer) - 1] = 0;
        strncpy(buffer, uri, sizeof(buffer) - 1);

        p = buffer + (p - uri);
    }
    else
    {
        static const char file[] = "file";

        if (snprintf(buffer, sizeof(buffer), "%s%s%s", file, Module::SchemaDelimiter, uri) >= sizeof(buffer))
            return current;

        p = buffer + sizeof(file) + Module::SchemaDelimiterLength - 1;
    }

    if (p[0] == '/')
    {
        Interface::Holder parent;
        Container::iterator items;
        INodeFactory *factory;
        bool firstTime = true;
        char *d = p;
        char c;

        c = p[1];
        p[1] = 0;

        {
            EFC::String key(buffer);

            items = root.items.lower_bound(key);

            if (items == root.items.end() || (root.items.key_comp()(key, items->first)))
                items = root.items.insert(items, Container::value_type(key, Container::value_type::second_type()));
        }

        Interface::Holder file = Module::open(buffer, error);

        while (file.isValid())
        {
            if (UNLIKELY(!file.isValid()))
                return current;

            parent = current;
            current.reset();

            if (UNLIKELY(firstTime))
            {
                firstTime = false;
                auto &item = items->second;

                p[1] = c;

                if (!item.isValid() && (factory = file->as<INodeFactory>()))
                    item = factory->createNode(file, parent);

                current = item;
            }
            else
            {
                ASSERT(parent.isValid());
                current = parent->as<Core::INode>()->node(file);

                if (!current.isValid() && (factory = file->as<INodeFactory>()))
                {
                    current = factory->createNode(file, parent);
                    parent->as<Core::INode>()->setNode(file, current);
                }
            }

            if (!current.isValid())
                return parent;

            if (!p)
                break;
            else
                *(p = d)++ = '/';

            if (d = strchr(p, '/'))
            {
                while (d == p)
                {
                    size_t len = strlen(p + 1);
                    memcpy(p, p + 1, len);
                    p[len] = 0;

                    if ((d = strchr(p, '/')) == NULL)
                        return current;
                }

                d[0] = 0;

                while (strcmp(p, ".") == 0)
                {
                    size_t len = strlen(++d);
                    memcpy(p, d, len);
                    p[len] = 0;

                    if ((d = strchr(p, '/')) == NULL)
                        return current;

                    d[0] = 0;
                }

                while (strcmp(p, "..") == 0)
                {
                    if (parent.isValid())
                    {
                        current = parent;
                        parent = parent->as<Core::INode>()->parent();
                    }
                    else
                        return current;

                    *(p - 1) = 0;
                    size_t len = strlen(++d);
                    memcpy(p = strrchr(buffer, '/') + 1, d, len);
                    p[len] = 0;

                    if ((d = strchr(p, '/')) == NULL)
                        return current;

                    d[0] = 0;
                }

                file = Module::open(buffer, error);
            }
            else
            {
                if (*p)
                    file = Module::open(buffer, error);
                else
                    return current;

                p = NULL;
            }
        }
    }

    return current;
}

void INode::cleanup()
{
    tasksPool.terminate();

    for (Container::iterator i = root.items.begin(); i != root.items.end(); i = root.items.erase(i))
        i->second->as<Core::INode>()->clear();
}

void INode::lastCheck()
{
    printf("!!!!!!!!!!!!!!There is %d nodes\n", DefaultNode_count);
}

bool INode::handleTask(EFC::Task::Holder &task)
{
    return tasksPool.handle(task);
}

void INode::cancelTask(const EFC::Task *task, bool wait)
{
    tasksPool.cancel(task, wait);
}

}}
