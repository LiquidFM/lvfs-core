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

#ifndef LVFS_CORE_SETTINGS_LISTENER_H_
#define LVFS_CORE_SETTINGS_LISTENER_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <lvfs-core/settings/Option>


class PLATFORM_MAKE_PRIVATE Listener : public QObject
{
    Q_OBJECT

public:
    typedef void (*IntValueChanged)(const LVFS::Core::Settings::Option *, int);
    typedef void (*StringValueChanged)(const LVFS::Core::Settings::Option *, const QString &);

public:
    Listener(const LVFS::Core::Settings::Option *option, IntValueChanged func, QObject *parent = 0) :
        QObject(parent),
        m_option(option)
    {
        m_func.i = func;
    }
    Listener(const LVFS::Core::Settings::Option *option, StringValueChanged func, QObject *parent = 0) :
        QObject(parent),
        m_option(option)
    {
        m_func.s = func;
    }
    virtual ~Listener();

private Q_SLOTS:
    void valueChanged(int value);
    void valueChanged(const QString &value);

private:
    const LVFS::Core::Settings::Option *m_option;
    union ValueChanged
    {
        IntValueChanged i;
        StringValueChanged s;
    } m_func;
};

#endif /* LVFS_CORE_SETTINGS_LISTENER_H_ */
