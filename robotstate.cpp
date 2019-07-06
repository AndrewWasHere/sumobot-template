/*
    Copyright 2019, Andrew Lin.

    This source code is released under the 3-Clause BSD license. See 
    LICENSE.txt, or https://opensource.org/licenses/BSD-3-Clause.
 */
#include "robotstate.h"

using namespace statemachine;

RobotState::RobotState(char const * name, State * parent, IRobot & robot) :
    State(name, parent), m_robot(robot)
{
}

Result RobotState::transition_to_state(State & state)
{
    Result r = State::transition_to_state(state);
    m_robot.display(active_state_name());

    return r;
}
bool RobotState::on_event(Event & event)
{
    bool handled = false;

    switch(event.m_id)
    {
        case BOUNDARY_EVENT:
            handled = on_event(static_cast<BoundaryEvent &>(event));
            break;
        case ENCODER_EVENT:
            handled = on_event(static_cast<EncoderEvent &>(event));
            break;
        case PROXIMITY_EVENT:
            handled = on_event(static_cast<ProximityEvent &>(event));
            break;
        case START_EVENT:
            handled = on_event(static_cast<StartButtonEvent &>(event));
            break;
        case TIMER_EVENT:
            handled = on_event(static_cast<TimerEvent &>(event));
            break;
        default:
            break;
    }
    return handled;
}

bool RobotState::on_event(BoundaryEvent & event)
{ 
    return false; 
}

bool RobotState::on_event(EncoderEvent & event)
{
    return false;
}

bool RobotState::on_event(ProximityEvent & event)
{
    return false;
}

bool RobotState::on_event(StartButtonEvent & event)
{
    return false; 
}

bool RobotState::on_event(TimerEvent & event)
{ 
    return false; 
}
