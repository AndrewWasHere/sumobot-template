/*
    Copyright 2019, Andrew Lin.

    This source code is released under the 3-Clause BSD license. See 
    LICENSE.txt, or https://opensource.org/licenses/BSD-3-Clause.

    Rename this file to be whatever the directory name is plus the .ino
    extension.
 */
#include <Wire.h>
#include <Zumo32U4.h>
#include "eventqueue.h"
#include "events.h"
#include "initstate.h"
#include "robot.h"
#include "robotstatemachine.h"
#include "standbystate.h"

// Robot interface.
IRobot robot;

// State machine.
RobotStateMachine machine(nullptr, robot);
InitState initialized(&machine, robot);
StandbyState standby(&machine, robot);

EventQueue queue;

void setup()
{
    // Initialize robot.
    robot.setup();

    // Initialize state machine.
    machine.transition_to_state(machine);
}

void loop()
{
    // Read sensors, and generate events.
    robot.generate_events(queue);

    // Process events.
    while (!queue.empty())
    {
        Event * e = queue.pop();
        machine.handle_event(*e);
    }
}
