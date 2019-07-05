/*
    Copyright 2019, Andrew Lin.

    This source code is released under the 3-Clause BSD license. See 
    LICENSE.txt, or https://opensource.org/licenses/BSD-3-Clause.
 */
#include "events.h"
#include "robot.h"

// Events.

BoundaryEvent boundary_event;
EncoderEvent encoder_event;
StartButtonEvent start_event;
TimerEvent timer_event;
ProximityEvent proximity_event;

// IRobot methods.

void IRobot::setup()
{
    m_end_time = 0;
    m_encoder_count = 0;

    // Set up accelerometer.
    m_accelerometer.init();

    // Set up line sensors.
    m_boundary_sensor.initThreeSensors();

    // Set up gyro.
//    m_gyro.init();

    // Set up proximity sensors.
    m_proximity_sensors.initFrontSensor();
}

void IRobot::generate_events(EventQueue & q)
{
    // Check start button.
    if (m_start_button.getSingleDebouncedPress())
    {
        q.push(&start_event);
    }

    // Check timer.
    if (m_end_time && millis() >= m_end_time)
    {
        q.push(&timer_event);
        m_end_time = 0;
    }

    // Check boundary sensors.
    switch(boundary_detect())
    {
    case BOUNDARY_AHEAD:
        boundary_event.m_direction = AHEAD;
        q.push(&boundary_event);
        break;
    case BOUNDARY_LEFT:
        boundary_event.m_direction = LEFT;
        q.push(&boundary_event);
        break;
    case BOUNDARY_RIGHT:
        boundary_event.m_direction = RIGHT;
        q.push(&boundary_event);
        break;
    default:
        boundary_event.m_direction = NONE;
        // Do not push event onto queue.
        break;
    }

    // Check encoders.
    if (m_encoder_count && abs(encoders.getCountsLeft()) > m_encoder_count)
    {
        q.push(&encoder_event);
        m_encoder_count = 0;
    }

    // Check proximity sensor.
    uint8_t const proximity_threshold = 1;
    proximity_sensors.read();
    uint8_t brightness_left = proximity_sensors.countsFrontWithLeftLeds();
    uint8_t brightness_right = proximity_sensors.countsFrontWithRightLeds();
    if (
        brightness_left >= proximity_threshold || 
        brightness_right >= proximity_threshold
    )
    {
        // Object detected.
        if (brightness_left > brightness_right)
        {
            proximity_event.m_direction = LEFT;
            q.push(&proximity_event);
        }
        else if (brightness_right > brightness_left)
        {
            proximity_event.m_direction = RIGHT;
            q.push(&proximity_event);
        }
        else
        {
            proximity_event.m_direction = AHEAD;
            q.push(&proximity_event);
        }
        */
    }
    else
    {
        proximity_event.m_direction = NONE;
        q.push(&proximity_event);
    }
}

//
// State Machine Interfaces
//
void IRobot::display(char const * msg)
{
    lcd.clear();
    lcd.write(msg, strlen(msg));
}

void IRobot::cancel_timer()
{
    m_end_time = 0;
}

void IRobot::start_timer(unsigned long timeout)
{
    m_end_time = millis() + timeout;
}

void IRobot::move_forward(int speed)
{
    motors.setSpeeds(speed, speed);
}

void IRobot::move_stop()
{
    motors.setSpeeds(0, 0);
}

void IRobot::rotate_left(long degrees, int speed)
{
    m_encoder_count = degrees * m_encoder_counts_per_degree_rotation;
    encoders.getCountsAndResetLeft();
    motors.setSpeeds(-speed, speed);
}

void IRobot::rotate_right(long degrees, int speed)
{
    m_encoder_count = degrees * m_encoder_counts_per_degree_rotation;
    encoders.getCountsAndResetLeft();
    motors.setSpeeds(speed, -speed);
}

void IRobot::cancel_encoder()
{
    m_encoder_count = 0;
}

//
// Private methods.
//
Boundary IRobot::boundary_detect()
{
    // below threshold => boundary.
    // above threshold => ring.
    const unsigned int threshold = 250;

    unsigned int sensor_values[3];
    boundary_sensor.read(sensor_values);
    bool left_boundary = sensor_values[0] < threshold;
    bool center_boundary = sensor_values[1] < threshold;
    bool right_boundary = sensor_values[2] < threshold;

    Boundary boundary = NO_BOUNDARY;
    if (center_boundary || (left_boundary && right_boundary))
    {
        boundary = BOUNDARY_AHEAD;
    }
    else if (left_boundary)
    {
        boundary = BOUNDARY_LEFT;
    }
    else if (right_boundary)
    {
        boundary = BOUNDARY_RIGHT;
    }

    return boundary;
}