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

#include "lvfs_core_TreeModel.h"


namespace LVFS {
namespace Core {
namespace Tools {

TreeModel::TreeModel()
{}

TreeModel::~TreeModel()
{}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<Item *>(parent.internalPointer())->size();
	else
    	return size();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<Item *>(index.internalPointer())->data(index.column(), role);
    else
    	return at(index.row())->data(index.column(), role);
}

::Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	return ::Qt::ItemIsSelectable | ::Qt::ItemIsEnabled;
}

QVariant TreeModel::headerData(int section, ::Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid())
			return createIndex(row, column, static_cast<Item *>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, at(row));
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (Item *parent = static_cast<Item *>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<Item *>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(indexOf(parent), 0, parent);

    return QModelIndex();
}

TreeModel::Item::Item(Item *parent) :
    m_parent(parent)
{}

TreeModel::Item::~Item()
{}

TreeModel::Item *TreeModel::Item::at(size_type index) const
{
    return NULL;
}

TreeModel::Item::size_type TreeModel::Item::size() const
{
    return 0;
}

TreeModel::Item::size_type TreeModel::Item::indexOf(Item *item) const
{
    return InvalidIndex;
}

QModelIndex TreeModel::index(Item *item) const
{
	if (Item *parent = item->parent())
		return createIndex(static_cast<Item *>(parent)->indexOf(item), 0, item);
	else
		return createIndex(indexOf(item), 0, item);
}

QModelIndex TreeModel::parent(Item *item) const
{
	if (Item *parent = item->parent())
		if (Item *parentParent = parent->parent())
			return createIndex(static_cast<Item *>(parentParent)->indexOf(parent), 0, parent);
		else
			return createIndex(indexOf(parent), 0, parent);
	else
		return QModelIndex();
}

QModelIndex TreeModel::parent(Item *item, size_type &row) const
{
	if (Item *parent = item->parent())
		if (Item *parentParent = parent->parent())
		{
			row = static_cast<Item *>(parent)->indexOf(item);
			return createIndex(static_cast<Item *>(parentParent)->indexOf(parent), 0, parent);
		}
		else
		{
			row = static_cast<Item *>(parent)->indexOf(item);
			return createIndex(indexOf(parent), 0, parent);
		}
	else
	{
		row = indexOf(item);
		return QModelIndex();
	}
}

}}}
