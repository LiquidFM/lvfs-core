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

#include "lvfs_core_qt_DefaultView.h"

#include <lvfs/IDirectory>
#include <lvfs-core/INode>
#include <lvfs-core/models/Qt/INode>

#include <brolly/assert.h>
#include <cstdlib>


namespace LVFS {
namespace Core {
namespace Qt {

DefaultView::DefaultView() :
    m_view(&m_eventHandler),
    m_eventHandler(this)
{
    m_view.setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_view.setContextMenuPolicy(::Qt::DefaultContextMenu);
    m_view.setItemDelegate(&m_styledItemDelegate);
    m_view.setSortingEnabled(true);
    m_sortFilterModel.setDynamicSortFilter(true);
    m_view.setModel(&m_sortFilterModel);

    m_eventHandler.registerMouseDoubleClickEventHandler(&DefaultView::goInto);
    m_eventHandler.registerShortcut(::Qt::NoModifier, ::Qt::Key_Return, &DefaultView::goInto);
    m_eventHandler.registerShortcut(::Qt::NoModifier, ::Qt::Key_Enter, &DefaultView::goInto);
    m_eventHandler.registerShortcut(::Qt::NoModifier, ::Qt::Key_Backspace, &DefaultView::goUp);
}

DefaultView::~DefaultView()
{
    ASSERT(!m_node.isValid());
}

QWidget *DefaultView::widget() const
{
    return const_cast<TreeView *>(&m_view);
}

const Interface::Holder &DefaultView::node() const
{
    return m_node;
}

bool DefaultView::setNode(const Interface::Holder &node)
{
    if (!node.isValid())
    {
        m_node.reset();
        return true;
    }

    return openNode(node, QModelIndex(), QModelIndex());
}

const Interface::Holder &DefaultView::opposite() const
{
    return m_opposite;
}

void DefaultView::goUp()
{
    Interface::Holder node = m_node->as<Core::INode>()->parent();

    if (node.isValid())
        openNode(node, m_node->as<Qt::INode>()->parentIndex(), QModelIndex());
}

void DefaultView::goInto()
{
    QModelIndex index1 = m_view.selectionModel()->currentIndex();
    QModelIndex index2 = m_sortFilterModel.mapToSource(index1);

    if (index2.isValid())
        openNode(m_node->as<Qt::INode>()->at(index2.row()), QModelIndex(), index1);
}

bool DefaultView::openNode(const Interface::Holder &node, const QModelIndex &currentIdx, const QModelIndex &parentIdx)
{
    Core::INode *coreNode = node->as<Core::INode>();

    if (coreNode->file()->as<IDirectory>())
        if (Qt::INode *qtNode = node->as<Qt::INode>())
        {
            if (parentIdx.isValid())
                qtNode->setParentIndex(parentIdx);
            else if (currentIdx.isValid())
                m_node->as<Core::INode>()->closed(Interface::Holder::fromRawData(this));

            m_node = node;
            m_sortFilterModel.setSourceModel(qtNode->model());
            coreNode->opened(Interface::Holder::fromRawData(this));

            qint32 column = 0;
            for (auto i : coreNode->geometry())
                m_view.setColumnWidth(column++, i);

            m_view.sortByColumn(coreNode->sorting().first, coreNode->sorting().second);

            coreNode->refresh(0);

            QModelIndex selected = currentIdx;

            if (selected.isValid())
                selected = m_sortFilterModel.index(selected.row(), selected.column());

            if (!selected.isValid())
                selected = m_sortFilterModel.index(0, 0);

            if (LIKELY(selected.isValid() == true))
            {
                m_view.setFocus();
                m_view.scrollTo(selected, QAbstractItemView::PositionAtCenter);
                m_view.selectionModel()->select(selected, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Columns);
                m_view.selectionModel()->setCurrentIndex(selected, QItemSelectionModel::ClearAndSelect);
            }

            return true;
        }

    return false;
}

}}}
