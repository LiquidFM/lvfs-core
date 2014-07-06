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

#ifndef LVFS_CORE_IVIEW_H_
#define LVFS_CORE_IVIEW_H_

#include <lvfs/IEntry>


class QWidget;


namespace LVFS {
namespace Core {

class PLATFORM_MAKE_PUBLIC IView
{
    DECLARE_INTERFACE(LVFS::Core::IView)

public:
    virtual ~IView();

    virtual QWidget *widget() const = 0;

    virtual const Interface::Holder &node() const = 0;
    virtual bool setNode(const Interface::Holder &node) = 0;

    virtual const Interface::Holder &opposite() const = 0;
};

}}

#endif /* LVFS_CORE_IVIEW_H_ */
