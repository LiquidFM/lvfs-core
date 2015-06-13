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

#ifndef LVFS_CORE_FILE_H_
#define LVFS_CORE_FILE_H_

#include <lvfs/IStream>
#include <lvfs/IProperties>


struct stat;


namespace LVFS {
namespace Core {

class PLATFORM_MAKE_PRIVATE File : public Implements<IStream, IProperties>
{
public:
    File(const char *fileName, Mode mode);
    virtual ~File();

    bool isValid() const;

public: /* IStream */
    virtual size_t read(void *buffer, size_t size);
    virtual size_t write(const void *buffer, size_t size);
    virtual bool advise(off64_t offset, off64_t len, Advise advise);
    virtual bool seek(off64_t offset, Whence whence = FromCurrent);
    virtual bool flush();

    virtual const Error &lastError() const;

public: /* IProperties */
    virtual off64_t size() const;
    virtual time_t cTime() const;
    virtual time_t mTime() const;
    virtual time_t aTime() const;
    virtual int permissions() const;

public:
    static int translatePermissions(const struct stat &st);

private:
    int m_file;
    off64_t m_size;
    time_t m_cTime;
    time_t m_mTime;
    time_t m_aTime;
    int m_permissions;
    Error m_lastError;
};

}}

#endif /* LVFS_CORE_FILE_H_ */
