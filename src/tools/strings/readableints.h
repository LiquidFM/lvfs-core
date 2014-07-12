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

#ifndef READABLEINTS_H_
#define READABLEINTS_H_

#include <QtCore/QString>


static inline QString humanReadableTime(quint64 msecs)
{
    if (quint64 hours = msecs / (1000 * 60 * 60))
        if (quint64 min = (msecs - hours * 1000 * 60 * 60) / (1000 * 60))
            return QString::number(hours).append(QChar(L':')).append(QString::number(min)).append(QLatin1String(" h"));
        else
            return QString::number(hours).append(QLatin1String(" h"));
    else
        if (hours = msecs / (1000 * 60))
            if (quint64 secs = (msecs - hours * 1000 * 60) / (1000))
                return QString::number(hours).append(QChar(L':')).append(QString::number(secs)).append(QLatin1String(" m"));
            else
                return QString::number(hours).append(QLatin1String(" m"));
        else
            if (hours = msecs / (1000))
                return QString::number(hours).append(QLatin1String(" s"));
            else
                return QString::number(hours).append(QLatin1String(" ms"));
}

static inline QString humanReadableSize(quint64 size)
{
    if (quint64 gigabytes = size / (1024 * 1024 * 1024))
        if (quint64 megabytes = (size - gigabytes * 1024 * 1024 * 1024) / (1024 * 1024))
            return QString::number(gigabytes).append(QChar(L',')).append(QString::number(megabytes)).append(QLatin1String(" Gb"));
        else
            return QString::number(gigabytes).append(QLatin1String(" Gb"));
    else
        if (gigabytes = size / (1024 * 1024))
            if (quint64 kilobytes = (size - gigabytes * 1024 * 1024) / (1024))
                return QString::number(gigabytes).append(QChar(L',')).append(QString::number(kilobytes)).append(QLatin1String(" Mb"));
            else
                return QString::number(gigabytes).append(QLatin1String(" Mb"));
        else
            if (gigabytes = size / (1024))
                if (quint64 bytes = (size - gigabytes * 1024) / (1024))
                    return QString::number(gigabytes).append(QChar(L',')).append(QString::number(bytes)).append(QLatin1String(" Kb"));
                else
                    return QString::number(gigabytes).append(QLatin1String(" Kb"));
            else
                return QString::number(size).append(QLatin1String(" b"));
}

static inline QString humanReadableShortSize(quint64 size)
{
    if (quint64 gigabytes = size / (1024 * 1024 * 1024))
        if (quint64 megabytes = (size - gigabytes * 1024 * 1024 * 1024) / (1024 * 1024))
            return QString::number(gigabytes).append(QChar(L',')).append(QString::number(megabytes)).append(QLatin1String(" Gb"));
        else
            return QString::number(gigabytes).append(QLatin1String(" Gb"));
    else
        if (gigabytes = size / (1024 * 1024))
            if (quint64 kilobytes = (size - gigabytes * 1024 * 1024) / (1024))
                return QString::number(gigabytes).append(QChar(L',')).append(QString::number(kilobytes).mid(0, 1)).append(QLatin1String(" Mb"));
            else
                return QString::number(gigabytes).append(QLatin1String(" Mb"));
        else
            if (gigabytes = size / (1024))
                if (quint64 bytes = (size - gigabytes * 1024) / (1024))
                    return QString::number(gigabytes).append(QChar(L',')).append(QString::number(bytes).mid(0, 1)).append(QLatin1String(" Kb"));
                else
                    return QString::number(gigabytes).append(QLatin1String(" Kb"));
            else
                return QString::number(size).append(QLatin1String(" b"));
}

#endif /* READABLEINTS_H_ */
