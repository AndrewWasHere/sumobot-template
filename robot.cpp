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

// Static helper functions.

static int16_t clip_speed(int16_t speed)
{
    int16_t const max_speed = 400;
    int16_t const min_speed = -max_speed;

    if (speed > max_speed)
    {
        speed = max_speed;
    }
    else if (speed < min_speed)
    {
        speed = min_speed;
    }

    return speed;
}

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
    if (m_encoder_count && abs(m_encoders.getCountsLeft()) > m_encoder_count)
    {
        q.push(&encoder_event);
        m_encoder_count = 0;
    }

    // Check proximity sensor.
    uint8_t const proximity_threshold = 1;
    m_proximity_sensors.read();
    uint8_t brightness_left = m_proximity_sensors.countsFrontWithLeftLeds();
    uint8_t brightness_right = m_proximity_sensors.countsFrontWithRightLeds();
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
    }
    else
    {
        proximity_event.m_direction = NONE;
        q.push(&proximity_event);
    }
}

void IRobot::display(char const * msg)
{
    m_lcd.clear();
    m_lcd.write(msg, strlen(msg));
}

void IRobot::cancel_timer()
{
    m_end_time = 0;
}

void IRobot::start_timer(unsigned long timeout_in_ms)
{
    m_end_time = millis() + timeout_in_ms;
    if (m_end_time == 0)
    {
        // End time cannot be zero, because event won't trigger.
        m_end_time = 1;
    }
}

void IRobot::change_speed_by(int16_t delta)
{
    m_left_motor_speed = clip_speed(m_left_motor_speed + delta);
    m_right_motor_speed = clip_speed(m_right_motor_speed + delta);
    m_motors.setSpeeds(m_left_motor_speed, m_right_motor_speed);
}

void IRobot::change_speed_by(int16_t left_delta, int16_t right_delta)
{
    m_left_motor_speed = clip_speed(m_left_motor_speed + left_delta);
    m_right_motor_speed = clip_speed(m_right_motor_speed + right_delta);
    m_motors.setSpeeds(m_left_motor_speed, m_right_motor_speed);
}

void IRobot::move(int16_t speed)
{
    m_left_motor_speed = m_right_motor_speed = clip_speed(speed);
    m_motors.setSpeeds(m_left_motor_speed, m_right_motor_speed);
}

void IRobot::move(int16_t left_speed, int16_t right_speed)
{
    m_left_motor_speed = clip_speed(left_speed);
    m_right_motor_speed = clip_speed(right_speed);
    m_motors.setSpeeds(m_left_motor_speed, m_right_motor_speed);
}

void IRobot::stop()
{
    m_left_motor_speed = m_right_motor_speed = 0;
    m_motors.setSpeeds(m_left_motor_speed, m_right_motor_speed);
}

void IRobot::spin_left(int16_t degrees, int16_t speed)
{
    m_left_motor_speed = clip_speed(-speed);
    m_right_motor_speed = clip_speed(speed);
    m_encoder_count = degrees * m_encoder_counts_per_degree_rotation;
    m_encoders.getCountsAndResetLeft();
    m_motors.setSpeeds(m_left_motor_speed, m_right_motor_speed);
}

void IRobot::spin_right(int16_t degrees, int16_t speed)
{
    m_left_motor_speed = clip_speed(speed);
    m_right_motor_speed = clip_speed(-speed);
    m_encoder_count = degrees * m_encoder_counts_per_degree_rotation;
    m_encoders.getCountsAndResetLeft();
    m_motors.setSpeeds(m_left_motor_speed, m_right_motor_speed);
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
    m_boundary_sensor.read(sensor_values);
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