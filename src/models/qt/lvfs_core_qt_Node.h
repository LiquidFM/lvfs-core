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

#include <efc/List>
#include <QtCore/QObject>
#include <lvfs-core/INode>
#include <lvfs-core/models/Qt/INode>
#include <lvfs-core/models/Qt/BaseNode>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PUBLIC Node : public Implements<Core::INode, Qt::INode>, public BaseNode
{
public:
    Node(const Interface::Holder &parent);
    virtual ~Node();

    /* Core::INode */
    virtual const Interface::Holder &parent() const;
    virtual void opened(const Interface::Holder &view);
    virtual void closed(const Interface::Holder &view);

protected: /* Actions section */
    void doListFile(int depth = 0);
    virtual void processListFile(EFC::List<Interface::Holder> &files, bool isFirstEvent) = 0;
    virtual void doneListFile(EFC::List<Interface::Holder> &files, bool isFirstEvent) = 0;

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
    EventsHandler m_eventsHandler;
    bool m_doListFile;
};

}}}

#endif /* LVFS_CORE_QT_NODE_H_ */
