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

#ifndef MOUSEEVENTHANDLER_H_
#define MOUSEEVENTHANDLER_H_

#include <QtGui/QMouseEvent>
#include <lvfs-core/tools/events/EventHandlerBase>


namespace LVFS {
namespace Tools {

template <
	typename BaseClass = EventHandlerBase<Templates::null_type>,
	typename FallbackToBaseClass = Templates::bool_value<false>,
    typename InterceptEvents = Templates::bool_value<true>
>
class PLATFORM_MAKE_PRIVATE MouseReleaseEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
    typedef typename Templates::select_first_type_if<typename BaseClass::Method1,
                                                     typename BaseClass::Method2,
                                                     InterceptEvents::value
                                                    >::type Method;

public:
	MouseReleaseEventHandler(Listener *object) :
		BaseClass(object),
		m_handler(0)
	{}

	virtual bool mouseReleaseEvent(QMouseEvent *event)
	{
		if (InterceptEvents::value)
			if (FallbackToBaseClass::value)
				return this->invokeMethod1(m_handler, event) ? true : BaseClass::mouseReleaseEvent(event);
			else
				return this->invokeMethod1(m_handler, event);
		else
			if (FallbackToBaseClass::value)
				return this->invokeMethod2(m_handler, event) ? true : BaseClass::mouseReleaseEvent(event);
			else
				return this->invokeMethod2(m_handler, event);
	}

	void registerMouseReleaseEventHandler(Method handler) { m_handler = handler; }
	void unregisterMouseReleaseEventHandler() { m_handler = 0; }

private:
	Method m_handler;
};


template <
    typename BaseClass = EventHandlerBase<Templates::null_type>,
    typename InterceptEvents = Templates::bool_value<true>
>
class PLATFORM_MAKE_PRIVATE MouseDoubleClickEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
    typedef typename Templates::select_first_type_if<typename BaseClass::Method1,
                                                     typename BaseClass::Method2,
                                                     InterceptEvents::value
                                                    >::type Method;

public:
	MouseDoubleClickEventHandler(Listener *object) :
		BaseClass(object),
		m_handler(0)
	{}

	virtual bool mouseDoubleClickEvent(QMouseEvent *event)
	{
		if (InterceptEvents::value)
			return this->invokeMethod1(m_handler, event) ? true : BaseClass::mouseDoubleClickEvent(event);
		else
			return this->invokeMethod2(m_handler, event) ? true : BaseClass::mouseDoubleClickEvent(event);
	}

	void registerMouseDoubleClickEventHandler(Method handler) { m_handler = handler; }
	void unregisterMouseDoubleClickEventHandler() { m_handler = 0; }

private:
	Method m_handler;
};

}}

#endif /* MOUSEEVENTHANDLER_H_ */
