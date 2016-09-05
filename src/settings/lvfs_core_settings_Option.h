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

#ifndef LVFS_CORE_SETTINGS_OPTION_H_
#define LVFS_CORE_SETTINGS_OPTION_H_

#include <cstddef>
#include <lvfs/settings/Option>


class QWidget;


namespace LVFS {
namespace Core {
namespace Settings {

class PLATFORM_MAKE_PUBLIC Option
{
    PLATFORM_MAKE_NONCOPYABLE(Option)
    PLATFORM_MAKE_NONMOVEABLE(Option)

public:
    Option(Option *parent = 0) :
        m_parent(parent),
        m_option(NULL)
    {}
    Option(LVFS::Settings::Option *option, Option *parent = 0) :
        m_parent(parent),
        m_option(option)
    {}
    virtual ~Option();

    Option *parent() const { return m_parent; }

    virtual QWidget *createEditor(QWidget *parent = 0) const = 0;

    template <typename R> inline
    R *as() { return static_cast<R *>(this); }

    template <typename R> inline
    const R *as() const { return static_cast<const R *>(this); }

protected:
    static LVFS::Settings::Option *option(const Option *option)
    { return option->m_option; }

    LVFS::Settings::Option *option() const { return m_option; }

private:
    Option *m_parent;
    LVFS::Settings::Option *m_option;
};

}}}

#endif /* LVFS_CORE_SETTINGS_OPTION_H_ */
