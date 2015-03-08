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

#include "lvfs_core_qt_RefreshTask.h"

#include <lvfs-core/INode>
#include <lvfs/IDirectory>
#include <lvfs-core/models/Qt/Node>

#include <QtCore/QDateTime>


namespace LVFS {
namespace Core {
namespace Qt {

RefreshTask::RefreshTask(QObject *receiver, const Interface::Holder &node, int depth) :
    FilesBaseTask(receiver),
    m_node(node),
    m_depth(depth)
{}

void RefreshTask::run(volatile bool &aborted)
{
    Files files;
    bool isFirstEvent = true;
    Interface::Holder file;

    QString error;
    QTime baseTime = QTime::currentTime();
    QTime currentTime;

    if (IDirectory *dir = m_node->as<Core::INode>()->file()->as<IDirectory>())
    {
        for (auto i = dir->begin(), end = dir->end(); i != end && !aborted; ++i)
        {
            currentTime = QTime::currentTime();
            file = (*i);

            if (UNLIKELY(!file.isValid()))
            {
                files.clear();
                break;
            }

            files.push_back(std::move(file));

            if (baseTime.msecsTo(currentTime) > 300)
            {
                baseTime = currentTime;

                if (!files.empty())
                {
                    postEvent(new (std::nothrow) Event(this, Event::ProcessListFileEventId, false, files, isFirstEvent));
                    isFirstEvent = false;
                }
            }
        }

        if (!dir->lastError().isOk())
            error = Qt::Node::toUnicode(dir->lastError().description());
    }

    postEvent(new (std::nothrow) Event(this, Event::DoneListFileEventId, aborted, files, isFirstEvent, error));
}

}}}
