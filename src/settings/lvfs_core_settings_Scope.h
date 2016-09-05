/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2016 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#ifndef LVFS_CORE_SETTINGS_SCOPE_H_
#define LVFS_CORE_SETTINGS_SCOPE_H_

#include <efc/List>
#include <lvfs-core/settings/Option>


namespace LVFS {
namespace Core {
namespace Settings {

class PLATFORM_MAKE_PUBLIC Scope : public Option
{
    PLATFORM_MAKE_NONCOPYABLE(Scope)
    PLATFORM_MAKE_NONMOVEABLE(Scope)

public:
    typedef EFC::List<Option *> Container;

public:
    Scope(Option *parent = 0) :
        Option(parent)
    {}
    virtual ~Scope();

protected:
    Container m_container;
};

}}}

#endif /* LVFS_CORE_SETTINGS_SCOPE_H_ */
