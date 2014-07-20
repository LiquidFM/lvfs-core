/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#ifndef LVFS_CORE_QT_TASKPROGRESS_H_
#define LVFS_CORE_QT_TASKPROGRESS_H_

#include <platform/utils.h>
#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QModelIndex>


namespace LVFS {
namespace Core {
namespace Qt {


class PLATFORM_MAKE_PRIVATE TaskProgress
{
public:
	TaskProgress(QObject *receiver);

	void init(const QModelIndex &item);
	void update(quint64 progressIncrement);
	void complete();
	void clear();

private:
	void postEvent();

private:
	QModelIndex m_item;
	quint64 m_progress;
	QObject *m_receiver;
	QDateTime m_baseTime;
	QDateTime m_currentTime;
	QDateTime m_startTime;
};

}}}

#endif /* LVFS_CORE_QT_TASKPROGRESS_H_ */
