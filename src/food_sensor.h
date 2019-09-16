/**
* @file food_sensor.h
*
* @copyright 2017 3081 Staff, All rights reserved.
*/

#ifndef SRC_FOOD_SENSOR_H_
#define SRC_FOOD_SENSOR_H_

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
  * @brief Class representing a food sensor.
  *
  * The sensor senses the infomation in the arena. All of the food sensors are
  * attached to robots. Sensors will calculate and update its reading
  * according to the distance between sensor itself and food that it is
  * sensing.
  *
  * Food sensor can be observed for food.
  */
class FoodSensor : public Sensor {
 public:
   /**
    * @brief FoodSensor's constructor.
    */

  FoodSensor() : Sensor() {
    set_sensor_type(kFoodSensor);
  }

  FoodSensor(const FoodSensor& other) = delete;
  FoodSensor& operator=(const FoodSensor& other) = delete;

  /**
   * @brief Calculate the reading of sensor according to the distance between
   * food and sensor.
   * @param[in] food_pose The current food pose.
   * @param[in] food_radius The current food radius
   * @param[in] sensor_pose The current sensor pose.
   */
  void CalculateSensorReading(Pose food_pose, double food_radius,
    Pose sensor_pose);
};

NAMESPACE_END(csci3081);

#endif  // SRC_FOOD_SENSOR_H_
