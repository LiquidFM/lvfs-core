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

#ifndef KEYBOARDEVENTHANDLER_H_
#define KEYBOARDEVENTHANDLER_H_

#include <QtCore/QMap>
#include <QtGui/QKeyEvent>
#include <lvfs-core/tools/events/EventHandlerBase>


namespace LVFS {
namespace Tools {

template <
	typename BaseClass = EventHandlerBase<Templates::null_type>,
	typename FallbackToBaseClass = Templates::bool_value<false>,
    typename InterceptEvents = Templates::bool_value<true>
>
class PLATFORM_MAKE_PRIVATE KeyboardEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
    typedef typename Templates::select_first_type_if<typename BaseClass::Method1,
                                                     typename BaseClass::Method2,
                                                     InterceptEvents::value
                                                    >::type Method;

public:
	KeyboardEventHandler(Listener *object) :
		BaseClass(object)
	{}

	virtual bool keyboardEvent(QKeyEvent *event)
	{
		if (InterceptEvents::value)
			if (FallbackToBaseClass::value)
				return this->invokeMethod1(m_hotkeys.value(event->modifiers() + event->key(), NULL), event) ? true : BaseClass::keyboardEvent(event);
			else
				return this->invokeMethod1(m_hotkeys.value(event->modifiers() + event->key(), NULL), event);
		else
			if (FallbackToBaseClass::value)
				return this->invokeMethod2(m_hotkeys.value(event->modifiers() + event->key(), NULL), event) ? true : BaseClass::keyboardEvent(event);
			else
				return this->invokeMethod2(m_hotkeys.value(event->modifiers() + event->key(), NULL), event);
	}

	void registerShortcut(quint32 modifier, Qt::Key key, Method method)
	{
		m_hotkeys[modifier + key] = method;
	}
	void registerShortcut(Qt::Modifier modifier, Qt::Key key, Method method)
	{
		m_hotkeys[modifier + key] = method;
	}
	void registerShortcut(Qt::KeyboardModifier modifier, Qt::Key key, Method method)
	{
		m_hotkeys[modifier + key] = method;
	}

	void unregisterShortcut(quint32 modifier, Qt::Key key)
	{
		m_hotkeys.remove(modifier + key);
	}
	void unregisterShortcut(Qt::Modifier modifier, Qt::Key key)
	{
		m_hotkeys.remove(modifier + key);
	}
	void unregisterShortcut(Qt::KeyboardModifier modifier, Qt::Key key)
	{
		m_hotkeys.remove(modifier + key);
	}

private:
	QMap<quint32, Method> m_hotkeys;
};

}}

#endif /* KEYBOARDEVENTHANDLER_H_ */
