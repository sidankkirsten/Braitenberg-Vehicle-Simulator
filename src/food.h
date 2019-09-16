/**
 * @file food.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_FOOD_H_
#define SRC_FOOD_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_immobile_entity.h"
#include "src/common.h"
#include "src/entity_type.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing a immobile food within the Arena.
 *
 * Food can feed a Robot. If a Robot touches the Food, it becomes
 * not hungry.
 *
 * Food has fixed size and random position in the Arena
 */
class Food : public ArenaImmobileEntity {
 public:
  /**
   * @brief Constructor.
   *
   * @param params A food_params passed down from main.cc for the
   * initialization of the Food.
   */
  Food();

  /**
   * @brief Reset the Food using the initialization parameters received
   * by the constructor.
   */
  void Reset() override;

  /**
   * @brief Change the color of food when it is captured
   */
  void ColorChange();

  /**
   * @brief Get the name of the Food for visualization purposes, and to
   * aid in debugging.
   *
   * @return Name of the Food.
   */
  std::string get_name() const override { return "Food"; }

  /**
   * @brief Getter for captured_, which is the state of the food
   *
   * @return true if captured.
   */
  bool IsCaptured() const { return captured_; }

  /**
   * @brief Setter for captured_, which is the state of the food
   * @param state true if captured
   */
  void set_captured(bool state) { captured_ = state; }

 private:
  bool captured_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_FOOD_H_
