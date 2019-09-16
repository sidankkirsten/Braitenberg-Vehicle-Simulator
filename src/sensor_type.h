/**
 * @file sensor_type.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_TYPE_H_
#define SRC_SENSOR_TYPE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

enum SensorType {
  kLightSensor, kFoodSensor
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_TYPE_H_
