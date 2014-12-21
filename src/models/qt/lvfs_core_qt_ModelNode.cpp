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

ModelNode::ModelNode(const Interface::Holder &parent) :
    Node(parent)
{}

ModelNode::~ModelNode()
{}

QAbstractItemModel *ModelNode::model() const
{
    return const_cast<ModelNode *>(this);
}

QModelIndex ModelNode::currentIndex() const
{
    return m_currentIndex;
}

void ModelNode::setCurrentIndex(const QModelIndex &index)
{
    m_currentIndex = index;
}

int ModelNode::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        if (Qt::INode *qtNode = static_cast<Item *>(parent.internalPointer())->node->as<Qt::INode>())
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
            return createIndex(row, column, const_cast<Item *>(&(*static_cast<Item *>(parent.internalPointer())).node->as<Qt::INode>()->at(row)));
        else
            return createIndex(row, column, const_cast<Item *>(&at(row)));
    else
        return QModelIndex();
}

QModelIndex ModelNode::parent(const QModelIndex &child) const
{
//    if (child.isValid())
//        if (Core::INode *coreNode = (*static_cast<Item *>(child.internalPointer())).node->as<Core::INode>())
//        {
//            const Interface::Holder &parent = coreNode->parent();
//
//            if (parent.isValid())
//                if (Core::INode *coreParentNode = parent->as<Core::INode>())
//                {
//                    const Interface::Holder &grandParent = coreParentNode->parent();
//
//                    if (grandParent.isValid())
//                        if (Qt::INode *qtGrandParent = grandParent->as<Qt::INode>())
//                            return createIndex(qtGrandParent->indexOf(parent), 0, const_cast<Item *>(&parent));
//                        else
//                            return createIndex(indexOf(parent), 0, const_cast<Item *>(&parent));
//                }
//        }

    return QModelIndex();
}

QModelIndex ModelNode::index(Item *item) const
{
    if (Core::INode *coreNode = item->node->as<Core::INode>())
        if (coreNode->parent().isValid())
            if (Qt::INode *qtParentNode = coreNode->parent()->as<Qt::INode>())
                return createIndex(qtParentNode->indexOf(*item), 0, item);

    return createIndex(indexOf(*item), 0, item);
}

QModelIndex ModelNode::parent(Item *item) const
{
//    if (Core::INode *coreNode = item->node->as<Core::INode>())
//        if (coreNode->parent().isValid())
//        {
//            if (Core::INode *coreParentNode = coreNode->parent()->as<Core::INode>())
//                if (coreParentNode->parent().isValid())
//                    if (Qt::INode *qtGrandParentNode = coreParentNode->parent()->as<Qt::INode>())
//                        return createIndex(qtGrandParentNode->indexOf(coreNode->parent()), 0, const_cast<Item *>(&coreNode->parent()));
//
//            return createIndex(indexOf(coreNode->parent()), 0, const_cast<Item *>(&coreNode->parent()));
//        }

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
