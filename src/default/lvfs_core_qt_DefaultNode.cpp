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

#include "lvfs_core_qt_DefaultNode.h"

#include <kitemviews/kstandarditem.h>

#include <lvfs/IEntry>
#include <brolly/assert.h>
#include <QtGui/QIcon>


namespace LVFS {
namespace Core {
namespace Qt {

DefaultNode::DefaultNode(const Interface::Holder &file, const Item &parent) :
    ModelNode(parent),
    m_title(toUnicode(file->as<IEntry>()->title())),
    m_schema(toUnicode(file->as<IEntry>()->schema())),
    m_location(toUnicode(file->as<IEntry>()->location())),
    m_file(file)
{
    ASSERT(m_file.isValid());
}

DefaultNode::~DefaultNode()
{}

const QString &DefaultNode::title() const
{
    return m_title;
}

const QString &DefaultNode::schema() const
{
    return m_schema;
}

const QString &DefaultNode::location() const
{
    return m_location;
}

const Interface::Holder &DefaultNode::file() const
{
    return m_file;
}

void DefaultNode::refresh(int depth)
{
    if (!m_files.empty())
        removeChildren();

    doListFile(m_file, depth);
}

DefaultNode::size_type DefaultNode::size() const
{
    return m_files.size();
}

const DefaultNode::Item &DefaultNode::at(size_type index) const
{
    return m_files[index];
}

DefaultNode::size_type DefaultNode::indexOf(const Item &node) const
{
    for (size_type i = 0, size = m_files.size(); i < size; ++i)
        if (m_files[i] == node)
            return i;

    return 0;
}

QVariant DefaultNode::data(int column, int role) const
{
    if (column == 0 && role == ::Qt::DisplayRole)
        return toUnicode(m_file->as<IEntry>()->title());

    if (role == ::Qt::DecorationRole)
    {
        QIcon icon;
        icon.addFile(toUnicode(m_file->as<IEntry>()->type()->icon()->as<IEntry>()->location()), QSize(16, 16));
        return icon;
    }

    return QVariant();
}

QVariant DefaultNode::headerData(int section, ::Qt::Orientation orientation, int role) const
{
    if (section == 0 && role == ::Qt::DisplayRole)
        return QObject::tr("Name");

    return QVariant();
}

void DefaultNode::removeChildren()
{
    beginRemoveRows(QModelIndex(), 0, m_files.size() - 1);
    m_files.clear();
    endRemoveRows();
}

void DefaultNode::processListFile(EFC::List<Item> &files)
{
    beginInsertRows(QModelIndex(), m_files.size(), m_files.size() + files.size() - 1);
    m_files.insert(m_files.end(), files.begin(), files.end());
    endInsertRows();
}

void DefaultNode::doListFileDone(EFC::List<Item> &files)
{
    beginInsertRows(QModelIndex(), m_files.size(), m_files.size() + files.size() - 1);
    m_files.insert(m_files.end(), files.begin(), files.end());
    endInsertRows();
}

}}}
