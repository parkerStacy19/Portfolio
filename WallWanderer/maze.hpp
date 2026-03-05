#pragma once

#include <CtrlUtils.h>  // Control Utility library.
#include <stdint.h>

#define GRID_WIDTH 30
#define GRID_SIZE GRID_WIDTH* GRID_WIDTH

#define MAX_PATH 12 * 6  // Full demo table

const uint8_t NS_VISITED = 0b00000001;
const uint8_t NS_NORTH = 0b00000010;
const uint8_t NS_WEST = 0b00000100;
const uint8_t NS_SOUTH = 0b00001000;
const uint8_t NS_EAST = 0b00010000;
const uint8_t NS_GOAL = 0b00100000;
const uint8_t NS_START = 0b01000000;

struct Node {
  uint8_t state;  // 0th visited
                  // 1st north wall
                  // 2nd west wall
                  // 3rd south wall
                  // 4th east wall
};

enum class Node_Type : uint8_t {
  GENERAL = 0,
  GOAL,
  DEAD_END,
  START,
  UNKNOWN,
};

enum class Bot_Dir : uint8_t { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

// Just keep track of which directions we've gone
typedef Bot_Dir Stack_t;
class Stack {
 public:
  Stack() : size(0) {}

  Stack_t dirs[MAX_PATH];
  uint8_t size;

  void append(Stack_t val) {
    dirs[size] = val;
    size++;
  }

  Stack_t pop() {
    size--;
    return dirs[size];
  }
};

struct Bot {
  Bot_Dir dir;
  ctl::ColorSensor color_sensor;
  ctl::SerialMonitor serial_monitor;
  ctl::PushButton test_button;
  ctl::IRProximitySwitch prox_left, prox_right, prox_back;
  ctl::DigitalLed prox_led_left, prox_led_right, prox_led_back;
  ctl::DigitalLed cs_led;
  ctl::AnalogInput battery_input;
  ctl::ActiveBuzzer buzzer;
  ctl::SG90Servo rs_servo;
  ctl::HCSR04RangeSensor ultra;
  ctl::BYJ48AccelStepperMotor right_motor, left_motor;
  ctl::StepperDriveTrain drive_train;

  // Constructor
  Bot(Bot_Dir dir, const ctl::ColorSensor& color_sensor,
      const ctl::SerialMonitor& serial_monitor,
      const ctl::PushButton& test_button,
      const ctl::IRProximitySwitch& prox_left,
      const ctl::IRProximitySwitch& prox_right,
      const ctl::IRProximitySwitch& prox_back,
      const ctl::DigitalLed& prox_led_left,
      const ctl::DigitalLed& prox_led_right,
      const ctl::DigitalLed& prox_led_back, const ctl::DigitalLed& cs_led,
      const ctl::AnalogInput& battery_input, const ctl::ActiveBuzzer& buzzer,
      const ctl::SG90Servo& rs_servo, const ctl::HCSR04RangeSensor& ultra,
      const ctl::BYJ48AccelStepperMotor& right_motor,
      const ctl::BYJ48AccelStepperMotor& left_motor,
      const ctl::StepperDriveTrain& drive_train)
      : dir(dir),
        color_sensor(color_sensor),
        serial_monitor(serial_monitor),
        test_button(test_button),
        prox_left(prox_left),
        prox_right(prox_right),
        prox_back(prox_back),
        prox_led_left(prox_led_left),
        prox_led_right(prox_led_right),
        prox_led_back(prox_led_back),
        cs_led(cs_led),
        battery_input(battery_input),
        buzzer(buzzer),
        rs_servo(rs_servo),
        ultra(ultra),
        right_motor(right_motor),
        left_motor(left_motor),
        drive_train(drive_train) {}
};
