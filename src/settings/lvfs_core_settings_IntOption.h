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

#ifndef LVFS_CORE_SETTINGS_INTOPTION_H_
#define LVFS_CORE_SETTINGS_INTOPTION_H_

#include <lvfs/settings/IntOption>
#include <lvfs-core/settings/Option>


namespace LVFS {
namespace Core {
namespace Settings {

class PLATFORM_MAKE_PUBLIC IntOption : public Option
{
    PLATFORM_MAKE_NONCOPYABLE(IntOption)
    PLATFORM_MAKE_NONMOVEABLE(IntOption)

public:
    IntOption(LVFS::Settings::IntOption *option, Option *parent = 0) :
        Option(option, parent)
    {}
    virtual ~IntOption();

    virtual QWidget *createEditor(QWidget *parent = 0) const;

private:
    static void valueChanged(const Option *self, int value);
};

}}}

#endif /* LVFS_CORE_SETTINGS_INTOPTION_H_ */
