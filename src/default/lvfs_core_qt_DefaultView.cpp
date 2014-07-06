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

#include <lvfs-core/INode>
#include <lvfs-core/models/Qt/INode>

#include <brolly/assert.h>
#include <cstdlib>


namespace LVFS {
namespace Core {
namespace Qt {

DefaultView::DefaultView(QWidget *parent) :
    QTreeView(parent)
{}

DefaultView::~DefaultView()
{}

QWidget *DefaultView::widget() const
{
    return const_cast<DefaultView *>(this);
}

const Interface::Holder &DefaultView::node() const
{
    return m_node;
}

bool DefaultView::setNode(const Interface::Holder &node)
{
    ASSERT(node.isValid());

    if (Qt::INode *qtNode = node->as<Qt::INode>())
    {
        node->as<Core::INode>()->refresh(1);

        m_node = node;
        setModel(qtNode->model());

        return true;
    }

    return false;
}

const Interface::Holder &DefaultView::opposite() const
{
    return m_opposite;
}

}}}
