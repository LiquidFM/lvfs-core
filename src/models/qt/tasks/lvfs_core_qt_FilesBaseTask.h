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

#include "lvfs_core_qt_Task.h"


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE FilesBaseTask : public Task
{
public:
    class Event : public Task::Event
    {
        PLATFORM_MAKE_NONCOPYABLE(Event)

    public:
        enum Type
        {
            ProcessListFileEventId = Task::Event::User,
            DoneListFileEventId = Task::Event::User + 1,
            DoneCopyFilesEventId = Task::Event::User + 2,

            ScanFilesForSize = Task::Event::User + 1,
            ScanFilesForRemove = Task::Event::User + 2,
            ScanFilesForCopy = Task::Event::User + 3,
            RemoveFiles = Task::Event::User + 4,
            CopyFiles = Task::Event::User + 5,
            SearchFiles_NewFile = Task::Event::User + 6,
            SearchFiles_Done = Task::Event::User + 7,
            ScanClipboardFiles_Update = Task::Event::User + 8
        };

    public:
        Event(Task *task, Type type, bool canceled) :
            Task::Event(task, static_cast<Task::Event::Type>(type), canceled)
        {}
    };

    class ExtendedEvent : public Task::ExtendedEvent
    {
        PLATFORM_MAKE_NONCOPYABLE(ExtendedEvent)

    public:
        typedef FilesBaseTask::Event::Type Type;

    public:
        ExtendedEvent(Task *task, Type type, bool canceled, const Interface::Holder &dest) :
            Task::ExtendedEvent(task, static_cast<Task::Event::Type>(type), canceled, dest)
        {}
    };

public:
    FilesBaseTask(QObject *receiver);
    FilesBaseTask(QObject *receiver, const Interface::Holder &destination);

protected:
    static uint64_t calculateSize(const Interface::Holder &file);
};

}}}

#endif /* LVFS_CORE_QT_FILESBASETASK_H_ */
