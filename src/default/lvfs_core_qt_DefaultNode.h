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

#ifndef LVFS_CORE_QT_DEFAULTNODE_H_
#define LVFS_CORE_QT_DEFAULTNODE_H_

#include <efc/Set>
#include <efc/Vector>
#include <lvfs-core/models/Qt/ModelNode>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE DefaultNode : public ModelNode
{
    Q_OBJECT

public:
    DefaultNode(const Interface::Holder &file, const Interface::Holder &parent);
    virtual ~DefaultNode();

public: /* Core::INode */
    virtual const Interface::Holder &file() const;

    virtual void refresh(int depth);
    virtual void opened(const Interface::Holder &view);
    virtual void closed(const Interface::Holder &view);

    virtual int refs() const;
    virtual void incRef();
    virtual int decRef();
    virtual void clear();

public: /* Qt::INode */
    virtual size_type size() const;
    virtual const Item &at(size_type index) const;
    virtual size_type indexOf(const Item &node) const;

    virtual const Geometry &geometry() const;
    virtual const Sorting &sorting() const;

    virtual void copyToClipboard(const QModelIndexList &files, bool move);

public: /* Qt::ModelNode */
    virtual int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, ::Qt::Orientation orientation, int role = ::Qt::DisplayRole) const;

protected: /* Core::INode */
    virtual Interface::Holder node(const Interface::Holder &file) const;
    virtual void setNode(const Interface::Holder &file, const Interface::Holder &node);

protected: /* Core::Qt::Node */
    virtual void processListFile(EFC::List<Interface::Holder> &files, bool isFirstEvent);
    virtual void doneListFile(EFC::List<Interface::Holder> &files, bool isFirstEvent);

private:
    int m_ref;
    Interface::Holder m_file;
    EFC::Vector<Item> m_files;
    EFC::Set<Interface::Holder> m_views;
    Geometry m_geometry;
    Sorting m_sorting;
};

}}}

#endif /* LVFS_CORE_QT_DEFAULTNODE_H_ */
