#pragma once

#include <CtrlUtils.h>  // Control Utility library.

#include "My_EduCar_Params.h"  // Car-specific parameters.
#include "maze.hpp"

void MoveWithObstaclePauses(Bot& bot);
void StopAndWait(Bot& bot, uint32_t waitInMillis);
void VisualSensorFeedback(Bot& bot);
void DisplayRangeMM(Bot& bot, uint16_t angle, double distanceMM);
double CheckBatteryVoltage(Bot& bot);
double ReadBatteryVoltage(Bot& bot);
double AvgBatteryVoltage(Bot& bot, uint16_t numSamples);
double AvgBatteryVoltage(Bot& bot, uint16_t numSamples);
double ReadRangeMM(Bot& bot, uint16_t angle);
void ShowAllProximityStatus(Bot& bot);
bool ShowProximityStatus(ctl::IRProximitySwitch& proxSwitch,
                         ctl::DigitalLed& proxLed);
void TurnProximityLedsOff(Bot& bot);
void FatalError(Bot& bot, const String& message);

// ----------------------------------------------------------------------
// Move car, pausing when an obstacle is encountered.

void MoveWithObstaclePauses(Bot& bot) {
  // Continue to step motors until distance has been reached.

  while (bot.drive_train.stepMotors()) {
    ShowAllProximityStatus(bot);
  }
}

// ----------------------------------------------------------------------
// Stop motion and wait the specified time (in milliseconds).
// After stopping, checks if the battery voltage is okay.

void StopAndWait(Bot& bot, uint32_t waitInMillis) {
  bot.drive_train.stop();

  CheckBatteryVoltage(bot);

  ctl::WaitMillis(waitInMillis);
}

// ----------------------------------------------------------------------
// To extent possible, provide visual feedback various sensors are working.

void VisualSensorFeedback(Bot& bot) {
  // Sound buzzer once so we know it's working, and to signal start of
  // visual feedback test.

  bot.buzzer.alert();

  // Take the average of several battery voltage readings.

  double avgVoltage = AvgBatteryVoltage(bot, 10);

  bot.serial_monitor.sendNewline();
  bot.serial_monitor.sendText("Battery voltage = ");
  bot.serial_monitor.sendDoubleValue(avgVoltage, 3);
  bot.serial_monitor.sendNewline();
  bot.serial_monitor.sendNewline();

  // Take (and display) an ultrasonic distance reading from left side,
  // then from the right side, then from the forward position.

  DisplayRangeMM(bot, RSPOS_RIGHT_FORWARD, ReadRangeMM(bot, RSPOS_RIGHT_FORWARD));
  DisplayRangeMM(bot, RSPOS_LEFT_FORWARD, ReadRangeMM(bot, RSPOS_LEFT_FORWARD));
  DisplayRangeMM(bot, RSPOS_FORWARD, ReadRangeMM(bot, RSPOS_FORWARD));

  // Blink each status LED so we know it's working.

  static const uint32_t ledBlinkTime = 1000000;  // Blink time (in microseconds)
  static const uint32_t ledOnOffTime = ledBlinkTime / 2;

  bot.prox_led_right.blink(ledOnOffTime, ledOnOffTime, ledBlinkTime);
  bot.prox_led_back.blink(ledOnOffTime, ledOnOffTime, ledBlinkTime);
  bot.prox_led_left.blink(ledOnOffTime, ledOnOffTime, ledBlinkTime);

  // Turn on color sensor white LED and take a calibration reading.
  // Note: This calibration reading is only useful if the sensor
  //       is over the standard "white" material.

  static const uint32_t ledOnTime = 1000;  // (In milliseconds.)

  bot.cs_led.on();
  ctl::WaitMillis(ledOnTime);

  double cScale, rScale, gScale, bScale;  // Holds calibration scaling values.

  bot.color_sensor.calibrateWhiteBalance(cScale, rScale, gScale, bScale);
  bot.color_sensor.setWhiteBalance(cScale, rScale, gScale, bScale);

  bot.cs_led.off();

  // Sound buzzer twice to indicate we're waiting for the
  // test button to be pressed.

  bot.buzzer.alert();
  bot.buzzer.alert();

  // Wait for test button to be pressed (i.e., closed) before proceeding.
  // While waiting, display proximity switch status in topside LEDs.

  while (bot.test_button.open()) {
    ShowAllProximityStatus(bot);
  }

  TurnProximityLedsOff(bot);
}

// ----------------------------------------------------------------------
// Display range value (in millimeters) taken at a specific angle (in degrees).

void DisplayRangeMM(Bot& bot, uint16_t angle, double distanceMM) {
  bot.serial_monitor.sendText("Range[");
  bot.serial_monitor.sendUnsignedIntegerValue(angle);
  bot.serial_monitor.sendText("] = ");
  bot.serial_monitor.sendDoubleValue(distanceMM);
  bot.serial_monitor.sendText(" (mm)");
  bot.serial_monitor.sendNewline();
}

// ----------------------------------------------------------------------
// Check the battery voltage to see if it's too low.
// Returns voltage after issuing any alert beeps.
// If you hear three (or more) beeps, stop car immediately and recharge.

double CheckBatteryVoltage(Bot& bot) {
  static const double warnLevel =
      6.80;  // Voltage level where audible warnings begin.
  static const double warnInterval =
      0.10;  // Extra alert beep each interval below "warnLevel".

  double batteryVoltage = AvgBatteryVoltage(bot, 10);  // Read average voltage.

  bot.serial_monitor.sendText("CheckBatteryVoltage = ");
  bot.serial_monitor.sendDoubleValue(batteryVoltage);
  bot.serial_monitor.sendNewline();

  // If voltage is <= the warning level, issue alert beeps.
  // Otherwise, do nothing.

  if (batteryVoltage <= warnLevel) {
    // Issue one alert beep for each interval voltage is below threshold.

    double voltage = batteryVoltage;

    do {
      bot.buzzer.alert();

      voltage += warnInterval;

    } while (voltage < warnLevel);
  }

  return batteryVoltage;
}

// ----------------------------------------------------------------------
// Returns the battery voltage as a floating point value (i.e., 7.35 volts).

double ReadBatteryVoltage(Bot& bot) {
  // Read battery input as fraction of possible input range.

  double rangeFraction = bot.battery_input.input();

  // Calculate maximum voltage we can measure.

  double maxVoltage = BAT_VoltageRef * BAT_VoltageRatio;

  // Determine actual voltage being measured.

  return maxVoltage * rangeFraction;
}

// ----------------------------------------------------------------------
// Return the average of several battery voltage readings.

double AvgBatteryVoltage(Bot& bot, uint16_t numSamples) {
  double sumVoltages = 0.0;

  for (uint16_t sample = 1; sample <= numSamples; sample++) {
    sumVoltages += ReadBatteryVoltage(bot);
    ctl::WaitMillis(100);  // Short delay between samples.
  }

  return sumVoltages / numSamples;
}

// ----------------------------------------------------------------------
// Read the range (in millimeters) at a specified ultrasonic sensor angle.

double ReadRangeMM(Bot& bot, uint16_t angle) {
  static const uint32_t servoSettleTime = 100;

  bot.rs_servo.moveTo(angle);
  ctl::WaitMillis(servoSettleTime);  // Wait for servo to mechanically settle.

  return bot.ultra.pingRangeCM() * 10.0;  // Ping to find range distance.
}

// ----------------------------------------------------------------------
// Show the state of all proximity sensors in the proximity LEDs.

void ShowAllProximityStatus(Bot& bot) {
  ShowProximityStatus(bot.prox_right, bot.prox_led_right);
  ShowProximityStatus(bot.prox_left, bot.prox_led_left);
  ShowProximityStatus(bot.prox_back, bot.prox_led_back);
}

// ----------------------------------------------------------------------
// Show a proximity sensor state in a proximity LED.
// Also returns proximity sensor state.
// "true" means proximity sensor is closed (i.e., we're too close).
// "false" means proximity sensor is open (i.e., we're NOT too close).

bool ShowProximityStatus(ctl::IRProximitySwitch& proxSwitch,
                         ctl::DigitalLed& proxLed) {
  bool proxState = proxSwitch.closed();

  // If proximity switch is on (i.e., closed), then turn proximity LED on.
  // Otherwise, turn proximity LED off.

  if (proxSwitch.closed()) {
    proxLed.on();
  } else {
    proxLed.off();
  }

  return proxState;
}

// ----------------------------------------------------------------------
// Turn all proximity LEDs off.

void TurnProximityLedsOff(Bot& bot) {
  bot.prox_led_right.off();
  bot.prox_led_left.off();
  bot.prox_led_back.off();
}

// ----------------------------------------------------------------------
// Call this routine on a fatal error.  It does NOT return!

void FatalError(Bot& bot, const String& message) {
  bot.serial_monitor.sendNewline();
  bot.serial_monitor.sendText("Fatal Error: ");
  bot.serial_monitor.sendText(message);
  bot.serial_monitor.sendNewline();
  bot.serial_monitor.sendText("Halting program!");
  bot.serial_monitor.sendNewline();

  // Sound a short burst alert.

  static const uint32_t fatalBeepTime = 50;  // (In milliseconds.)

  for (size_t numTimes = 1; numTimes <= 10; numTimes++) {
    bot.buzzer.on();
    ctl::WaitMillis(fatalBeepTime);
    bot.buzzer.off();
    ctl::WaitMillis(fatalBeepTime);
  }

  // Enter an infinite loop...

  while (true) {
    ctl::WaitMillis(100);
  }
}
