/**
 * @file graphics_arena_viewer.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_GRAPHICS_ARENA_VIEWER_H_
#define SRC_GRAPHICS_ARENA_VIEWER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <MinGfx-1.0/mingfx.h>

#include "src/arena.h"
#include "src/controller.h"
#include "src/common.h"
#include "src/communication.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class Controller;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 *  @brief An application that uses the MinGfx library to open up a window that
 *  includes a few buttons for controlling the simulation and can be used to
 *  draw circles and other computer graphics.
 *
 *  After constructing a new GraphicsArenaViewer, call Run to start and run the
 *  application. Run will not return until the application window is closed.
 *  Example:
 *
 *  ```
 *  int main(int argc, char **argv) {
 *      RobotViewer *app = new RobotViewer();
 *      app->Run();
 *      return 0;
 *  }
 *  ```
 *
 *  While the window is open UpdateSimulation will be called repeatedly,
 *  once per frame. Fill this in to update your simulation or perform any other
 *  processing that should happen over time as the simulation progresses.
 *
 *  Fill in the `On*()` methods as desired to respond to user input events.
 *
 *  Fill in the `Draw*()` methods to draw graphics on the screen using
 *  either the `nanovg` library or raw `OpenGL`.
 */
class GraphicsArenaViewer : public GraphicsApp {
 public:
  /**
   * @brief Constructor.
   *
   * @param params A arena_params passed down from main.cc for the
   * initialization of the Arena and the entities therein.
   */
  explicit GraphicsArenaViewer(const struct arena_params *const params,
                               Arena *arena, Controller *controller);

  /**
   * @brief Destructor.
   *
   * `delete` the contained Arena.
   */
  ~GraphicsArenaViewer() override { delete arena_; }

  /**
   * @brief Informs the Arena of the new time, so that it can update.
   *
   * @param dt The new timestep.
   */
  void UpdateSimulation(double dt) override;

  /**
   * @brief Handle the user pressing the pause button on the GUI.
   *
   * This will freeze the graphics--no update, until the pause button is
   * pressed again.
   */
  void OnPauseBtnPressed();

  /**
   * @brief Handle the user pressing the new game button on the GUI.
   *
   * This will reset and update the graphics
   */
  void OnStartNewGameBtnPressed();

  /**
   * @brief Handle the user pressing the turn food on button on the GUI.
   *
   * This will turn on or off the food
   */
  void OnFoodOnBtnPressed();


  /**
   * @brief Called each time the mouse moves on the screen within the GUI
   * window.
   *
   * Origin is at the lower left of the window. This function is a stub.
   *
   * @param[in] pos The position of the release.
   * @param[in] delta How far the mouse has moved.
   */
  void OnMouseMove(__unused const Point2& pos,
                   __unused const Vector2& delta) override {};

  /**
   * @brief Called each time the left mouse button is clicked.
   *
   * Origin is at the lower left of the window. This function is a stub.
   *
   * @param[in] pos The position of the release.
   */
  void OnLeftMouseDown(__unused const Point2& pos) override {};

  /**
   * @brief Called each time the left mouse button is released.
   *
   * Origin is at the lower left of the window. This function is a stub.
   *
   * @param[in] pos The position of the release.
   */
  void OnLeftMouseUp(__unused const Point2& pos) override {};

  /**
   * @brief Called each time the right mouse button is clicked.
   *
   * Origin is at the lower left of the window. This function is a stub.
   *
   * @param[in] pos The position of the release.
   */
  void OnRightMouseDown(__unused const Point2& pos) override {};

  /**
   * @brief Called each time the right mouse button is released.
   *
   * Origin is at the lower left of the window. This function is a stub.
   *
   * @param[in] pos The position of the release.
   */
  void OnRightMouseUp(__unused const Point2& pos) override {};

  /**
   * @brief Called each time a character key is pressed.
   *
   * @param[in] c Character representing a key that was pressed.
   * @param[in] modifiers Any modifier keys that were also pressed.
   */
  void OnKeyDown(__unused const char *c, __unused int modifiers) override {};

  /**
   * @brief Called each time a character key is released.
   *
   * @param[in] c Character representing a key that was released.
   * @param[in] modifiers Any modifier keys that were held with the key.
   */
  void OnKeyUp(__unused const char *c, __unused int modifiers) override {};

  /**
   * @brief Called each time a special (non-alphabetic) key is pressed.
   *
   * @param[in] key The key that was pressed.
   * @param[in] scancode The scancode corresponding to the key.
   * @param[in] modifiers Any modifier keys that were also pressed.
   */
  void OnSpecialKeyDown(int key, __unused int scancode,
                        __unused int modifiers) override;

  /**
   * @brief Called each time a special (non-alphabetic) key is released.
   *
   * @param[in] key The key that was released.
   * @param[in] scancode The scancode corresponding to the key.
   * @param[in] modifiers Any modifier keys that were also pressed.
   */
  void OnSpecialKeyUp(__unused int key, __unused int scancode,
                      __unused int modifiers) override {};

  /**
   * @brief Draw the Arena with all of its entities using `nanogui`.
   *
   * This is the primary driver for drawing all entities in the Arena. It is
   * called at each iteration of `nanogui::mainloop()`.
   *
   * @param[in] ctx Context for nanogui.
   */
  void DrawUsingNanoVG(NVGcontext *ctx) override;

  /**
   * @brief Draw using `OpenGL`. This method is unimplemented, as currently
   * we are doing all drawing with `nanovg` in this application, so it is empty.
   */
  void DrawUsingOpenGL() override {}

  /**
   * @brief Under certain circumstance, the compiler requires that the
   * assignment operator is not defined. This `deletes` the default
   * assignment operator.
   */
  GraphicsArenaViewer &operator=(const GraphicsArenaViewer &other) = delete;

  /**
   * @brief Under certain circumstance, the compiler requires that the copy
   * constructor is not defined. This `deletes` the default copy constructor.
   */
  GraphicsArenaViewer(const GraphicsArenaViewer &other) = delete;

  // void ChangeToNewArena(Arena *arena){
    // *arena_ =arena;
  // }

  int get_robot_count() {
    return robot_count_;
  }

  int get_light_count() {
    return light_count_;
  }

  int get_ratio() {
    return ratio_;
  }

  int get_light_sensitivity() {
    return light_sense_;
  }

  int get_food_count() {
    return food_count_;
  }

  bool is_food_on() {
    return has_food_;
  }

 private:
  void DrawArena(NVGcontext *ctx);
  /**
   * @brief Draw a Robot using `nanogui`.
   *
   * This function requires an active `nanovg` drawing context (`ctx`), so it
   * should probably only be called from with DrawUsingNanoVG.
   *
   * @param[in] ctx The `nanovg` context.
   * @param[in] robot The Robot handle.
   */
  void DrawRobot(NVGcontext *ctx, const class Robot *const robot);

  /**
   * @brief Draw an Light in the Arena using `nanogui`.
   *
   * This function requires an active `nanovg` drawing context (`ctx`), so it
   * should probably only be called from with DrawUsingNanoVG.
   *
   * @param[in] ctx The `nanovg` context.
   * @param[in] light The Light handle.
   */
  void DrawEntity(NVGcontext *ctx, const class ArenaEntity *const entity);

  /**
   * @brief Draw an WIN or LOSE indication in the Arena using `nanogui`.
   *
   * This function requires an active `nanovg` drawing context (`ctx`), so it
   * should probably only be called from with DrawUsingNanoVG.
   *
   * @param[in] ctx The `nanovg` context.
   */
  void DrawIndication(NVGcontext *ctx);

  Controller *controller_;
  Arena *arena_;
  bool paused_{true};
  bool has_food_{true};
  int robot_count_ {10};
  int light_count_{4};
  int ratio_{50};
  int light_sense_{80};
  int food_count_{4};

  // buttons
  nanogui::Button *playing_button_{nullptr};
  nanogui::Button *pause_button_{nullptr};
  nanogui::Button *start_new_game_button_{nullptr};
  nanogui::Button *food_on_button_{nullptr};
};

NAMESPACE_END(csci3081);

#endif  // SRC_GRAPHICS_ARENA_VIEWER_H_
