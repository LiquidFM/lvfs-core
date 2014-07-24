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

#include "lvfs_core_INode.h"
#include "default/lvfs_core_qt_DefaultNode.h"
#include "default/lvfs_core_qt_DefaultView.h"

#include <lvfs-core/INodeFactory>
#include <lvfs-core/IViewFactory>

#include <brolly/assert.h>
#include <cstring>
#include <cstdio>

#include <linux/limits.h>


namespace LVFS {
namespace Core {

INode::~INode()
{}

Interface::Holder INode::open(const char *uri, Module::Error &error)
{
    char buffer[PATH_MAX + Module::MaxSchemaLength + Module::SchemaDelimiterLength];
    Interface::Holder parent;
    Interface::Holder current;
    char *p;

    buffer[sizeof(buffer) - 1] = 0;
    strncpy(buffer, uri, sizeof(buffer) - 1);

    if (p = strstr(buffer, Module::SchemaDelimiter))
        ++p;
    else
        p = buffer;

    if (p[0] == '/' && p - buffer < Module::MaxSchemaLength)
    {
        INodeFactory *factory;
        char *d;
        char c = p[1];
        p[1] = 0;

        Interface::Holder file = Module::open(buffer, error);

        (*++p) = c;

        while (file.isValid())
        {
            parent = current;
            current.reset();

            if (factory = file->as<INodeFactory>())
                current = factory->createNode(file, parent);

            if (!current.isValid())
                current.reset(new (std::nothrow) Qt::DefaultNode(file, parent));

            if (!p)
                break;

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
                *(p = d)++ = '/';
            }
            else
            {
                file = Module::open(buffer, error);
                p = NULL;
            }
        }
    }

    return current;
}

Interface::Holder INode::view(const Interface::Holder &node)
{
    ASSERT(node.isValid());
    Interface::Holder res;

    if (IViewFactory *factory = node->as<Core::INode>()->file()->as<IViewFactory>())
        res = factory->createView();

    if (!res.isValid())
        res.reset(new (std::nothrow) Qt::DefaultView());

    return res;
}

}}
