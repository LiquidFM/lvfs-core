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

#include "lvfs_core_settings_StringOption.h"
#include "lvfs_core_settings_Listener.h"

#include <lvfs-core/models/Qt/Node>
#include <QtGui/QLineEdit>


namespace LVFS {
namespace Core {
namespace Settings {

StringOption::~StringOption()
{}

QWidget *StringOption::createEditor(QWidget *parent) const
{
    QLineEdit *editor = new QLineEdit(parent);
    Listener *listener = new Listener(this, &valueChanged, editor);

    editor->setText(Qt::Node::toUnicode(option()->as<LVFS::Settings::StringOption>()->value()));
    QObject::connect(editor, SIGNAL(textChanged(const QString &)), listener, SLOT(valueChanged(const QString &)));

    return editor;
}

void StringOption::valueChanged(const Option *self, const QString &value)
{
    option(self)->as<LVFS::Settings::StringOption>()->setValue(Qt::Node::fromUnicode(value).data());
}

}}}
