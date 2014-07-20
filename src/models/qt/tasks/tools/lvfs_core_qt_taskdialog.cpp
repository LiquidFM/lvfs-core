/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#include "lvfs_core_qt_taskdialog.h"
#include "lvfs_core_qt_userinputdialog.h"


namespace LVFS {
namespace Core {
namespace Qt {

void QuestionEvent::showDialog(QWidget *parent)
{
	EFC::Mutex::Locker lock(m_result->m_mutex);
	m_result->m_answer = QMessageBox::question(parent, m_title, m_question, QMessageBox::StandardButtons(m_buttons));
	m_result->m_done = true;
	m_result->m_condition.wakeAll();
}

void UserInputEvent::showDialog(QWidget *parent)
{
    EFC::Mutex::Locker lock(m_result->m_mutex);
	UserInputDialog dialog(UserInputDialog::Question, m_title, m_question, m_buttons, parent);

	dialog.exec();
	m_result->m_answer = dialog.answer();
	m_result->m_value = dialog.value();
	m_result->m_done = true;
	m_result->m_condition.wakeAll();
}

}}}
