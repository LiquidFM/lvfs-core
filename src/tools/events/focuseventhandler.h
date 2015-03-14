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

#ifndef FOCUSEVENTHANDLER_H_
#define FOCUSEVENTHANDLER_H_

#include <QtGui/QFocusEvent>
#include <lvfs-core/tools/events/EventHandlerBase>


namespace LVFS {
namespace Tools {

template <
	typename BaseClass = EventHandlerBase<Templates::null_type>,
	typename FallbackToBaseClass = Templates::bool_value<false>,
    typename InterceptEvents = Templates::bool_value<true>
>
class PLATFORM_MAKE_PRIVATE FocusOutEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
    typedef typename Templates::select_first_type_if<typename BaseClass::Method1,
                                                     typename BaseClass::Method2,
                                                     InterceptEvents::value
                                                    >::type Method;

public:
	FocusOutEventHandler(Listener *object) :
		BaseClass(object),
		m_handler(NULL)
	{}

	virtual bool focusOutEvent(QFocusEvent *event)
	{
		if (InterceptEvents::value)
			if (FallbackToBaseClass::value)
				return this->invokeMethod1(m_handler, event) ? true : BaseClass::focusOutEvent(event);
			else
				return this->invokeMethod1(m_handler, event);
		else
			if (FallbackToBaseClass::value)
				return this->invokeMethod2(m_handler, event) ? true : BaseClass::focusOutEvent(event);
			else
				return this->invokeMethod2(m_handler, event);
	}

	void registerFocusOutEventHandler(Method handler) { m_handler = handler; }
	void unregisterFocusOutEventHandler() { m_handler = 0; }

private:
	Method m_handler;
};


template <
	typename BaseClass = EventHandlerBase<Templates::null_type>,
	typename FallbackToBaseClass = Templates::bool_value<false>,
    typename InterceptEvents = Templates::bool_value<true>
>
class PLATFORM_MAKE_PRIVATE FocusInEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
    typedef typename Templates::select_first_type_if<typename BaseClass::Method1,
                                                     typename BaseClass::Method2,
                                                     InterceptEvents::value
                                                    >::type Method;

public:
	FocusInEventHandler(Listener *object) :
		BaseClass(object),
		m_handler(NULL)
	{}

	virtual bool focusInEvent(QFocusEvent *event)
	{
		if (InterceptEvents::value)
			if (FallbackToBaseClass::value)
				return this->invokeMethod1(m_handler, event) ? true : BaseClass::focusInEvent(event);
			else
				return this->invokeMethod1(m_handler, event);
		else
			if (FallbackToBaseClass::value)
				return this->invokeMethod2(m_handler, event) ? true : BaseClass::focusInEvent(event);
			else
				return this->invokeMethod2(m_handler, event);
	}

	void registerFocusInEventHandler(Method handler) { m_handler = handler; }
	void unregisterFocusInEventHandler() { m_handler = NULL; }

private:
	Method m_handler;
};

}}

#endif /* FOCUSEVENTHANDLER_H_ */
