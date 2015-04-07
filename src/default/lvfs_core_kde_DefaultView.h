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

#ifndef LVFS_CORE_KDE_DEFAULTVIEW_H_
#define LVFS_CORE_KDE_DEFAULTVIEW_H_

#include <lvfs-core/IView>
#include <kitemviews/kstandarditemlistview.h>
#include <kitemviews/kitemlistcontroller.h>
#include <kitemviews/kitemlistcontainer.h>


namespace LVFS {
namespace Core {
namespace Kde {

class PLATFORM_MAKE_PRIVATE DefaultView : public Implements<IView>, public KStandardItemListView
{
public:
    DefaultView();
    virtual ~DefaultView();

    virtual QWidget *widget() const;

    virtual const Interface::Holder &node() const;
    virtual void setNode(const Interface::Holder &node);

    virtual bool isAbleToView(const Interface::Holder &node) const;

protected:
    virtual void onModelChanged(KItemModelBase *current, KItemModelBase *previous);

private:
    KItemListController m_controller;
    KItemListContainer m_container;

private:
    Interface::Holder m_node;
};

}}}

#endif /* LVFS_CORE_KDE_DEFAULTVIEW_H_ */
