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

#ifndef LVFS_CORE_QT_USERINPUTDIALOG_H_
#define LVFS_CORE_QT_USERINPUTDIALOG_H_

#include <platform/utils.h>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>


class PLATFORM_MAKE_PRIVATE UserInputDialog : public QDialog
{
	Q_OBJECT

public:
    enum Icon
    {
        Information,
        Warning,
        Critical,
        Question
    };

public:
	UserInputDialog(Icon icon, const QString &title, const QString &question, qint32 buttons, QWidget *parent = 0);

	int answer() const { return m_answer; }
	QString value() const { return m_lineEdit.text(); }

private Q_SLOTS:
	void buttonClicked(QAbstractButton *button);

private:
	int m_answer;
	QVBoxLayout m_layout;
	QHBoxLayout m_hLayout;
	QVBoxLayout m_vLayout;
	QLabel m_iconLabel;
	QLabel m_text;
	QLineEdit m_lineEdit;
	QDialogButtonBox m_buttonBox;
};

#endif /* LVFS_CORE_QT_USERINPUTDIALOG_H_ */
