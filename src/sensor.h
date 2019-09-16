/**
 * @file sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <utility>
#include <vector>
#include <iostream>

#include "src/common.h"
#include "src/pose.h"
#include "src/sensor_type.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/
/**
 * @brief Class representing a sensor.
 *
 * The sensor senses the infomation in the arena. All of the sensors are
 * attached to robots. Sensors will calculate its reading and update it
 * according to the distance between sensor itself and entities that it is
 * sensing.
 *
 * Sensor can be observed for lights, food.
 */
class Sensor {
 public:
  /**
   * @brief Constructor.
   */
  Sensor() {
    zero_reading();
  }

  virtual ~Sensor() = default;

  /**
   * @brief Sensor reading setter.
   * @param reading The reading that we want to set.
   */
  void set_reading(double reading) {
    if (reading >= MAX_READING) {
      reading_ = MAX_READING;
    } else if (reading <= MIN_READING) {
      reading_ = MIN_READING;
    } else {
      reading_ = reading;
    }
  }

  /**
   * @brief Sensor reading getter.
   * return the current reading of the sensor
   */
  double get_reading() {return reading_; }

  /**
   * @brief Sensor type setter.
   * @param sensor_type The sensor type that we want to set.
   */
  void set_sensor_type(SensorType sensor_type) {sensor_type_ = sensor_type; }

  /**
   * @brief Sensor type getter.
   * return The sensor type of the current sensor.
   */
  SensorType get_sensor_type () {return sensor_type_;}
  /**
   * @brief Set the sensor reading to 0.
   */
  void zero_reading() {reading_ = 0;}


 private:
  // type of the current sensor
  SensorType sensor_type_ {kLightSensor};
  // the current sensor reading
  double reading_ {0};
};





NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_H_
