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

#ifndef LVFS_CORE_QT_TASK_H_
#define LVFS_CORE_QT_TASK_H_

#include <efc/Task>
#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <lvfs/Interface>


namespace LVFS {
namespace Core {
namespace Qt {


class PLATFORM_MAKE_PRIVATE Task : public EFC::Task
{
public:
    class Event : public QEvent
    {
        PLATFORM_MAKE_NONCOPYABLE(Event)

    public:
        enum Type
        {
            Question = User,
            UserInput = User + 1,
            InitProgress = User + 2,
            UpdateProgress = User + 3,
            CompleteProgress = User + 4,
            Action = User + 5,

            /* First user event id. */
            User = Action + 1
        };

    public:
        Task *task;
        bool canceled;

    protected:
        Event(Task *task, Type type, bool canceled) :
            QEvent(static_cast<QEvent::Type>(type)),
            task(task),
            canceled(canceled)
        {}
    };

    class ExtendedEvent : public Event
    {
        PLATFORM_MAKE_NONCOPYABLE(ExtendedEvent)

    public:
        Interface::Holder destination;

    protected:
        ExtendedEvent(Task *task, Type type, bool canceled, const Interface::Holder &dest) :
            Task::Event(task, type, canceled),
            destination(dest)
        {}
    };

public:
    Task(QObject *receiver);
    Task(QObject *receiver, const Interface::Holder &dest);

    const Interface::Holder &destination() const { return m_destination; }

protected:
    /* Posts event to receiver(). */
    void postEvent(Event *event) const;

    /* Sync post event to the GUI thread. */
    qint32 askUser(const QString &title, const QString &question, qint32 buttons, const volatile bool &aborted) const;
    qint32 askForUserInput(const QString &title, const QString &question, qint32 buttons, QString &value, const volatile bool &aborted) const;

private:
    QObject *m_receiver;
    Interface::Holder m_destination;
};

}}}

#endif /* LVFS_CORE_QT_TASK_H_ */
