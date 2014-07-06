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

#ifndef LVFS_CORE_QT_DEFAULTVIEW_H_
#define LVFS_CORE_QT_DEFAULTVIEW_H_

#include <QtGui/QTreeView>
#include <lvfs-core/IView>
#include <lvfs-core/models/Qt/SortFilterModel>
#include <lvfs-core/models/Qt/StyledItemDelegate>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE DefaultView : public Implements<IView>, public QTreeView
{
public:
    DefaultView(QWidget *parent = 0);
    virtual ~DefaultView();

    virtual QWidget *widget() const;

    virtual const Interface::Holder &node() const;
    virtual bool setNode(const Interface::Holder &node);

    virtual const Interface::Holder &opposite() const;

private:
    Interface::Holder m_node;
    Interface::Holder m_opposite;
    SortFilterModel m_sortFilterModel;
    StyledItemDelegate m_styledItemDelegate;
};

}}}

#endif /* LVFS_CORE_QT_DEFAULTVIEW_H_ */
