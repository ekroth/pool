/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef THREADPOOL_STATE_HPP
#define THREADPOOL_STATE_HPP

namespace kke
{
    class State;
    // Type of work to execute
    enum State_Work
    {
        State_Load,
        State_Unload,
        State_Update
    };

    // Data used by state work
    struct State_Work_Data
    {
        State_Work Work;
        State* State;
        float Elapsed;
    };
}

#endif
