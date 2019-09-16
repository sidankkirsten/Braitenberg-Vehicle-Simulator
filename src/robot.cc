/**
 * @file robot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/robot.h"
#include "src/params.h"
#include "src/food.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

 /* When the robot is doing a reverse arc, the direc_angle_ is set to 180.
  * its heading will change -3 degrees in every timestep, the direc_angle_ will
  * decrease by 3 every timestep. When the direc_angle_ is equal to 120, which
  * means the robot has already reversed 60 degrees, the reverse arc motion is
  * done, and reset the reverse status is_reverse_arc to false.
  */

  Robot::Robot()
  : ArenaMobileEntity(),
      motion_behavior_(this),
      light_sensor_left_(),
      light_sensor_right_(),
      food_sensor_left_(),
      food_sensor_right_(),
      type_{kFear},
      is_reverse_arc{false},
      direc_angle_{180},
      hungry_t_{0},
      status_{PLAYING} {
    set_type(kRobot);
    set_color(ROBOT_COLOR);
    motion_handler_ = new MotionHandlerFear(this);
  }

void Robot::TimestepUpdate(unsigned int dt) {
  // When the robot is not dead
  if (!is_dead()) {
  //  When doing a reverse arc
    if (is_reverse_arc) {
      ReverseArc();
    } else {
     // no reverse arc is needed , moving in a regular manner
      motion_handler_->UpdateVelocity(light_sensor_left_.get_reading(),
      light_sensor_right_.get_reading(), food_sensor_left_.get_reading(),
      food_sensor_right_.get_reading(), hungry_t_, food_exist_);
    }

  // Use velocity and position to update position
    motion_behavior_.UpdatePose(dt, motion_handler_->get_velocity());

  } else {
    set_status(LOST);
  }
} /* TimestepUpdate() */


void Robot::Reset() {
  Pose robot_pose = {static_cast<double>((30+(random()%19)*50)),
       static_cast<double>((30+(random()%14)*50))};
  set_pose(robot_pose);
  motion_handler_->set_max_speed(ROBOT_MAX_SPEED);
  motion_handler_->set_max_angle(ROBOT_MAX_ANGLE);
  set_heading(random_num(0, 360));
  set_radius(random_num(8, 14));
  motion_handler_->set_velocity(0, 0);
  reset_hungry_counter();
  reset_sensor_reading();
  is_reverse_arc = false;
  direc_angle_ = 180;
  set_status(PLAYING);
} /* Reset() */

void Robot::HandleCollision() {
is_reverse_arc = true;  // Starts to move in a reverse arc
}

void Robot::ReverseArc() {
  if (direc_angle_ > 120) {
  direc_angle_-= 3;
  //  Change heading by -3
  RelativeChangeHeading(-3);
  //  revese
  motion_handler_->set_velocity(-5, -5);
  } else if (direc_angle_ == 120) {
    //  reverse arc is done
    //  reset all the flags
    is_reverse_arc = false;
    direc_angle_ = 180;
    motion_handler_->set_velocity(5, 5);
  }
}

bool Robot::IsFeeding(Pose food_pose, double food_radius) {
  double delta_x = get_pose().x - food_pose.x;
  double delta_y = get_pose().y - food_pose.y;
  // distance between robot and food
  double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y)
  -food_radius - get_radius();
  // if the robot is feed by food, distace should be within 5
  return(distance_between <= 5 );
}



void Robot::LightNotify(Pose light_pose, double light_radius) {
  light_sensor_left_.CalculateSensorReading(light_pose, light_radius,
    get_sensor_position(LEFT_SENSOR));
  light_sensor_right_.CalculateSensorReading(light_pose, light_radius,
    get_sensor_position(RIGHT_SENSOR));
}

void Robot::FoodNotify(Pose food_pose, double food_radius) {
  food_sensor_left_.CalculateSensorReading(food_pose, food_radius,
    get_sensor_position(LEFT_SENSOR));
  food_sensor_right_.CalculateSensorReading(food_pose, food_radius,
    get_sensor_position(RIGHT_SENSOR));
}


void Robot::ChangeToExplore() {
  motion_handler_ = new MotionHandlerExplore(this);
}

void Robot::ChangeToFear() {
  motion_handler_ = new MotionHandlerFear(this);
}


NAMESPACE_END(csci3081);
