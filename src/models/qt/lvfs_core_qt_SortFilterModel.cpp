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

#include "lvfs_core_qt_SortFilterModel.h"

#include <lvfs-core/models/Qt/INode>
#include <cstring>


namespace LVFS {
namespace Core {
namespace Qt {

SortFilterModel::SortFilterModel(const Interface::Holder &node, QObject *parent) :
    QSortFilterProxyModel(parent),
    m_node(node)
{}

SortFilterModel::~SortFilterModel()
{}

#include <QtCore/QChar>
#define nat_char QChar
#define nat_isdigit(a) (a).isDigit()
#define nat_isspace(a) (a).isSpace()
#define nat_toupper(a) (a).toUpper()
#include <lvfs-core/tools/strings/strnatcmp.h>

bool SortFilterModel::compareFileNames(const QString &str1, const QString &str2)
{
    return strnatcasecmp(str1.data(), str2.data()) < 0;
}


#undef nat_char
#undef nat_isdigit
#undef nat_isspace
#undef nat_toupper
#include <lvfs-core/tools/strings/strnatcmp.h>

bool SortFilterModel::compareFileNames(const char *str1, const char *str2)
{
    return strnatcasecmp(str1, str2) < 0;
}

bool SortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return m_node->as<Qt::INode>()->compareItems(left, right, sortOrder());
}

}}}
