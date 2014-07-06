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


namespace LVFS {
namespace Core {

INode::~INode()
{}

Interface::Holder INode::open(const char *uri, Module::Error &error)
{
    Interface::Holder file = Module::open(uri, error);
    Interface::Holder res;

    if (file.isValid())
    {
        if (INodeFactory *factory = file->as<INodeFactory>())
            res = factory->createNode(file);

        if (!res.isValid())
            res.reset(new (std::nothrow) Qt::DefaultNode(file));
    }

    return res;
}

Interface::Holder INode::view(const Interface::Holder &node)
{
    ASSERT(node.isValid());
    Interface::Holder res;

    if (IViewFactory *factory = node->as<IViewFactory>())
        res = factory->createView(node);

    if (!res.isValid())
        res.reset(new (std::nothrow) Qt::DefaultView());

    if (LIKELY(res.isValid() == true))
    {
        res->as<IView>()->setNode(node);
    }

    return res;
}

}}
