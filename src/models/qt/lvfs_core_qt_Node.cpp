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

#include "lvfs_core_qt_Node.h"
#include "tasks/lvfs_core_qt_RefreshTask.h"
#include "tasks/lvfs_core_qt_CopyTask.h"
#include "tasks/lvfs_core_qt_RemoveTask.h"
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
{}

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

void Node::doListFile(void *id, const Interface::Holder &file)
{
    if (!m_doListFile)
    {
        Task::Holder task(new (std::nothrow) RefreshTask(&m_eventsHandler, id, file));

        m_doListFile = true;
        handleTask(task);
    }
}

void Node::doCopyFiles(FilesToCopy &files, const Interface::Holder &dest, const Interface::Holder &node, bool move)
{
    Task::Holder task(new (std::nothrow) CopyTask(&m_eventsHandler, files, dest, node, move));
    handleTask(task);
}

void Node::doRemoveFiles(FilesToRemove &files)
{
    Task::Holder task(new (std::nothrow) RemoveTask(&m_eventsHandler, files));
    handleTask(task);
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
        case Qt::Task::Event::InitProgress:
        {
            InitProgressEvent *e = static_cast<InitProgressEvent *>(event);

            event->accept();
            m_node->initProgress(e->id, e->item, e->total);
            m_node->doneTask(e->task);
            return true;
        }

        case Qt::Task::Event::UpdateProgress:
        {
            UpdateProgressEvent *e = static_cast<UpdateProgressEvent *>(event);

            event->accept();
            m_node->updateProgress(e->id, e->item, e->progress, e->timeElapsed);
            m_node->doneTask(static_cast<Qt::Task::Event *>(event)->task);
            return true;
        }

        case Qt::Task::Event::CompleteProgress:
        {
            CompleteProgressEvent *e = static_cast<CompleteProgressEvent *>(event);

            event->accept();
            m_node->completeProgress(e->id, e->item, e->timeElapsed);
            m_node->doneTask(static_cast<Qt::Task::Event *>(event)->task);
            return true;
        }

        case Qt::Task::Event::Question:
        {
            event->accept();
            static_cast<QuestionEvent *>(event)->showDialog(QApplication::focusWidget());
            return true;
        }

        case Qt::Task::Event::UserInput:
        {
            event->accept();
            static_cast<UserInputEvent *>(event)->showDialog(QApplication::focusWidget());
            return true;
        }

        case FilesBaseTask::Event::ProcessListFileEventId:
        {
            event->accept();
            m_node->processListFile(static_cast<RefreshTask::Event *>(event)->snapshot, static_cast<RefreshTask::Event *>(event)->isFirstEvent);
            m_node->doneTask(static_cast<Qt::Task::Event *>(event)->task);
            return true;
        }

        case FilesBaseTask::Event::DoneListFileEventId:
        {
            event->accept();
            m_node->m_doListFile = false;
            m_node->doneListFile(static_cast<RefreshTask::Event *>(event)->snapshot, static_cast<RefreshTask::Event *>(event)->error, static_cast<RefreshTask::Event *>(event)->isFirstEvent);
            m_node->doneTask(static_cast<Qt::Task::Event *>(event)->task);
            return true;
        }

        case FilesBaseTask::Event::DoneCopyFilesEventId:
        {
            event->accept();
            m_node->doneCopyFiles(static_cast<CopyTask::Event *>(event)->destination, static_cast<CopyTask::Event *>(event)->files, static_cast<CopyTask::Event *>(event)->move);
            m_node->doneTask(static_cast<Qt::Task::Event *>(event)->task);
            return true;
        }

        case FilesBaseTask::Event::DoneRemoveFilesEventId:
        {
            event->accept();
            m_node->doneRemoveFiles(static_cast<RemoveTask::Event *>(event)->files);
            m_node->doneTask(static_cast<Qt::Task::Event *>(event)->task);
            return true;
        }

        default:
            break;
    }

    return QObject::event(event);
}

}}}
