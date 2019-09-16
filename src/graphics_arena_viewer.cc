/**
 * @file graphics_arena_viewer.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <vector>
#include <iostream>
#include <string>

#include "src/graphics_arena_viewer.h"
#include "src/arena_params.h"


/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
GraphicsArenaViewer::GraphicsArenaViewer(
    const struct arena_params *const params,
    Arena * arena, Controller * controller) :
    GraphicsApp(
        params->x_dim + GUI_MENU_WIDTH + GUI_MENU_GAP * 2,
        params->y_dim,
        "Robot Simulation"),
    controller_(controller),
    arena_(arena) {
  auto *gui = new nanogui::FormHelper(screen());
  nanogui::ref<nanogui::Window> window =
      gui->addWindow(
          Eigen::Vector2i(1100 + GUI_MENU_GAP, 10),
          "Menu");
  // gui->addGroup creates a heading within the window
  window->setLayout(new nanogui::GroupLayout());
  gui->addGroup("Simulation Control");
  // add buttons
  pause_button_ =
    gui->addButton(
      "Play",
      std::bind(&GraphicsArenaViewer::OnPauseBtnPressed, this));
    pause_button_->setFixedWidth(100);
  start_new_game_button_ =
    gui->addButton(
      "New Game",
      std::bind(&GraphicsArenaViewer::OnStartNewGameBtnPressed, this));
  start_new_game_button_->setFixedWidth(100);
  food_on_button_ =
    gui->addButton(
      "Turn off Food",
      std::bind(&GraphicsArenaViewer::OnFoodOnBtnPressed, this));
  food_on_button_->setFixedWidth(100);
  gui->addGroup("Arena Configuration");

  // Creating a panel impacts the layout. Widgets, sliders, buttons can be
  // assigned to either the window or the panel.
  nanogui::Widget *panel = new nanogui::Widget(window);

  // *************** SLIDER 1 ************************//
  new nanogui::Label(panel, "Number of Robots", "sans-bold");
  nanogui::Slider *slider = new nanogui::Slider(panel);
  slider->setValue(1.0f);
  slider->setFixedWidth(100);
  // Display the corresponding value of the slider in this textbox
  nanogui::TextBox *text_box = new nanogui::TextBox(panel);
  text_box->setFixedSize(nanogui::Vector2i(60, 25));
  text_box->setFontSize(20);
  text_box->setValue("10");
  // This is the lambda function called while the user is moving the slider
  slider->setCallback(
  [text_box](float value) {
    text_box->setValue(std::to_string(static_cast<int>(value*10)));
  });

  slider->setFinalCallback(
    [&](float value) {
      robot_count_ = static_cast<int>(value*10);
      std::cout << "Final slider value: " << value;
      std::cout << " robot " << robot_count_ << std::endl;
    });
  // *************** SLIDER 2 ************************//
  new nanogui::Label(panel, "Number of Lights", "sans-bold");
  nanogui::Slider *slider2 = new nanogui::Slider(panel);
  slider2->setValue(0.4f);
  slider2->setFixedWidth(100);

  nanogui::TextBox *text_box2 = new nanogui::TextBox(panel);
  text_box2->setFixedSize(nanogui::Vector2i(60, 25));
  text_box2->setFontSize(20);
  text_box2->setValue("4");

  slider2->setCallback(
    [text_box2](float value) {
      text_box2->setValue(std::to_string(static_cast<int>(value*10)));
    });

  slider2->setFinalCallback(
    [&](float value) {
      light_count_ = static_cast<int>(value*10);
      std::cout << "Final slider2 value: " << value;
      std::cout << " light " << light_count_ << std::endl;
    });
  // *************** SLIDER 3 ************************//
  new nanogui::Label(panel, "Ratio of the Fearful Robot", "sans-bold");
  nanogui::Slider *slider3 = new nanogui::Slider(panel);
  slider3->setValue(0.5f);
  slider3->setFixedWidth(100);

  nanogui::TextBox *text_box3 = new nanogui::TextBox(panel);
  text_box3->setFixedSize(nanogui::Vector2i(60, 25));
  text_box3->setFontSize(20);
  text_box3->setValue("50");
  text_box3->setUnits("%");

  slider3->setCallback(
    [text_box3](float value) {
      text_box3->setValue(std::to_string(static_cast<int>(value*100)));
    });

  slider3->setFinalCallback(
    [&](float value) {
      ratio_ = static_cast<int>(value*100);
      std::cout << "Final slider3 value: " << value;
      std::cout << "ratio " << ratio_ << std::endl;
    });

  // *************** SLIDER 4 ************************//
  new nanogui::Label(panel, "Light Sensor Sensitivity", "sans-bold");
  nanogui::Slider *slider4 = new nanogui::Slider(panel);
  slider4->setValue(0.8f);
  slider4->setFixedWidth(100);

  nanogui::TextBox *text_box4 = new nanogui::TextBox(panel);
  text_box4->setFixedSize(nanogui::Vector2i(60, 25));
  text_box4->setFontSize(20);
  text_box4->setValue("80");
  text_box4->setUnits("%");

  slider4->setCallback(
    [text_box4](float value) {
      text_box4->setValue(std::to_string(static_cast<int>(value*100)));
    });

  slider4->setFinalCallback(
    [&](float value) {
      light_sense_ = static_cast<int>(value*100);
      std::cout << "Final slider4 value: " << value;
      std::cout << " light sensitivity " << light_sense_ << std::endl;
    });

  // *************** SLIDER 5 ************************//
  new nanogui::Label(panel, "Number of Food", "sans-bold");
  nanogui::Slider *slider5 = new nanogui::Slider(panel);
  slider5->setValue(0.4f);
  slider5->setFixedWidth(100);

  nanogui::TextBox *text_box5 = new nanogui::TextBox(panel);
  text_box5->setFixedSize(nanogui::Vector2i(60, 25));
  text_box5->setFontSize(20);
  text_box5->setValue("4");

  slider5->setCallback(
    [text_box5](float value) {
      text_box5->setValue(std::to_string(static_cast<int>(value*10)));
    });

  slider5->setFinalCallback(
    [&](float value) {
      food_count_ = static_cast<int>(value*10);
      std::cout << "Final slider5 value: " << value;
      std::cout << " light sensitivity " << light_sense_ << std::endl;
    });
  // Lays out all the components with "15" units of inbetween spacing
  panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
    nanogui::Alignment::Middle, 0, 15));
  screen()->setSize({1300, 800});
  screen()->performLayout();
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

// This is the primary driver for state change in the arena.
// It will be called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::UpdateSimulation(double dt) {
  if (!paused_) {
    controller_->AdvanceTime(dt);
  }
}

/*******************************************************************************
 * Handlers for User Keyboard and Mouse Events
 ******************************************************************************/

void GraphicsArenaViewer::OnPauseBtnPressed() {
  paused_ = !paused_;
  if (paused_) {
    pause_button_->setCaption("Play");
    controller_->AcceptCommunication(kPause);
  } else {
    pause_button_->setCaption("Pause");
    controller_->AcceptCommunication(kPlay);
  }
}

void GraphicsArenaViewer::OnStartNewGameBtnPressed() {
  paused_ = true;
  pause_button_->setCaption("Play");
  arena_->Reset();
  controller_->AcceptCommunication(kNewGame);
  arena_ = controller_->getArena();
}

void GraphicsArenaViewer::OnFoodOnBtnPressed() {
  has_food_ = !has_food_;
  if (has_food_) {
    food_on_button_->setCaption("Turn off Food");
    controller_->AcceptCommunication(kFoodOn);
  } else {
    food_on_button_->setCaption("Turn on Food");
    controller_->AcceptCommunication(kFoodOff);
  }
}


/** OnSpecialKeyDown is called when the user presses down on one of the
  * special keys (e.g. the arrow keys).
  */
/**
 * @TODO: Check for arrow key presses using GLFW macros, then
 * convert to appropriate enum Communication and relay to controller
 */
void GraphicsArenaViewer::OnSpecialKeyDown(int key,
  __unused int scancode, __unused int modifiers) {
    Communication key_value = kNone;
    switch (key) {
      case GLFW_KEY_LEFT: key_value = kKeyLeft;
        break;
      case GLFW_KEY_RIGHT: key_value = kKeyRight;
        break;
      case GLFW_KEY_UP: key_value = kKeyUp;
        break;
      case GLFW_KEY_DOWN: key_value = kKeyDown;
        break;
      default: {}
    }
  controller_->AcceptCommunication(key_value);
}

/*******************************************************************************
 * Drawing of Entities in Arena
 ******************************************************************************/
void GraphicsArenaViewer::DrawRobot(NVGcontext *ctx,
                                     const Robot *const robot) {
  // translate and rotate all graphics calls that follow so that they are
  // centered, at the position and heading of this robot
  nvgSave(ctx);
  nvgTranslate(ctx,
               static_cast<float>(robot->get_pose().x),
               static_cast<float>(robot->get_pose().y));
  nvgRotate(ctx,
            static_cast<float>(robot->get_pose().theta * M_PI / 180.0));

  // robot's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, static_cast<float>(robot->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(robot->get_color().r, robot->get_color().g,
                       robot->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  nvgSave(ctx);
  nvgRotate(ctx, static_cast<float>(M_PI / 2.0));
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, 0.0, 0.0, robot->get_name().c_str(), nullptr);

  nvgRotate(ctx, (-40 * M_PI / 180.0)+ M_PI);
  nvgText(ctx, 0.0, robot->get_radius(), "I", nullptr);
  nvgRotate(ctx, (+80 * M_PI/180.0));
  nvgText(ctx, 0.0, robot->get_radius(), "I", nullptr);

  nvgRestore(ctx);
  nvgRestore(ctx);
}

void GraphicsArenaViewer::DrawArena(NVGcontext *ctx) {
  nvgBeginPath(ctx);
  // Creates new rectangle shaped sub-path.
  nvgRect(ctx, 0, 0, arena_->get_x_dim(), arena_->get_y_dim());
  nvgStrokeColor(ctx, nvgRGBA(255, 255, 255, 255));
  nvgStroke(ctx);
}

void GraphicsArenaViewer::DrawEntity(NVGcontext *ctx,
                                       const ArenaEntity *const entity) {
  // light's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx,
            static_cast<float>(entity->get_pose().x),
            static_cast<float>(entity->get_pose().y),
            static_cast<float>(entity->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(entity->get_color().r, entity->get_color().g,
                       entity->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // light id text label
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx,
          static_cast<float>(entity->get_pose().x),
          static_cast<float>(entity->get_pose().y),
          entity->get_name().c_str(), nullptr);
}

void GraphicsArenaViewer::DrawIndication(NVGcontext *ctx) {
  // indication's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx, 400, 400, 100);
  nvgFillColor(ctx, nvgRGBA(255, 192, 203, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // indication text label
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, 400, 400, "A Robot is dead!", nullptr);
}

void GraphicsArenaViewer::DrawUsingNanoVG(NVGcontext *ctx) {
  // initialize text rendering settings
  nvgFontSize(ctx, 18.0f);
  nvgFontFace(ctx, "sans-bold");
  nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  DrawArena(ctx);
  std::vector<ArenaEntity *> entities = arena_->get_entities();
  for (auto &entity : entities) {
    if (kRobot == entity->get_type()) {
      DrawRobot(ctx, dynamic_cast<Robot*>(entity));
    } else {
      DrawEntity(ctx, entity);
    }
  } /* for(i..) */
  if (arena_->get_game_status()== LOST) {
  DrawIndication(ctx);}
}

NAMESPACE_END(csci3081);
