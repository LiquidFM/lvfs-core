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

#ifndef WORKSPACE_LVFS_CORE_SRC_MODELS_QT_TASKS_LVFS_CORE_QT_COPYTASK_H_
#define WORKSPACE_LVFS_CORE_SRC_MODELS_QT_TASKS_LVFS_CORE_QT_COPYTASK_H_

#include <QtCore/QCoreApplication>
#include <lvfs-core/models/Qt/Node>
#include "lvfs_core_qt_FilesBaseTask.h"
#include "tools/lvfs_core_qt_InteractiveTools.h"


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE CopyTask : public FilesBaseTask
{
    Q_DECLARE_TR_FUNCTIONS(CopyTask)

public:
    typedef Qt::Node::Files Files;

    class Event : public FilesBaseTask::ExtendedEvent
    {
    public:
        Event(Task *task, Type type, bool canceled, const Interface::Holder &dest, Files &files) :
            FilesBaseTask::ExtendedEvent(task, type, canceled, dest),
            files(std::move(files))
        {}

        Files files;
    };

public:
    CopyTask(QObject *receiver, Files &files, const Interface::Holder &dest, bool move);
    virtual ~CopyTask();

    const Files &files() const { return m_files; }

protected:
    virtual void run(volatile bool &aborted);

protected:
    class BaseFunctor
    {
    public:
        typedef void (CopyTask::*Cancel)();
        typedef qint32 (CopyTask::*AskUser)(const QString &, const QString &, qint32, const volatile bool &) const;

        struct Methods
        {
            CopyTask *object;
            AskUser askUser;
            volatile bool *aborted;
        };

    public:
        BaseFunctor(const Methods &methods) :
            m_methods(methods)
        {}

    protected:
        void cancel() const
        { *m_methods.aborted = true; }
        qint32 askUser(const QString &title, const QString &question, qint32 buttons, const volatile bool &aborted) const
        { return (m_methods.object->*m_methods.askUser)(title, question, buttons, aborted); }

    private:
        const Methods &m_methods;
    };

    class CreateDestinationFolder : public BaseFunctor
    {
        Q_DECLARE_TR_FUNCTIONS(CopyTask::CreateDestinationFolder)

    public:
        CreateDestinationFolder(const Methods &methods, const Interface::Holder &container, const Interface::Holder &entry, Interface::Holder &result) :
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

    class OverwriteFile : public BaseFunctor
    {
        Q_DECLARE_TR_FUNCTIONS(CopyTask::OverwriteFile)

    public:
        OverwriteFile(const Methods &methods, const Interface::Holder &destination, const Interface::Holder &source) :
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
    static void progress(void *arg, off64_t processed);

private:
    bool m_move;
    Files m_files;

    Tryier *m_tryier;
    Tryier *m_overwrite;
    TaskProgress m_progress;
    BaseFunctor::Methods m_methods;

    Interface::Holder m_destFile;
};

}}}

#endif /* WORKSPACE_LVFS_CORE_SRC_MODELS_QT_TASKS_LVFS_CORE_QT_COPYTASK_H_ */
