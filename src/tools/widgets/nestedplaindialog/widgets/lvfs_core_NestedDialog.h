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

#ifndef LVFS_CORE_NESTEDDIALOG_H_
#define LVFS_CORE_NESTEDDIALOG_H_

#include <lvfs-core/tools/widgets/NestedWidget>


class PLATFORM_MAKE_PUBLIC BaseNestedDialog
{
public:
	virtual ~BaseNestedDialog();

protected:
	void accept(NestedWidget *widget) { widget->accept(); }
	void reject(NestedWidget *widget) { widget->reject(); }
};


class PLATFORM_MAKE_PUBLIC IntermediateNestedDialog : public BaseNestedDialog
{
protected:
    virtual ~IntermediateNestedDialog();

	friend class BaseNestedWidget;
	virtual void accepted() = 0;
	virtual void rejected() = 0;

	/* Message box */
	virtual void warning(const QString &text) = 0;
	virtual void warning(const QString &title, const QString &text) = 0;
	virtual void critical(const QString &text) = 0;
	virtual void critical(const QString &title, const QString &text) = 0;
};


class PLATFORM_MAKE_PUBLIC NestedDialog : public IntermediateNestedDialog
{
protected:
    virtual ~NestedDialog();

	friend class NestedWidget;
	virtual int open(NestedWidget *widget) = 0;
};

#endif /* LVFS_CORE_NESTEDDIALOG_H_ */
