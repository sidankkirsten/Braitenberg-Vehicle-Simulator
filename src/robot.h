/**
 * @file robot.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <math.h>
#include <string>
#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/motion_handler_fear.h"
#include "src/motion_handler_explore.h"
#include "src/motion_behavior_differential.h"
#include "src/entity_type.h"
#include "src/light_sensor.h"
#include "src/food_sensor.h"
#include "src/params.h"
#include "src/robot_type.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class MotionBehaviorDifferential;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing a robot within the arena.
 *
 * Robots are composed of a motion handler, motion behavior, left light sensor,
 * right light sensor, left food sensor and right food sensor.
 *
 * These classes interact to maintain the pose (position and heading) of the
 * robot. At each time step, the wheel velocities are used to calculate the
 * next pose of the robot. The handler manages the pose and user requests. The
 * behavior calculates the new pose foodd on wheel velocities.
 *
 * Robots perform different behaviors towards light depending on its type
 * and perform different behaviors towards food according to its hungry leve;/
 *
 * The sensors sense entities within the arena when the robot is alive.
 * The heading is modified after a collision to move the robot away from the
 * other object.
 */
class Robot : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor using initialization values from params.h.
   */
  Robot();
  Robot(const Robot& other) = delete;
  Robot& operator=(const Robot& other) = delete;

  /**
   * @brief Reset the Robot to a newly constructed state (needed for reset
   * button to work in GUI).
   */
  void Reset() override;

  /**
   * @brief Update the Robot's position and velocity after the specified
   * duration has passed.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   */
  void TimestepUpdate(unsigned int dt) override;


  /**
   * @brief Handles the collision by setting the sensor to activated.
   */
  void HandleCollision();

  /**
   * @brief When the robot is doing a reverse arc, the direc_angle_ is set to
   * 180, its heading will change -3 degrees in every timestep, the direc_angle_
   * will decrease by 3 every timestep. When the direc_angle_ is equal to 120,
   * which means the light has already reversed 60 degrees, the reverse arc
   * motion is done, and reset the is_reverse_arc to false.
   */
  void ReverseArc();


  /**
   * @brief Get the name of the Robot for visualization.
   */
  std::string get_name() const override {
  if (kExplorer == type_) {
      return "E";  // explorer
    } else {
      return "C";  //  coward
    }
  }

  /**
   * @brief Get the position of the sensor in the robot
   * @param angle The angle that the sensor is located at from the robot's
   *        heading
   * @return the pose of the sensor
   */
  Pose get_sensor_position(double angle) {
    double theta = get_pose().theta + angle;
    double theta2 = theta*M_PI/180.0;
    double x = get_radius()*cos(theta2) + get_pose().x;
    double y = get_radius()*sin(theta2) + get_pose().y;
    return Pose(x, y, theta);
  }

  /**
   * @brief invoke CalculateReading() in LightSensor to update the sensor
   * reading.
   * @param light_pose The pose of light
   * @param light_radius The radius of the light
   */
  void LightNotify(Pose light_pose, double light_radius);

  /**
   * @brief invoke CalculateReading() in FoodSensor to update the sensor
   * reading.
   * @param food_pose The pose of food.
   * @param food_radius The radius of the food.
   */
  void FoodNotify(Pose food_pose, double food_radius);


  /**
   * @brief Change the motion handler to MotionHandlerExplore for those robots
   * of type kExplorer.
   */
  void ChangeToExplore();

  /**
   * @brief Change the motion handler to MotionHandlerFear for those robots
   * of type kFear.
   */
  void ChangeToFear();

  /**
   * @brief Determine if the robot is eating food .
   * @param food_pose The pose of food.
   * @param food_radius The radius of food.
   * @return true if the robot is within 5 units from food
   */
  bool IsFeeding(Pose food_pose, double food_radius);

 /**
  * @brief Set all sensor readings to 0.
  */
  void reset_sensor_reading() {
    light_sensor_left_.zero_reading();
    light_sensor_right_.zero_reading();
    food_sensor_left_.zero_reading();
    food_sensor_right_.zero_reading();
  }

  /**
   * @brief get light sensor reading, for unit test.
   * @param direction Angle that the sensor is located at from the robot's
   *        heading.
   * @return the light sensor reading
   */
  double get_light_sensor_reading(int direction) {
    if (direction == LEFT_SENSOR) {
      return light_sensor_left_.get_reading();
    } else {
      return light_sensor_right_.get_reading();
    }
  }

  /**
   * @brief set light sensor reading, for unit test.
   * @param direction Angle that the sensor is located at from the robot's
   *        heading.
   * @param reading  The reading that we want to set .
   */
  void set_light_sensor_reading(int direction, double reading) {
    if (direction == LEFT_SENSOR) {
      light_sensor_left_.set_reading(reading);
    } else {
      light_sensor_right_.set_reading(reading);
    }
  }

  /**
   * @brief set light sensor sensitivity, for graphic arena viewer.
   * @param sense the sensitivity to set
   */
  void set_light_sensitivity(double sense) {
    light_sensor_left_.set_sensitivity(sense);
    light_sensor_right_.set_sensitivity(sense);
  }

  /**
   * @brief robot type setter.
   * @param the robot type that we need to set.
   */
  void set_robot_type(RobotType type) {
    type_ = type;
  }

  /**
   * @brief robot type Setter.
   * return the current robot type.
   */
  RobotType get_robot_type() {
    return type_;
  }

  /**
   * @brief set the hungry counter to 0.
   */
  void reset_hungry_counter() {
    hungry_t_ = 0;
  }

  /**
   * @brief Determine if a robot is dead by check robot's hungry level.
   */
  bool is_dead() {
    return ((hungry_t_== DEAD)&&food_exist_);
  }

  /**
   * @brief game status setter
   * @param status The game status including PLAYING, LOST, WIN.
   */
  void set_status(int status) {
    status_ = status;
  }
  /**
   * @brief game status setter
   * @return the game status which could be PLAYING, LOST and WIN.
   */
  int get_status() {
    return status_;
  }
  /**
   * @brief hungry level/counter increment
   * Called by arena
   */
  void increase_hungry() {
    hungry_t_ = hungry_t_+ 1;
  }

  void set_food_existence(bool food_exist) {
    food_exist_ = food_exist;
  }

  double get_left_velocity() {
    return motion_handler_->get_left_velocity();
  }

  double get_right_velocity() {
    return motion_handler_->get_right_velocity();
  }

  MotionHandler *get_motion_handler() {
    return motion_handler_;
  }



 private:
  // Calculates changes in pose based on elapsed time and wheel velocities.
  MotionBehaviorDifferential motion_behavior_;
  // Left Light Sensor
  LightSensor light_sensor_left_;
  // Right Light Sensor
  LightSensor light_sensor_right_;
  // Left Food Sensor
  FoodSensor food_sensor_left_;
  // Right Food Sensor
  FoodSensor food_sensor_right_;

  // Type of the robot
  RobotType type_;
  // Determine whether the robot is moving in a reverse arc
  bool is_reverse_arc;
  // the reverse arc motion counter
  int direc_angle_;
  // hungry level. Start from 0. Increase by 1 for every update.
  int hungry_t_;
  // game status
  int status_;
  // if food is turned on
  bool food_exist_{true};

 protected:
  // Manages pose and wheel velocities that change with time and collisions.
  MotionHandler * motion_handler_ {nullptr};
};

NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_H_
