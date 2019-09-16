/**
 * @file controller.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <nanogui/nanogui.h>
#include <string>
#include "src/arena_params.h"
#include "src/common.h"
#include "src/controller.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

Controller::Controller() : last_dt(0) {
  // Initialize default properties for various arena entities
  arena_params aparams;
  aparams.x_dim = ARENA_X_DIM;
  aparams.y_dim = ARENA_Y_DIM;

  arena_ = new Arena(&aparams);

  // Start up the graphics (which creates the arena).
  // Run() will enter the nanogui::mainloop().
  viewer_ = new GraphicsArenaViewer(&aparams, arena_, this);
}

void Controller::Run() { viewer_->Run(); }

void Controller::AdvanceTime(double dt) {
  if ((last_dt + dt) <= .05) {
    last_dt += dt;
    return;
  }
  last_dt = 0;
  arena_->AdvanceTime(dt);
}

void Controller::AcceptCommunication(Communication com) {
  arena_->AcceptCommand(ConvertComm(com));
}

void Controller::Reset() {
  // delete the original arena
  delete arena_;

  /*
   * initialize a new params to store the new features for the arena to be
   * created
   */
  arena_params aparams;

  aparams.x_dim = ARENA_X_DIM;
  aparams.y_dim = ARENA_Y_DIM;

  // change values according to graphic arena viewer
  aparams.n_robots = viewer_->get_robot_count();
  aparams.n_lights = viewer_->get_light_count();
  aparams.n_food = viewer_->get_food_count();
  aparams.n_light_sensitivity = viewer_->get_light_sensitivity();
  aparams.n_ratio = viewer_->get_ratio();
  aparams.food_on = viewer_->is_food_on();

  // create new arena
  arena_ = new Arena(&aparams);
}



/** Converts communication from one source to appropriate communication to
 * the other source. For example, the viewer sends a kKeyUp communication,
 * and this translate to a kIncreaseSpeed communication to Arena.
 */
Communication Controller::ConvertComm(Communication com) {
  switch (com) {
    case (kPlay) : return kPlay;
    case (kPause) : return kPause;
    case (kNewGame) : Reset();
    case (kFoodOff) : return kFoodOff;
    case (kFoodOn) : return kFoodOn;
    default: return kNone;
  }
}

NAMESPACE_END(csci3081);
