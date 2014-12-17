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

#ifndef WORKSPACE_LVFS_CORE_SRC_TOOLS_TEMPLATES_URI_H_
#define WORKSPACE_LVFS_CORE_SRC_TOOLS_TEMPLATES_URI_H_

#include <efc/List>
#include <stdint.h>


namespace LVFS {
namespace Tools {
namespace Templates {

class PLATFORM_MAKE_PUBLIC Uri
{
    PLATFORM_MAKE_NONCOPYABLE(Uri)
    PLATFORM_MAKE_NONMOVEABLE(Uri)

public:
    Uri(const char *uri);
    ~Uri();

    bool isValid() const { return m_valid; }

//    const QString &schema() const { return m_schema; }
//    const QString &userName() const { return m_userName; }
//    const QString &password() const { return m_password; }
//    ushort port() const { return m_port; }
//    const QString &domain() const { return m_domain; }
//
//    iterator begin() const { return iterator(m_path); }
//    iterator erase(const iterator &iterator) { m_path.removeAt(iterator.m_pos); return iterator; }
//    QString toString() const { return m_path.join(QChar(L'/')); }

private:
    void doNothing(char *string, int size);
    void schema(char *string, int size);
    void userName(char *string, int size);
    void password(char *string, int size);
    void domain(char *string, int size);
    void port(char *string, int size);
    void path(char *string, int size);
    void userNameIsDomain_thisIsPort(char *string, int size);

private:
    bool m_valid;
    char *m_buffer;
    const char *m_schema;
    const char *m_userName;
    const char *m_password;
    uint16_t m_port;
    const char *m_domain;
    EFC::List<const char *> m_path;
};

}}}

#endif /* WORKSPACE_LVFS_CORE_SRC_TOOLS_TEMPLATES_URI_H_ */
