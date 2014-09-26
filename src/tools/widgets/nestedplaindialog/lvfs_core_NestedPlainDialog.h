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

#ifndef LVFS_CORE_NESTEDPLAINDIALOG_H_
#define LVFS_CORE_NESTEDPLAINDIALOG_H_

#include <platform/utils.h>
#include <QtGui/QDialog>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include <lvfs-core/tools/widgets/NestedDialog>


class QEventLoop;

class PLATFORM_MAKE_PUBLIC NestedPlainDialog : public QDialog, public NestedDialog
{
public:
	NestedPlainDialog(QWidget *parent = 0);

    virtual void accept();
	virtual void reject();

protected:
	/* NestedDialog */
	virtual void accepted();
	virtual void rejected();

	/* Message box */
	virtual void warning(const QString &text);
	virtual void warning(const QString &title, const QString &text);
	virtual void critical(const QString &text);
	virtual void critical(const QString &title, const QString &text);

	virtual int open(NestedWidget *widget);

	void setCentralWidget(BaseNestedWidget *widget);

private:
	QEventLoop *m_loop;
	BaseNestedWidget *m_mainWidget;
	NestedWidget *m_currentWidget;

private:
	QVBoxLayout m_vlayout;
	QSplitter m_splitter;
	QDialogButtonBox m_buttonBox;
};

#endif /* LVFS_CORE_NESTEDPLAINDIALOG_H_ */
