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

#ifndef LVFS_CORE_QT_INTERACTIVETOOLS_H_
#define LVFS_CORE_QT_INTERACTIVETOOLS_H_

#include <platform/utils.h>
#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <lvfs/Interface>


namespace LVFS {
namespace Core {
namespace Qt {

class PLATFORM_MAKE_PRIVATE TaskProgress
{
public:
    TaskProgress(QObject *receiver);

    void init(void *id, const Interface::Holder &file, quint64 total);
    void update(quint64 progressIncrement);
    void complete();
    void clear();

private:
    void postInitEvent();
    void postUpdateEvent();

private:
    void *m_id;
    quint64 m_total;
    quint64 m_progress;
    QObject *m_receiver;
    QDateTime m_baseTime;
    QDateTime m_currentTime;
    QDateTime m_startTime;
    Interface::Holder m_file;
};


class PLATFORM_MAKE_PRIVATE Tryier
{
public:
    class Flag
    {
    public:
        inline Flag() :
            m_initialized(false)
        {}

        inline Flag &operator=(bool value) { m_initialized = true; m_value = value; return *this; }

        inline operator bool() const { return m_value; }
        inline operator bool() const volatile { return m_value; }

        inline bool isInitialized() const { return m_initialized; }

    private:
        bool m_initialized;
        bool m_value;
    };

public:
    inline Tryier(const volatile bool &aborted) :
        m_aborted(aborted)
    {}

    template <typename F>
    inline bool askFor(F functor)
    {
        bool res;

        if (!(res = functor()))
            if (m_flag.isInitialized())
                return m_flag;
            else
                res = functor(m_flag, m_aborted);

        return res;
    }

    template <typename F>
    inline bool tryTo(F functor)
    {
        bool res;

        do
            if (res = functor())
                break;
            else
                if (m_flag.isInitialized())
                    break;
                else
                    res = functor(m_flag, m_aborted);
        while (res && !m_aborted);

        return res;
    }

private:
    Flag m_flag;
    const volatile bool &m_aborted;
};

}}}

#endif /* LVFS_CORE_QT_INTERACTIVETOOLS_H_ */
