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

#ifndef LVFS_CORE_QT_DEFAULTVIEW_H_
#define LVFS_CORE_QT_DEFAULTVIEW_H_

#include <QtCore/QEvent>
#include <QtGui/QTreeView>
#include <lvfs-core/IView>
#include <lvfs-core/models/Qt/IView>
#include <lvfs-core/models/Qt/StyledItemDelegate>

#include <lvfs-core/tools/events/ContextMenuEventHandler>
#include <lvfs-core/tools/events/ContextMenuEventSource>
#include <lvfs-core/tools/events/KeyboardEventHandler>
#include <lvfs-core/tools/events/KeyboardEventSource>
#include <lvfs-core/tools/events/EventHandlerDefault>
#include <lvfs-core/tools/events/MouseEventHandler>
#include <lvfs-core/tools/events/MouseEventSource>

#include "lvfs_core_qt_DefaultSortFilterModel.h"


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE DefaultView : public Implements<Core::IView, Qt::IView>
{
public:
    DefaultView();
    virtual ~DefaultView();

    /* Core::IView */
    virtual QWidget *widget() const;

    virtual const Interface::Holder &mainView() const;
    virtual void setMainView(const Interface::Holder &mainView);

    virtual const Interface::Holder &node() const;
    virtual bool setNode(const Interface::Holder &node);

    /* Qt::IView */
    virtual void select(const QModelIndex &index, bool expand = false);

private:
    void goUpShortcut();
    void goIntoShortcut();
    void pathToClipboardShortcut();
    void renameShortcut();
    void createFileShortcut();
    void createDirectoryShortcut();
    void removeShortcut();
    void calculateSizeShortcut();
    void copyShortcut();
    void moveShortcut();
    void searchShortcut();
    void copyToClipboardShortcut();
    void cutToClipboardShortcut();
    void pasteFromClipboardShortcut();
    bool handleShortcut(QEvent *event);

private:
    inline bool openChildNode(const Interface::Holder &node, const QModelIndex &selected);
    inline bool openParentNode(const Interface::Holder &node, const QModelIndex &selected);

private:
    typedef Tools::MouseDoubleClickEventSource<
                Tools::ContextMenuEventSource<
                    Tools::KeyboardEventSource<
                        Tools::EventSourceBase<
                            QTreeView
                        >
                    >
                >
            > TreeView;

    typedef Tools::MouseDoubleClickEventHandler<
                Tools::KeyboardEventHandler<
                    Tools::EventHandlerDefault<DefaultView>
                >
            > EventHandler;

private:
    Interface::Holder m_node;
    Interface::Holder m_mainView;

private:
    TreeView m_view;
    EventHandler m_eventHandler;
    DefaultSortFilterModel m_sortFilterModel;
    StyledItemDelegate m_styledItemDelegate;
};

}}}

#endif /* LVFS_CORE_QT_DEFAULTVIEW_H_ */
