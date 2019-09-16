/**
 * @file light.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/light.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Light::Light() :
     motion_handler_(this),
     motion_behavior_(this),
     is_reverse_arc{false},
     direc_angle_{180} {
  set_color(LIGHT_COLOR);
  set_pose(LIGHT_POSITION);
  set_radius(LIGHT_RADIUS);
  set_heading(random_num(0, 360));
  set_type(kLight);
  motion_handler_.set_velocity(LIGHT_SPEED, LIGHT_SPEED);
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/

 /* When the light is doing a reverse arc, the direc_angle_ is set to 180.
  * its heading will change -3 degrees in every timestep, the direc_angle_ will
  * decrease by 3 every timestep. When the direc_angle_ is equal to 120, which
  * means the light has already reversed 60 degrees, the reverse arc motion is
  * done, and reset the is_reverse_arc to false.
  */
void Light::TimestepUpdate(unsigned int dt) {
  // When doing a reverse arc
  if (is_reverse_arc) {
    ReverseArc();
    } else {
      // no reverse arc is needed, moving in a regular manner
      motion_handler_.set_velocity(LIGHT_SPEED, LIGHT_SPEED);
    }

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());

  // Reset Sensor for next cycle
} /* TimestepUpdate() */

void Light::Reset() {
  Pose light_pose = {static_cast<double>((30+(random()%19)*50)),
       static_cast<double>((30+(random()%14)*50))};
  set_pose(light_pose);
  set_heading(random_num(0, 360));
  set_radius(random_num(LIGHT_MIN_RADIUS, LIGHT_MAX_RADIUS));
  motion_handler_.set_velocity(LIGHT_SPEED, LIGHT_SPEED);;
} /* Reset() */

void Light::HandleCollision() {
  is_reverse_arc = true;  // Starts to move in a reverse arc
  }

void Light::ReverseArc() {
  if (direc_angle_ > 120) {
    direc_angle_-= 3;
    //  Change heading by -3
    RelativeChangeHeading(-3);
    //  reverse
    motion_handler_.set_velocity(-LIGHT_SPEED, -LIGHT_SPEED);
  } else if (direc_angle_ == 120) {
    //  reverse arc is done
    //  reset all the flags
    is_reverse_arc = false;
    direc_angle_ = 180;
    motion_handler_.set_velocity(LIGHT_SPEED, LIGHT_SPEED);
  }
}




NAMESPACE_END(csci3081);
