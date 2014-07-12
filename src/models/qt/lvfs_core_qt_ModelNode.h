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

#ifndef LVFS_CORE_QT_MODELNODE_H_
#define LVFS_CORE_QT_MODELNODE_H_

#include <QtCore/QAbstractItemModel>
#include <lvfs-core/models/Qt/Node>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PUBLIC ModelNode : public Node, public QAbstractItemModel
{
public:
    ModelNode(const Item &parent = Item());
    virtual ~ModelNode();

    /* Qt::INode */
    virtual QAbstractItemModel *model() const;
    virtual QModelIndex parentIndex() const;
    virtual void setParentIndex(const QModelIndex &index);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role = ::Qt::DisplayRole) const;
    virtual ::Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, ::Qt::Orientation orientation, int role = ::Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

private:
    QModelIndex index(Item *item) const;
    QModelIndex parent(Item *item) const;
//    QModelIndex parent(Item *item, size_type &row) const;

private:
    QModelIndex m_index;
};

}}}

#endif /* LVFS_CORE_QT_MODELNODE_H_ */
