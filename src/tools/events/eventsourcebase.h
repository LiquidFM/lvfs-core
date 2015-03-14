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

#ifndef EVENTSOURCEBASE_H_
#define EVENTSOURCEBASE_H_

#include <QtGui/QWidget>
#include <brolly/assert.h>
#include <lvfs-core/tools/templates/mtp.h>
#include <lvfs-core/tools/events/EventHandler>


namespace LVFS {
namespace Tools {

template <typename BaseClass>
class PLATFORM_MAKE_PRIVATE EventSourceBase : public BaseClass
{
public:
	EventSourceBase(EventHandler *eventHandler) :
		BaseClass(),
	    m_eventHandler(eventHandler)
	{
		ASSERT(m_eventHandler != NULL);
	}

    template <typename ... Arguments>
    EventSourceBase(EventHandler *eventHandler, Arguments ... arguments) :
        BaseClass(arguments ...),
        m_eventHandler(eventHandler)
    {
        ASSERT(m_eventHandler != NULL);
    }

protected:
    inline EventHandler *eventHandler() const { return m_eventHandler; }

private:
    EventHandler *m_eventHandler;
};

}}

#endif /* EVENTSOURCEBASE_H_ */
