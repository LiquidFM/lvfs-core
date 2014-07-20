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

#ifndef LVFS_CORE_QT_NODE_H_
#define LVFS_CORE_QT_NODE_H_

#include <platform/utils.h>
#include <efc/Task>
#include <efc/List>
#include <QtCore/QObject>
#include <lvfs-core/INode>
#include <lvfs-core/models/Qt/INode>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PUBLIC Node : public Implements<Core::INode, Qt::INode>
{
public:
    Node(const Item &parent = Item());
    virtual ~Node();

    virtual const Item &parent() const;

    virtual void opened(const Interface::Holder &view);
    virtual void closed(const Interface::Holder &view);

protected: /* Actions section */
    void doListFile(int depth = 0);
    virtual void processListFile(EFC::List<Item> &files, bool isFirstEvent) = 0;
    virtual void doneListFile(EFC::List<Item> &files, bool isFirstEvent) = 0;

protected: /* Aux section */
    virtual void removeChildren() = 0;

    static QString toUnicode(const char *string);

private:
    class EventsHandler : public QObject
    {
    public:
        EventsHandler(Node *node);
        virtual ~EventsHandler();

        virtual bool event(QEvent *event);

    private:
        Node *m_node;
    };

private:
    int m_links;
    Item m_parent;
    EventsHandler m_eventsHandler;
    bool m_doListFile;
};

}}}

#endif /* LVFS_CORE_QT_NODE_H_ */
