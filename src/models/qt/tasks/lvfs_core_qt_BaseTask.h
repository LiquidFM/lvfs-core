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

#ifndef LVFS_CORE_QT_BASETASK_H_
#define LVFS_CORE_QT_BASETASK_H_

#include <efc/Task>
#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <lvfs-core/models/Qt/Node>


namespace LVFS {
namespace Core {
namespace Qt {


class PLATFORM_MAKE_PRIVATE BaseTask : public EFC::Task
{
public:
    typedef Qt::Node::Snapshot Snapshot;

    class Event : public QEvent
    {
        PLATFORM_MAKE_NONCOPYABLE(Event)

    public:
        enum Type
        {
            Question = User,
            UserInput = User + 1,
            Progress = User + 2,
            Completed = User + 3,
            Action = User + 4,

            /* First user event id. */
            User = Action + 1
        };

    public:
        BaseTask *task;
        bool canceled;

    protected:
        Event(BaseTask *task, Type type, bool canceled) :
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
        ExtendedEvent(BaseTask *task, Type type, const Interface::Holder &destination, bool canceled) :
            BaseTask::Event(task, type, canceled),
            destination(destination)
        {}
    };

public:
    BaseTask(QObject *receiver);
    BaseTask(QObject *receiver, const Interface::Holder &destination);

    void cancel() { m_canceled = true; }
    const Interface::Holder &destination() const { return m_destination; }

protected:
    /* Posts event to receiver(). */
    void postEvent(Event *event) const;

    /* Sync post event to the GUI thread. */
    qint32 askUser(const QString &title, const QString &question, qint32 buttons, const volatile bool &aborted) const;
    qint32 askForUserInput(const QString &title, const QString &question, qint32 buttons, QString &value, const volatile bool &aborted) const;

private:
    void taskHandled();

private:
    QObject *m_receiver;
    bool m_canceled;
    Interface::Holder m_destination;
};

}}}

#endif /* LVFS_CORE_QT_BASETASK_H_ */
