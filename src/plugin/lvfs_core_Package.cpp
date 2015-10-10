/**
 * This file is part of lvfs-db.
 *
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * lvfs-db is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lvfs-db is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lvfs-db. If not, see <http://www.gnu.org/licenses/>.
 */

#include "lvfs_core_Package.h"
#include "lvfs_core_Plugin.h"


namespace LVFS {
namespace Core {

Package::Package()
{}

Package::~Package()
{}

const char *Package::name() const
{
    return "Core";
}

Settings::Scope *Package::settings() const
{
    return NULL;
}

const Package::Plugin **Package::contentPlugins() const
{
    return NULL;
}

const Package::Plugin **Package::protocolPlugins() const
{
    static const Core::Plugin plugin;
    static const Plugin types[] =
    {
        { "file", plugin }
    };
    enum { Count = sizeof(types) / sizeof(Plugin) };

    static const Plugin *res[Count + 1] = {};

    for (unsigned i = 0; i < Count; ++i)
        res[i] = &types[i];

    return res;
}

}}


DECLARE_PLUGIN(::LVFS::Core::Package)
