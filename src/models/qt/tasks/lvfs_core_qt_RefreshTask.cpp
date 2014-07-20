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

#include "lvfs_core_qt_RefreshTask.h"
#include "../../../default/lvfs_core_qt_DefaultNode.h"

#include <QtCore/QDateTime>

#include <lvfs-core/INode>
#include <lvfs-core/INodeFactory>
#include <lvfs/IDirectory>


namespace LVFS {
namespace Core {
namespace Qt {

RefreshTask::RefreshTask(QObject *receiver, const Interface::Holder &node, int depth) :
    FilesBaseTask(receiver),
    m_node(node),
    m_depth(depth)
{}

void RefreshTask::run(const volatile bool &aborted)
{
    Snapshot nodes;
    bool isFirstEvent = true;
    Interface::Holder file;
    Interface::Holder node;

    QTime baseTime = QTime::currentTime();
    QTime currentTime;

    if (IDirectory *dir = m_node->as<Core::INode>()->file()->as<IDirectory>())
        for (auto i = dir->begin(), end = dir->end(); i != end && !aborted; ++i)
        {
            currentTime = QTime::currentTime();

            file = Module::open(*i);

            if (INodeFactory *factory = file->as<INodeFactory>())
                node.reset(factory->createNode(file, m_node));

            if (!node.isValid())
                node.reset(new (std::nothrow) DefaultNode(file, m_node));

            if (node.isValid())
            {
                if (m_depth > 0)
                    node->as<Core::INode>()->refresh(m_depth - 1);

                nodes.push_back(std::move(node));
            }

            if (baseTime.msecsTo(currentTime) > 300)
            {
                baseTime = currentTime;

                if (!nodes.empty())
                {
                    postEvent(new (std::nothrow) ListFileEvent(this, Event::ProcessListFileEventId, isFirstEvent, nodes, false));
                    isFirstEvent = false;
                }
            }
        }

    postEvent(new (std::nothrow) ListFileEvent(this, Event::DoneListFileEventId, isFirstEvent, nodes, aborted));
}

}}}
