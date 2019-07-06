/*
    Copyright 2019, Andrew Lin.

    This source code is released under the 3-Clause BSD license. See 
    LICENSE.txt, or https://opensource.org/licenses/BSD-3-Clause.
 */
#pragma once

#include <Zumo32U4.h>
#include "statemachine.h"

using namespace statemachine;

// There must be one event enumerated value for each event class.
enum RobotEvent
{
    BOUNDARY_EVENT,
    ENCODER_EVENT,
    PROXIMITY_EVENT,
    START_EVENT,
    TIMER_EVENT
};

// Direction of detection. Used by boundary and proximity sensors.
enum DetectDirection
{
    NONE,
    LEFT,
    AHEAD,
    RIGHT
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

// Ring boundary detected.
class BoundaryEvent : public Event
{
public:
    BoundaryEvent() : Event(BOUNDARY_EVENT, "bdy"), m_direction(NONE) {}

    DetectDirection m_direction;
};

// Wheel encoder has spun desired amount.
class EncoderEvent : public Event
{
public:
    EncoderEvent() : Event(ENCODER_EVENT, "enc") {}
};

// Proximity sensor detection.
class ProximityEvent : public Event
{
public:
    ProximityEvent() : 
        Event(PROXIMITY_EVENT, "prox"), 
        m_direction(NONE), 
        m_left_brightness(0), 
        m_right_brightness(0) 
    {}
    
    DetectDirection m_direction;
    uint8_t m_left_brightness;
    uint8_t m_right_brightness;
};
