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

#include "lvfs_core_ChooseDialog.h"
#include <QtGui/QMessageBox>


ChooseDialog::ChooseDialog(const QString &title, const List &values, QWidget *parent) :
	QDialog(parent),
	m_layout(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	setWindowTitle(title);
	m_values.reserve(values.size());

    QFont font;
    font.setBold(true);
    font.setWeight(75);

    m_layout.setSpacing(6);
    m_layout.setMargin(6);

    QRadioButton *button;
    for (List::size_type i = 0, size = values.size(); i < size; ++i)
    {
    	button = new QRadioButton(values.at(i).first, this);
    	button->setFont(font);

    	m_values.push_back(Container::value_type(values.at(i).second, button));
        m_layout.addWidget(button);
    }

    m_layout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void *ChooseDialog::value() const
{
    for (Container::size_type i = 0, size = m_values.size(); i < size; ++i)
    	if (m_values.at(i).second->isChecked())
    		return m_values.at(i).first;

    return 0;
}

void ChooseDialog::accept()
{
	if (check())
		QDialog::accept();
	else
		QMessageBox::information(this, windowTitle(), tr("You must choose one of the values!"));
}

bool ChooseDialog::check()
{
    for (Container::size_type i = 0, size = m_values.size(); i < size; ++i)
    	if (m_values.at(i).second->isChecked())
    		return true;

	return false;
}
