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

#include "lvfs_core_Directory.h"
#include "lvfs_core_Entry.h"
#include "lvfs_core_File.h"

#include "../default/lvfs_core_qt_DefaultNode.h"
#include "../default/lvfs_core_qt_DefaultView.h"

#include <lvfs/Module>
#include <brolly/assert.h>

#include <cstring>
#include <cstdlib>

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


namespace {
using namespace LVFS;
using namespace LVFS::Core;

class Iterator : public Directory::const_iterator
{
public:
    Iterator() :
        const_iterator(new (std::nothrow) Imp())
    {}

    Iterator(const char *path, Error &lastError) :
        const_iterator(new (std::nothrow) Imp(path, lastError))
    {}

protected:
    class Imp : public const_iterator::Implementation
    {
        PLATFORM_MAKE_NONCOPYABLE(Imp)
        PLATFORM_MAKE_NONMOVEABLE(Imp)

    public:
        Imp() :
            m_dir(NULL),
            m_lastError(NULL),
            m_entry(NULL)
        {
            ::memset(&m_path, 0, sizeof(m_path));
        }

        Imp(const char *path, Error &lastError) :
            m_dir(NULL),
            m_lastError(&lastError),
            m_entry(NULL)
        {
            ::strncpy(m_path, path, sizeof(m_path));

            if (m_dir = ::opendir(m_path))
            {
                if (::strcmp(m_path, "/") == 0)
                    m_path[0] = 0;

                next();
            }
            else
                (*m_lastError) = errno;
        }

        virtual ~Imp()
        {
            ::closedir(m_dir);
        }

        virtual bool isEqual(const Holder &other) const
        {
            return m_entry == other.as<Imp>()->m_entry;
        }

        virtual reference asReference() const
        {
            return m_file;
        }

        virtual pointer asPointer() const
        {
            return &m_file;
        }

        virtual void next()
        {
            Module::Error error;
            Interface::Holder file;
            char path[Module::MaxUriLength];

            m_file.reset();

            while (m_entry = ::readdir(m_dir))
                if (::strcmp(m_entry->d_name, ".") == 0 || ::strcmp(m_entry->d_name, "..") == 0)
                    continue;
                else
                {
                    if (UNLIKELY(std::snprintf(path, sizeof(path), "%s/%s", m_path, m_entry->d_name) < 0))
                    {
                        m_entry = NULL;
                        return;
                    }

                    file = Entry::open(path, error);

                    if (file.isValid())
                        m_file = Module::open(file);

                    if (!m_file.isValid())
                        m_file = file;

                    break;
                }

            if (!m_file.isValid())
                m_entry = NULL;
        }

    private:
        DIR *m_dir;
        Error *m_lastError;
        struct dirent *m_entry;
        Interface::Holder m_file;
        char m_path[Module::MaxUriLength];
    };
};

}

namespace LVFS {
namespace Core {

Directory::Directory(const char *fileName, const struct stat &st) :
    m_filePath(::strdup(fileName)),
    m_fileName(::strrchr(m_filePath, '/') + 1),
    m_type(Module::desktop().typeOfDirectory()),
    m_cTime(st.st_ctime),
    m_mTime(st.st_mtime),
    m_aTime(st.st_atime),
    m_permissions(File::translatePermissions(st))
{}

Directory::~Directory()
{
    ::free(m_filePath);
}

const char *Directory::title() const
{
    return m_fileName;
}

const char *Directory::schema() const
{
    return "file";
}

const char *Directory::location() const
{
    return m_filePath;
}

const IType *Directory::type() const
{
    return m_type;
}

Interface::Holder Directory::open(IStream::Mode mode) const
{
    return Interface::Holder();
}

Directory::const_iterator Directory::begin() const
{
    m_lastError = 0;
    return Iterator(m_filePath, m_lastError);
}

Directory::const_iterator Directory::end() const
{
    return Iterator();
}

bool Directory::exists(const char *name) const
{
    ASSERT(name != NULL);
    char path[Module::MaxUriLength];

    if (UNLIKELY(std::snprintf(path, sizeof(path), "%s/%s", m_filePath, name) < 0))
        return false;

    struct stat st;

    if (::lstat(path, &st) != 0)
        return false;

    return true;
}

Interface::Holder Directory::entry(const char *name, const IType *type, bool create)
{
    ASSERT(name != NULL);
    char path[Module::MaxUriLength];

    if (UNLIKELY(std::snprintf(path, sizeof(path), "%s/%s", m_filePath, name) < 0))
        return Interface::Holder();

    if (create)
    {
        if (type == NULL)
            return Interface::Holder();

        if (::strcmp(type->name(), Module::DirectoryTypeName) == 0)
        {
            if (::mkdir(path, S_IRWXU | (S_IRGRP | S_IXGRP) | (S_IROTH | S_IXOTH)) != 0)
            {
                m_lastError = errno;
                return Interface::Holder();
            }
        }
        else
        {
            int dest_file = ::creat(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

            if (dest_file == -1)
            {
                m_lastError = errno;
                return Interface::Holder();
            }

            ::close(dest_file);
        }
    }

    Interface::Holder file = Entry::open(path, m_lastError);

    if (file.isValid())
        file = Module::open(file);

    return file;
}

bool Directory::copy(const Progress &callback, const Interface::Holder &file, bool move)
{
    ASSERT(file.isValid());

    if (::strcmp(file->as<IEntry>()->type()->name(), Module::DirectoryTypeName) == 0)
        return false;

    char path[Module::MaxUriLength];

    if (UNLIKELY(std::snprintf(path, sizeof(path), "%s/%s", m_filePath, file->as<IEntry>()->title()) < 0))
        return false;

    if (move)
    {
        if (::link(file->as<IEntry>()->location(), path) == 0 && ::unlink(file->as<IEntry>()->location()) == 0)
            return true;
        else
            if (errno == EEXIST &&
                ::unlink(path) == 0 &&
                ::link(file->as<IEntry>()->location(), path) == 0 &&
                ::unlink(file->as<IEntry>()->location()) == 0)
                return true;

        m_lastError = errno;
    }

    enum { BufferSize = 65536 };
    char *buffer = new (std::nothrow) char [BufferSize];

    if (UNLIKELY(buffer == NULL))
        return false;

    int dest_file = ::creat(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (dest_file == -1)
    {
        m_lastError = errno;
        delete [] buffer;
        return false;
    }

    Interface::Adaptor<IStream> src_file(file->as<IEntry>()->open());

    if (!src_file.isValid())
    {
        ::close(dest_file);
        ::unlink(path);
        m_lastError = EIO;
        delete [] buffer;
        return false;
    }

    off64_t read;

    while ((read = src_file->read(buffer, BufferSize)) && !callback.aborted)
        if (::write(dest_file, buffer, read) == read)
            callback.function(callback.arg, read);
        else
        {
            ::close(dest_file);
            ::unlink(path);
            m_lastError = EIO;
            delete [] buffer;
            return false;
        }

    ::close(dest_file);
    delete [] buffer;

    if (move)
        if (::unlink(file->as<IEntry>()->location()) == 0)
            m_lastError = 0;
        else
        {
            m_lastError = errno;
            return false;
        }

    return true;
}

bool Directory::rename(const Interface::Holder &file, const char *name)
{
    ASSERT(file.isValid());
    char path[Module::MaxUriLength];

    if (UNLIKELY(std::snprintf(path, sizeof(path), "%s/%s", m_filePath, name) < 0))
        return false;

    if (::rename(file->as<IEntry>()->location(), path) != 0)
    {
        m_lastError = errno;
        return false;
    }

    return true;
}

bool Directory::remove(const Interface::Holder &file)
{
    ASSERT(file.isValid());
    char path[Module::MaxUriLength];

#if PLATFORM_OS(WINDOWS)
    DWORD attr = GetFileAttributesW((const wchar_t*)filePath.utf16());
    if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
        SetFileAttributesW((const wchar_t*)filePath.utf16(), attr &= ~FILE_ATTRIBUTE_READONLY);
#endif

    if (UNLIKELY(std::snprintf(path, sizeof(path), "%s/%s", m_filePath, file->as<IEntry>()->title()) < 0))
        return false;

    struct stat st;

    if (::stat(path, &st) == 0)
        if (S_ISDIR(st.st_mode))
        {
            if (::rmdir(path) == 0)
                return true;
        }
        else
            if (::unlink(path) == 0)
                return true;

    m_lastError = errno;
    return false;
}

const Error &Directory::lastError() const
{
    return m_lastError;
}

off64_t Directory::size() const
{
    return 0;
}

time_t Directory::cTime() const
{
    return m_cTime;
}

time_t Directory::mTime() const
{
    return m_mTime;
}

time_t Directory::aTime() const
{
    return m_aTime;
}

int Directory::permissions() const
{
    return m_permissions;
}

Interface::Holder Directory::createNode(const Interface::Holder &file, const Interface::Holder &parent) const
{
    return Interface::Holder(new (std::nothrow) Qt::DefaultNode(file, parent));
}

Interface::Holder Directory::createView() const
{
    return Interface::Holder(new (std::nothrow) Qt::DefaultView());
}

}}
