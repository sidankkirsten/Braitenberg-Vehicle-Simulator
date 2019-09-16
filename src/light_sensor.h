/**
* @file light_sensor.h
*
* @copyright 2017 3081 Staff, All rights reserved.
*/

#ifndef SRC_LIGHT_SENSOR_H_
#define SRC_LIGHT_SENSOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>

#include "src/sensor.h"
#include "src/sensor_type.h"
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);
/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
 /**
  * @brief Class representing a light sensor.
  *
  * The sensor senses the infomation in the arena. All of the light sensors are
  * attached to robots. Sensors will calculate and update its reading
  * according to the distance between sensor itself and lights that it is
  * sensing.
  *
  * Light sensor can be observed for lights.
  */
class LightSensor : public Sensor {
 public:
   /**
    * @brief LightSensor's constructor.
    */
  LightSensor() : Sensor() {
     set_sensor_type(kLightSensor);
  }

  LightSensor(const LightSensor& other) = delete;
  LightSensor& operator=(const LightSensor& other) = delete;

 /**
  * @brief Calculate the reading of sensor according to the distance between
  * food and sensor.
  * @param[in] light_pose The current food pose.
  * @param[in] light_radius The current food radius.
  * @param[in] sensor_pose The current sensor pose.
  */
  void CalculateSensorReading(Pose light_pose, double light_radius,
     Pose sensor_pose);

  void set_sensitivity(double sense) {
     sensitivity_ = sense;
  }

 private:
  // sensitivity is 1.08 as default
  double sensitivity_{1.08};
};

NAMESPACE_END(csci3081);

#endif  //  SRC_LIGHT_SENSOR_H_
