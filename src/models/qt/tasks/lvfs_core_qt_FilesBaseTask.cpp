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

#include "lvfs_core_qt_FilesBaseTask.h"

#include <efc/List>
#include <efc/Pair>
#include <lvfs/IFile>
#include <lvfs/IDirectory>
#include <lvfs/IProperties>
#include <lvfs/Module>


namespace LVFS {
namespace Core {
namespace Qt {

FilesBaseTask::FilesBaseTask(QObject *receiver) :
    Task(receiver)
{}

FilesBaseTask::FilesBaseTask(QObject *receiver, const Interface::Holder &destination) :
    Task(receiver, destination)
{}

uint64_t FilesBaseTask::calculateSize(const Interface::Holder &file)
{
    typedef EFC::List<EFC::Pair<IDirectory::const_iterator, IDirectory::const_iterator>> Stack;

    if (IDirectory *dir = file->as<IDirectory>())
        if (::strcmp(file->as<IEntry>()->type()->name(), Module::DirectoryTypeName) != 0)
        {
            if (IProperties *properties = file->as<IProperties>())
                return properties->size();
        }
        else
        {
            Stack stack;
            uint64_t res = 0;
            IProperties *properties;
            Interface::Holder holder;
            Stack::value_type current(dir->begin(), dir->end());

            do
            {
                while (current.first != current.second)
                {
                    holder = (*current.first);
                    ++current.first;

                    if (dir = holder->as<IDirectory>())
                        if (::strcmp(holder->as<IEntry>()->type()->name(), Module::DirectoryTypeName) != 0)
                        {
                            if (properties = holder->as<IProperties>())
                                res += properties->size();
                        }
                        else
                        {
                            stack.push_back(current);
                            current.first = dir->begin();
                            current.second = dir->end();
                        }
                    else
                        if (properties = holder->as<IProperties>())
                            res += properties->size();
                }

                if (stack.empty())
                    break;
                else
                {
                    current = stack.back();
                    stack.pop_back();
                }
            }
            while (true);

            return res;
        }
    else
        if (IProperties *properties = file->as<IProperties>())
            return properties->size();

    return 0;
}

}}}
