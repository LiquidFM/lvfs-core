/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2016 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#include "lvfs_core_settings_IntOption.h"
#include "lvfs_core_settings_Listener.h"

#include <QtGui/QSpinBox>


namespace LVFS {
namespace Core {
namespace Settings {

IntOption::~IntOption()
{}

QWidget *IntOption::createEditor(QWidget *parent) const
{
    QSpinBox *editor = new QSpinBox(parent);
    Listener *listener = new Listener(this, &valueChanged, editor);

    editor->setValue(option()->as<LVFS::Settings::IntOption>()->value());
    QObject::connect(editor, SIGNAL(valueChanged(int)), listener, SLOT(valueChanged(int)));

    return editor;
}

void IntOption::valueChanged(const Option *self, int value)
{
    option(self)->as<LVFS::Settings::IntOption>()->setValue(value);
}

}}}
