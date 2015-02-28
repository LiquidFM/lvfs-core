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

#include "lvfs_core_qt_RemoveTask.h"

#include <lvfs/IFile>
#include <lvfs/IDirectory>
#include <lvfs/IProperties>

#include <QtGui/QMessageBox>


namespace LVFS {
namespace Core {
namespace Qt {

RemoveTask::RemoveTask(QObject *receiver, const Interface::Holder &container, Files &files) :
    FilesBaseTask(receiver),
    m_container(container),
    m_files(std::move(files)),
    m_tryier(NULL),
    m_progress(receiver),
    m_methods({ this, &RemoveTask::askUser, NULL })
{}

RemoveTask::~RemoveTask()
{}

void RemoveTask::run(volatile bool &aborted)
{
    struct StackEntry
    {
        IDirectory::const_iterator first;
        IDirectory::const_iterator second;
        Interface::Holder container;
    };
    typedef EFC::List<StackEntry> Stack;

    Interface::Holder holder;
    IDirectory *dir;

    Tryier tryier(aborted);
    m_tryier = &tryier;

    m_methods.aborted = &aborted;

    for (Files::const_iterator it = m_files.begin(), end = m_files.end(); it != end && !aborted; ++it)
    {
        m_progress.init(*it, calculateSize(*it));

        if (::strcmp((*it)->as<IEntry>()->type()->name(), Module::DirectoryTypeName) != 0)
            m_tryier->tryTo(RemoveFile(m_methods, m_container, *it));
        else if (dir = (*it)->as<IDirectory>())
        {
            Stack stack;
            Stack::value_type current({ dir->begin(), dir->end(), *it });

            do
            {
                while (current.first != current.second && !aborted)
                {
                    holder = (*current.first);
                    ++current.first;

                    if (::strcmp(holder->as<IEntry>()->type()->name(), Module::DirectoryTypeName) != 0)
                        m_tryier->tryTo(RemoveFile(m_methods, current.container, holder));
                    else if (dir = holder->as<IDirectory>())
                    {
                        stack.push_back(current);
                        current.first = dir->begin();
                        current.second = dir->end();
                        current.container = holder;
                    }
                }

                if (aborted)
                    break;

                if (stack.empty())
                {
                    m_tryier->tryTo(RemoveFile(m_methods, m_container, current.container));
                    break;
                }
                else
                    if (m_tryier->tryTo(RemoveFile(m_methods, stack.back().container, current.container)))
                    {
                        current = stack.back();
                        stack.pop_back();
                    }
            }
            while (true);
        }

        m_progress.complete();
    }

    postEvent(new (std::nothrow) Event(this, FilesBaseTask::Event::DoneRemoveFilesEventId, aborted, m_files));
}

bool RemoveTask::RemoveFile::operator()()
{
    if (m_container->as<IDirectory>()->remove(m_file))
    {
        if (IProperties *properties = m_file->as<IProperties>())
            progress(task(), properties->size());

        return true;
    }

    return false;
}

bool RemoveTask::RemoveFile::operator()(Tryier::Flag &flag, const volatile bool &aborted) const
{
    qint32 answer = askUser(tr("Failed to remove..."),
                            tr("Failed to remove \"%1\"").
                                arg(Qt::Node::toUnicode(m_file->as<IEntry>()->title())).
                                append(QChar(L'\n')).
                                append(tr("Error: \"%1\"").arg(Qt::Node::toUnicode(m_container->as<IDirectory>()->lastError().description()))).
                                append(QChar(L'\n')).
                                append(tr("Skip it?")),
                            QMessageBox::Yes |
                            QMessageBox::YesToAll |
                            QMessageBox::Retry |
                            QMessageBox::Cancel,
                            aborted);

    switch (answer)
    {
        case QMessageBox::Yes:
            return false;

        case QMessageBox::YesToAll:
            return flag = false;

        case QMessageBox::Retry:
            return true;

        case QMessageBox::Cancel:
            cancel();
            break;
    }

    return false;
}

void RemoveTask::progress(void *arg, off64_t processed)
{
    static_cast<RemoveTask *>(arg)->m_progress.update(processed);
}

}}}
