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

#ifndef LVFS_CORE_TREEMODEL_H_
#define LVFS_CORE_TREEMODEL_H_

#include <platform/utils.h>
#include <QtCore/QAbstractItemModel>


namespace LVFS {
namespace Core {
namespace Tools {

/**
 * @note
 * Any QModelIndex instance must have internalPointer() == some "TreeModel::Item" instance!
 */
class PLATFORM_MAKE_PUBLIC TreeModel : public QAbstractItemModel
{
public:
    class Item;
    typedef size_t size_type;
    enum { InvalidIndex = (size_type)-1 };

public:
    TreeModel();
    virtual ~TreeModel();

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = ::Qt::DisplayRole) const;
	virtual ::Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, ::Qt::Orientation orientation, int role = ::Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

public:
    class Item
    {
    public:
        typedef Item Base;
        typedef TreeModel::size_type size_type;
        enum { InvalidIndex = TreeModel::InvalidIndex };

    public:
        Item(Base *parent);
        virtual ~Item();

        Base *parent() const { return m_parent; }

        virtual Base *at(size_type index) const;
        virtual size_type size() const;
        virtual size_type indexOf(Base *item) const;

        virtual QVariant data(qint32 column, qint32 role) const = 0;

    private:
        Base *m_parent;
    };

protected:
    virtual size_type size() const = 0;
    virtual Item *at(size_type index) const = 0;
    virtual size_type indexOf(Item *item) const = 0;

protected:
	QModelIndex index(Item *item) const;
	QModelIndex parent(Item *item) const;
	QModelIndex parent(Item *item, size_type &row) const;
};

}}}

#endif /* LVFS_CORE_TREEMODEL_H_ */
