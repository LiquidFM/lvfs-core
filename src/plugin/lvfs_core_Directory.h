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

#ifndef LVFS_CORE_DIRECTORY_H_
#define LVFS_CORE_DIRECTORY_H_

#include <lvfs/IStream>
#include <lvfs/IDirectory>
#include <lvfs/IProperties>
#include <lvfs-core/INodeFactory>
#include <lvfs-core/IViewFactory>


struct stat;


namespace LVFS {
namespace Core {

class PLATFORM_MAKE_PRIVATE Directory : public Implements<IEntry, IDirectory, IProperties, INodeFactory, IViewFactory>
{
public:
    Directory(const char *fileName, const struct stat &st);
    virtual ~Directory();

public: /* IEntry */
    virtual const char *title() const;
    virtual const char *schema() const;
    virtual const char *location() const;
    virtual const IType *type() const;
    virtual Interface::Holder open(IStream::Mode mode) const;

public: /* IDirectory */
    virtual const_iterator begin() const;
    virtual const_iterator end() const;

    virtual bool exists(const char *name) const;
    virtual Interface::Holder entry(const char *name, const IType *type = NULL, bool create = false);

    virtual bool copy(const Progress &callback, const Interface::Holder &file, bool move = false);
    virtual bool rename(const Interface::Holder &file, const char *name);
    virtual bool remove(const Interface::Holder &file);

    virtual const Error &lastError() const;

public: /* IProperties */
    virtual off64_t size() const;
    virtual time_t cTime() const;
    virtual time_t mTime() const;
    virtual time_t aTime() const;
    virtual int permissions() const;

public: /* INodeFactory */
    virtual Interface::Holder createNode(const Interface::Holder &file, const Interface::Holder &parent) const;

public: /* IViewFactory */
    virtual Interface::Holder createView() const;

private:
    char *m_filePath;
    const char *m_fileName;
    Interface::Adaptor<IType> m_type;

private:
    time_t m_cTime;
    time_t m_mTime;
    time_t m_aTime;
    int m_permissions;
    mutable Error m_lastError;
};

}}

#endif /* LVFS_CORE_DIRECTORY_H_ */
