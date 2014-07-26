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

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include <platform/utils.h>


class QEvent;
class QMouseEvent;
class QKeyEvent;
class QFocusEvent;
class QContextMenuEvent;


namespace LVFS {
namespace Tools {

class PLATFORM_MAKE_PUBLIC EventHandler
{
public:
	virtual ~EventHandler();

	virtual bool mouseReleaseEvent(QMouseEvent *event) = 0;
	virtual bool mouseDoubleClickEvent(QMouseEvent *event) = 0;
	virtual bool keyboardEvent(QKeyEvent *event) = 0;
    virtual bool focusOutEvent(QFocusEvent *event) = 0;
    virtual bool focusInEvent(QFocusEvent *event) = 0;
    virtual bool contextMenuEvent(QContextMenuEvent *event) = 0;
};

}}

#endif /* EVENTHANDLER_H_ */
