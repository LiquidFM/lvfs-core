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

#ifndef LVFS_CORE_QT_TASKPROGRESSEVENTS_H_
#define LVFS_CORE_QT_TASKPROGRESSEVENTS_H_

#include <platform/utils.h>
#include <QtCore/QModelIndex>
#include "../lvfs_core_qt_BaseTask.h"


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE InitProgressEvent : public BaseTask::Event
{
public:
    InitProgressEvent(const Interface::Holder &item, quint64 total) :
        BaseTask::Event(NULL, BaseTask::Event::InitProgress, false),
        item(item),
        total(total)
    {}

    Interface::Holder item;
    quint64 total;
};

class PLATFORM_MAKE_PRIVATE UpdateProgressEvent : public BaseTask::Event
{
public:
	UpdateProgressEvent(const Interface::Holder &item, quint64 progress, quint64 timeElapsed) :
		BaseTask::Event(NULL, BaseTask::Event::UpdateProgress, false),
		item(item),
		progress(progress),
		timeElapsed(timeElapsed)
	{}

	Interface::Holder item;
	quint64 progress;
	quint64 timeElapsed;
};


class PLATFORM_MAKE_PRIVATE CompleteProgressEvent : public BaseTask::Event
{
public:
	CompleteProgressEvent(const Interface::Holder &item, quint64 timeElapsed) :
		BaseTask::Event(NULL, BaseTask::Event::CompleteProgress, false),
		item(item),
		timeElapsed(timeElapsed)
	{}

	Interface::Holder item;
	quint64 timeElapsed;
};

}}}

#endif /* LVFS_CORE_QT_TASKPROGRESSEVENTS_H_ */
