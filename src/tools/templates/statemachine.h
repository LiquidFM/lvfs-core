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

#ifndef WORKSPACE_LVFS_CORE_SRC_TOOLS_TEMPLATES_STATEMACHINE_H_
#define WORKSPACE_LVFS_CORE_SRC_TOOLS_TEMPLATES_STATEMACHINE_H_

#include <stddef.h>
#include <platform/platform.h>
#include "loki-0.1.7/Typelist.h"


#define RULE(FromState, Event, ToState, Context, ContextMethod) \
    ::LVFS::Tools::Templates::Rule<Context>::Definition<FromState, Event, ToState, &Context::ContextMethod>


namespace LVFS {
namespace Tools {
namespace Templates {

template <typename Context>
struct PLATFORM_MAKE_PRIVATE Rule
{
    typedef void (Context::*Action)(char *input, int size);

    template <int FromState, int Token, int ToState, Action action>
    struct Definition
    {
        typedef Context rule_context;

        enum
        {
            FROM_STATE = FromState,
            TOKEN = Token,
            TO_STATE = ToState
        };

        static inline void invoke(Context *context, char *input, int size)
        {
            (context->*action)(input, size);
        }
    };
};


struct PLATFORM_MAKE_PRIVATE Data
{
    void *context;
    int state;

    char *iterator;
    char *input;
};


template <typename Context, typename RulesTypeList>
struct PLATFORM_MAKE_PRIVATE ProcessEvent
{
    static inline void process(Data &data)
    {
        typedef typename RulesTypeList::Head RulesTypeList_Item;

        if (RulesTypeList_Item::FROM_STATE == data.state && RulesTypeList_Item::TOKEN == *data.iterator)
        {
            data.state = RulesTypeList_Item::TO_STATE;
            RulesTypeList_Item::invoke(static_cast<typename RulesTypeList_Item::rule_context *>(data.context), data.input, data.iterator - data.input);
            data.input = data.iterator + 1;
            return;
        }

        ProcessEvent<Context, typename RulesTypeList::Tail>::process(data);
    }
};

template <typename Context>
struct PLATFORM_MAKE_PRIVATE ProcessEvent<Context, ::Loki::NullType>
{
    static inline void process(Data &data)
    {}
};


template <typename RulesTypeList>
class PLATFORM_MAKE_PRIVATE StateMachine
{
public:
    typedef typename RulesTypeList::Head::rule_context Context;

public:
    StateMachine(Context &context)
    {
        m_data.context = &context;
        m_data.state = RulesTypeList::Head::FROM_STATE;
        m_data.iterator = NULL;
        m_data.input = NULL;
    }

    StateMachine(Context *context)
    {
        m_data.context = context;
        m_data.state = RulesTypeList::Head::FROM_STATE;
        m_data.iterator = NULL;
        m_data.input = NULL;
    }

    inline int process(char *string)
    {
        for (m_data.input = m_data.iterator = string; *m_data.iterator; ++m_data.iterator)
            ProcessEvent<Context, RulesTypeList>::process(m_data);

        ProcessEvent<Context, RulesTypeList>::process(m_data);

        return m_data.state;
    }

protected:
    Data m_data;
};

}}}

#endif /* WORKSPACE_LVFS_CORE_SRC_TOOLS_TEMPLATES_STATEMACHINE_H_ */
