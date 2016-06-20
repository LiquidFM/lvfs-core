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

#ifndef LVFS_CORE_QT_COPYTASK_H_
#define LVFS_CORE_QT_COPYTASK_H_

#include "lvfs_core_qt_FilesBaseTask.h"


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE CopyTask : public FilesBaseTask
{
    Q_DECLARE_TR_FUNCTIONS(CopyTask)

public:
    typedef Node::FilesToCopy Files;

    class Event : public FilesBaseTask::ExtendedEvent
    {
    public:
        Event(Task *task, Type type, bool canceled, const Interface::Holder &dest, Files &files, bool move) :
            FilesBaseTask::ExtendedEvent(task, type, canceled, dest),
            files(std::move(files)),
            move(move)
        {}

        Files files;
        bool move;
    };

public:
    CopyTask(QObject *receiver, Files &files, const Interface::Holder &dest, const Interface::Holder &node, bool move);
    virtual ~CopyTask();

    virtual FilesBaseTask::Files files() const;

protected:
    virtual void run(volatile bool &aborted);

protected:
    class CreateDestinationFolder : public BaseFunctor
    {
        Q_DECLARE_TR_FUNCTIONS(CopyTask::CreateDestinationFolder)

    public:
        CreateDestinationFolder(const Methods &methods,
                                const Interface::Holder &container,
                                const Interface::Holder &entry,
                                Interface::Holder &result) :
            BaseFunctor(methods),
            m_container(container),
            m_entry(entry),
            m_result(result)
        {}

        inline bool operator()();
        inline bool operator()(Tryier::Flag &flag, const volatile bool &aborted) const;

    private:
        const Interface::Holder &m_container;
        const Interface::Holder &m_entry;
        Interface::Holder &m_result;
    };

    class CopyFile : public BaseFunctor
    {
        Q_DECLARE_TR_FUNCTIONS(CopyTask::CopyFile)

    public:
        CopyFile(const Methods &methods,
                 const Interface::Holder &dest,
                 const Interface::Holder &source,
                 bool move) :
            BaseFunctor(methods),
            m_dest(dest),
            m_source(source),
            m_move(move)
        {}

        inline bool operator()();
        inline bool operator()(Tryier::Flag &flag, const volatile bool &aborted) const;

    private:
        const Interface::Holder &m_dest;
        const Interface::Holder &m_source;
        bool m_move;
    };

    class OverwriteFile : public BaseFunctor
    {
        Q_DECLARE_TR_FUNCTIONS(CopyTask::OverwriteFile)

    public:
        OverwriteFile(const Methods &methods,
                      const Interface::Holder &destination,
                      const Interface::Holder &source) :
            BaseFunctor(methods),
            m_source(source),
            m_destination(destination)
        {}

        inline bool operator()();
        inline bool operator()(Tryier::Flag &flag, const volatile bool &aborted) const;

    private:
        const Interface::Holder &m_destination;
        const Interface::Holder &m_source;
    };

private:
    bool m_move;
    Files m_files;
    Interface::Holder m_dest;

    Tryier *m_tryier;
    Tryier *m_overwrite;
    BaseFunctor::Methods m_methods;
};

}}}

#endif /* LVFS_CORE_QT_COPYTASK_H_ */
