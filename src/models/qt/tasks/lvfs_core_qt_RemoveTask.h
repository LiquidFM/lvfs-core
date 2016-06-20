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

#ifndef LVFS_CORE_QT_REMOVETASK_H_
#define LVFS_CORE_QT_REMOVETASK_H_

#include "lvfs_core_qt_FilesBaseTask.h"


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE RemoveTask : public FilesBaseTask
{
public:
    typedef Node::FilesToRemove Files;

    class Event : public FilesBaseTask::Event
    {
    public:
        Event(Task *task, Type type, bool canceled, Files &files) :
            FilesBaseTask::Event(task, type, canceled),
            files(std::move(files))
        {}

        Files files;
    };

public:
    RemoveTask(QObject *receiver, Files &files);
    virtual ~RemoveTask();

    virtual FilesBaseTask::Files files() const;

protected:
    virtual void run(volatile bool &aborted);

private:
    Files m_files;

    Tryier *m_tryier;
    BaseFunctor::Methods m_methods;
};

}}}

#endif /* LVFS_CORE_QT_REMOVETASK_H_ */
