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

#include "lvfs_core_qt_DefaultSortFilterModel.h"
#include "lvfs_core_qt_DefaultNode.h"


namespace LVFS {
namespace Core {
namespace Qt {

DefaultSortFilterModel::DefaultSortFilterModel(QObject *parent) :
    SortFilterModel(parent)
{}

DefaultSortFilterModel::~DefaultSortFilterModel()
{}

bool DefaultSortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    DefaultNode::Item *leftItem = static_cast<DefaultNode::Item *>(left.internalPointer());
    DefaultNode::Item *rightItem = static_cast<DefaultNode::Item *>(right.internalPointer());

    if (sortOrder() == ::Qt::AscendingOrder)
        if (leftItem->isDir)
            if (rightItem->isDir)
                return compareFileNames(leftItem->title, rightItem->title);
            else
                return true;
        else
            if (rightItem->isDir)
                return false;
            else
                return compareFileNames(leftItem->title, rightItem->title);
    else
        if (leftItem->isDir)
            if (rightItem->isDir)
                return !compareFileNames(leftItem->title, rightItem->title);
            else
                return false;
        else
            if (rightItem->isDir)
                return true;
            else
                return !compareFileNames(leftItem->title, rightItem->title);

    return true;
}

}}}
