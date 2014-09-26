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

#ifndef LVFS_CORE_CHOOSEDIALOG_H_
#define LVFS_CORE_CHOOSEDIALOG_H_

#include <platform/utils.h>
#include <QtCore/QList>
#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QDialogButtonBox>


class PLATFORM_MAKE_PRIVATE ChooseDialog : public QDialog
{
    Q_OBJECT

public:
    typedef QList<QPair<QString, void *>> List;

public:
    ChooseDialog(const QString &title, const List &values, QWidget *parent = 0);

    void *value() const;
    virtual void accept();

private:
    bool check();

private:
    typedef QList<QPair<void *, QRadioButton *>> Container;

private:
    Container m_values;
    QVBoxLayout m_layout;
    QDialogButtonBox m_buttonBox;
};

#endif /* LVFS_CORE_CHOOSEDIALOG_H_ */
