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

#include <lvfs/IFile>
#include <lvfs/IEntry>
#include <lvfs/IFsFile>
#include <brolly/assert.h>

#include <QtGui/QIcon>
#include <QtGui/QClipboard>
#include <QtGui/QApplication>
#include <QtCore/QMimeData>
#include <QtCore/QSet>

#include <lvfs-core/tools/strings/readableints.h>
#include <efc/ScopedPointer>

#include <platform/platform.h>
#define PLATFORM_DE(PLATFORM_FEATURE) PLATFORM_IS_SET(PLATFORM_DE_##PLATFORM_FEATURE)


namespace {

static const QString uriListMimetype = QString::fromLatin1("text/uri-list");
#if PLATFORM_DE(KDE)
    static const QString kdeCutMimetype = QString::fromLatin1("application/x-kde-cutselection");
#endif


class ClipboardMimeData : public QMimeData
{
    PLATFORM_MAKE_NONCOPYABLE(ClipboardMimeData)

public:
    ClipboardMimeData(bool move) :
        QMimeData(),
        m_move(move)
    {}

    void setData(const QByteArray &data)
    {
        QMimeData::setData(uriListMimetype, data);
    }

#if PLATFORM_DE(KDE)
    virtual bool hasFormat(const QString &mimetype) const
    {
        return (m_move && mimetype == kdeCutMimetype) || QMimeData::hasFormat(mimetype);
    }

    virtual QStringList formats() const
    {
        if (m_move)
            return QMimeData::formats() << kdeCutMimetype;

        return QMimeData::formats();
    }

protected:
    virtual QVariant retrieveData(const QString &mimetype, QVariant::Type preferredType) const
    {
        if (mimetype == kdeCutMimetype && m_move)
            return QByteArray("1");

        return QMimeData::retrieveData(mimetype, preferredType);
    }
#endif

private:
    bool m_move;
};

}


namespace LVFS {
namespace Core {
namespace Qt {

DefaultNode::DefaultNode(const Interface::Holder &file, const Item &parent) :
    ModelNode(parent),
    m_file(file),
    m_title(toUnicode(file->as<IEntry>()->title())),
    m_schema(toUnicode(file->as<IEntry>()->schema())),
    m_location(toUnicode(file->as<IEntry>()->location())),
    m_icon(),
    m_geometry({ 300, 80, 50 }),
    m_sorting(0, ::Qt::AscendingOrder),
    m_size(file->as<IFile>() ? humanReadableSize(file->as<IFile>()->size()) : QString::fromLatin1("<DIR>")),
    m_modified(file->as<IFsFile>() ? QDateTime::fromTime_t(file->as<IFsFile>()->mTime()).toLocalTime() : QDateTime())
{
    ASSERT(m_file.isValid());
    ASSERT(m_geometry.size() == columnCount(QModelIndex()));
    ASSERT(m_sorting.first < columnCount(QModelIndex()));
    m_icon.addFile(toUnicode(m_file->as<IEntry>()->type()->icon()->as<IEntry>()->location()), QSize(16, 16));
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

const DefaultNode::Geometry &DefaultNode::geometry() const
{
    return m_geometry;
}

const DefaultNode::Sorting &DefaultNode::sorting() const
{
    return m_sorting;
}

void DefaultNode::refresh(int depth)
{
    removeChildren();
    doListFile(depth);
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
    switch (role)
    {
        case ::Qt::DisplayRole:
            switch (column)
            {
                case 0:
                    return m_title;

                case 1:
                    return m_size;

                case 2:
                    return m_modified;

                default:
                    break;
            }
            break;

        case ::Qt::DecorationRole:
            if (column == 0)
                return m_icon;
            break;

        default:
            break;
    }

    return QVariant();
}

void DefaultNode::copyToClipboard(const QModelIndexList &files, bool move)
{
#if PLATFORM_DE(KDE)
    static const QByteArray suffix = QByteArray::fromRawData("\r\n", qstrlen("\r\n"));
#else
    static const QByteArray suffix = QByteArray::fromRawData("\n", qstrlen("\n"));
#endif

    const IEntry *file;
    EFC::ScopedPointer<ClipboardMimeData> data(new (std::nothrow) ClipboardMimeData(move));

    if (LIKELY(data != NULL))
    {
        QByteArray res;
        QSet<const IEntry *> set;

        set.reserve(files.size());

        for (int i = 0; i < files.size(); ++i)
        {
            file = (*static_cast<Interface::Holder *>(files.at(i).internalPointer()))->as<Core::INode>()->file()->as<IEntry>();

            if (!set.contains(file))
            {
                set.insert(file);
                res.append(file->schema()).append(Module::SchemaDelimiter).append(file->location()).append(suffix);
            }
        }

        data->setData(res);
        QApplication::clipboard()->setMimeData(data.release(), QClipboard::Clipboard);
    }
}

int DefaultNode::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant DefaultNode::headerData(int section, ::Qt::Orientation orientation, int role) const
{
    if (orientation == ::Qt::Horizontal && role == ::Qt::DisplayRole)
        switch (section)
        {
            case 0:
            {
                return tr("Name");
                break;
            }

            case 1:
            {
                return tr("Size");
                break;
            }

            case 2:
            {
                return tr("Modified");
                break;
            }

            default:
                break;
        }

    return QVariant();
}

void DefaultNode::removeChildren()
{
    if (!m_files.empty())
    {
        beginRemoveRows(QModelIndex(), 0, m_files.size() - 1);
        m_files.clear();
        endRemoveRows();
    }
}

void DefaultNode::processListFile(EFC::List<Item> &files, bool isFirstEvent)
{
    if (isFirstEvent)
        removeChildren();

    beginInsertRows(QModelIndex(), m_files.size(), m_files.size() + files.size() - 1);
    m_files.insert(m_files.end(), files.begin(), files.end());
    endInsertRows();
}

void DefaultNode::doneListFile(EFC::List<Item> &files, bool isFirstEvent)
{
    if (isFirstEvent)
        removeChildren();

    if (!files.empty())
    {
        beginInsertRows(QModelIndex(), m_files.size(), m_files.size() + files.size() - 1);
        m_files.insert(m_files.end(), files.begin(), files.end());
        endInsertRows();
    }
}

}}}
