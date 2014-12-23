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

#include "lvfs_core_qt_Node.h"
#include "tasks/lvfs_core_qt_RefreshTask.h"

#include <QtCore/QThread>
#include <QtCore/QTextCodec>

#include <brolly/assert.h>


namespace LVFS {
namespace Core {
namespace Qt {

static QThread *mainThread = QThread::currentThread();


Node::Node(const Interface::Holder &file, const Interface::Holder &parent) :
    Core::Node(file, parent),
    m_eventsHandler(this),
    m_doListFile(false)
{}

Node::~Node()
{}

QString Node::toUnicode(const char *string)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    return codec->toUnicode(string);
}

QByteArray Node::fromUnicode(const QString &string)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    return codec->fromUnicode(string);
}

void Node::doListFile(int depth)
{
    if (!m_doListFile)
    {
        EFC::Task::Holder task(new (std::nothrow) RefreshTask(&m_eventsHandler, Interface::Holder::fromRawData(this), depth));

        m_doListFile = true;
        handleTask(task);
    }
}

Node::EventsHandler::EventsHandler(Node *node) :
    m_node(node)
{
    moveToThread(mainThread);
}

Node::EventsHandler::~EventsHandler()
{}

bool Node::EventsHandler::event(QEvent *event)
{
    switch (static_cast<FilesBaseTask::Event::Type>(event->type()))
    {
        case FilesBaseTask::Event::ProcessListFileEventId:
        {
            event->accept();
            m_node->processListFile(static_cast<RefreshTask::ListFileEvent *>(event)->snapshot, static_cast<RefreshTask::ListFileEvent *>(event)->isFirstEvent);
            return true;
        }

        case FilesBaseTask::Event::DoneListFileEventId:
        {
            event->accept();
            m_node->m_doListFile = false;
            m_node->doneListFile(static_cast<RefreshTask::ListFileEvent *>(event)->snapshot, static_cast<RefreshTask::ListFileEvent *>(event)->isFirstEvent);
            return true;
        }

        default:
            break;
    }

    return QObject::event(event);
}

}}}
