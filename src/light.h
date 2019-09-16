/**
 * @file light.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_LIGHT_H_
#define SRC_LIGHT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/motion_handler.h"
#include "src/motion_behavior_differential.h"
#include "src/entity_type.h"
#include "src/pose.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class MotionBehaviorDifferential;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing a light within the arena.
 *
 * Lights are composed of a motion handler, motion behavior, and touch sensor.
 * These classes interact to maintain the pose (position and heading) of the
 * light. At each time step, the wheel velocities are used to calculate the
 * next pose of the robot. The handler manages the pose and user requests. The
 * behavior calculates the new pose based on wheel velocities.
 *
 * The touch sensor is activated when the light collides with an object.
 * The heading is modified after a collision to move the light away from the
 * other object and head back to the center of the arena
 */

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing an immobile light within the Arena.
 *
 * Since lights are immobile, the Light class is very simple.
 */
class Light : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor.
   */
  Light();

  /**
   * @brief Update the Light's position and velocity after the specified
   * duration has passed.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   */
  void TimestepUpdate(unsigned int dt) override;

  /**
   * @brief Reset the light to a newly constructed state (needed for reset
   * button to work in GUI).
   */
  void Reset() override;


  /**
   * @brief Handles the collision by setting the sensor to activated.
   * When the light is colliding with wall or other lights, it should firstly
   * move in a reverse arc of 60 degrees. During the reverse arc, the light will
   * not react to any other stimuli.
   */
  void HandleCollision();

  /**
   * @brief When the light is doing a reverse arc, the direc_angle_ is set to
   * 180, its heading will change -3 degrees in every timestep, the direc_angle_
   * will decrease by 3 every timestep. When the direc_angle_ is equal to 120,
   * which means the light has already reversed 60 degrees, the reverse arc
   * motion is done, and reset the is_reverse_arc to false.
   */
  void ReverseArc();


  /**
   * @brief Get the name of the Light for visualization purposes, and to
   * aid in debugging.
   */
  std::string get_name() const override {
    return "Light" + std::to_string(get_id());
  }



 private:
  // Manages pose and wheel velocities that change with time and collisions.
  MotionHandler motion_handler_;
  // Calculates changes in pose based on elapsed time and wheel velocities.
  MotionBehaviorDifferential motion_behavior_;
  // check if the light is doing a reverse arc
  bool is_reverse_arc;
  // Flag to determine whether the arc reverse should stop
  int direc_angle_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_LIGHT_H_
