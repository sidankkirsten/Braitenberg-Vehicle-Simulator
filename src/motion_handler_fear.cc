/**
 * @file motion_handler_fear.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_fear.h"
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


void MotionHandlerFear::UpdateVelocity(double lt_left_reading,
 double lt_right_reading, double fd_left_reading, double fd_right_reading,
int hungry_level, bool hunger_exist) {
  if (!hunger_exist) {
    set_velocity(LeftSpeedFear(lt_left_reading),
    RightSpeedFear(lt_right_reading));

  } else {
    if (hungry_level > HUNGRY && hungry_level < STARVE) {
      set_velocity(LeftSpeedHungry(lt_left_reading, fd_right_reading),
      RightSpeedHungry(lt_right_reading, fd_left_reading));

    } else if (hungry_level >= STARVE) {
      set_velocity(LeftSpeedAggressive(fd_right_reading),
      RightSpeedAggressive(fd_left_reading));

    } else {
      set_velocity(LeftSpeedFear(lt_left_reading),
      RightSpeedFear(lt_right_reading));
    }
  }
}


double MotionHandlerFear::LeftSpeedFear(double lt_left_reading) {
  return clamp_vel(10 * (lt_left_reading/1000));
}

double MotionHandlerFear::RightSpeedFear(double lt_right_reading) {
  return clamp_vel(10 * (lt_right_reading/1000));
}

double MotionHandlerFear::LeftSpeedAggressive(double fd_right_reading) {
  return clamp_vel(10 * (fd_right_reading/1000));
}

double MotionHandlerFear::RightSpeedAggressive(double fd_left_reading) {
  return clamp_vel(10 * (fd_left_reading/1000));
}

double MotionHandlerFear::LeftSpeedHungry(double lt_left_reading,
  double fd_right_reading) {
    return clamp_vel(0.6 * LeftSpeedFear(lt_left_reading) +
    0.4 * LeftSpeedAggressive(fd_right_reading));
  }

  double MotionHandlerFear::RightSpeedHungry(double lt_right_reading,
    double fd_left_reading) {
      return clamp_vel(0.6 * RightSpeedFear(lt_right_reading) +
      0.4 * RightSpeedAggressive(fd_left_reading));
    }

double MotionHandlerFear::clamp_vel(double vel) {
  // @TODO: don't go backwards
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
