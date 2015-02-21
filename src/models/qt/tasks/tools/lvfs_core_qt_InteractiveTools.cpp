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

#include "lvfs_core_qt_InteractiveTools.h"
#include "lvfs_core_qt_InteractiveEvents.h"

#include <efc/ScopedPointer>
#include <brolly/assert.h>
#include <QtGui/QApplication>


namespace LVFS {
namespace Core {
namespace Qt {

TaskProgress::TaskProgress(QObject *receiver) :
    m_total(0),
    m_progress(0),
    m_receiver(receiver)
{}


void TaskProgress::init(const Interface::Holder &file, quint64 total)
{
    ASSERT(file.isValid());
    m_file = file;
    m_total = total;
    m_progress = 0;
    m_baseTime = m_currentTime = m_startTime = QDateTime::currentDateTime();
    postInitEvent();
}

void TaskProgress::update(quint64 progressIncrement)
{
    ASSERT(m_file.isValid());
    m_progress += progressIncrement;

    if (m_baseTime.secsTo(m_currentTime = QDateTime::currentDateTime()) > 1)
    {
        postUpdateEvent();
        m_baseTime = m_currentTime;
    }
}

void TaskProgress::complete()
{
    typedef CompleteProgressEvent Event;

    EFC::ScopedPointer<Event> event(new (std::nothrow) Event(m_file, m_startTime.msecsTo(QDateTime::currentDateTime())));
    QApplication::postEvent(m_receiver, event.release());
}

void TaskProgress::clear()
{
    m_file.reset();
}

void TaskProgress::postInitEvent()
{
    typedef InitProgressEvent Event;

    EFC::ScopedPointer<Event> event(new (std::nothrow) Event(m_file, m_total));
    QApplication::postEvent(m_receiver, event.release());
}

void TaskProgress::postUpdateEvent()
{
    typedef UpdateProgressEvent Event;

    EFC::ScopedPointer<Event> event(new (std::nothrow) Event(m_file, m_progress, m_startTime.msecsTo(m_currentTime)));
    QApplication::postEvent(m_receiver, event.release());
}

}}}
