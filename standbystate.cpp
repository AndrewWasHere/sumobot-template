/*
    Copyright 2019, Andrew Lin.

    This source code is released under the 3-Clause BSD license. See 
    LICENSE.txt, or https://opensource.org/licenses/BSD-3-Clause.
 */
#include "standbystate.h"

StandbyState::StandbyState(State * parent, IRobot & robot) :
    RobotState("standby", parent, robot)
{}

Result StandbyState::on_entry() 
{
    m_robot.start_timer(5000);
    return OK;
}

bool StandbyState::on_event(TimerEvent & event)
{
    return true;
}