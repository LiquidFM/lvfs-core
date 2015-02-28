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

#ifndef LVFS_CORE_QT_REMOVETASK_H_
#define LVFS_CORE_QT_REMOVETASK_H_

#include <QtCore/QCoreApplication>
#include <lvfs-core/models/Qt/Node>
#include "lvfs_core_qt_FilesBaseTask.h"
#include "tools/lvfs_core_qt_InteractiveTools.h"


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE RemoveTask : public FilesBaseTask
{
    Q_DECLARE_TR_FUNCTIONS(RemoveTask)

public:
    typedef Qt::Node::Files Files;

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
    RemoveTask(QObject *receiver, const Interface::Holder &container, Files &files);
    virtual ~RemoveTask();

    const Files &files() const { return m_files; }

protected:
    virtual void run(volatile bool &aborted);

protected:
    class RemoveFile : public BaseFunctor
    {
        Q_DECLARE_TR_FUNCTIONS(RemoveTask::RemoveFile)

    public:
        RemoveFile(const Methods &methods,
                   const Interface::Holder &container,
                   const Interface::Holder &file) :
            BaseFunctor(methods),
            m_container(container),
            m_file(file)
        {}

        inline bool operator()();
        inline bool operator()(Tryier::Flag &flag, const volatile bool &aborted) const;

    private:
        const Interface::Holder &m_container;
        const Interface::Holder &m_file;
    };

private:
    static void progress(void *arg, off64_t processed);

private:
    Interface::Holder m_container;
    Files m_files;

    Tryier *m_tryier;
    TaskProgress m_progress;
    BaseFunctor::Methods m_methods;
};

}}}

#endif /* LVFS_CORE_QT_REMOVETASK_H_ */
