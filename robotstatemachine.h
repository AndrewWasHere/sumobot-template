/*
    Copyright 2019, Andrew Lin.

    This source code is released under the 3-Clause BSD license. See 
    LICENSE.txt, or https://opensource.org/licenses/BSD-3-Clause.
 */
#pragma once

#include "robotstate.h"

class RobotStateMachine : public RobotState
{
public:
    RobotStateMachine(State * parent, IRobot & robot);

protected:
    Result on_initialize() override;
};
