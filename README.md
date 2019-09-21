# Braitenberg Vehicle Simulator

## Overview
The goal of this software project is to develop a rudimentary robot simulator in which robot behavior is visualized within a graphics window. The robots are modeled after Braitenberg Vehicles, which can autonomously move around based on its sensor inputs. The resulting behavior may appear complex or even intelligent towards stimuli. The environment will have multiple robots, as well as stimuli such as lights and food. Robots will exhibit different behavior towards these stimuli, depending on their sensor-motor connections related to that stimulus. For example, some robots will explore the lights and some fear the lights. And the relationship to food will change over time as the robot gets hungrier. User can build the environment manually by selecting the number of robots, number of lights, number of food, ratio of fearful and exploratory robots, and inclusion of food.

## Developer Guide
 **Note: For detailed design pattern explanation, please refer to: https://github.com/sidankkirsten/Braitenberg-Vehicle-Simulator/blob/master/docs/Design_Doc.pdf

 **Note: UML Diagram is at https://github.com/sidankkirsten/Braitenberg-Vehicle-Simulator/blob/master/docs/UML.pdf

### 1. Observer Pattern
The implementation idea is based on the idea that the Subject is the Robot Class, which serves to update robot’s velocity and position and notify all the other sensors when an entity, such as food and light, has been updated for each time step in the arena. Observer is the main Sensor class. The `Robot::LightNotify()` and `Robot::FoodNotify` will be called whenever the observed object, such as food and light, is changed.

The benefit of this implementation is that robot can directly notify sensors and trigger motion handler to control the wheels at the same time, which is efficient. The drawback of this implementation is that the information that the sensor receives is not directly from the arena, which holds all the information of entities. This makes the process of notify observers (sensors) in the arena more complicated and difficult to understand.

### 2. Strategy Pattern
The implementation is based on the idea that the context will be the Robot class and the interface is the `MotionHandler` class. Two strategies will be `MotionHandlerFear` and `MotionHandlerExplore`. The robot of fear (coward) will use `MotionHandlerFear` as its motion handler and robot of explore will use `MotionHandlerExplore`. Aggressive behavior will be realized in both `MotionHandlerExplore` and `MotionHandlerFear` and will be triggered when the robot is really hungry.

The benefit of this implementation is that when the robot is not hungry after eating food, the robot does not need to change motion handler, which cause less time and space. However, as for the audience/ new developer, it can be not clear enough for them that the robot has aggressive behavior towards food because the aggressive behavior is hidden/handled in the `MotionHandlerFear` and `MotionHandlerExplore`.

### 3. Feature Enhancement
#### 3.1 Scenario
There are three different sensor-wheel connections in this simulation, including Coward, Aggressive and Explore.

Coward has positive sensor-wheel connection.

Explore has negative and crossed sensor-wheel connection.

Aggressive has positive and crossed sensor-wheel connection.
#### 3.2 Tutorial
In this tutorial, we will add a new stimulus, water, and the water sensor to the simulator. Code examples are provided to provide an idea of how the stimulus and sensor will be implemented given the current code from current simulation. We will use the food and food sensor as the code to implement our new code.
3.3.1 Adding a new stimulus (water)

1. Make the header and source file for water.

   Water will inherit from the ArenaImmobileEntity class.

   Create constructors and methods related to water and make sure the position will be set randomly and the radius will be set randomly as needed.Refer to Food::Food().

   In the source file, there will be a reset function to reset the attributes and the position of the water object when the Reset game function is called.Refer to void Food::Reset()

2. Create the water entity in the EntityFactory class

   Include the header file of the water class and make a private variable to create water. Refer to Food* EntityFactory::CreateFood() {

3. Create a water type `kWater` in the entity type class

4. Add water Entity in the `Arena` class

   Define the amount of water entities in the `params` class

   Add the amount of water entities in Arena::Arena

3.3.2 Adding a new sensor

1. Make header and source file for sensor

   The sensor will inherit from the Sensor class.

   Add new sensor type to `SensorType` class.

2. Create water sensors in the `robot` class

   Include the header file of the water sensor class and make two private  variables to create left water sensor and right water sensor.

3. Add notify functionality in the Robot class. Refer to Robot::FoodNotify(Pose food_pose, double food_radius)

4. Add reset water sensor reading functionality in robot class

3.3.3 Pass new sensor reading to motion handler in robot

1. Pass the left and right water sensor reading into `MotionHandler::UpdateVelocity`.

2. Modify the `MotionHandlerExplore::UpdateVelocity` and `MotionHandlerFear::UpdateVelocity` as above. Make sure to relate the speed to both newly added readings.

3.3.4 Get updated from Arena

1. Enable the robot to notify the water sensor in arena.

  In `Arena:: UpdateEntitiesTimestep()`, we add a new if statement to enable the robot to notify water sensor on the updates of water. Follow the pattern that how food sensor be notified.

3.3.5 Compile and Finish

1. At command prompt,  `make` in the `iteration3/src`.

2. At command prompt, `./../build/bin/arenaviewer`, check your new simulation.

Wonderful! You made it!

## Non-Technical User Guide

### Download
1. Go to https://github.com/sidankkirsten/Braitenberg-Vehicle-Simulator  
2. _pull_ the Braitenberg-Vehicle-Simulator

### Configuration
1. libMinGfx should be installed. If it is not installed, please refer to the installation guide of libMinGfx at
 https://ivlab.github.io/MinGfx/installation.html

2. Support Mac OS 10.11 or higher, Ubuntu 14.04 or higher

### Operation

#### Build
1. Modify the `CS3081DIR` variable in `src/Makefile`
2. At command prompt, `cd _mypath_/Braitenberg-Vehicle-Simulator/src`.
  `_mypath_` refers to the path of the project-qixxx259 folder.
3. At command prompt, `make`
4. At command prompt, `./../build/bin/arenaviewer`

#### Adjustment
The Menu box is located to the right of the Arena

##### Simulation Control

`Play and Pause`

 Pressing the Play/Pause button to toggle between pausing and playing a game

`New Game`

 Pressing the New Game button to start a new game

`Turn off food`/`Turn on food`

 Pressing the Turn off food/Turn on food button to toggle between including and excluding the food element.

**Note: If the food is turned off, the robot will never get hungry or starve and will not exhibit aggression.**

##### Arena Configuration
`Number of Robots`

 Moving the slider below Number of Robots to select the number of robots for the next simulation

`Number of Lights`

 Moving the slider below Number of Lights to select the number of lights for the next simulation

`Ratio of Fearful Robots`

 Moving the slider below Ratio of Fearful Robots to select the ratio of fearful robots for the next simulation

`Light Sensor Sensitivity`

 Moving the slider below Light Sensor Sensitivity to select the sensitivity of light sensor for the next simulation. The light sensor will be more acute towards stimuli as sensitivity increases.

`Number of Food`

 Moving the slider below Number of Food to select the number of food for the next simulation. This slider will not work when the food is turned off.


**Note: In Arena Configuration, the number will increase as the slider is moved to the right. Move the sliders first, then press the New Game button. Every adjustment will be applied after the New Game button is pressed.**
