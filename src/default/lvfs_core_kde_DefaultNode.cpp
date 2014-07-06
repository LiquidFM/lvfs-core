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

#include "lvfs_core_kde_DefaultNode.h"

#include <kitemviews/kstandarditem.h>

#include <lvfs/IEntry>
#include <brolly/assert.h>
#include <QtGui/QIcon>


namespace LVFS {
namespace Core {
namespace Kde {

DefaultNode::DefaultNode(const Interface::Holder &file) :
    m_file(file)
{
    ASSERT(m_file.isValid());

    KStandardItem *parent = new KStandardItem(QString::fromLatin1("Parent 3"));
    parent->addChildren(new KStandardItem(QString::fromLatin1("Item 3.1"), parent));
    parent->addChildren(new KStandardItem(QString::fromLatin1("Item 3.2"), parent));

    appendItem(parent);
}

DefaultNode::~DefaultNode()
{}

void DefaultNode::refresh()
{
//    doListFile(m_file);
}

const IEntry *DefaultNode::entry() const
{
    Interface::Adaptor<IEntry> ent(m_file);
    ASSERT(ent.isValid());
    return ent.get();
}

const Interface::Holder &DefaultNode::file() const
{
    return m_file;
}

DefaultNode::size_type DefaultNode::size() const
{
    return m_files.size();
}

Interface::Holder DefaultNode::at(size_type index) const
{
    return m_files[index];
}

DefaultNode::size_type DefaultNode::indexOf(const Interface::Holder &node) const
{
    for (size_type i = 0, size = m_files.size(); i < size; ++i)
        if (m_files[i] == node)
            return i;

    return 0;
}

KItemModelBase *DefaultNode::model() const
{
    return const_cast<DefaultNode *>(this);
}

bool DefaultNode::setExpanded(int index, bool expanded)
{
    if (index == 0)
        return true;

    return false;
}

bool DefaultNode::isExpanded(int index) const
{
    if (index == 0)
        return true;

    return false;
}

bool DefaultNode::isExpandable(int index) const
{
    if (index == 0)
        return true;

    return false;
}

int DefaultNode::expandedParentsCount(int index) const
{
    if (index == 1 || index == 2)
        return 1;

    return 0;
}

//QVariant DefaultNode::data(int column, int role) const
//{
//    if (column == 0 && role == Qt::DisplayRole)
//        return QString::fromUtf8(m_file->as<IEntry>()->title());
//
//    if (role == Qt::DecorationRole)
//    {
//        QIcon icon;
//        icon.addFile(QString::fromUtf8(m_file->as<IEntry>()->type()->icon()->as<IEntry>()->location()), QSize(16, 16));
//        return icon;
//    }
//
//    return QVariant();
//}
//
//void DefaultNode::processListFile(EFC::List<Node::Holder> &files)
//{
//    m_files.insert(m_files.end(), files.begin(), files.end());
//}
//
//void DefaultNode::doListFileDone(EFC::List<Node::Holder> &files)
//{
//    m_files.insert(m_files.end(), files.begin(), files.end());
//}

}}}
