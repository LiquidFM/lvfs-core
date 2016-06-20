/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2016 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#include "lvfs_core_qt_DefaultView.h"

#include <lvfs/IDirectory>
#include <lvfs-core/INode>
#include <lvfs-core/IMainView>
#include <lvfs-core/models/Qt/INode>
#include <lvfs-core/models/Qt/Node>
#include <lvfs-core/tools/widgets/StringDialog>

#include <brolly/assert.h>

#include <QtGui/QMessageBox>

#include <cstdlib>


namespace LVFS {
namespace Core {
namespace Qt {

DefaultView::DefaultView() :
    m_view(&m_eventHandler),
    m_eventHandler(this),
    m_styledItemDelegate(m_node)
{
    m_view.setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_view.setContextMenuPolicy(::Qt::DefaultContextMenu);
    m_view.setItemDelegate(&m_styledItemDelegate);
    m_view.setSortingEnabled(true);

    m_eventHandler.registerMouseDoubleClickEventHandler(&DefaultView::goIntoShortcut);
    m_eventHandler.setDefaultHandler(EventHandler::KeyboardEvent, &DefaultView::handleShortcut);

    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_Return, &DefaultView::goIntoShortcut);
    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_Enter, &DefaultView::goIntoShortcut);
    m_eventHandler.registerShortcut(::Qt::CTRL,               ::Qt::Key_Return, &DefaultView::newWindowShortcut);
    m_eventHandler.registerShortcut(::Qt::CTRL,               ::Qt::Key_Enter, &DefaultView::newWindowShortcut);
    m_eventHandler.registerShortcut(::Qt::CTRL + ::Qt::SHIFT, ::Qt::Key_Left, &DefaultView::newWindowOnOppositeTabShortcut);
    m_eventHandler.registerShortcut(::Qt::CTRL + ::Qt::SHIFT, ::Qt::Key_Right, &DefaultView::newWindowOnOppositeTabShortcut);
    m_eventHandler.registerShortcut(::Qt::CTRL,               ::Qt::Key_W, &DefaultView::closeShortcut);
    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_F3, &DefaultView::viewShortcut);
    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_Backspace, &DefaultView::goUpShortcut);
    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_Escape, &DefaultView::cancelShortcut);
    m_eventHandler.registerShortcut(::Qt::ALT + ::Qt::CTRL,   ::Qt::Key_X, &DefaultView::pathToClipboardShortcut);
    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_F2, &DefaultView::renameShortcut);
    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_F8, &DefaultView::createFileShortcut);
    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_F7, &DefaultView::createDirectoryShortcut);
    m_eventHandler.registerShortcut(::Qt::SHIFT,              ::Qt::Key_Delete, &DefaultView::removeShortcut);
    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_Space, &DefaultView::calculateSizeShortcut);
    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_F5, &DefaultView::copyShortcut);
    m_eventHandler.registerShortcut(::Qt::NoModifier,         ::Qt::Key_F6, &DefaultView::moveShortcut);
    m_eventHandler.registerShortcut(::Qt::CTRL,               ::Qt::Key_F, &DefaultView::searchShortcut);
    m_eventHandler.registerShortcut(::Qt::CTRL,               ::Qt::Key_C, &DefaultView::copyToClipboardShortcut);
    m_eventHandler.registerShortcut(::Qt::CTRL,               ::Qt::Key_X, &DefaultView::cutToClipboardShortcut);
    m_eventHandler.registerShortcut(::Qt::CTRL,               ::Qt::Key_V, &DefaultView::pasteFromClipboardShortcut);
}

DefaultView::~DefaultView()
{}

QWidget *DefaultView::widget() const
{
    return const_cast<TreeView *>(&m_view);
}

const Interface::Holder &DefaultView::mainView() const
{
    return m_mainView;
}

void DefaultView::setMainView(const Interface::Holder &mainView)
{
    ASSERT(mainView.isValid());
    ASSERT(mainView->as<IMainView>() != NULL);
    m_mainView = mainView;
}

const Interface::Holder &DefaultView::node() const
{
    return m_node;
}

void DefaultView::setNode(const Interface::Holder &node)
{
    if (!node.isValid())
    {
        m_node.reset();
        return;
    }

    ASSERT(node->as<Core::INode>()->file()->as<IDirectory>() != NULL);
    ASSERT(node->as<Qt::INode>() != NULL);
    Qt::INode *qtNode = node->as<Qt::INode>();

    m_node = node;
    m_view.setModel(qtNode->model());


    qint32 column = 0;
    for (auto i : qtNode->geometry())
        m_view.setColumnWidth(column++, i);

    m_view.sortByColumn(qtNode->sorting().first, qtNode->sorting().second);
}

bool DefaultView::isAbleToView(const Interface::Holder &node) const
{
    return node->as<Core::INode>()->file()->as<IDirectory>() && node->as<Qt::INode>();
}

QModelIndex DefaultView::currentIndex() const
{
    return m_view.selectionModel()->currentIndex();
}

void DefaultView::select(const QModelIndex &index, bool expand)
{
    if (LIKELY(index.isValid() == true))
    {
        m_view.setFocus();
        m_view.scrollTo(index, QAbstractItemView::PositionAtCenter);
        m_view.selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Columns);
        m_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

        if (expand)
            m_view.expand(index);
    }
}

void DefaultView::goUpShortcut()
{
    const Interface::Holder &node = m_node->as<Core::INode>()->parent();

    if (node.isValid())
        m_mainView->as<Core::IMainView>()->show(Interface::self(), node);
}

void DefaultView::goIntoShortcut()
{
    QModelIndex index = m_view.selectionModel()->currentIndex();

    if (index.isValid())
        m_node->as<Qt::INode>()->activated(Interface::self(), index, false);
}

void DefaultView::newWindowShortcut()
{
    QModelIndex index = m_view.selectionModel()->currentIndex();

    if (index.isValid())
        m_node->as<Qt::INode>()->activated(Interface::self(), index, true);
}

void DefaultView::newWindowOnOppositeTabShortcut()
{
    QModelIndex index = m_view.selectionModel()->currentIndex();

    if (index.isValid())
    {
        Interface::Holder opposite = m_mainView->as<IMainView>()->opposite(Interface::self());
        m_node->as<Qt::INode>()->activated(opposite, index, true);
    }
}

void DefaultView::closeShortcut()
{
    m_mainView->as<Core::IMainView>()->close(Interface::self());
}

void DefaultView::viewShortcut()
{
    QModelIndex index = m_view.selectionModel()->currentIndex();

    if (index.isValid())
        m_node->as<Qt::INode>()->view(Interface::self(), index);
}

void DefaultView::cancelShortcut()
{
    QModelIndexList items = m_view.selectionModel()->selectedIndexes();

    if (!items.isEmpty())
        m_node->as<Qt::INode>()->cancel(items);
}

void DefaultView::pathToClipboardShortcut()
{

}

void DefaultView::renameShortcut()
{
    QModelIndex index = m_view.selectionModel()->currentIndex();

    if (index.isValid())
        m_node->as<Qt::INode>()->rename(Interface::self(), index);
}

void DefaultView::createFileShortcut()
{

}

void DefaultView::createDirectoryShortcut()
{
    QModelIndex index = m_view.selectionModel()->currentIndex();

    if (index.isValid())
        m_node->as<Qt::INode>()->createDirectory(Interface::self(), index);
}

void DefaultView::removeShortcut()
{
    QModelIndexList items = m_view.selectionModel()->selectedIndexes();

    if (!items.isEmpty())
        m_node->as<Qt::INode>()->remove(Interface::self(), items);
}

void DefaultView::calculateSizeShortcut()
{

}

void DefaultView::copyShortcut()
{
    copyMoveShortcut(false);
}

void DefaultView::moveShortcut()
{
    copyMoveShortcut(true);
}

void DefaultView::searchShortcut()
{

}

void DefaultView::copyToClipboardShortcut()
{
    copyToClipboard(false);
}

void DefaultView::cutToClipboardShortcut()
{
    copyToClipboard(true);
}

void DefaultView::pasteFromClipboardShortcut()
{

}

bool DefaultView::handleShortcut(QEvent *event)
{
    return false;
}

void DefaultView::copyMoveShortcut(bool move)
{
    QModelIndexList items = m_view.selectionModel()->selectedIndexes();

    if (!items.isEmpty())
    {
        Core::INode::Files files = m_node->as<Qt::INode>()->mapToFile(items);

        if (!files.empty())
        {
            Interface::Holder self = Interface::self();
            Interface::Holder opposite = m_mainView->as<IMainView>()->opposite(self);
            Interface::Holder node = opposite->as<Core::IView>()->node();
            Interface::Holder dest = node->as<Core::INode>()->accept(opposite, files);

            if (dest.isValid())
                m_node->as<Qt::INode>()->copy(self, files, dest, node, move);
        }
    }
}

void DefaultView::copyToClipboard(bool move)
{
    QModelIndexList items = m_view.selectionModel()->selectedIndexes();

    if (!items.isEmpty())
        m_node->as<Qt::INode>()->copyToClipboard(Interface::self(), items, move);
}

}}}
