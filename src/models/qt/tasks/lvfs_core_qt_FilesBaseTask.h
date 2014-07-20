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

#ifndef LVFS_CORE_QT_FILESBASETASK_H_
#define LVFS_CORE_QT_FILESBASETASK_H_

#include "lvfs_core_qt_BaseTask.h"


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE FilesBaseTask : public BaseTask
{
public:
    class Event : public BaseTask::Event
    {
        PLATFORM_MAKE_NONCOPYABLE(Event)

    public:
        enum Type
        {
            ProcessListFileEventId = BaseTask::Event::User,
            DoneListFileEventId = BaseTask::Event::User + 1,

            ScanFilesForSize = BaseTask::Event::User + 1,
            ScanFilesForRemove = BaseTask::Event::User + 2,
            ScanFilesForCopy = BaseTask::Event::User + 3,
            RemoveFiles = BaseTask::Event::User + 4,
            CopyFiles = BaseTask::Event::User + 5,
            SearchFiles_NewFile = BaseTask::Event::User + 6,
            SearchFiles_Done = BaseTask::Event::User + 7,
            ScanClipboardFiles_Update = BaseTask::Event::User + 8
        };

    public:
        Event(BaseTask *task, Type type, bool canceled, Snapshot &snapshot) :
            BaseTask::Event(task, static_cast<BaseTask::Event::Type>(type), canceled),
            snapshot(std::move(snapshot))
        {}

        Snapshot snapshot;
    };

    class ExtendedEvent : public BaseTask::ExtendedEvent
    {
        PLATFORM_MAKE_NONCOPYABLE(ExtendedEvent)

    public:
        typedef FilesBaseTask::Event::Type Type;

    public:
        ExtendedEvent(BaseTask *task, Type type, const Interface::Holder &destination, bool canceled, Snapshot &snapshot) :
            BaseTask::ExtendedEvent(task, static_cast<BaseTask::Event::Type>(type), destination, canceled),
            snapshot(std::move(snapshot))
        {}

        Snapshot snapshot;
    };

public:
    FilesBaseTask(QObject *receiver);
    FilesBaseTask(QObject *receiver, const Interface::Holder &destination);
};

}}}

#endif /* LVFS_CORE_QT_FILESBASETASK_H_ */
