/**
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

#include "lvfs_core_qt_BaseTask.h"
#include "tools/lvfs_core_qt_taskdialog.h"

#include <QtGui/QApplication>
#include <QtCore/QThread>

#include <brolly/assert.h>


namespace LVFS {
namespace Core {
namespace Qt {

BaseTask::BaseTask(QObject *receiver) :
    Task(),
    m_receiver(receiver),
    m_canceled(false)
{
    ASSERT(m_receiver != NULL);
}

BaseTask::BaseTask(QObject *receiver, const Interface::Holder &destination) :
    Task(),
    m_receiver(receiver),
    m_canceled(false),
    m_destination(destination)
{
    ASSERT(m_receiver != NULL);
    ASSERT(m_destination.isValid());
    ASSERT(m_receiver->thread() == QThread::currentThread());
}

void BaseTask::postEvent(Event *event) const
{
    QApplication::postEvent(m_receiver, event);
}

qint32 BaseTask::askUser(const QString &title, const QString &question, qint32 buttons, const volatile bool &aborted) const
{
    QuestionEvent::Result result;
    postEvent(new QuestionEvent(title, question, buttons, &result));
    result.waitFor(aborted);
    return result.answer();
}

qint32 BaseTask::askForUserInput(const QString &title, const QString &question, qint32 buttons, QString &value, const volatile bool &aborted) const
{
    UserInputEvent::Result result;
    postEvent(new UserInputEvent(title, question, buttons, &result));
    result.waitFor(aborted);
    value = result.value();
    return result.answer();
}

}}}
