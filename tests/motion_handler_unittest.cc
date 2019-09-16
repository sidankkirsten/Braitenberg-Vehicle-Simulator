/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include "src/arena.h"
#include "src/arena_params.h"
#include "src/entity_type.h"
#include "src/motion_handler_explore.h"
#include "src/motion_handler_fear.h"
#include "src/motion_behavior.h"
#include "src/motion_behavior_differential.h"
#include "src/robot.h"
#include "src/robot_type.h"
#include "src/params.h"
#include "src/motion_handler.h"
#include "src/wheel_velocity.h"


#ifdef MOTIONHANDLER_TESTS

/******************************************************
* TEST FEATURE SetUp
*******************************************************/
class MotionHandlerTest : public ::testing::Test {

protected:
  virtual void SetUp() {
    // Initialize Robot and Motion Handler for TESTS

      robot_fear = new csci3081::Robot;
      robot_explorer = new csci3081::Robot;
      motion_handler = new csci3081::MotionHandler(robot_fear);
      motion_handler_fear = new csci3081::MotionHandlerFear(robot_fear);
      motion_handler_explore = new csci3081::MotionHandlerExplore(robot_explorer);


      //robot size
      radius = 20;

      //robot set up
      robot_fear->set_radius(radius);
      robot_fear->set_position(0,0);
      robot_fear->set_heading(0);
      robot_fear->ChangeToFear();


      robot_explorer->set_radius(radius);
      robot_explorer->set_position(0,200);
      robot_explorer->set_heading(0);
      robot_explorer->ChangeToExplore();
    }

    csci3081::Robot * robot_fear;
    csci3081::Robot * robot_explorer;
    csci3081::MotionHandler * motion_handler;
    csci3081::MotionHandlerFear * motion_handler_fear;
    csci3081::MotionHandlerExplore * motion_handler_explore;


    // Robot radius
    double radius;
  };

/*******************************************************************************
 * Test Cases
 ******************************************************************************/
/*
 * As for Fear, the speed for each wheel will be ranged from 0 to 10.
 * As for Explorer, the speed for each wheel will be ranged from 0 to 10.
 * Test both robot motion handler by setting sensor reading and check if the left
 * or right speed is out of bound.
 * And for different right and left sensor reading, whether the left speed and
 * right speed differ.
 */

/*********************Sanity Test***************************************/

TEST_F(MotionHandlerTest, ConstructorAndGetter) {
// temperary pointer
  csci3081::MotionHandler * temp_motion_handler;
  csci3081::WheelVelocity  wheel_velocity;
  temp_motion_handler = new csci3081::MotionHandler(robot_fear);
  wheel_velocity = csci3081::WheelVelocity(0.0,0.0);
  // test initialized member values
  EXPECT_EQ(temp_motion_handler->get_max_speed(), 10)<<"\nFAIL MAX speed is not 10";
  EXPECT_EQ(temp_motion_handler->get_max_angle(), 360)<<"\nFAIL MAX angle is not 360";
  EXPECT_EQ(temp_motion_handler->get_left_velocity(), 0.0)<<"\nFAIL left velocity is not 0";
  EXPECT_EQ(temp_motion_handler->get_right_velocity(), 0.0)<<"\nFAIL right velocity is not 0";
  EXPECT_EQ(temp_motion_handler->get_velocity().left, wheel_velocity.left)<<"\nFAIL velocity is not (0,0)";
  EXPECT_EQ(temp_motion_handler->get_velocity().right, wheel_velocity.right)<<"\nFAIL velocity is not (0,0)";

}

TEST_F(MotionHandlerTest, SetMaxAngle) {

  motion_handler->set_max_angle(50);
  EXPECT_EQ(motion_handler->get_max_angle(), 50) <<"n/FAIL MAX angle should be 50";

}

TEST_F(MotionHandlerTest, SetMaxSpeed) {

  motion_handler->set_max_speed(50);
  EXPECT_EQ(motion_handler->get_max_speed(), 50) <<"n/FAIL MAX speed should be 50";

}


TEST_F(MotionHandlerTest, SetVelocity_I) {

  csci3081::WheelVelocity  wheel_velocity;

  wheel_velocity = csci3081::WheelVelocity(5,5);

  motion_handler->set_velocity(wheel_velocity);

  EXPECT_EQ(motion_handler->get_velocity().left, wheel_velocity.left)<<"\nFAIL velocity is not (5,5)";
  EXPECT_EQ(motion_handler->get_velocity().right, wheel_velocity.right)<<"\nFAIL velocity is not (5,5)";
}

TEST_F(MotionHandlerTest, SetVelocity_II) {

  csci3081::WheelVelocity  wheel_velocity;
  wheel_velocity = csci3081::WheelVelocity(5,4);

  motion_handler->set_velocity(5,4);
  EXPECT_EQ(motion_handler->get_velocity().left, wheel_velocity.left)<<"\nFAIL velocity is not (5,5)";
  EXPECT_EQ(motion_handler->get_velocity().right, wheel_velocity.right)<<"\nFAIL velocity is not (5,5)";
}



/*********************Fear Boundary Test***************************************/

 TEST_F(MotionHandlerTest,FearBothSensorToMax) {
    // Test if Fear robot's velocity go to max when setting both sensor to max
    robot_fear->set_heading(0);
    robot_fear->set_position(0,0);
    robot_fear->set_robot_type(csci3081::kFear);
    robot_fear->set_heading(0);

    // Update velocity
     motion_handler_fear->UpdateVelocity(1000.0,1000.0,0.0,0.0,0,false);


    double speed_l = motion_handler_fear->get_left_velocity();
    double speed_r = motion_handler_fear->get_right_velocity();


    EXPECT_EQ(speed_l, speed_r)<<"\nFAIL left wheel speed not equal right wheel";
    EXPECT_EQ(speed_l, 10)<<"\nFAIL left wheel speed is not the max";
    EXPECT_EQ(speed_r, 10)<<"\nFAIL right wheel speed is not the max";

// Update velocity, when reading is over bound
    motion_handler_fear->UpdateVelocity(1200.0,1200.0,0.0,0.0,0,false);


   speed_l = motion_handler_fear->get_left_velocity();
   speed_r = motion_handler_fear->get_right_velocity();


   EXPECT_EQ(speed_l, speed_r)<<"\nFAIL left wheel speed not equal right wheel";
   EXPECT_EQ(speed_l, 10)<<"\nFAIL left wheel speed exceed the max";
   EXPECT_EQ(speed_r, 10)<<"\nFAIL right wheel speed exceed the max";
}


// Boundary Test
TEST_F(MotionHandlerTest,FearBothSensorToMin) {
  // Test if Fear robot's velocity go to min when setting both sensor to min
  // which is 0.0

  robot_fear->set_heading(0);
  robot_fear->set_position(0,0);
  robot_fear->set_robot_type(csci3081::kFear);
  robot_fear->set_heading(0);
// Update velocity
  motion_handler_fear->UpdateVelocity(0.0,0.0,0.0,0.0,0,false);

  double speed_l = motion_handler_fear->get_left_velocity();
  double speed_r = motion_handler_fear->get_right_velocity();

  robot_fear->reset_sensor_reading();


  EXPECT_EQ(speed_l, speed_r)<<"\nFAIL left wheel speed not equal right wheel";
  EXPECT_EQ(speed_l, 0)<<"\nFAIL left wheel speed is not the min speed";
  EXPECT_EQ(speed_r, 0)<<"\nFAIL right wheel speed is not the min speed";
}

/*********************Fear Behavior Test***************************************/

//Functional Test
TEST_F(MotionHandlerTest,FearWheelFunctionalTest) {
  // Test if Fear robot's velocity correct
  robot_fear->set_heading(0);
  robot_fear->set_position(0,200);
  robot_fear->set_robot_type(csci3081::kFear);
  robot_fear->set_heading(0);
// case 1
  motion_handler_fear->UpdateVelocity(500.0,10.0,0.0,0.0,0,false);

  double speed_l = motion_handler_fear->get_left_velocity();
  double speed_r = motion_handler_fear->get_right_velocity();

  EXPECT_GT(speed_l, speed_r)<<"\nFAIL left wheel speed is less than right wheel";
  EXPECT_EQ(speed_l,5.0 )<<"\nFAIL left wheel speed is not correct, should be 5.0";
  EXPECT_EQ(speed_r,0.1 )<<"\nFAIL right wheel speed is not correct, should be 0.1";

// case 2
  motion_handler_fear->UpdateVelocity(10.0,500.0,0.0,0.0,0,false);

  speed_l = motion_handler_fear->get_left_velocity();
  speed_r = motion_handler_fear->get_right_velocity();

  EXPECT_GT(speed_r, speed_l)<<"\nFAIL left wheel speed is greater than right wheel";
  EXPECT_EQ(speed_l,0.1 )<<"\nFAIL left wheel speed is not correct, should be 0.1";
  EXPECT_EQ(speed_r,5.0 )<<"\nFAIL right wheel speed is not correct, should be 5.0";
// case 3
  motion_handler_fear->UpdateVelocity(700.0,700.0,0.0,0.0,0,false);

  speed_l = motion_handler_fear->get_left_velocity();
  speed_r = motion_handler_fear->get_right_velocity();

  EXPECT_EQ(speed_r, speed_l)<<"\nFAIL left wheel speed is not equal to right wheel";
  EXPECT_EQ(speed_l,7 )<<"\nFAIL left wheel speed is not correct, should be 3.6";
  EXPECT_EQ(speed_r,7)<<"\nFAIL right wheel speed is not correct, should be 3.6";
// case 4
  motion_handler_fear->UpdateVelocity(80.0,800.0,0.0,0.0,0,false);

  speed_l = motion_handler_fear->get_left_velocity();
  speed_r = motion_handler_fear->get_right_velocity();

  EXPECT_GT(speed_r, speed_l)<<"\nFAIL left wheel speed is greater than right wheel";
  EXPECT_EQ(speed_l,0.8 )<<"\nFAIL left wheel speed is not correct, should be 0.8";
  EXPECT_EQ(speed_r,8.0 )<<"\nFAIL right wheel speed is not correct, should be 8.0";

// case 5
  motion_handler_fear->UpdateVelocity(800.0,80,0.0,0.0,0,false);

  speed_l = motion_handler_fear->get_left_velocity();
  speed_r = motion_handler_fear->get_right_velocity();

  EXPECT_GT(speed_l, speed_r)<<"\nFAIL left wheel speed is less than right wheel";
  EXPECT_EQ(speed_l,8.0 )<<"\nFAIL left wheel speed is not correct, should be 8.0";
  EXPECT_EQ(speed_r,0.8 )<<"\nFAIL right wheel speed is not correct, should be 0.8";


}


/*********************Explore Boundary Test************************************/

TEST_F(MotionHandlerTest,ExplorerBothSensorToMax) {
  // Test if Explore robot's velocity go to max when setting both sensor to max
  robot_explorer->set_heading(0);
  robot_explorer->set_position(0,200);
  robot_explorer->set_robot_type(csci3081::kExplorer);
  robot_explorer->set_heading(0);

// Update velocity
   motion_handler_explore->UpdateVelocity(1000.0,1000.0,0.0,0.0,0,false);


  double speed_l = motion_handler_explore->get_left_velocity();
  double speed_r = motion_handler_explore->get_right_velocity();


  EXPECT_EQ(speed_l, speed_r)<<"\nFAIL left wheel speed not equal right wheel";
  EXPECT_EQ(speed_l, 0)<<"\nFAIL left wheel speed is not the min";
  EXPECT_EQ(speed_r, 0)<<"\nFAIL right wheel speed is not the min";

}

TEST_F(MotionHandlerTest,ExplorerBothSensorToMin) {
  // Test if Fear robot's velocity go to min when setting both sensor to min

  robot_explorer->set_heading(0);
  robot_explorer->set_position(0,200);
  robot_explorer->set_robot_type(csci3081::kExplorer);
  robot_explorer->set_heading(0);
// Update velocity
  motion_handler_explore->UpdateVelocity(0.0,0.0,0.0,0.0,0,false);

  double speed_l = motion_handler_explore->get_left_velocity();
  double speed_r = motion_handler_explore->get_right_velocity();


  EXPECT_EQ(speed_l, speed_r)<<"\nFAIL left wheel speed not equal right wheel";
  EXPECT_EQ(speed_l, 10)<<"\nFAIL left wheel speed is not the max speed";
  EXPECT_EQ(speed_r, 10)<<"\nFAIL right wheel speed is not the max speed";

}

/*********************Explore Behavior Test************************************/

TEST_F(MotionHandlerTest,ExplorerWheelFunctionalTest) {
  // Test if explore robot's velocity correct

  robot_explorer->set_heading(0);
  robot_explorer->set_position(0,200);
  robot_explorer->set_robot_type(csci3081::kExplorer);
  robot_explorer->set_heading(0);
// case 1
  motion_handler_explore->UpdateVelocity(500.0,10.0,0.0,0.0,0,false);

  double speed_l = motion_handler_explore->get_left_velocity();
  double speed_r = motion_handler_explore->get_right_velocity();

  EXPECT_GT(speed_l, speed_r)<<"\nFAIL left wheel speed is less than right wheel";
  EXPECT_EQ(speed_l,9.9 )<<"\nFAIL left wheel speed is not correct, should be 5.0";
  EXPECT_EQ(speed_r,5.0 )<<"\nFAIL right wheel speed is not correct, should be 9.0";

// case 2
  motion_handler_explore->UpdateVelocity(10.0,500.0,0.0,0.0,0,false);

  speed_l = motion_handler_explore->get_left_velocity();
  speed_r = motion_handler_explore->get_right_velocity();

  EXPECT_GT(speed_r, speed_l)<<"\nFAIL left wheel speed is greater than right wheel";
  EXPECT_EQ(speed_l,5.0 )<<"\nFAIL left wheel speed is not correct, should be 9.0";
  EXPECT_EQ(speed_r,9.9 )<<"\nFAIL right wheel speed is not correct, should be 5.0";
// case 3
  motion_handler_explore->UpdateVelocity(360.0,360.0,0.0,0.0,0,false);

  speed_l = motion_handler_explore->get_left_velocity();
  speed_r = motion_handler_explore->get_right_velocity();

  EXPECT_EQ(speed_r, speed_l)<<"\nFAIL left wheel speed is not equal to right wheel";
  EXPECT_EQ(speed_l,6.4 )<<"\nFAIL left wheel speed is not correct, should be 0.1";
  EXPECT_EQ(speed_r,6.4 )<<"\nFAIL right wheel speed is not correct, should be 5.0";

// case 4
  motion_handler_explore->UpdateVelocity(100.0,370.0,0.0,0.0,0,false);

  speed_l = motion_handler_explore->get_left_velocity();
  speed_r = motion_handler_explore->get_right_velocity();

  EXPECT_LT(speed_l, speed_r)<<"\nFAIL left wheel speed is less than right wheel";
  EXPECT_EQ(speed_l,6.3 )<<"\nFAIL left wheel speed is not correct, should be 1";
  EXPECT_EQ(speed_r,9.0)<<"\nFAIL right wheel speed is not correct, should be 2.0";

// case 5
  motion_handler_explore->UpdateVelocity(370.0,100.0,0.0,0.0,0,false);

  speed_l = motion_handler_explore->get_left_velocity();
  speed_r = motion_handler_explore->get_right_velocity();

  EXPECT_GT(speed_l, speed_r)<<"\nFAIL left wheel speed is less than right wheel";
  EXPECT_EQ(speed_l, 9.0 )<<"\nFAIL left wheel speed is not correct, should be 2.0";
  EXPECT_EQ(speed_r, 6.3 )<<"\nFAIL right wheel speed is not correct, should be 1";

}

#endif /* MOTION_HANDLER_TESTS */
