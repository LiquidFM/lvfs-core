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

#ifndef LVFS_CORE_QT_NODE_H_
#define LVFS_CORE_QT_NODE_H_

#include <efc/List>
#include <efc/Pair>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <lvfs-core/models/Node>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PUBLIC Node : public Core::Node
{
public:
    typedef EFC::List<EFC::Pair<Interface::Holder, Interface::Holder>> Snapshot;

public:
    Node(const Interface::Holder &file, const Interface::Holder &parent);
    virtual ~Node();

    static QString toUnicode(const char *string);
    static QByteArray fromUnicode(const QString &string);

protected: /* Actions section */
    void doListFile(int depth = 0);
    virtual void processListFile(Snapshot &files, bool isFirstEvent) = 0;
    virtual void doneListFile(Snapshot &files, bool isFirstEvent) = 0;

    void doCopyFiles(const Interface::Holder &dest, Files &files, bool move = false);
    virtual void doneCopyFiles(Files &files) = 0;

    virtual void initProgress(const Interface::Holder &file, quint64 total) = 0;
    virtual void updateProgress(const Interface::Holder &file, quint64 progress, quint64 timeElapsed) = 0;
    virtual void completeProgress(const Interface::Holder &file, quint64 timeElapsed) = 0;

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
