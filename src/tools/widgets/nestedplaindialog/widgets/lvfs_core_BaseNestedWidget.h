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

#ifndef LVFS_CORE_BASENESTEDWIDGET_H_
#define LVFS_CORE_BASENESTEDWIDGET_H_

#include <platform/utils.h>
#include <QtCore/QObject>
#include <QtCore/QString>


class NestedDialog;

class PLATFORM_MAKE_PUBLIC BaseNestedWidget : public QObject
{
	Q_OBJECT

public:
	BaseNestedWidget(NestedDialog *parent);
	virtual ~BaseNestedWidget();

	virtual QWidget *centralWidget() = 0;
	virtual void setReadOnly(bool value) = 0;
	virtual void setFocus() = 0;

protected:
	NestedDialog *parent() { return m_parent; }

	/* Message box */
	void warning(const QString &text);
	void warning(const QString &title, const QString &text);
	void critical(const QString &text);
	void critical(const QString &title, const QString &text);

protected Q_SLOTS:
	friend class BaseNestedDialog;
	virtual void accept();
	virtual void reject();

private:
	NestedDialog *m_parent;
};

#endif /* LVFS_CORE_BASENESTEDWIDGET_H_ */
