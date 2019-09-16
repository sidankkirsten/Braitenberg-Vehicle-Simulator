/**
 * @file motion_handler_explore.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_explore.h"
#include "src/motion_behavior_differential.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
// @TODO add clamped


void MotionHandlerExplore::UpdateVelocity(double lt_left_reading,
  double lt_right_reading, double fd_left_reading, double fd_right_reading,
  int hungry_level, bool hunger_exist) {
  if (!hunger_exist) {
    // Food Turn off
    set_velocity(LeftSpeedExplore(lt_right_reading),
    RightSpeedExplore(lt_left_reading));

  } else {
    if (hungry_level > HUNGRY && hungry_level < STARVE) {
      set_velocity(LeftSpeedHungry(lt_right_reading, fd_right_reading),
      RightSpeedHungry(lt_left_reading, fd_left_reading));

    } else if (hungry_level >= STARVE) {
      set_velocity(LeftSpeedAggressive(fd_right_reading),
      RightSpeedAggressive(fd_left_reading));

    } else {
      set_velocity(LeftSpeedExplore(lt_right_reading),
      RightSpeedExplore(lt_left_reading));
    }
  }
}

double MotionHandlerExplore::LeftSpeedExplore(double lt_right_reading) {
  return clamp_vel(10 * (1 - lt_right_reading/1000));
}

double MotionHandlerExplore::RightSpeedExplore(double lt_left_reading) {
  return clamp_vel(10 * (1 - lt_left_reading/1000));
}

double MotionHandlerExplore::LeftSpeedAggressive(double fd_right_reading) {
  return clamp_vel(10 * (fd_right_reading/1000));
}

double MotionHandlerExplore::RightSpeedAggressive(double fd_left_reading) {
  return clamp_vel(10 * (fd_left_reading/1000));
}

double MotionHandlerExplore::LeftSpeedHungry(double lt_right_reading,
  double fd_right_reading) {
    return clamp_vel(0.5*LeftSpeedExplore(lt_right_reading) +
    0.5 * LeftSpeedAggressive(fd_right_reading));
  }

  double MotionHandlerExplore::RightSpeedHungry(double lt_left_reading,
    double fd_left_reading) {
      return clamp_vel(0.5*RightSpeedExplore(lt_left_reading) +
      0.5 * RightSpeedAggressive(fd_left_reading));
    }


double MotionHandlerExplore::clamp_vel(double vel) {
  double clamped = 0.0;
  if (vel > 0) {
    clamped = (vel > get_max_speed()) ?
              get_max_speed():
              vel;
  } else {
    clamped = (vel < -get_max_speed()) ?
              -get_max_speed():
              vel;
  }
  return clamped;
} /* clamp_vel() */

NAMESPACE_END(csci3081);
