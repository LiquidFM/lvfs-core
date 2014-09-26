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

#include "lvfs_core_ValidStringDialog.h"
#include <QtGui/QMessageBox>


ValidStringDialog::ValidStringDialog(const QString &title, const QString &label, const QString &regexp, QWidget *parent) :
	StringDialog(title, label, QString(), parent),
    m_regexp(regexp)
{}

void ValidStringDialog::accept()
{
	QString res = value();

	if (res.isEmpty())
		QMessageBox::information(this, windowTitle(), tr("You must enter the value!"));
	else
		if (m_regexp.exactMatch(res))
			QDialog::accept();
		else
			QMessageBox::information(this, windowTitle(), tr("You must enter the valid value!"));
}
