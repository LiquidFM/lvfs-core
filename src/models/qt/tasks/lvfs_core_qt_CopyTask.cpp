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

#include "lvfs_core_qt_CopyTask.h"

#include <lvfs/IFile>
#include <lvfs/IDirectory>

#include <QtGui/QMessageBox>


namespace LVFS {
namespace Core {
namespace Qt {

static void progress(void *arg, off64_t processed)
{

}


CopyTask::CopyTask(QObject *receiver, Snapshot &snapshot, const Interface::Holder &dest, bool move) :
    FilesBaseTask(receiver, dest),
    m_move(move),
    m_snapshot(std::move(snapshot)),
    m_tryier(NULL),
    m_progress(receiver),
    m_methods({ this, &CopyTask::askUser, &CopyTask::cancel })
{}

CopyTask::~CopyTask()
{}

void CopyTask::run(const volatile bool &aborted)
{
    struct StackEntry
    {
        IDirectory::const_iterator first;
        IDirectory::const_iterator second;
        Interface::Holder dest;
    };
    typedef EFC::List<StackEntry> Stack;

    IDirectory::Progress callback = { this, progress, aborted };
    Interface::Holder holder;
    IDirectory *dir;

    Tryier tryier(aborted);
    m_tryier = &tryier;

    Tryier overwrite(aborted);
    m_overwrite = &overwrite;

    for (Snapshot::const_iterator it = m_snapshot.begin(), end = m_snapshot.end(); it != end && !aborted; ++it)
    {
        m_progress.init(*it, calculateSize(*it));

        if (m_overwrite->askFor(OverwriteFile(m_methods, destination(), *it)))
            if (::strcmp((*it)->as<IEntry>()->type()->name(), Module::DirectoryTypeName) != 0)
                destination()->as<IDirectory>()->copy(callback, *it, m_move);
            else if ((dir = (*it)->as<IDirectory>()) &&
                     m_tryier->tryTo(CreateDestinationFolder(m_methods, destination(), *it, holder)))
            {
                Stack stack;
                Stack::value_type current({ dir->begin(), dir->end(), holder });

                do
                {
                    while (current.first != current.second)
                    {
                        holder = (*current.first);
                        ++current.first;

                        if (::strcmp(holder->as<IEntry>()->type()->name(), Module::DirectoryTypeName) != 0)
                        {
                            if (m_overwrite->askFor(OverwriteFile(m_methods, current.dest, holder)))
                                current.dest->as<IDirectory>()->copy(callback, holder, m_move);
                        }
                        else if ((dir = holder->as<IDirectory>()) &&
                                m_tryier->tryTo(CreateDestinationFolder(m_methods, current.dest, holder, holder)))
                        {
                            stack.push_back(current);
                            current.first = dir->begin();
                            current.second = dir->end();
                            current.dest = holder;
                        }
                    }

                    if (stack.empty())
                        break;
                    else
                    {
                        current = stack.back();
                        stack.pop_back();
                    }
                }
                while (true);
            }

        m_progress.complete();
    }

//    postEvent(new ExtendedEvent(this, Event::CopyFiles, destination(), aborted, m_snapshot, m_move));
}

bool CopyTask::CreateDestinationFolder::operator()()
{
    m_result = m_container->as<IDirectory>()->entry(m_entry->as<IEntry>()->title(), m_entry->as<IEntry>()->type(), true);
    return m_result.isValid();
}

bool CopyTask::CreateDestinationFolder::operator()(Tryier::Flag &flag, const volatile bool &aborted) const
{
    qint32 answer = askUser(tr("Failed to copy..."),
                            tr("Cannot create destination folder \"%1\"").
                                arg(Qt::Node::toUnicode(m_entry->as<IEntry>()->title())).
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

bool CopyTask::OverwriteFile::operator()()
{
    return !m_destination->as<IDirectory>()->exists(m_source->as<IEntry>()->title());
}

bool CopyTask::OverwriteFile::operator()(Tryier::Flag &flag, const volatile bool &aborted) const
{
    qint32 answer = askUser(
                        tr("Failed to copy..."),
                        tr("File \"%1\" from \"%2\" already exists in \"%3\".").
                            arg(Qt::Node::toUnicode(m_source->as<IEntry>()->title())).
                            arg(Qt::Node::toUnicode(m_source->as<IEntry>()->location())).
                            arg(Qt::Node::toUnicode(m_destination->as<IEntry>()->location())).
                            append(QChar(L'\n')).
                            append(tr("Overwrite it?")),
                        QMessageBox::Yes |
                        QMessageBox::YesToAll |
                        QMessageBox::No |
                        QMessageBox::NoToAll |
                        QMessageBox::Cancel,
                        aborted);

    switch (answer)
    {
        case QMessageBox::Yes:
            return true;

        case QMessageBox::YesToAll:
            return flag = true;

        case QMessageBox::No:
            return false;

        case QMessageBox::NoToAll:
            return flag = false;

        case QMessageBox::Cancel:
            cancel();
            break;
    }

    return false;
}

}}}
