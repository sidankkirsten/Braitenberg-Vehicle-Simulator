

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include "src/arena.h"
#include "src/arena_params.h"
#include "src/communication.h"
#include "src/entity_type.h"
#include "src/sensor_type.h"
#include "src/robot.h"
#include "src/params.h"

#ifdef SENSORLIGHT_TESTS

/******************************************************
* TEST FEATURE SetUp
*******************************************************/
class SensorLightTest : public ::testing::Test {

protected:
  virtual void SetUp() {
    // Initialize Robot and Light for TESTS
    robot = new csci3081::Robot;

    light1 = new csci3081::Light;
    light2 = new csci3081::Light;
    light3 = new csci3081::Light;
    light4 = new csci3081::Light;

    light_sensor = new csci3081::LightSensor;



    radius = 20;
    robot->set_radius(radius);
    robot->set_position(0,0);
    robot->set_heading(0);

    // Initialize and calculate sensor location

    //left sensor x
    sensor_l_x = radius*cos((-40)*M_PI/180.0);
    //left sensor y
    sensor_l_y = radius*sin((-40)*M_PI/180.0);
    //right sensor x
    sensor_r_x = radius*cos((40)*M_PI/180.0);
    //right sensor y
    sensor_r_y = radius*sin((40)*M_PI/180.0);

    // set raius for light
    light1->set_radius(radius);
    light2->set_radius(radius);
    light3->set_radius(radius);
    light4->set_radius(radius);

  }


  csci3081::Robot * robot;
  csci3081::Light * light1;
  csci3081::Light * light2;
  csci3081::Light * light3;
  csci3081::Light * light4;
  csci3081::LightSensor * light_sensor;


  double radius;
  double sensor_l_x;
  double sensor_l_y;
  double sensor_r_x;
  double sensor_r_y;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/
 /** With a radius of 20 for each of robot and light, to place the light
   * at the different positions to check if the sensor reading is right
   **/

/**************************Sanity Check****************************************/

TEST_F(SensorLightTest, SensorPosition) {
  // Test if the sensor position is initialized correctly
  robot->set_radius(radius);
  robot->set_position(0,0);
  robot->set_heading(0);

  EXPECT_EQ(sensor_l_x, robot->get_sensor_position(-40).x);
  EXPECT_EQ(sensor_r_x, robot->get_sensor_position(40).x);
  EXPECT_EQ(sensor_l_y, robot->get_sensor_position(-40).y);
  EXPECT_EQ(sensor_r_y, robot->get_sensor_position(40).y);
}

 TEST_F(SensorLightTest, Constructor) {
   // Test if light sensor constructor works
   csci3081::LightSensor ls_;
   EXPECT_EQ(ls_.get_sensor_type(), csci3081::kLightSensor)
   << "\n FAIL sensor type is not light";
   EXPECT_EQ(ls_.get_reading(),0.0);
 }


 TEST_F(SensorLightTest,ReadingSetter) {
   // Test Reading setters
   light_sensor->set_reading(5.0);
   EXPECT_EQ(light_sensor->get_reading(), 5.0)
   << "\n FAIL sensor reading should be 5.0";
 }


 TEST_F(SensorLightTest, ZeroReading) {
   // Test zero_reading function
   light_sensor->set_reading(5.0);
   light_sensor->zero_reading();
   EXPECT_EQ(light_sensor->get_reading(), 0.0)
   << "\n FAIL sensor reading should be 0";

 }

 TEST_F(SensorLightTest, ReadingGetter) {
   light_sensor->set_reading(0.0);
   EXPECT_EQ(light_sensor->get_reading(), 0.0)
   << "\n FAIL sensor reading should be 0";
 }

 TEST_F(SensorLightTest, MaxReading) {
   light_sensor->set_reading(2000);
   EXPECT_EQ(light_sensor->get_reading(), 1000)
   << "\n FAIL sensor reading should be 1000";
 }

 TEST_F(SensorLightTest, MinReading) {
   light_sensor->set_reading(-9);
   EXPECT_EQ(light_sensor->get_reading(), 0)
   << "\n FAIL sensor reading should be 0";
 }

/*************************One Far Light****************************************/

 TEST_F(SensorLightTest, LeftSensorReadingOnOneFarLight) {
  // Test if left sensor reading is right when one light is 30 unit from it
   robot->set_radius(radius);
   robot->set_position(0,0);
   robot->set_heading(0);

 // Set light position and radius
   light1->set_position(sensor_l_x+45, sensor_l_y+108);
   light1->set_radius(radius);

   robot->reset_sensor_reading();
   // Notify
   robot->LightNotify(light1->get_pose(),radius);

   EXPECT_LT(robot->get_light_sensor_reading(LEFT_SENSOR),0.69)
   << "\nFAIL left sensor reading too large";

   EXPECT_GT(robot->get_light_sensor_reading(LEFT_SENSOR),0.68)
   << "\nFAIL left sensor reading too small";

 }

 TEST_F(SensorLightTest, RightSensorReadingOnOneFarLight) {
   // Test if right sensor reading is right when one light is 30 unit from it

   // Set robot position and radius
   robot->set_radius(radius);
   robot->set_position(0,0);
   robot->set_heading(0);

  // Set light position and radius
   light1->set_position(sensor_r_x+45, sensor_r_y+108);
   light1->set_radius(radius);

   robot->reset_sensor_reading();

  // Notify
   robot->LightNotify(light1->get_pose(),radius);

   EXPECT_LT(robot->get_light_sensor_reading(RIGHT_SENSOR),0.69)
    << "\nFAIL right sensor reading too large";

   EXPECT_GT(robot->get_light_sensor_reading(RIGHT_SENSOR),0.68)
    << "\nFAIL right sensor reading too small";

 }

/*************************One Close Light****************************************/
 TEST_F(SensorLightTest, LeftSensorReadingOnOneCloseLight) {
  // Test if left sensor reading is right when one light is 6 unit from it
   robot->set_radius(radius);
   robot->set_position(0,0);
   robot->set_heading(0);

 // Set light position and radius
   light1->set_position(sensor_l_x+10, sensor_l_y+24);
   light1->set_radius(radius);

   robot->reset_sensor_reading();
   // Notify
   robot->LightNotify(light1->get_pose(),radius);

   EXPECT_LT(robot->get_light_sensor_reading(LEFT_SENSOR),756.5)
   << "\nFAIL left sensor reading too large";

   EXPECT_GT(robot->get_light_sensor_reading(LEFT_SENSOR),756.1)
   << "\nFAIL left sensor reading too small";

 }

 TEST_F(SensorLightTest, RightSensorReadingOnOneCloseLight) {
   // Test if right sensor reading is right when one light is 6 unit from it

   // Set robot position and radius
   robot->set_radius(radius);
   robot->set_position(0,0);
   robot->set_heading(0);

  // Set light position and radius
   light1->set_position(sensor_r_x+10, sensor_r_y+24);
   light1->set_radius(radius);

   robot->reset_sensor_reading();

  // Notify
   robot->LightNotify(light1->get_pose(),radius);

   EXPECT_LT(robot->get_light_sensor_reading(RIGHT_SENSOR),756.5)
    << "\nFAIL right sensor reading too large";

   EXPECT_GT(robot->get_light_sensor_reading(RIGHT_SENSOR),756.1)
    << "\nFAIL right sensor reading too small";

 }

 /***************2+ Lights Equivalence Partitions******************************/

 TEST_F(SensorLightTest, LeftSensorReaingOnFourLightsEquivalencePartition) {
   // Test if left sensor reading is right when 4 lights around it

   robot->set_radius(radius);
   robot->set_position(0,0);
   robot->set_heading(0);
   robot->reset_sensor_reading();
   // Set light position
   light1->set_position(sensor_l_x+30, sensor_l_y+40);
   light1->set_radius(radius);
   light2->set_position(sensor_l_x-30, sensor_l_y-40);
   light2->set_radius(radius);
   light3->set_position(sensor_l_x+25, sensor_l_y+60);
   light3->set_radius(radius);
   light4->set_position(sensor_l_x-25, sensor_l_y-60);
   light4->set_radius(radius);
   // Notify
   robot->LightNotify(light1->get_pose(),radius);
   robot->LightNotify(light2->get_pose(),radius);
   robot->LightNotify(light3->get_pose(),radius);
   robot->LightNotify(light4->get_pose(),radius);

   EXPECT_LT(robot->get_light_sensor_reading(LEFT_SENSOR),330)
    << "\nFAIL right sensor reading too large";

   EXPECT_GT(robot->get_light_sensor_reading(LEFT_SENSOR),0)
    << "\nFAIL right sensor reading too small";

   robot->reset_sensor_reading();
   // Set light position
   light1->set_position(sensor_l_x+25, sensor_l_y+30);
   light1->set_radius(radius);
   light2->set_position(sensor_l_x-25, sensor_l_y-30);
   light2->set_radius(radius);
   light3->set_position(sensor_l_x+25, sensor_l_y+60);
   light3->set_radius(radius);
   light4->set_position(sensor_l_x-25, sensor_l_y-60);
   light4->set_radius(radius);
   // Notify
   robot->LightNotify(light1->get_pose(),radius);
   robot->LightNotify(light2->get_pose(),radius);
   robot->LightNotify(light3->get_pose(),radius);
   robot->LightNotify(light4->get_pose(),radius);

   EXPECT_LT(robot->get_light_sensor_reading(LEFT_SENSOR),660)
    << "\nFAIL right sensor reading too large";

   EXPECT_GT(robot->get_light_sensor_reading(LEFT_SENSOR),330)
    << "\nFAIL right sensor reading too small";

    robot->reset_sensor_reading();
    // Set light position
    light1->set_position(sensor_l_x+20, sensor_l_y+40);
    light1->set_radius(radius);
    light2->set_position(sensor_l_x-20, sensor_l_y-40);
    light2->set_radius(radius);
    light3->set_position(sensor_l_x+20, sensor_l_y+40);
    light3->set_radius(radius);
    light4->set_position(sensor_l_x-20, sensor_l_y-40);
    light4->set_radius(radius);
    // Notify
    robot->LightNotify(light1->get_pose(),radius);
    robot->LightNotify(light2->get_pose(),radius);
    robot->LightNotify(light3->get_pose(),radius);
    robot->LightNotify(light4->get_pose(),radius);

    EXPECT_LT(robot->get_light_sensor_reading(LEFT_SENSOR),1000)
     << "\nFAIL right sensor reading too large";

    EXPECT_GT(robot->get_light_sensor_reading(LEFT_SENSOR),660)
     << "\nFAIL right sensor reading too small";


 }

 TEST_F(SensorLightTest, RightSensorReaingOnFourLightsEquivalencePartition) {
    // Test if right sensor reading is right when 4 lights around it

    robot->set_radius(radius);
    robot->set_position(0,0);
    robot->set_heading(0);
    robot->reset_sensor_reading();
    // Set light position
    light1->set_position(sensor_r_x+30, sensor_r_y+40);
    light1->set_radius(radius);
    light2->set_position(sensor_r_x-30, sensor_r_y-40);
    light2->set_radius(radius);
    light3->set_position(sensor_r_x+25, sensor_r_y+60);
    light3->set_radius(radius);
    light4->set_position(sensor_r_x-25, sensor_r_y-60);
    light4->set_radius(radius);

    robot->LightNotify(light1->get_pose(),radius);
    robot->LightNotify(light2->get_pose(),radius);
    robot->LightNotify(light3->get_pose(),radius);
    robot->LightNotify(light4->get_pose(),radius);

    EXPECT_LT(robot->get_light_sensor_reading(RIGHT_SENSOR),330)
     << "\nFAIL right sensor reading too large";

    EXPECT_GT(robot->get_light_sensor_reading(RIGHT_SENSOR),0)
     << "\nFAIL right sensor reading too small";

    robot->reset_sensor_reading();
    // Set light position
    light1->set_position(sensor_r_x+25, sensor_r_y+30);
    light1->set_radius(radius);
    light2->set_position(sensor_r_x-25, sensor_r_y-30);
    light2->set_radius(radius);
    light3->set_position(sensor_r_x+25, sensor_r_y+60);
    light3->set_radius(radius);
    light4->set_position(sensor_r_x-25, sensor_r_y-60);
    light4->set_radius(radius);

    robot->LightNotify(light1->get_pose(),radius);
    robot->LightNotify(light2->get_pose(),radius);
    robot->LightNotify(light3->get_pose(),radius);
    robot->LightNotify(light4->get_pose(),radius);

    EXPECT_LT(robot->get_light_sensor_reading(RIGHT_SENSOR),660)
     << "\nFAIL right sensor reading too large";

    EXPECT_GT(robot->get_light_sensor_reading(RIGHT_SENSOR),330)
     << "\nFAIL right sensor reading too small";

     robot->reset_sensor_reading();
     // Set light position
     light1->set_position(sensor_r_x+20, sensor_r_y+40);
     light1->set_radius(radius);
     light2->set_position(sensor_r_x-20, sensor_r_y-40);
     light2->set_radius(radius);
     light3->set_position(sensor_r_x+20, sensor_r_y+40);
     light3->set_radius(radius);
     light4->set_position(sensor_r_x-20, sensor_r_y-40);
     light4->set_radius(radius);

     robot->LightNotify(light1->get_pose(),radius);
     robot->LightNotify(light2->get_pose(),radius);
     robot->LightNotify(light3->get_pose(),radius);
     robot->LightNotify(light4->get_pose(),radius);

     EXPECT_LT(robot->get_light_sensor_reading(RIGHT_SENSOR),1000)
      << "\nFAIL right sensor reading too large";

     EXPECT_GT(robot->get_light_sensor_reading(RIGHT_SENSOR),660)
      << "\nFAIL right sensor reading too small";

 }


 /***************2+ Lights Boundary Conditions*********************************/
 TEST_F(SensorLightTest, LeftSensorReadingOnFourLightsMax){
   // Test if left sensor reading is out of bound when 4 lights are close to it
   robot->set_radius(radius);
   robot->set_position(0,0);
   robot->set_heading(0);
// Set light position
   light1->set_position(sensor_l_x+12, sensor_l_y+16);
   light1->set_radius(radius);
   light2->set_position(sensor_l_x-12, sensor_l_y-16);
   light2->set_radius(radius);
   light3->set_position(sensor_l_x+10, sensor_l_y+24);
   light3->set_radius(radius);
   light4->set_position(sensor_l_x-10, sensor_l_y-24);
   light4->set_radius(radius);

   robot->LightNotify(light1->get_pose(),radius);
   robot->LightNotify(light2->get_pose(),radius);
   robot->LightNotify(light3->get_pose(),radius);
   robot->LightNotify(light4->get_pose(),radius);

   EXPECT_EQ(robot->get_light_sensor_reading(LEFT_SENSOR),1000)
    << "\nFAIL left sensor reading out of bound";
 }


 TEST_F(SensorLightTest, RightSensorReadingOnFourLightsMax) {
   // Test if right sensor reading is out of bound when 4 lights are close to it

   robot->set_radius(radius);
   robot->set_position(0,0);
   robot->set_heading(0);
// Set light position
   light1->set_position(sensor_r_x+12, sensor_r_y+16);
   light1->set_radius(radius);
   light2->set_position(sensor_r_x-12, sensor_r_y-16);
   light2->set_radius(radius);
   light3->set_position(sensor_r_x+10, sensor_r_y+24);
   light3->set_radius(radius);
   light4->set_position(sensor_r_x-10, sensor_r_y-24);
   light4->set_radius(radius);

   robot->LightNotify(light1->get_pose(),radius);
   robot->LightNotify(light2->get_pose(),radius);
   robot->LightNotify(light3->get_pose(),radius);
   robot->LightNotify(light4->get_pose(),radius);
   EXPECT_EQ(robot->get_light_sensor_reading(RIGHT_SENSOR),1000)
    << "\nFAIL right sensor reading out of bound";
 }



 TEST_F(SensorLightTest, LeftSensorOnLightCollision) {
   // Test if left sensor reading is max when collide with one light

   robot->set_radius(radius);
   robot->set_position(0,0);
   robot->set_heading(0);

   robot->reset_sensor_reading();
   // Set light position
   light1->set_pose(robot->get_sensor_position(LEFT_SENSOR));
   light1->set_radius(radius);
   light2->set_position(sensor_l_x-30, sensor_l_y-40);
   light2->set_radius(radius);
   light3->set_position(sensor_l_x+25, sensor_l_y+60);
   light3->set_radius(radius);
   light4->set_position(sensor_l_x-25, sensor_l_y-60);
   light4->set_radius(radius);

   robot->LightNotify(light1->get_pose(),radius);
   robot->LightNotify(light2->get_pose(),radius);
   robot->LightNotify(light3->get_pose(),radius);
   robot->LightNotify(light4->get_pose(),radius);

   EXPECT_EQ(robot->get_light_sensor_reading(LEFT_SENSOR), 1000)
   << "\nFAIL left sensor reading is not 1000 while colliding";

   robot->reset_sensor_reading();
 }

 TEST_F(SensorLightTest, RightSensorOnLightCollision) {
   // Test if right sensor reading is max when collide with one light

   robot->set_radius(radius);
   robot->set_position(0,0);
   robot->set_heading(0);

   robot->reset_sensor_reading();
   // Set light position
   // light 1 collides with robot right sensor
   light1->set_pose(robot->get_sensor_position(RIGHT_SENSOR));
   light1->set_radius(radius);
   light2->set_position(sensor_r_x-30, sensor_r_y-40);
   light2->set_radius(radius);
   light3->set_position(sensor_r_x+25, sensor_r_y+60);
   light3->set_radius(radius);
   light4->set_position(sensor_r_x-25, sensor_r_y-60);
   light4->set_radius(radius);

   robot->LightNotify(light1->get_pose(),radius);
   robot->LightNotify(light2->get_pose(),radius);
   robot->LightNotify(light3->get_pose(),radius);
   robot->LightNotify(light4->get_pose(),radius);

   EXPECT_EQ(robot->get_light_sensor_reading(RIGHT_SENSOR),1000)
   << "\nFAIL right sensor reading not 1000 while colliding";

   robot->reset_sensor_reading();
 }
 TEST_F(SensorLightTest, LeftSensorOnFourVeryFarLights) {
   // Test if left sensor reading is max when collide with one light

      robot->set_radius(radius);
      robot->set_position(0,0);
      robot->set_heading(0);

      // Set light position
      robot->reset_sensor_reading();
      light1->set_position(sensor_l_x+300, sensor_l_y+400);
      light1->set_radius(radius);
      light2->set_position(sensor_l_x-300, sensor_l_y-400);
      light2->set_radius(radius);
      light3->set_position(sensor_l_x+250, sensor_l_y+600);
      light3->set_radius(radius);
      light4->set_position(sensor_l_x-250, sensor_l_y-600);
      light4->set_radius(radius);

      robot->LightNotify(light1->get_pose(),radius);
      robot->LightNotify(light2->get_pose(),radius);
      robot->LightNotify(light3->get_pose(),radius);
      robot->LightNotify(light4->get_pose(),radius);

      EXPECT_GT(robot->get_light_sensor_reading(LEFT_SENSOR),0)
      << "\nFAIL left sensor reading less than 0 while lights very far away";
      EXPECT_LT(robot->get_light_sensor_reading(LEFT_SENSOR),2.2e-13)
      << "\nFAIL left sensor reading is too large while lights very far away";

      robot->reset_sensor_reading();

 }

 TEST_F(SensorLightTest, RightSensorOnFourVeryFarLights) {

      robot->set_radius(radius);
      robot->set_position(0,0);
      robot->set_heading(0);

      robot->reset_sensor_reading();
      // Set light position
      light1->set_position(sensor_r_x+300, sensor_r_y+400);
      light1->set_radius(radius);
      light2->set_position(sensor_r_x-300, sensor_r_y-400);
      light2->set_radius(radius);
      light3->set_position(sensor_r_x+250, sensor_r_y+600);
      light3->set_radius(radius);
      light4->set_position(sensor_r_x-250, sensor_r_y-600);
      light4->set_radius(radius);

      robot->LightNotify(light1->get_pose(),radius);
      robot->LightNotify(light2->get_pose(),radius);
      robot->LightNotify(light3->get_pose(),radius);
      robot->LightNotify(light4->get_pose(),radius);

      EXPECT_GT(robot->get_light_sensor_reading(RIGHT_SENSOR),0)
      << "\nFAIL right sensor reading less than 0 while lights very far away";
      EXPECT_LT(robot->get_light_sensor_reading(RIGHT_SENSOR),2.2e-13)
      << "\nFAIL right sensor reading is too large while lights very far away";
 }


#endif /*SENSORLIGHT_TESTS */
