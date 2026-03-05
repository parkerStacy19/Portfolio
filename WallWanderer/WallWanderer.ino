#include <CtrlUtils.h>               // Control Utility library.
#include "My_EduCar_Params.h"  // Car-specific parameters.
#include "maze.hpp"
#include "solver.hpp"
#include "drive.hpp"

Node nodes[GRID_SIZE]; // 1600


Bot bot(
    Bot_Dir::NORTH, // dir
    ctl::ColorSensor(), // color_sensor
    ctl::SerialMonitor(38400), // serial_monitor
    ctl::PushButton(TEST_BUTTON_Pin, TEST_BUTTON_Pressed), // test_button
    ctl::IRProximitySwitch(PROX_LeftPin, PROX_AlertState), // prox_left
    ctl::IRProximitySwitch(PROX_RightPin, PROX_AlertState), // prox_right
    ctl::IRProximitySwitch(PROX_RearPin, PROX_AlertState), // prox_back
    ctl::DigitalLed(STATUS_LED_LeftPin, STATUS_LED_ON), // prox_led_left
    ctl::DigitalLed(STATUS_LED_RightPin, STATUS_LED_ON), // prox_led_right
    ctl::DigitalLed(STATUS_LED_CenterPin, STATUS_LED_ON), // prox_led_back
    ctl::DigitalLed(CS_LED_Pin, CS_LED_ON), // cs_led
    ctl::AnalogInput(BAT_Pin, BAT_MinValue, BAT_MaxValue), // battery_input
    ctl::ActiveBuzzer(ALERT_BUZZER_Pin, ALERT_BUZZER_ON), // buzzer
    ctl::SG90Servo(RS_AnglePin), // rs_servo
    ctl::HCSR04RangeSensor(URS_TriggerPin, URS_EchoPin), // ultra
    ctl::BYJ48AccelStepperMotor(
        ctl::BYJ48AccelStepperMotor::HALF4WIRE,
        LEFT_MOTOR_IN1, LEFT_MOTOR_IN2, LEFT_MOTOR_IN3, LEFT_MOTOR_IN4), // left_motor
    ctl::BYJ48AccelStepperMotor(
        ctl::BYJ48AccelStepperMotor::HALF4WIRE,
        RIGHT_MOTOR_IN1, RIGHT_MOTOR_IN2, RIGHT_MOTOR_IN3, RIGHT_MOTOR_IN4), // right_motor
    ctl::StepperDriveTrain(
        bot.right_motor,
        bot.left_motor,
        WHEEL_DIAMETER_MM, WHEEL_BASE_MM) // drive_train
);

uint8_t x = GRID_WIDTH / 2;
uint8_t y = GRID_WIDTH / 2;

Stack stack;

void setup()
{
  for (uint16_t i = 0; i < GRID_SIZE; i++) {
    nodes[i].state = 0;
  }

  // Finish initialization of IDE Serial Monitor window.

  bot.serial_monitor.setup();

  // Finish initialization of Range Finder Servo control object.

  bot.rs_servo.setup();
  bot.rs_servo.moveTo(RSPOS_FORWARD);

  // Finish initialization of Color Sensor object.

  if ( !bot.color_sensor.setup() )
  {
    // This error is fatal! Sound major alert and do NOT return!

    FatalError(bot, "Color Sensor setup error!");
  }

  // Turn off color sensor LED to conserve power.

  bot.cs_led.off();

  analogReference(INTERNAL2V56);  // Use internal "2.56" volt ADC reference voltage.

  // Take several battery voltage readings, so ADC reference voltage is
  // used in subsequent single voltage readings.

  AvgBatteryVoltage(bot, 10);

  // Provide visual feedback that sensors are operational.
  // Will wait for test button press before returning.

  VisualSensorFeedback(bot);

  double speedMM = ( bot.right_motor.getMaxReliableSpeed() /
                     bot.right_motor.getStepsPerRevolution() ) * WHEEL_CIRCUMFERENCE_MM;  

  bot.drive_train.setSpeed(speedMM);
  bot.drive_train.setAcceleration(speedMM / 2.0);

  // Wait for test button release.
  while ( bot.test_button.open() )
  { }


  bot.cs_led.on();
  ctl::WaitMillis(1000);
  double cscale, rscale, gscale, bscale;
  bot.color_sensor.calibrateWhiteBalance(cscale, rscale, gscale, bscale);
  bot.color_sensor.setWhiteBalance(cscale, rscale, gscale, bscale);
  bot.cs_led.off();

  // Wait for test button release.
  while ( bot.test_button.open() )
  { }

  bot.serial_monitor.sendText("Moving on to looooooop");
  bot.serial_monitor.sendNewline();

  bot.serial_monitor.sendText("Starting X coord: ");
  bot.serial_monitor.sendIntegerValue(x);
  bot.serial_monitor.sendNewline();

  bot.serial_monitor.sendText("Starting Y coord: ");
  bot.serial_monitor.sendIntegerValue(y);
  bot.serial_monitor.sendNewline();
}

// Called repeatedly until Arduino reset button is pressed.

void loop()
{
  do_node(x, y, nodes, bot, stack);
}
