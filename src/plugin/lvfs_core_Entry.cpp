/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2015 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#include "lvfs_core_Entry.h"
#include "lvfs_core_Directory.h"
#include "lvfs_core_File.h"

#include <lvfs/Module>
#include <brolly/assert.h>

#include <cstring>
#include <cstdlib>

#include <linux/limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


namespace LVFS {
namespace Core {

Entry::Entry(const char *fileName, const struct stat &st) :
    m_filePath(::strdup(fileName)),
    m_fileName(::strrchr(m_filePath, '/') + 1),
    m_type(Module::desktop().typeOfFile(this)),
    m_size(st.st_size),
    m_cTime(st.st_ctime),
    m_mTime(st.st_mtime),
    m_aTime(st.st_atime),
    m_permissions(File::translatePermissions(st))
{}

Entry::~Entry()
{
    ::free(m_filePath);
}

const char *Entry::title() const
{
    return m_fileName;
}

const char *Entry::schema() const
{
    return "file";
}

const char *Entry::location() const
{
    return m_filePath;
}

const IType *Entry::type() const
{
    return m_type;
}

Interface::Holder Entry::open(IStream::Mode mode) const
{
    File *file = new (std::nothrow) File(m_filePath, mode);

    if (LIKELY(file != NULL))
        if (file->isValid())
            return Interface::Holder(file);
        else
            delete file;

    return Interface::Holder();
}

off64_t Entry::size() const
{
    return m_size;
}

time_t Entry::cTime() const
{
    return m_cTime;
}

time_t Entry::mTime() const
{
    return m_mTime;
}

time_t Entry::aTime() const
{
    return m_aTime;
}

int Entry::permissions() const
{
    return m_permissions;
}

Interface::Holder Entry::open(const char *uri, Error &error)
{
    ASSERT(uri != NULL);
    char file[Module::MaxUriLength];

    if (uri[0] == '/')
        ::strncpy(file, uri, sizeof(file));
    else
        if (::getcwd(file, sizeof(file)))
        {
            if (std::snprintf(file + ::strlen(file), sizeof(file) - ::strlen(file) - 1, "/%s", uri) < 0)
            {
                error = errno;
                return Interface::Holder();
            }
        }
        else
            ::strncpy(file, uri, sizeof(file));

    struct stat st;

    if (::lstat(file, &st) != 0)
        error = errno;
    else
        if (S_ISDIR(st.st_mode))
            return Interface::Holder(new (std::nothrow) Directory(file, st));
        else
            return Interface::Holder(new (std::nothrow) Entry(file, st));

    return Interface::Holder();
}

}}
