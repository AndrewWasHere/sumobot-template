/*
    Copyright 2019, Andrew Lin.

    This source code is released under the 3-Clause BSD license. See 
    LICENSE.txt, or https://opensource.org/licenses/BSD-3-Clause.
 */
#pragma once

#include "statemachine.h"

using namespace statemachine;

// There must be one event enumerated value for each event class.
enum PongEvent
{
    START_EVENT,
    TIMER_EVENT,
    BOUNDARY_AHEAD_EVENT,
    BOUNDARY_LEFT_EVENT,
    BOUNDARY_RIGHT_EVENT,
    ENCODER_EVENT
};

// Start button pressed.
class StartButtonEvent : public Event
{
public:
    StartButtonEvent() : Event(START_EVENT, "start") {}
};

// Timer expiration.
class TimerEvent : public Event
{
public:
    TimerEvent() : Event(TIMER_EVENT, "timer") {}
};

// Ring boundary detected to front of robot.
class BoundaryAheadEvent : public Event
{
public:
    BoundaryAheadEvent() : Event(BOUNDARY_AHEAD_EVENT, "bdy-a") {}
};

// Ring boundary detected to left of robot.
class BoundaryLeftEvent : public Event
{
public:
    BoundaryLeftEvent() : Event(BOUNDARY_LEFT_EVENT, "bdy-l") {}
};

// Ring boundary detected to right of robot.
class BoundaryRightEvent : public Event
{
public:
    BoundaryRightEvent() : Event(BOUNDARY_RIGHT_EVENT, "bdy-r") {}
};

// Wheel encoder has spun desired amount.
class EncoderEvent : public Event
{
public:
    EncoderEvent() : Event(ENCODER_EVENT, "enc") {}
};
