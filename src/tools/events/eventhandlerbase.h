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

#ifndef EVENTHANDLERBASE_H_
#define EVENTHANDLERBASE_H_

#include <brolly/assert.h>
#include <lvfs-core/tools/templates/mtp.h>
#include <lvfs-core/tools/events/EventHandler>


namespace LVFS {
namespace Tools {

template <typename ListenerType>
class EventHandlerBase : public EventHandler
{
public:
	typedef ListenerType Listener;
	typedef void (Listener::*Method)();

public:
	EventHandlerBase(Listener *object) :
	    m_object(object)
	{
		ASSERT(m_object != NULL);
	}

	virtual bool mouseReleaseEvent(QMouseEvent *event) { return false; }
	virtual bool mouseDoubleClickEvent(QMouseEvent *event) { return false; }
	virtual bool keyboardEvent(QKeyEvent *event) { return false; }
    virtual bool focusOutEvent(QFocusEvent *event) { return false; }
    virtual bool focusInEvent(QFocusEvent *event) { return false; }
    virtual bool contextMenuEvent(QContextMenuEvent *event) { return false; }

protected:
    inline Listener *listener() const { return m_object; }

    inline bool invokeMethod1(Method method, QEvent *event)
    {
		if (method)
		{
			(m_object->*method)();
			event->accept();
			return true;
		}

		return false;
    }
    inline bool invokeMethod2(Method method, QEvent *event)
    {
		if (method)
			(m_object->*method)();

		return false;
    }

private:
    Listener *m_object;
};

}}

#endif /* EVENTHANDLERBASE_H_ */
