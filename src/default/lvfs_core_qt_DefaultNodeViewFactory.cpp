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

#include "lvfs_core_qt_DefaultNodeViewFactory.h"
#include "lvfs_core_qt_DefaultNode.h"
#include "lvfs_core_qt_DefaultView.h"


namespace LVFS {
namespace Core {
namespace Qt {

DefaultNodeViewFactory::DefaultNodeViewFactory(const Interface::Holder &original) :
    ExtendsBy<Core::INodeFactory, Core::IViewFactory>(original)
{}

DefaultNodeViewFactory::~DefaultNodeViewFactory()
{}

Interface::Holder DefaultNodeViewFactory::createNode(const Interface::Holder &file, const Interface::Holder &parent) const
{
    return Interface::Holder(new (std::nothrow) DefaultNode(file, parent));
}

Interface::Holder DefaultNodeViewFactory::createView() const
{
    return Interface::Holder(new (std::nothrow) DefaultView());
}

}}}
