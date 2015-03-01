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

#ifndef LVFS_CORE_ENTRY_H_
#define LVFS_CORE_ENTRY_H_

#include <lvfs/IEntry>
#include <lvfs/IProperties>


struct stat;


namespace LVFS {
namespace Core {

class PLATFORM_MAKE_PRIVATE Entry : public Implements<IEntry, IProperties>
{
public:
    Entry(const char *fileName, const struct stat &st);
    virtual ~Entry();

public: /* IEntry */
    virtual const char *title() const;
    virtual const char *schema() const;
    virtual const char *location() const;
    virtual const IType *type() const;
    virtual Interface::Holder open(IFile::Mode mode) const;

public: /* IProperties */
    virtual off64_t size() const;
    virtual time_t cTime() const;
    virtual time_t mTime() const;
    virtual time_t aTime() const;
    virtual int permissions() const;

public:
    static Interface::Holder open(const char *uri, Error &error);

private:
    char *m_filePath;
    const char *m_fileName;
    Interface::Adaptor<IType> m_type;

private:
    off64_t m_size;
    time_t m_cTime;
    time_t m_mTime;
    time_t m_aTime;
    int m_permissions;
};

}}

#endif /* LVFS_CORE_ENTRY_H_ */
