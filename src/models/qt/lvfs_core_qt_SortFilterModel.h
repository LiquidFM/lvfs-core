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

#ifndef LVFS_CORE_QT_SORTFILTERMODEL_H_
#define LVFS_CORE_QT_SORTFILTERMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include <platform/utils.h>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PUBLIC SortFilterModel : public QSortFilterProxyModel
{
    PLATFORM_MAKE_NONCOPYABLE(SortFilterModel)

public:
    SortFilterModel(QObject *parent = 0);
    virtual ~SortFilterModel();

    static bool compareFileNames(const QString &str1, const QString &str2);
    static bool compareFileNames(const char *str1, const char *str2);
};

}}}

#endif /* LVFS_CORE_QT_SORTFILTERMODEL_H_ */
