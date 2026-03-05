# WallWanderer: Maze-Solving Robot

## Overview

This project is an autonomous maze-solving robot project that demonstrates embedded systems programming and algorithm design with memory contraints. Built on Arduino Mega, this project showcases the integration of multiple sensors and actuators to control a robot capable of navigating grid-based mazes using a combination of wall-following techniques and depth-first search algorithms.

This was a collaborative project with **Levi Smith**, **Trinity Johnson**, and **Carlos Ortiz**.

## Technologies Used

- **Programming Language**: C++ (Arduino framework)
- **Hardware Platform**: Arduino Mega
- **Sensors**: IR proximity sensors, color sensor, ultrasonic rangefinder
- **Actuators**: Stepper motors, servo motor, LEDs, buzzer
- **Libraries**: CtrlUtils (custom control utility library)

## Key Features

### Autonomous Navigation
- Precise stepper motor control for drive system
- Real-time obstacle detection and avoidance

### Maze Solving Algorithm
- Grid-based maze representation (30x30 nodes)
- Depth-first search with backtracking using a custom stack design
- Wall detection and mapping using IR sensors
- Color-based start/goal detection

### Sensor Integration
- Multi-sensor fusion for reliable navigation
- Ultrasonic ranging for long-distance obstacle detection
- Color sensor calibration and white balance adjustment
- Proximity sensor feedback with visual LED indicators

## Code Highlights

### Maze Representation
The project implements a grid system using bit-packed data structures. Each node in the 30x30 maze grid stores wall presence, visit status, and start/goal indicators in a single byte. This compact representation is due to the Arduino Mega's memory constraints and allows for storing the entire maze state in just 900 bytes while tracking wall positions, visited nodes, and special locations.

```cpp
const uint8_t NS_VISITED = 0b00000001;
const uint8_t NS_NORTH = 0b00000010;
const uint8_t NS_WEST = 0b00000100;
const uint8_t NS_SOUTH = 0b00001000;
const uint8_t NS_EAST = 0b00010000;
const uint8_t NS_GOAL = 0b00100000;
const uint8_t NS_START = 0b01000000;

struct Node {
  uint8_t state;  // Bit flags for walls and status
};
```

### Algorithm
The core maze-solving algorithm combines wall-following with depth-first search and backtracking. The `do_node` function implements the main decision logic:

```cpp
Node_Type do_node(uint8_t& x, uint8_t& y, Node* nodes, Bot& bot, Stack& stack) {
  // Check for goal via color sensor
  // Map surrounding walls using IR proximity sensors
  // Determine next move prioritizing unexplored directions
  // Use stack for backtracking when dead ends are reached
}
```

The algorithm maintains a stack to track the path, allowing the robot to backtrack when encountering dead ends and ensuring new nodes are visited first.

### Sensor Integration
Multiple sensors work together to provide environmental awareness. The system merges data from IR proximity sensors for wall detection, color sensors for start/goal identification, and ultrasonic rangefinders to avoid long-range obstacles.

```cpp
// Color sensor calibration and tape detection
bot.cs_led.on();
ctl::TapeColor tapeColor = bot.color_sensor.getTapeColor();
bot.cs_led.off();

// Wall detection using IR sensors
bool wallNorth = bot.prox_front.isAlert();
bool wallLeft = bot.prox_left.isAlert();
bool wallRight = bot.prox_right.isAlert();
```

## File Structure

- `WallWanderer.ino`: Main Arduino sketch and program flow
- `maze.hpp`: Maze grid data structures and node management
- `solver.hpp`: Core maze-solving algorithm implementation
- `drive.hpp`: Motor control, sensor reading, and hardware interfaces, *Provided by Instructor*
- `utils.hpp`: Direction calculations and maze utility functions
- `My_EduCar_Params.h`: Hardware pin definitions and configuration constants, *Provided by Instructor*