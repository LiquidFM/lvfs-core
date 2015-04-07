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

#ifndef LVFS_CORE_QT_DEFAULTNODE_H_
#define LVFS_CORE_QT_DEFAULTNODE_H_

#include <efc/Set>
#include <efc/Vector>
#include <QtGui/QIcon>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <lvfs-core/models/Qt/INode>
#include <lvfs-core/models/Qt/Node>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE DefaultNode : public QAbstractItemModel, public Complements<Qt::Node, Qt::INode>
{
    Q_OBJECT

public:
    DefaultNode(const Interface::Holder &file, const Interface::Holder &parent);
    virtual ~DefaultNode();

public: /* Core::INode */
    virtual void refresh(int depth = 0);
    virtual void opened(const Interface::Holder &view);
    virtual void closed(const Interface::Holder &view);
    virtual Interface::Holder accept(const Interface::Holder &view, Files &files);

    virtual void clear();

    virtual Interface::Holder node(const Interface::Holder &file) const;
    virtual void setNode(const Interface::Holder &file, const Interface::Holder &node);

public: /* Qt::INode */
    virtual QAbstractItemModel *model() const;

    virtual const Geometry &geometry() const;
    virtual const Sorting &sorting() const;

    virtual Interface::Holder mapToFile(const QModelIndex &index) const;
    virtual Core::INode::Files mapToFile(const QModelIndexList &indices) const;

    virtual bool isLocked(const QModelIndex &index, quint64 &progress, quint64 &total) const;
    virtual bool compareItems(const QModelIndex &left, const QModelIndex &right, ::Qt::SortOrder sortOrder) const;

    virtual void activated(const Interface::Holder &view, const QModelIndex &file);
    virtual void view(const Interface::Holder &view, const QModelIndex &index);
    virtual void rename(const Interface::Holder &view, const QModelIndex &index);
    virtual void copy(const Interface::Holder &view, Core::INode::Files &files, const Interface::Holder &dest, const Interface::Holder &node, bool move = false);
    virtual void copyToClipboard(const Interface::Holder &view, const QModelIndexList &indices, bool move = false);
    virtual void remove(const Interface::Holder &view, const QModelIndexList &indices);
    virtual void createDirectory(const Interface::Holder &view, const QModelIndex &index);
    virtual void cancel(const QModelIndexList &indices);

public: /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role = ::Qt::DisplayRole) const;
    virtual ::Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, ::Qt::Orientation orientation, int role = ::Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

protected: /* Core::Qt::Node */
    virtual void processListFile(Files &files, bool isFirstEvent);
    virtual void doneListFile(Files &files, const QString &error, bool isFirstEvent);
    virtual void doneCopyFiles(const Interface::Holder &node, Files &files, bool move);
    virtual void doneRemoveFiles(Files &files);

    virtual void initProgress(const Interface::Holder &file, quint64 total);
    virtual void updateProgress(const Interface::Holder &file, quint64 progress, quint64 timeElapsed);
    virtual void completeProgress(const Interface::Holder &file, quint64 timeElapsed);

private:
    struct Item
    {
        Item(Item *parent = 0) :
            isDir(false),
            parent(parent),
            progress(0),
            totalSize(0),
            refreshDest(false)
        {}

        inline bool operator==(const Item &other) const
        { return other.file == file; }

        inline bool isLocked() const
        { return !lockReason.isEmpty(); }

        inline void lock(const QString &reason, const QIcon &icon)
        { lockReason = reason; lockIcon = icon; }

        inline void lock(const QString &reason, const QIcon &icon, const Interface::Holder &dest)
        { lockReason = reason; lockIcon = icon; destNode = dest; refreshDest = true; }

        inline void unlock()
        { lockIcon = QIcon(); lockReason.clear(); progress = 0; totalSize = 0; destNode.reset(); }

        bool isDir;
        Item *parent;
        QString title;
        QString schema;
        QString location;
        QIcon icon;
        QString size;
        QDateTime modified;
        Interface::Holder file;
        Interface::Holder node;
        EFC::Vector<Item> items;

        QIcon lockIcon;
        QString lockReason;
        quint64 progress;
        quint64 totalSize;
        bool refreshDest;
        Interface::Holder destNode;
    };

    Item createItem(const Interface::Holder &file) const;
    Item createItem(const Interface::Holder &file, const Interface::Holder &node) const;

    void safeClear(Files &files);
    void unsafeClear();

    QModelIndex index(Item *item) const;
    QModelIndex parent(Item *item) const;
//    QModelIndex parent(Item *item, size_type &row) const;

private:
    EFC::Vector<Item> m_files;
    QModelIndex m_currentIndex;
    Geometry m_geometry;
    Sorting m_sorting;
};

}}}

#endif /* LVFS_CORE_QT_DEFAULTNODE_H_ */
