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

#include "lvfs_core_qt_ModelNode.h"

#include <brolly/assert.h>
#include <QtGui/QIcon>


namespace LVFS {
namespace Core {
namespace Qt {

ModelNode::ModelNode(const Item &parent) :
    Node(parent),
    QAbstractItemModel()
{}

ModelNode::~ModelNode()
{}

QAbstractItemModel *ModelNode::model() const
{
    return const_cast<ModelNode *>(this);
}

QModelIndex ModelNode::parentIndex() const
{
    return m_index;
}

void ModelNode::setParentIndex(const QModelIndex &index)
{
    m_index = index;
}

int ModelNode::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        if (Qt::INode *qtNode = (*static_cast<Item *>(parent.internalPointer()))->as<Qt::INode>())
            return qtNode->size();
        else
            return 0;
    else
        return size();
}

int ModelNode::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ModelNode::data(const QModelIndex &index, int role) const
{
    Item indexNode(index.isValid() ? *static_cast<Item *>(index.internalPointer()) : at(index.row()));

    if (Qt::INode *qtNode = indexNode->as<Qt::INode>())
        return qtNode->data(index.column(), role);
    else
        if (role == ::Qt::DisplayRole)
        {
            ASSERT(indexNode->as<Core::INode>() != NULL);
            return indexNode->as<Core::INode>()->title();
        }
        else
            if (role == ::Qt::DecorationRole)
            {
                ASSERT(indexNode->as<Core::INode>() != NULL);

                QIcon icon;
                icon.addFile(toUnicode(indexNode->as<Core::INode>()->file()->as<IEntry>()->type()->icon()->as<IEntry>()->location()), QSize(16, 16));
                return icon;
            }

    return QVariant();
}

::Qt::ItemFlags ModelNode::flags(const QModelIndex &index) const
{
    return ::Qt::ItemIsSelectable | ::Qt::ItemIsEnabled;
}

QVariant ModelNode::headerData(int section, ::Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QModelIndex ModelNode::index(int row, int column, const QModelIndex &parent) const
{
    if (hasIndex(row, column, parent))
        if (parent.isValid())
            return createIndex(row, column, const_cast<Item *>(&(*static_cast<Item *>(parent.internalPointer()))->as<Qt::INode>()->at(row)));
        else
            return createIndex(row, column, const_cast<Item *>(&at(row)));
    else
        return QModelIndex();
}

QModelIndex ModelNode::parent(const QModelIndex &child) const
{
    if (child.isValid())
        if (Qt::INode *qtNode = (*static_cast<Item *>(child.internalPointer()))->as<Qt::INode>())
        {
            const Item &parent = qtNode->parent();

            if (parent.isValid())
                if (Qt::INode *qtParentNode = parent->as<Qt::INode>())
                {
                    const Item &grandParent = qtParentNode->parent();

                    if (grandParent.isValid())
                        if (Qt::INode *qtGrandParent = grandParent->as<Qt::INode>())
                            return createIndex(qtGrandParent->indexOf(parent), 0, const_cast<Item *>(&parent));
                        else
                            return createIndex(indexOf(parent), 0, const_cast<Item *>(&parent));
                }
        }

    return QModelIndex();
}

QModelIndex ModelNode::index(Item *item) const
{
    if (Qt::INode *qtNode = (*item)->as<Qt::INode>())
        if (qtNode->parent().isValid())
            if (Qt::INode *qtParentNode = qtNode->parent()->as<Qt::INode>())
                return createIndex(qtParentNode->indexOf(*item), 0, item);

    return createIndex(indexOf(*item), 0, item);
}

QModelIndex ModelNode::parent(Item *item) const
{
    if (Qt::INode *qtNode = (*item)->as<Qt::INode>())
        if (qtNode->parent().isValid())
        {
            if (Qt::INode *qtParentNode = qtNode->parent()->as<Qt::INode>())
                if (qtParentNode->parent().isValid())
                    if (Qt::INode *qtGrandParentNode = qtParentNode->parent()->as<Qt::INode>())
                        return createIndex(qtGrandParentNode->indexOf(qtNode->parent()), 0, const_cast<Item *>(&qtNode->parent()));

            return createIndex(indexOf(qtNode->parent()), 0, const_cast<Item *>(&qtNode->parent()));
        }

    return QModelIndex();
}

//QModelIndex ModelNode::parent(Item *item, size_type &row) const
//{
//    if (Item *parent = item->parent().as<Node>())
//        if (Item *parentParent = parent->parent().as<Node>())
//        {
//            row = static_cast<Item *>(parent)->indexOf(item);
//            return createIndex(static_cast<Item *>(parentParent)->indexOf(parent), 0, parent);
//        }
//        else
//        {
//            row = static_cast<Item *>(parent)->indexOf(item);
//            return createIndex(indexOf(parent), 0, parent);
//        }
//    else
//    {
//        row = indexOf(item);
//        return QModelIndex();
//    }
//}

}}}
