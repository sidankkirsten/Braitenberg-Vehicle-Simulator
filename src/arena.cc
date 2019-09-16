/**
 * @file arena.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>
#include <iostream>

#include "src/arena.h"
#include "src/arena_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);
/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Arena::Arena(const struct arena_params *const params)
    : x_dim_(params->x_dim),
      y_dim_(params->y_dim),
      factory_(new EntityFactory),
      entities_(),
      mobile_entities_(),
      game_status_(PLAYING),
      game_paused_(false),
      food_off_(false) {

  robot_count_ = params->n_robots;
  AddRobot(params->n_robots, params->n_ratio, params->n_light_sensitivity,
  params->food_on);

  AddLights(params->n_lights);

  if (params->food_on) {
    AddFood(params->n_food);
  } else {
    food_off_ = true;
  }
}

Arena::~Arena() {
  for (auto ent : entities_) {
    delete ent;
  } /* for(ent..) */
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Arena::AddRobot(int quantity, int ratio, int light_sense, bool food_on) {
  //  Create Robots, every robot's type and motion handler type are originnally
  //  declared as Coward /Fear.
  double rate = static_cast<double>(ratio)/100;
  double quant = static_cast<double>(quantity)*rate;
  quant = static_cast<int>(quant);
  double sense = 1.001 + static_cast<double>(light_sense)/1000;


  for (int i = 0; i < quantity; i++) {
    robot_[i] = dynamic_cast<Robot *>(factory_->CreateEntity(kRobot));
    robot_[i]->set_light_sensitivity(sense);
    entities_.push_back(robot_[i]);
    mobile_entities_.push_back(robot_[i]);
    robot_[i]->set_food_existence(food_on);
  }

  for (int j = quant; j < quantity; j++) {
    robot_[j]->set_robot_type(kExplorer);
    //  Change motion handler to MotionHandlerExplore
    robot_[j]->ChangeToExplore();
  }
}

void Arena::AddFood(int quantity) {
  for (int i = 0; i < quantity; i++) {
    entities_.push_back(factory_->CreateEntity(kFood));
  }
}

void Arena::AddLights(int quantity) {
for (int i = 0; i < quantity; i++) {
  light_[i] = dynamic_cast<Light *>(factory_->CreateEntity(kLight));
  entities_.push_back(light_[i]);
  mobile_entities_.push_back(light_[i]);
}
}

void Arena::Reset() {
  set_game_status(PLAYING);
  for (auto ent : entities_) {
    ent->Reset();
  } /* for(ent..) */
} /* reset() */

// The primary driver of simulation movement. Called from the Controller
// but originated from the graphics viewer.
void Arena::AdvanceTime(double dt) {
  if (!game_paused_) {
    if (!(dt > 0)) {
    return;
    }
    for (size_t i = 0; i < 1; ++i) {
      UpdateEntitiesTimestep();
    } /* for(i..) */
  }
} /* AdvanceTime() */

void Arena::UpdateEntitiesTimestep() {
  /*
   * First, update the position of all entities, according to their current
   * velocities.
   */
//  Check for the game status.
if (get_game_status() == PLAYING) {
  //  set all the robot's sensor reading to 0
  for (int i = 0; i < robot_count_; i++) {
    robot_[i]->reset_sensor_reading();
  }
  //  starts to update position and velocity of entities
  for (auto ent : entities_) {
    /* For non-robot entities, update their velocity and position, and notify
     * each robot's sensors their position
     */
     if (kRobot != ent->get_type()) {
      ent->TimestepUpdate(1);
      //  Notify the light sensors of each robot about each light's position and
      //  radius
      if (kLight == ent->get_type()) {
        for (int i = 0; i <robot_count_; i++) {
          robot_[i] ->LightNotify(ent->get_pose(), ent->get_radius());
        }
      }
      //  Notify the food sensors of each robot about each food's position and
      //  radius
      if (kFood == ent->get_type()) {
        for (int i = 0; i < robot_count_; i++) {
          robot_[i]->FoodNotify(ent->get_pose(), ent->get_radius());

          /* determine if the distance between robot and food is within 5 pixels
           * if so, the robot is not hungry and reset the hungry level of robot
           */
          if (robot_[i]->IsFeeding(ent->get_pose(), ent->get_radius())) {
            robot_[i]->reset_hungry_counter();
          }
        }
      }
    }
}
  // For robots, update their velocity, position and hungry level according to
  // their sensor readings

  for (int i = 0; i < robot_count_; i++) {
    robot_[i]->TimestepUpdate(1);
    robot_[i]->increase_hungry();

    //  if one of the robots is dead, set the game status to LOST, the game
    //  should be stop.
    if (robot_[i]->get_status() == LOST) {
      set_game_status(LOST);
      break;
    }
  }

   /* Determine if any mobile entity is colliding with wall.
   * Adjust the position accordingly so it doesn't overlap.
   */
  for (auto &ent1 : mobile_entities_) {
    EntityType wall = GetCollisionWall(ent1);
    if (kUndefined != wall) {
      if (ent1->get_type() == kRobot) {
        AdjustWallOverlap(ent1, wall);
        dynamic_cast<Robot*> (ent1)->HandleCollision();
      } else {
        AdjustWallOverlap(ent1, wall);
        dynamic_cast<Light*>(ent1)->HandleCollision();
      }
    }

    /* Determine if that mobile entity is colliding with any other entity.
    * Adjust the position accordingly so they don't overlap.
    */
    for (auto &ent2 : entities_) {
      if (ent2 == ent1) {continue;}
       if (IsColliding(ent1, ent2)) {
      if (ent1->get_type() == kLight && ent2->get_type() == kLight) {
        AdjustEntityOverlap(ent1, ent2);
        dynamic_cast<Light*>(ent1)->HandleCollision();
      }
      if (ent1->get_type() == kRobot && ent2->get_type() == kRobot) {
        AdjustEntityOverlap(ent1, ent2);
        dynamic_cast<Robot*>(ent1)->HandleCollision();
      }
    }
  }
}
}
}  // UpdateEntitiesTimestep()


// Determine if the entity is colliding with a wall.
// Always returns an entity type. If not collision, returns kUndefined.
EntityType Arena::GetCollisionWall(ArenaMobileEntity *const ent) {
  if (ent->get_pose().x + ent->get_radius() >= x_dim_) {
    return kRightWall;  // at x = x_dim_
  } else if (ent->get_pose().x - ent->get_radius() <= 0) {
    return kLeftWall;  // at x = 0
  } else if (ent->get_pose().y + ent->get_radius() >= y_dim_) {
    return kBottomWall;  // at y = y_dim
  } else if (ent->get_pose().y - ent->get_radius() <= 0) {
    return kTopWall;  // at y = 0
  } else {
    return kUndefined;
  }
} /* GetCollisionWall() */

/* The entity type indicates which wall the entity is colliding with.
* This determines which way to move the robot to set it slightly off the wall. */
void Arena::AdjustWallOverlap(ArenaMobileEntity *const ent,
  EntityType object) {
  Pose entity_pos = ent->get_pose();
  switch (object) {
    case (kRightWall):  // at x = x_dim_
    ent->set_position(x_dim_-(ent->get_radius()+5), entity_pos.y);
    break;
    case (kLeftWall):  // at x = 0
    ent->set_position(ent->get_radius()+5, entity_pos.y);
    break;
    case (kTopWall):  // at y = 0
    ent->set_position(entity_pos.x, ent->get_radius()+5);
    break;
    case (kBottomWall):  // at y = y_dim_
    ent->set_position(entity_pos.x, y_dim_-(ent->get_radius()+5));
    break;
    default:
    {}
  }
}

/* Calculates the distance between the center points to determine overlap */
bool Arena::IsColliding(
  ArenaMobileEntity * const mobile_e,
  ArenaEntity * const other_e) {
    double delta_x = other_e->get_pose().x - mobile_e->get_pose().x;
    double delta_y = other_e->get_pose().y - mobile_e->get_pose().y;
    double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
    return
    (distance_between <= (mobile_e->get_radius() + other_e->get_radius()));
}

/* This is called when it is known that the two entities overlap.
* We determine by how much they overlap then move the mobile entity to
* the edge of the other
*/
void Arena::AdjustEntityOverlap(ArenaMobileEntity * const mobile_e,
  ArenaEntity *const other_e) {
        double delta_x = mobile_e->get_pose().x - other_e->get_pose().x;
        double delta_y = mobile_e->get_pose().y - other_e->get_pose().y;
        double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
        double distance_to_move = mobile_e->get_radius() + other_e->get_radius()
        - distance_between + 5;
        double angle = atan2(delta_y, delta_x);
        mobile_e->set_position(
           mobile_e->get_pose().x+cos(angle)*distance_to_move,
           mobile_e->get_pose().y+sin(angle)*distance_to_move);
}


// Accept communication from the controller. Dispatching as appropriate.
/** Call the appropriate Robot functions to implement user input
  * for controlling the robot.
  */
void Arena::AcceptCommand(Communication com) {
  switch (com) {
    case(kPlay): game_paused_ = false;
    break;
    case(kPause): game_paused_ = true;
    break;
    case(kReset):  Reset();
    break;
    case(kFoodOn): food_off_ = false;
    break;
    case(kFoodOff): food_off_ = true;
    case(kNone):
    break;
    default: break;
  }
} /* AcceptCommand */

NAMESPACE_END(csci3081);
