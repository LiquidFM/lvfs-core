/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2015 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#include <lvfs/IEntry>
#include <lvfs/IProperties>
#include <lvfs/IApplication>
#include <lvfs/IApplications>

#include <lvfs-core/IView>
#include <lvfs-core/IEditor>
#include <lvfs-core/IMainView>
#include <lvfs-core/INodeFactory>
#include <lvfs-core/models/Qt/IView>
#include <lvfs-core/tools/strings/readableints.h>
#include <lvfs-core/tools/widgets/StringDialog>

#include <QtGui/QIcon>
#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtCore/QMimeData>
#include <QtCore/QSet>

#include <efc/Set>
#include <efc/ScopedPointer>
#include <brolly/assert.h>

#include <platform/platform.h>
#define PLATFORM_DE(PLATFORM_FEATURE) PLATFORM_IS_SET(PLATFORM_DE_##PLATFORM_FEATURE)


#include <cstring>

#include <QtCore/QChar>
#define nat_char QChar
#define nat_isdigit(a) (a).isDigit()
#define nat_isspace(a) (a).isSpace()
#define nat_toupper(a) (a).toUpper()
#include <lvfs-core/tools/strings/strnatcmp.h>

static inline bool compareFileNames(const QString &str1, const QString &str2)
{
    return strnatcasecmp(str1.data(), str2.data()) < 0;
}


#undef nat_char
#undef nat_isdigit
#undef nat_isspace
#undef nat_toupper
#include <lvfs-core/tools/strings/strnatcmp.h>

static inline bool compareFileNames(const char *str1, const char *str2)
{
    return strnatcasecmp(str1, str2) < 0;
}


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


extern int DefaultNode_count;


namespace LVFS {
namespace Core {
namespace Qt {

DefaultNode::DefaultNode(const Interface::Holder &file, const Interface::Holder &parent) :
    Complements<Qt::Node, Qt::INode>(file, parent),
    m_geometry({ 300, 80, 50 }),
    m_sorting(0, ::Qt::AscendingOrder)
{
    ASSERT(m_geometry.size() == columnCount(QModelIndex()));
    ASSERT(m_sorting.first < columnCount(QModelIndex()));
    ++DefaultNode_count;
}

DefaultNode::~DefaultNode()
{
    ASSERT(m_files.empty());
    --DefaultNode_count;
}

void DefaultNode::refresh(int depth)
{
    doListFile(depth);
}

void DefaultNode::opened(const Interface::Holder &view)
{
    Complements::opened(view);
    view->as<Qt::IView>()->select(m_currentIndex);
}

void DefaultNode::closed(const Interface::Holder &view)
{
    m_currentIndex = view->as<Qt::IView>()->currentIndex();
    Complements::closed(view);
}

Interface::Holder DefaultNode::accept(const Interface::Holder &view, Files &files)
{
    return file();
}

void DefaultNode::clear()
{
    unsafeClear();
}

Interface::Holder DefaultNode::node(const Interface::Holder &file) const
{
    ASSERT(file.isValid());
    const char *title = file->as<IEntry>()->title();

    for (auto i = m_files.begin(); i != m_files.end(); ++i)
        if (strcmp(i->node->as<Core::INode>()->file()->as<IEntry>()->title(), title) == 0)
            return i->node;

    return Interface::Holder();
}

void DefaultNode::setNode(const Interface::Holder &file, const Interface::Holder &node)
{
    ASSERT(file.isValid());

    if (node.isValid())
    {
        beginInsertRows(QModelIndex(), m_files.size(), m_files.size());
        m_files.push_back(createItem(file, node));
        endInsertRows();
    }
}

QAbstractItemModel *DefaultNode::model() const
{
    return const_cast<DefaultNode *>(this);
}

const DefaultNode::Geometry &DefaultNode::geometry() const
{
    return m_geometry;
}

const DefaultNode::Sorting &DefaultNode::sorting() const
{
    return m_sorting;
}

Interface::Holder DefaultNode::mapToFile(const QModelIndex &index) const
{
    ASSERT(index.isValid());
    return static_cast<Item *>(index.internalPointer())->file;
}

Core::INode::Files DefaultNode::mapToFile(const QModelIndexList &indices) const
{
    ASSERT(!indices.isEmpty());
    EFC::Set<Interface::Holder> set;
    Core::INode::Files res;

    for (int i = 0; i < indices.size(); ++i)
        if (set.insert(static_cast<Item *>(indices.at(i).internalPointer())->file).second)
            res.push_back(static_cast<Item *>(indices.at(i).internalPointer())->file);

    return res;
}

bool DefaultNode::isLocked(const QModelIndex &index, quint64 &progress, quint64 &total) const
{
    ASSERT(index.isValid());
    Item *leftItem = static_cast<Item *>(index.internalPointer());

    if (leftItem->isLocked())
    {
        progress = leftItem->progress;
        total = leftItem->totalSize;

        return true;
    }

    return false;
}

bool DefaultNode::compareItems(const QModelIndex &left, const QModelIndex &right, ::Qt::SortOrder sortOrder) const
{
    Item *leftItem = static_cast<Item *>(left.internalPointer());
    Item *rightItem = static_cast<Item *>(right.internalPointer());

    if (sortOrder == ::Qt::AscendingOrder)
        if (leftItem->isDir)
            if (rightItem->isDir)
                return compareFileNames(leftItem->title, rightItem->title);
            else
                return true;
        else
            if (rightItem->isDir)
                return false;
            else
                return compareFileNames(leftItem->title, rightItem->title);
    else
        if (leftItem->isDir)
            if (rightItem->isDir)
                return !compareFileNames(leftItem->title, rightItem->title);
            else
                return false;
        else
            if (rightItem->isDir)
                return true;
            else
                return !compareFileNames(leftItem->title, rightItem->title);

    return true;
}

void DefaultNode::activated(const Interface::Holder &view, const QModelIndex &index)
{
    ASSERT(index.isValid());
    Item *item = static_cast<Item *>(index.internalPointer());

    if (item->node.isValid())
        view->as<Core::IView>()->mainView()->as<Core::IMainView>()->show(view, item->node);
    else
    {
        if (INodeFactory *factory = item->file->as<INodeFactory>())
            item->node = factory->createNode(item->file, Interface::Holder::fromRawData(this));

        if (item->node.isValid())
            view->as<Core::IView>()->mainView()->as<Core::IMainView>()->show(view, item->node);
        else
        {
            if (item->file->as<IDirectory>())
                item->node = Interface::Holder(new (std::nothrow) Qt::DefaultNode(item->file, Interface::Holder::fromRawData(this)));

            if (item->node.isValid())
                view->as<Core::IView>()->mainView()->as<Core::IMainView>()->show(view, item->node);
            else
            {
                Interface::Holder apps = Module::desktop().applications(item->file->as<IEntry>()->type());

                if (apps.isValid())
                {
                    ASSERT(apps->as<IApplications>() != NULL);
                    IApplications::const_iterator iterator = apps->as<IApplications>()->begin();

                    if (iterator != apps->as<IApplications>()->end())
                    {
                        ASSERT((*iterator)->as<IApplication>() != NULL);
                        (*iterator)->as<IApplication>()->open(item->file->as<IEntry>());
                    }
                }
            }
        }
    }
}

void DefaultNode::view(const Interface::Holder &view, const QModelIndex &index)
{
    ASSERT(index.isValid());
    Item *item = static_cast<Item *>(index.internalPointer());

    if (!item->isLocked())
        if (IEditor *editor = item->file->as<IEditor>())
            editor->view(view->as<Core::IView>()->widget());
}

void DefaultNode::rename(const Interface::Holder &view, const QModelIndex &index)
{
    ASSERT(index.isValid());
    Item *item = static_cast<Item *>(index.internalPointer());

    if (!item->isLocked())
    {
        QString name = item->title;
        StringDialog dialog(tr("Enter new name for \"%1\"").arg(name), tr("Name"), name, view->as<Core::IView>()->widget());

        if (dialog.exec() == QDialog::Accepted)
        {
            QByteArray new_name = fromUnicode(dialog.value());
            Interface::Adaptor<IDirectory> dir = file();

            if (dir->rename(item->file, new_name.data()))
            {
                Interface::Holder new_file = dir->entry(new_name.data());
                ASSERT(new_file.isValid());

                item->file = new_file;
                item->title = toUnicode(new_file->as<IEntry>()->title());
                item->node.reset();

                emit dataChanged(createIndex(index.row(), 0, item), createIndex(index.row(), 0, item));
            }
            else
                QMessageBox::critical(view->as<Core::IView>()->widget(),
                                      tr("Failed to rename \"%1\"").arg(name),
                                      toUnicode(dir->lastError().description()));
        }
    }
}

void DefaultNode::copy(const Interface::Holder &view, Core::INode::Files &files, const Interface::Holder &dest, const Interface::Holder &node, bool move)
{
    QString reason = move ? tr("Moving...") : tr("Copying...");

    for (auto &i : files)
        for (auto q = 0; q < m_files.size(); ++q)
            if (i == m_files[q].file)
            {
                m_files[q].lock(reason, standardIcon(QStyle::SP_BrowserReload, view->as<Core::IView>()->widget()), node);
                emit dataChanged(createIndex(q, 0, &m_files[q]), createIndex(q, 1, &m_files[q]));
                break;
            }

    doCopyFiles(files, dest, node, move);
}

void DefaultNode::copyToClipboard(const Interface::Holder &view, const QModelIndexList &indices, bool move)
{
#if PLATFORM_DE(KDE)
    static const QByteArray suffix = QByteArray::fromRawData("\r\n", qstrlen("\r\n"));
#else
    static const QByteArray suffix = QByteArray::fromRawData("\n", qstrlen("\n"));
#endif

    EFC::ScopedPointer<ClipboardMimeData> data(new (std::nothrow) ClipboardMimeData(move));

    if (LIKELY(data != NULL))
    {
        EFC::Set<Interface::Holder> set;
        const IEntry *file;
        QByteArray res;
        Item *item;

        for (int i = 0; i < indices.size(); ++i)
        {
            item = static_cast<Item *>(indices.at(i).internalPointer());

            if (set.insert(item->file).second)
            {
                file = item->file->as<IEntry>();
                res.append(file->schema()).append(Module::SchemaDelimiter).append(file->location()).append(suffix);
            }
        }

        data->setData(res);
        QApplication::clipboard()->setMimeData(data.release(), QClipboard::Clipboard);
    }
}

void DefaultNode::remove(const Interface::Holder &view, const QModelIndexList &indices)
{
    EFC::Set<Interface::Holder> set;
    EFC::List<QModelIndex> items;
    QStringList list;
    Files files;
    Item *item;

    list.reserve(indices.size());

    for (int i = 0; i < indices.size(); ++i)
    {
        item = static_cast<Item *>(indices.at(i).internalPointer());

        if (set.insert(item->file).second && !item->isLocked())
        {
            list.push_back(toUnicode(item->file->as<IEntry>()->title()));
            items.push_back(indices.at(i));
            files.push_back(item->file);
        }
    }

    if (!files.empty())
    {
        int answer = QMessageBox::question(view->as<Core::IView>()->widget(),
                                           tr("Removing..."),
                                           tr("Would you like remove files:").
                                               append(QChar(L'\n')).
                                               append(list.join(QChar(L'\n'))),
                                           QMessageBox::Yes | QMessageBox::No);

        if (answer == QMessageBox::Yes)
        {
            QString reason = tr("Removing...");

            for (auto &i : items)
            {
                item = static_cast<Item *>(i.internalPointer());
                item->lock(reason, standardIcon(QStyle::SP_BrowserReload, view->as<Core::IView>()->widget()));
                emit dataChanged(createIndex(i.row(), 0, item), createIndex(i.row(), 1, item));
            }

            doRemoveFiles(files);
        }
    }
}

void DefaultNode::createDirectory(const Interface::Holder &view, const QModelIndex &index)
{
    QString name;

    if (index.isValid())
        name = static_cast<Item *>(index.internalPointer())->title;

    StringDialog dialog(tr("Enter name for new directory"), tr("Name"), name, view->as<Core::IView>()->widget());

    if (dialog.exec() == QDialog::Accepted)
    {
        Interface::Adaptor<IDirectory> file = this->file();
        Interface::Holder type = Module::desktop().typeOfDirectory();
        Interface::Holder dir = file->entry(Qt::Node::fromUnicode(dialog.value()), type->as<IType>(), true);

        if (dir.isValid())
        {
            beginInsertRows(QModelIndex(), m_files.size(), m_files.size());
            m_files.push_back(createItem(dir));
            endInsertRows();

            view->as<Qt::IView>()->select(createIndex(m_files.size() - 1, 0, &m_files[m_files.size() - 1]));
        }
        else
            QMessageBox::critical(view->as<Core::IView>()->widget(),
                                  tr("Error"),
                                  Qt::Node::toUnicode(file->lastError().description()));
    }
}

void DefaultNode::cancel(const QModelIndexList &indices)
{
    cancelTasks(mapToFile(indices));
}

int DefaultNode::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<Item *>(parent.internalPointer())->items.size();
    else
        return m_files.size();
}

int DefaultNode::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant DefaultNode::data(const QModelIndex &index, int role) const
{
    const Item &indexNode(index.isValid() ? *static_cast<Item *>(index.internalPointer()) : m_files.at(index.row()));

    switch (role)
    {
        case ::Qt::DisplayRole:
            switch (index.column())
            {
                case 0:
                    return indexNode.title;

                case 1:
                    return indexNode.size;

                case 2:
                    return indexNode.modified;

                default:
                    break;
            }
            break;

        case ::Qt::DecorationRole:
            if (index.column() == 0)
                if (indexNode.isLocked())
                    return indexNode.lockIcon;
                else
                    return indexNode.icon;
            break;

        case ::Qt::ToolTipRole:
            if (indexNode.isLocked())
                return indexNode.lockReason;
            else
                break;

        default:
            break;
    }

    return QVariant();
}

::Qt::ItemFlags DefaultNode::flags(const QModelIndex &index) const
{
    return ::Qt::ItemIsSelectable | ::Qt::ItemIsEnabled;
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

QModelIndex DefaultNode::index(int row, int column, const QModelIndex &parent) const
{
    if (hasIndex(row, column, parent))
        if (parent.isValid())
            return createIndex(row, column, const_cast<Item *>(&(*static_cast<Item *>(parent.internalPointer())).items.at(row)));
        else
            return createIndex(row, column, const_cast<Item *>(&m_files.at(row)));
    else
        return QModelIndex();
}

QModelIndex DefaultNode::parent(const QModelIndex &child) const
{
//    if (child.isValid())
//        if (Core::INode *coreNode = (*static_cast<Item *>(child.internalPointer())).node->as<Core::INode>())
//        {
//            const Interface::Holder &parent = coreNode->parent();
//
//            if (parent.isValid())
//                if (Core::INode *coreParentNode = parent->as<Core::INode>())
//                {
//                    const Interface::Holder &grandParent = coreParentNode->parent();
//
//                    if (grandParent.isValid())
//                        if (Qt::INode *qtGrandParent = grandParent->as<Qt::INode>())
//                            return createIndex(qtGrandParent->indexOf(parent), 0, const_cast<Item *>(&parent));
//                        else
//                            return createIndex(indexOf(parent), 0, const_cast<Item *>(&parent));
//                }
//        }

    return QModelIndex();
}

void DefaultNode::processListFile(Files &files, bool isFirstEvent)
{
    if (isFirstEvent)
        safeClear(files);

    beginInsertRows(QModelIndex(), m_files.size(), m_files.size() + files.size() - 1);
    for (auto i = files.begin(); i != files.end(); i = files.erase(i))
        m_files.push_back(createItem(*i));
    endInsertRows();
}

void DefaultNode::doneListFile(Files &files, const QString &error, bool isFirstEvent)
{
    if (isFirstEvent)
        safeClear(files);

    if (!files.empty())
    {
        beginInsertRows(QModelIndex(), m_files.size(), m_files.size() + files.size() - 1);
        for (auto i = files.begin(); i != files.end(); i = files.erase(i))
            m_files.push_back(createItem(*i));
        endInsertRows();
    }

    if (!m_files.empty())
        for (auto i : views())
            i->as<Qt::IView>()->select(m_currentIndex);

    if (!error.isEmpty())
        QMessageBox::critical(QApplication::focusWidget(), toUnicode(file()->as<IEntry>()->location()), error);
}

void DefaultNode::doneCopyFiles(const Interface::Holder &node, Files &files, bool move)
{
    node->as<Core::INode>()->refresh();

    for (auto i = files.begin(); i != files.end(); i = files.erase(i))
        for (auto q = 0; q < m_files.size(); ++q)
            if ((*i) == m_files[q].file)
            {
                m_files[q].unlock();

                emit dataChanged(createIndex(q, 0, &m_files[q]), createIndex(q, 1, &m_files[q]));

                break;
            }

    if (move)
        refresh();
}

void DefaultNode::doneRemoveFiles(Files &files)
{
    for (auto i = files.begin(); i != files.end(); i = files.erase(i))
        for (auto q = 0; q < m_files.size(); ++q)
            if ((*i) == m_files[q].file)
            {
                m_files[q].unlock();

                emit dataChanged(createIndex(q, 0, &m_files[q]), createIndex(q, 1, &m_files[q]));

                break;
            }

    refresh();
}

void DefaultNode::initProgress(const Interface::Holder &file, quint64 total)
{
    for (auto i = 0; i < m_files.size(); ++i)
        if (m_files[i].file == file)
        {
            m_files[i].totalSize = total;

            QModelIndex index = createIndex(i, 1, &m_files[i]);
            emit dataChanged(index, index);

            break;
        }
}

void DefaultNode::updateProgress(const Interface::Holder &file, quint64 progress, quint64 timeElapsed)
{
    for (auto i = 0; i < m_files.size(); ++i)
        if (m_files[i].file == file)
        {
            m_files[i].progress = progress;

            QModelIndex index = createIndex(i, 1, &m_files[i]);
            emit dataChanged(index, index);

            if (m_files[i].refreshDest)
            {
                m_files[i].refreshDest = false;
                m_files[i].destNode->as<Core::INode>()->refresh();
            }

            break;
        }
}

void DefaultNode::completeProgress(const Interface::Holder &file, quint64 timeElapsed)
{
    for (auto i = 0; i < m_files.size(); ++i)
        if (m_files[i].file == file)
        {
            m_files[i].progress = m_files[i].totalSize;

            QModelIndex index = createIndex(i, 1, &m_files[i]);
            emit dataChanged(index, index);

            break;
        }
}

DefaultNode::Item DefaultNode::createItem(const Interface::Holder &file) const
{
    Item item;
    Interface::Adaptor<IEntry> entry(file);
    Interface::Adaptor<IProperties> properties(file);

    item.isDir = ::strcmp(entry->type()->name(), Module::DirectoryTypeName) == 0;
    item.title = toUnicode(entry->title());
    item.schema = toUnicode(entry->schema());
    item.location = toUnicode(entry->location());
    item.icon.addFile(toUnicode(entry->type()->icon()->as<IEntry>()->location()), QSize(16, 16));

    if (properties.isValid())
    {
        item.size = !item.isDir ? humanReadableSize(properties->size()) : QString::fromLatin1("<DIR>");
        item.modified = QDateTime::fromTime_t(properties->mTime()).toLocalTime();
    }
    else
    {
        item.size = !item.isDir ? tr("<UNKNOWN>") : QString::fromLatin1("<DIR>");
        item.modified = QDateTime();
    }

    item.file = file;
    return item;
}

DefaultNode::Item DefaultNode::createItem(const Interface::Holder &file, const Interface::Holder &node) const
{
    Item item = createItem(file);
    item.node = node;
    return item;
}

void DefaultNode::safeClear(Files &files)
{
    if (!m_files.empty())
    {
        struct LocalAdaptor
        {
            inline Interface::Holder &operator()(EFC::Vector<Item>::iterator &iterator)
            { return iterator->node; }
        };

        EFC::Vector<Item> tmp_files;
        tmp_files.reserve(m_files.size());

        beginRemoveRows(QModelIndex(), 0, m_files.size() - 1);
        {
            for (auto q = m_files.begin(); q != m_files.end();)
                if (q->isLocked())
                {
                    tmp_files.push_back(std::move(*q));
                    q = m_files.erase(q);
                }
                else
                    ++q;

            Core::INode::clear(m_files, LocalAdaptor());
            std::move(m_files.begin(), m_files.end(), std::back_inserter(tmp_files));
            m_files.clear();
        }
        endRemoveRows();

        if (!tmp_files.empty())
        {
            beginInsertRows(QModelIndex(), m_files.size(), m_files.size() + tmp_files.size() - 1);
            m_files = std::move(tmp_files);
            endInsertRows();

            for (auto i = files.begin(), tmp = i; i != files.end(); tmp = i)
            {
                for (auto q = m_files.begin(), end = m_files.end(); q != end; ++q)
                    if (::strcmp((*i)->as<IEntry>()->title(), q->file->as<IEntry>()->title()) == 0)
                    {
                        i = files.erase(i);
                        break;
                    }

                if (tmp == i)
                    ++i;
            }
        }
    }
}

void DefaultNode::unsafeClear()
{
    if (!m_files.empty())
    {
        struct LocalAdaptor
        {
            inline const Interface::Holder &operator()(EFC::Vector<Item>::iterator &iterator) const
            { return iterator->node; }
        };

        beginRemoveRows(QModelIndex(), 0, m_files.size() - 1);
        Core::INode::clear(m_files, LocalAdaptor());
        EFC::Vector<Item> files(std::move(m_files));
        endRemoveRows();

        if (!files.empty())
        {
            beginInsertRows(QModelIndex(), m_files.size(), m_files.size() + files.size() - 1);
            m_files = std::move(files);
            endInsertRows();
        }
    }
}

QModelIndex DefaultNode::index(Item *item) const
{
    if (item->parent)
        for (auto i = item->parent->items.begin(), end = item->parent->items.end(); i != end; ++i)
            if (*i == *item)
                return createIndex(i - item->parent->items.begin(), 0, item);

    for (auto i = m_files.begin(), end = m_files.end(); i != end; ++i)
        if (*i == *item)
            return createIndex(i - m_files.begin(), 0, item);

    return QModelIndex();
}

QModelIndex DefaultNode::parent(Item *item) const
{
//    if (Core::INode *coreNode = item->node->as<Core::INode>())
//        if (coreNode->parent().isValid())
//        {
//            if (Core::INode *coreParentNode = coreNode->parent()->as<Core::INode>())
//                if (coreParentNode->parent().isValid())
//                    if (Qt::INode *qtGrandParentNode = coreParentNode->parent()->as<Qt::INode>())
//                        return createIndex(qtGrandParentNode->indexOf(coreNode->parent()), 0, const_cast<Item *>(&coreNode->parent()));
//
//            return createIndex(indexOf(coreNode->parent()), 0, const_cast<Item *>(&coreNode->parent()));
//        }

    return QModelIndex();
}

//QModelIndex DefaultNode::parent(Item *item, size_type &row) const
//{
//    if (Item *parent = item->parent().as<Node>())
//        if (Item *parentParent = parent->parent().as<Node>())
//        {
//            row = static_cast<Item *>(parent)->indexOf(item);
//            return createIndex(static_cast<Item *>(parentParent)->indexOf(parent), 0, parent);
//        }
//        else
//        {
//            row = static_cast<Item *>(parent)->indexOf(item);
//            return createIndex(indexOf(parent), 0, parent);
//        }
//    else
//    {
//        row = indexOf(item);
//        return QModelIndex();
//    }
//}

}}}
