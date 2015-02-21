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
#include "tasks/lvfs_core_qt_CopyTask.h"
#include "tasks/tools/lvfs_core_qt_InteractiveEvents.h"

#include <QtGui/QWidget>
#include <QtCore/QThread>
#include <QtCore/QTextCodec>
#include <QtGui/QApplication>

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
{
    cancelTasks();
}

void Node::opened(const Interface::Holder &view)
{
    m_views.insert(view);
}

void Node::closed(const Interface::Holder &view)
{
    m_views.erase(view);
}

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

QIcon Node::standardIcon(QStyle::StandardPixmap icon, QWidget *widget)
{
    QStyle *style = widget ? widget->style() : QApplication::style();
    return style->standardIcon(icon, NULL, widget);
}

QPixmap Node::standardIcon(QStyle::StandardPixmap icon, QStyle::PixelMetric metric, QWidget *widget)
{
    QStyle *style = widget ? widget->style() : QApplication::style();
    QIcon tmpIcon = style->standardIcon(icon, 0, widget);

    if (tmpIcon.isNull())
        return QPixmap();
    else
    {
        int iconSize = style->pixelMetric(metric, NULL, widget);
        return tmpIcon.pixmap(iconSize, iconSize);
    }
}

void Node::doListFile(int depth)
{
    if (!m_doListFile)
    {
        EFC::Task::Holder task(new (std::nothrow) RefreshTask(&m_eventsHandler, Interface::Holder::fromRawData(this), depth));

        m_doListFile = true;
        handleTask(task, file());
    }
}

void Node::doCopyFiles(const Interface::Holder &dest, Files &files, bool move)
{
    if (!m_doListFile)
    {
        EFC::Task::Holder task(new (std::nothrow) CopyTask(&m_eventsHandler, files, dest, move));

        m_doListFile = true;
        handleTask(task, static_cast<CopyTask *>(task.get())->files());
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
        case Task::Event::InitProgress:
        {
            event->accept();
            m_node->doneTask(static_cast<Task::Event *>(event)->task);
            m_node->initProgress(static_cast<InitProgressEvent *>(event)->item, static_cast<InitProgressEvent *>(event)->total);
            return true;
        }

        case Task::Event::UpdateProgress:
        {
            event->accept();
            m_node->doneTask(static_cast<Task::Event *>(event)->task);
            m_node->updateProgress(static_cast<UpdateProgressEvent *>(event)->item, static_cast<UpdateProgressEvent *>(event)->progress, static_cast<UpdateProgressEvent *>(event)->timeElapsed);
            return true;
        }

        case Task::Event::CompleteProgress:
        {
            event->accept();
            m_node->doneTask(static_cast<Task::Event *>(event)->task);
            m_node->completeProgress(static_cast<CompleteProgressEvent *>(event)->item, static_cast<CompleteProgressEvent *>(event)->timeElapsed);
            return true;
        }

        case Task::Event::Question:
        {
            event->accept();
            static_cast<QuestionEvent *>(event)->showDialog(QApplication::focusWidget());
            return true;
        }

        case Task::Event::UserInput:
        {
            event->accept();
            static_cast<UserInputEvent *>(event)->showDialog(QApplication::focusWidget());
            return true;
        }

        case FilesBaseTask::Event::ProcessListFileEventId:
        {
            event->accept();
            m_node->doneTask(static_cast<Task::Event *>(event)->task);
            m_node->processListFile(static_cast<RefreshTask::Event *>(event)->snapshot, static_cast<RefreshTask::Event *>(event)->isFirstEvent);
            return true;
        }

        case FilesBaseTask::Event::DoneListFileEventId:
        {
            event->accept();
            m_node->doneTask(static_cast<Task::Event *>(event)->task);
            m_node->m_doListFile = false;
            m_node->doneListFile(static_cast<RefreshTask::Event *>(event)->snapshot, static_cast<RefreshTask::Event *>(event)->isFirstEvent);
            return true;
        }

        case FilesBaseTask::Event::DoneCopyFilesEventId:
        {
            event->accept();
            m_node->doneTask(static_cast<Task::Event *>(event)->task);
            m_node->doneCopyFiles(static_cast<CopyTask::Event *>(event)->files);
            return true;
        }

        default:
            break;
    }

    return QObject::event(event);
}

}}}
