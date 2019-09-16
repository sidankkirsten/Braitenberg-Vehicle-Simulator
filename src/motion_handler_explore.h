/**
 * @file motion_handler_explore.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_HANDLER_EXPLORE_H_
#define SRC_MOTION_HANDLER_EXPLORE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <cassert>
#include <iostream>

#include "src/common.h"
#include "src/motion_handler.h"
#include "src/communication.h"
#include "src/robot_type.h"


/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/

/**
 * @brief Class managing a Robot's speed and heading angle based
 * on collisions and user inputs.
 *
 * Currently, both wheels are always going at maximum speed, and
 * cannot be controlled independently.
 */
class MotionHandlerExplore : public MotionHandler {
 public:
  explicit MotionHandlerExplore(ArenaMobileEntity * ent)
       : MotionHandler(ent) {}

  MotionHandlerExplore(const MotionHandlerExplore& other) = default;
  MotionHandlerExplore& operator=(const MotionHandlerExplore& other) = default;

  /**
  * @brief Update the speed and the pose angle according to the sensor readings.
  *
  * Currently does not change speed.
  *
  * @param[in] pose The current pose.
  * @param[in] st A SensorTouch to be read.
  */
  void UpdateVelocity(double lt_left_reading, double lt_right_reading,
   double fd_left_reading, double fd_right_reading, int hungry_level,
   bool hunger_exist)
    override;

  /**
   * @brief Calculate the left wheel speed at exploration mode
   * @param lt_right_reading right light sensor reading
   * @return the right wheel speed at exploration mode
   */
  double LeftSpeedExplore(double lt_right_reading);

  /**
   * @brief Calculate the right wheel speed at exploration mode
   * @param lt_left_reading left light sensor reading
   * @return the right wheel speed at exploration mode
   */
  double RightSpeedExplore(double lt_left_reading);

  /**
   * @brief Calculate the left wheel speed at aggression mode
   * @param fd_right_reading right food sensor reading
   * @return the left wheel speed at aggression mode
   */
  double LeftSpeedAggressive(double fd_right_reading);

  /**
   * @brief Calculate the right wheel speed at aggression mode
   * @param fd_left_reading left food sensor reading
   * @return the right wheel speed at aggression mode
   */
  double RightSpeedAggressive(double fd_left_reading);

  /**
   * @brief Calculate the left wheel speed at hungry mode
   * @param lt_right_reading right light sensor reading
   * @param fd_right_reading right food sensor reading
   * @return the left wheel speed at hungry mode
   */
  double LeftSpeedHungry(double lt_right_reading, double fd_right_reading);

  /**
   * @brief Calculate the right wheel speed at hungry mode
   * @param lt_left_reading left light sensor reading
   * @param fd_left_reading left food sensor reading
   * @return the right wheel speed at hungry mode
   */
  double RightSpeedHungry(double lt_left_reading, double fd_left_reading);


 private:
   /**
    * @brief Keep the velocity not excceding the max velocity
    * @param vel The max velocity.
    * return The velocity that is already checked
    */
  double clamp_vel(double vel);
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_HANDLER_EXPLORE_H_
