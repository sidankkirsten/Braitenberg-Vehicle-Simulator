/**
 * @file robot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>
#include <iostream>

#include "src/light_sensor.h"
#include "src/params.h"
#include "src/sensor.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

 /* This is called when the robot is notified by food in the arena
 */
void LightSensor::CalculateSensorReading(Pose light_pose, double light_radius,
  Pose sensor_pose) {
  double delta_x = sensor_pose.x - light_pose.x;
  double delta_y = sensor_pose.y - light_pose.y;
  double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y)
  - light_radius;

  if (distance_between < 0) {
    distance_between = 0;
  }

  double reading_to_set = get_reading() + 1200/(pow(sensitivity_,
    distance_between));

  //  Keep the reading no greater than the maximum reading
  if (reading_to_set > MAX_READING) {
    set_reading(MAX_READING);
  } else {
    set_reading(reading_to_set);
  }
}

NAMESPACE_END(csci3081);
