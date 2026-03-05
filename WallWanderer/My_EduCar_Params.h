// Constants specific to your particular EduCar (Car #5).
// Arduino Mega controller board with stepper motor drive train.

#pragma once

#include <Arduino.h>    // In case it's not already been included.

// Define test control pushbutton parameters.

const int TEST_BUTTON_Pin = A3;       // GPIO pin button is connected to.
const int TEST_BUTTON_Pressed = LOW;  // State indicating button is pressed.

// Define alert buzzer parameters.

const int ALERT_BUZZER_Pin = 53;      // Pin active buzzer is connected to.
const int ALERT_BUZZER_ON = HIGH;     // Logic state to turn buzzer on.

// Define status LED parameters.

const int STATUS_LED_RightPin = 51;   // Right side status LED GPIO pin.
const int STATUS_LED_CenterPin = 49;  // Center rear status LED GPIO pin.
const int STATUS_LED_LeftPin = 47;    // Left side status LED GPIO pin.
const int STATUS_LED_ON = HIGH;       // Logic state to turn on a status LED.

// Define params related to measuring the battery voltage.

const int BAT_Pin = A7;     // Analog GPIO pin used to read battery voltage.

const int BAT_MinValue = 0;     // Analog input minimum value.
const int BAT_MaxValue = 1023;  // Analog input maximum value.

const double BAT_VoltageRef = 2.48;     // Analog AREF voltage reference.
const double BAT_VoltageRatio = 11.073; // Input voltage divider ratio.

// Define infrared proximity parameters.

const int PROX_RightPin = 4;      // Right side proximity sensor GPIO pin.
const int PROX_LeftPin = 5;       // Left side proximity sensor GPIO pin.
const int PROX_RearPin = A2;      // Rear proximity sensor GPIO pin.
const int PROX_AlertState = LOW;  // Proximity sensor alert (too close) state

// Define color sensor LED params.

const int CS_LED_Pin = 12;    // White LED GPIO control pin.
const int CS_LED_ON = HIGH;   // State which turns white LED on.

// ULN2003 stepper motor driver module GPIO pin assignments.
// See ULN2003 documentation for what these mean.

const int LEFT_MOTOR_IN1 = A8;
const int LEFT_MOTOR_IN2 = A9;
const int LEFT_MOTOR_IN3 = A10;
const int LEFT_MOTOR_IN4 = A11;

const int RIGHT_MOTOR_IN1 = A12;
const int RIGHT_MOTOR_IN2 = A13;
const int RIGHT_MOTOR_IN3 = A14;
const int RIGHT_MOTOR_IN4 = A15;

// Define HCSR04 Ultrasonic Range Sensor control parameters.

const int URS_TriggerPin = A0;  // GPIO Trigger pin
const int URS_EchoPin = A1;     // GPIO Echo pin

// Define range sensor servo control parameters.

const int RS_AnglePin = 6;      // GPIO pin controlling servo angle.

// Define some specific range sensor servo positions.

const uint16_t RSPOS_RIGHT = 180;         // Pointing to the right.
const uint16_t RSPOS_RIGHT_FORWARD = 135; // Pointing right-forward.
const uint16_t RSPOS_FORWARD = 90;	      // Pointing forward.
const uint16_t RSPOS_LEFT_FORWARD = 45;   // Pointing left-forward.
const uint16_t RSPOS_LEFT = 0;     	      // Pointing to the left.

// Define wheel-related dimensions.

const double WHEEL_DIAMETER_MM = 61.5;	// Outer edge tire distance.
const double WHEEL_CIRCUMFERENCE_MM = PI * WHEEL_DIAMETER_MM;

const double WHEEL_BASE_MM = 122.5;		// Center to center distance between tires.
const double CAR_CIRCUMFERENCE_MM = PI * WHEEL_BASE_MM;

// EEPROM diagnostic data store parameters.
// EEPROM holds 2 bytes per integer value.

const size_t EEPROM_MAX_VALUES = ctl::GetEEPROMSize() / sizeof(int);
