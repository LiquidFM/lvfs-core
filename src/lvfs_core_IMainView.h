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

#ifndef LVFS_CORE_IMAINVIEW_H_
#define LVFS_CORE_IMAINVIEW_H_

#include <lvfs/Interface>


namespace LVFS {
namespace Core {

class PLATFORM_MAKE_PUBLIC IMainView
{
    DECLARE_INTERFACE(LVFS::Core::IMainView)

public:
    virtual ~IMainView();

    virtual const Interface::Holder &opposite(const Interface::Holder &view) const = 0;
    virtual void show(const Interface::Holder &view, const Interface::Holder &node) = 0;
};

}}

#endif /* LVFS_CORE_IMAINVIEW_H_ */
