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

#ifndef KEYBOARDEVENTSOURCE_H_
#define KEYBOARDEVENTSOURCE_H_

#include <lvfs-core/tools/events/EventSourceBase>


namespace LVFS {
namespace Tools {

template <typename BaseClass = EventSourceBase<QWidget>>
class PLATFORM_MAKE_PRIVATE KeyboardEventSource : public BaseClass
{
public:
	KeyboardEventSource(EventHandler *eventHandler) :
		BaseClass(eventHandler)
	{}

    template <typename ... Arguments>
    KeyboardEventSource(EventHandler *eventHandler, Arguments ... arguments) :
        BaseClass(eventHandler, arguments ...)
    {}

protected:
    virtual void keyPressEvent(QKeyEvent *event)
    {
    	if (!BaseClass::eventHandler()->keyboardEvent(event))
    		BaseClass::keyPressEvent(event);
    }
};

}}

#endif /* KEYBOARDEVENTSOURCE_H_ */
