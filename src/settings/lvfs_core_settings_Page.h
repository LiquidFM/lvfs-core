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

#ifndef LVFS_CORE_SETTINGS_PAGE_H_
#define LVFS_CORE_SETTINGS_PAGE_H_

#include <lvfs-core/settings/Scope>
#include <QtCore/QString>


namespace LVFS {
namespace Core {
namespace Settings {

class PLATFORM_MAKE_PUBLIC Page : public Scope
{
    PLATFORM_MAKE_NONCOPYABLE(Page)
    PLATFORM_MAKE_NONMOVEABLE(Page)

public:
    Page(const QString &title, Scope *option, Option *parent = 0) :
        Scope(parent),
        m_title(title),
        m_option(option)
    {}
    virtual ~Page();

    virtual QWidget *createEditor(QWidget *parent = 0) const;

    const Container &subpages() const { return m_container; }
    void manageSubpage(Page *page) { m_container.push_back(page); }

private:
    QString m_title;
    Scope *m_option;
};

}}}

#endif /* LVFS_CORE_SETTINGS_PAGE_H_ */
