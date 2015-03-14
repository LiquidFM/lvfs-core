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

#ifndef FOCUSEVENTSOURCE_H_
#define FOCUSEVENTSOURCE_H_

#include <lvfs-core/tools/events/EventSourceBase>


namespace LVFS {
namespace Tools {

template <typename BaseClass = EventSourceBase<QWidget>>
class PLATFORM_MAKE_PRIVATE FocusOutEventSource : public BaseClass
{
public:
	FocusOutEventSource(EventHandler *eventHandler) :
		BaseClass(eventHandler)
	{}

    template <typename ... Arguments>
    FocusOutEventSource(EventHandler *eventHandler, Arguments ... arguments) :
        BaseClass(eventHandler, arguments ...)
    {}

protected:
    virtual void focusOutEvent(QFocusEvent *event)
    {
    	if (!BaseClass::eventHandler()->focusOutEvent(event))
    		BaseClass::focusOutEvent(event);
    }
};


template <typename BaseClass = EventSourceBase<QWidget>>
class PLATFORM_MAKE_PRIVATE FocusInEventSource : public BaseClass
{
public:
	FocusInEventSource(EventHandler *eventHandler) :
		BaseClass(eventHandler)
	{}

    template <typename ... Arguments>
    FocusInEventSource(EventHandler *eventHandler, Arguments ... arguments) :
        BaseClass(eventHandler, arguments ...)
    {}

protected:
	virtual void focusInEvent(QFocusEvent *event)
    {
    	if (!BaseClass::eventHandler()->focusInEvent(event))
    		BaseClass::focusInEvent(event);
    }
};

}}

#endif /* FOCUSEVENTSOURCE_H_ */
