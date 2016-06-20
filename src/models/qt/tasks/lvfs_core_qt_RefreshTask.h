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

#ifndef LVFS_CORE_QT_REFRESHTASK_H_
#define LVFS_CORE_QT_REFRESHTASK_H_

#include <lvfs-core/models/Qt/Node>
#include "lvfs_core_qt_FilesBaseTask.h"


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE RefreshTask : public FilesBaseTask
{
public:
    typedef Qt::Node::Files Files;

    class Event : public FilesBaseTask::Event
    {
    public:
        Event(Task *task, Type type, bool canceled, Files &&snapshot, bool isFirstEvent) :
            FilesBaseTask::Event(task, type, canceled),
            snapshot(std::move(snapshot)),
            isFirstEvent(isFirstEvent),
            error()
        {}

        Event(Task *task, Type type, bool canceled, Files &&snapshot, bool isFirstEvent, QString &error) :
            FilesBaseTask::Event(task, type, canceled),
            snapshot(std::move(snapshot)),
            isFirstEvent(isFirstEvent),
            error(error)
        {}

        Files snapshot;
        bool isFirstEvent;
        QString error;
    };

public:
    RefreshTask(QObject *receiver, void *id, const Interface::Holder &file);

    virtual FilesBaseTask::Files files() const;

protected:
    virtual void run(volatile bool &aborted);

private:
    void *m_id;
    Interface::Holder m_file;
};

}}}

#endif /* LVFS_CORE_QT_REFRESHTASK_H_ */
