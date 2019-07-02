/*
    Copyright 2019, Andrew Lin.

    This source code is released under the 3-Clause BSD license. See 
    LICENSE.txt, or https://opensource.org/licenses/BSD-3-Clause.
 */
#pragma once

#include "events.h"

// QUEUE_SIZE should equal, at most, the number of event types, since each event
// is statically allocated in the .ino file, and in a worst case, at most all
// events get triggered.
#define QUEUE_SIZE 6

class EventQueue
{
public:
    EventQueue();
    bool empty();
    void push(Event * e);
    Event * pop();

private:
    unsigned int m_write_idx;
    unsigned int m_read_idx;
    Event * m_queue[6];
};