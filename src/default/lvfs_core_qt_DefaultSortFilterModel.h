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

#ifndef WORKSPACE_LVFS_CORE_SRC_DEFAULT_LVFS_CORE_QT_DEFAULTSORTFILTERMODEL_H_
#define WORKSPACE_LVFS_CORE_SRC_DEFAULT_LVFS_CORE_QT_DEFAULTSORTFILTERMODEL_H_

#include <lvfs-core/models/Qt/SortFilterModel>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE DefaultSortFilterModel : public SortFilterModel
{
    PLATFORM_MAKE_NONCOPYABLE(DefaultSortFilterModel)

public:
    DefaultSortFilterModel(QObject *parent = 0);
    virtual ~DefaultSortFilterModel();

protected:
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

}}}

#endif /* WORKSPACE_LVFS_CORE_SRC_DEFAULT_LVFS_CORE_QT_DEFAULTSORTFILTERMODEL_H_ */
