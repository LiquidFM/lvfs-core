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

#include "lvfs_core_qt_RefreshTask.h"

#include <lvfs-core/INode>
#include <lvfs/IDirectory>
#include <lvfs-core/models/Qt/Node>

#include <QtCore/QDateTime>


namespace LVFS {
namespace Core {
namespace Qt {

RefreshTask::RefreshTask(QObject *receiver, void *id, const Interface::Holder &file) :
    FilesBaseTask(receiver),
    m_id(id),
    m_file(file)
{}

FilesBaseTask::Files RefreshTask::files() const
{
    FilesBaseTask::Files res;

    res.push_back(m_file);

    return res;
}

void RefreshTask::run(volatile bool &aborted)
{
    Files res;
    Files::mapped_type *files = &res[m_id];
    bool isFirstEvent = true;
    Interface::Holder file;

    QString error;
    QTime baseTime = QTime::currentTime();
    QTime currentTime;

    if (IDirectory *dir = m_file->as<IDirectory>())
    {
        for (auto i = dir->begin(), end = dir->end(); i != end && !aborted; ++i)
        {
            currentTime = QTime::currentTime();
            file = (*i);

            if (UNLIKELY(!file.isValid()))
            {
                files->clear();
                break;
            }

            files->push_back(std::move(file));

            if (baseTime.msecsTo(currentTime) > 300)
            {
                baseTime = currentTime;

                if (!files->empty())
                {
                    postEvent(new (std::nothrow) Event(this, Event::ProcessListFileEventId, false, std::move(res), isFirstEvent));
                    isFirstEvent = false;
                    files = &res[m_id];
                }
            }
        }

        if (!dir->lastError().isOk())
            error = Qt::Node::toUnicode(dir->lastError().description());
    }

    postEvent(new (std::nothrow) Event(this, Event::DoneListFileEventId, aborted, std::move(res), isFirstEvent, error));
}

}}}
