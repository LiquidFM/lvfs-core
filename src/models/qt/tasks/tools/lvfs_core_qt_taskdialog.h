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

#ifndef LVFS_CORE_QT_TASKDIALOG_H_
#define LVFS_CORE_QT_TASKDIALOG_H_

#include <efc/Mutex>
#include <efc/WaitCondition>
#include <QtGui/QWidget>
#include <QtGui/QMessageBox>
#include <brolly/assert.h>

#include "../lvfs_core_qt_BaseTask.h"


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE QuestionEvent: public BaseTask::Event
{
public:
	class Result
	{
	public:
		Result() :
			m_answer(0),
			m_done(false)
		{}

		void waitFor(const volatile bool &aborted)
		{
		    EFC::Mutex::Locker lock(m_mutex);

			while (!m_done && !aborted)
				m_condition.wait(m_mutex, 1000);
		}

		qint32 answer() const { return m_answer; }

	private:
		friend class QuestionEvent;
		qint32 m_answer;
		volatile bool m_done;
		EFC::Mutex m_mutex;
		EFC::WaitCondition m_condition;
	};

public:
	QuestionEvent(const QString &title, const QString &question, qint32 buttons, Result *result) :
		Event(NULL, Question, false),
		m_title(title),
		m_question(question),
		m_buttons(buttons),
		m_result(result)
	{
		ASSERT(m_result);
	}

	/* Must be called from the main thread. */
	void showDialog(QWidget *parent);

private:
	QString m_title;
	QString m_question;
	qint32 m_buttons;
	Result *m_result;
};


class PLATFORM_MAKE_PRIVATE UserInputEvent: public BaseTask::Event
{
public:
	class Result
	{
	public:
		Result() :
			m_answer(0),
			m_done(false)
		{}

		void waitFor(const volatile bool &aborted)
		{
		    EFC::Mutex::Locker lock(m_mutex);

			while (!m_done && !aborted)
				m_condition.wait(m_mutex, 1000);
		}

		qint32 answer() const { return m_answer; }
		const QString &value() const { return m_value; }

	private:
		friend class UserInputEvent;
		qint32 m_answer;
		QString m_value;
		volatile bool m_done;
		EFC::Mutex m_mutex;
		EFC::WaitCondition m_condition;
	};

public:
	UserInputEvent(const QString &title, const QString &question, qint32 buttons, Result *result) :
		Event(NULL, UserInput, false),
		m_title(title),
		m_question(question),
		m_buttons(buttons),
		m_result(result)
	{
		ASSERT(m_result);
	}

	/* Must be called from the main thread. */
	void showDialog(QWidget *parent);

private:
	QString m_title;
	QString m_question;
	qint32 m_buttons;
	Result *m_result;
};

}}}

#endif /* LVFS_CORE_QT_TASKDIALOG_H_ */
