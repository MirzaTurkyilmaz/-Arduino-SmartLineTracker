/* 
 * LINE FOLLOWER ROBOT - ARDUINO NANO
 * ==================================
 * 
 * GitHub Repository: https://github.com/yourusername/line-follower-robot
 * 
 * Author: [Mirza_TURKYILMAZ]
 * Date: [05.02.2025]
 * License: MIT
 * 
 * Description:
 * Implements line following logic using two IR sensors
 * Controls differential drive motors with PWM speed control
 * Includes collision avoidance and basic motion primitives
 * 
 * Wiring Diagram:
 * - Right Motor: IN1(8), IN2(9), ENR(PWM3)
 * - Left Motor: IN3(10), IN4(11), ENL(PWM5)
 * - Right Sensor: D6
 * - Left Sensor: D7
 * - Status LED: Built-in D13
 */

// DEPENDENCIES
#include <Arduino.h>

// HARDWARE CONFIGURATION
// ======================
// Motor Control Pins
const int MR1 = 8;      // Right motor forward
const int MR2 = 9;      // Right motor reverse
const int ML1 = 10;     // Left motor forward
const int ML2 = 11;     // Left motor reverse

// Sensor Interface
const int SENSOR_R = 6; // Right IR sensor (LOW=line detected)
const int SENSOR_L = 7; // Left IR sensor (LOW=line detected)

// PWM Control
const int ENR = 3;      // Right motor speed (PWM)
const int ENL = 5;      // Left motor speed (PWM)

// SYSTEM PARAMETERS
// =================
const int STATUS_LED = 13;    // System status indicator
const int BASE_SPEED = 100;   // Normal operation speed (0-255)
const int TURN_SPEED = 255;   // Maximum turning speed
const int TURN_DURATION = 20; // Turning maneuver time (ms)

// GLOBAL VARIABLES
// ================
bool sensorStateR = LOW;      // Right sensor reading
bool sensorStateL = LOW;      // Left sensor reading

void setup() {
  // Initialize motor control pins
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  
  // Configure sensor inputs
  pinMode(SENSOR_R, INPUT);
  pinMode(SENSOR_L, INPUT);

  // System status indicator
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, HIGH); // Show initialization complete
  
  // Safety delay before movement
  delay(5000); // Allow time for positioning robot
}

void loop() {
  updateSensorReadings();
  executeLineFollowingLogic();
}

// SENSOR MANAGEMENT
// =================
void updateSensorReadings() {
  sensorStateR = digitalRead(SENSOR_R);
  sensorStateL = digitalRead(SENSOR_L);
}

// DECISION MAKING
// ===============
void executeLineFollowingLogic() {
  if (sensorsBothActive()) {
    emergencyStop();
  } else if (rightSensorActive()) {
    executeRightTurn();
  } else if (leftSensorActive()) {
    executeLeftTurn();
  } else {
    maintainForwardMotion();
  }
}

bool sensorsBothActive() {
  return sensorStateR == HIGH && sensorStateL == HIGH;
}

bool rightSensorActive() {
  return sensorStateR == HIGH && sensorStateL == LOW;
}

bool leftSensorActive() {
  return sensorStateR == LOW && sensorStateL == HIGH;
}

// MOTION CONTROL
// ==============
void maintainForwardMotion() {
  setMotorDirection(HIGH, LOW, HIGH, LOW);
  setMotorSpeed(BASE_SPEED, BASE_SPEED);
}

void executeRightTurn() {
  setMotorDirection(LOW, HIGH, HIGH, LOW);
  setMotorSpeed(TURN_SPEED, TURN_SPEED);
  delay(TURN_DURATION);
}

void executeLeftTurn() {
  setMotorDirection(HIGH, LOW, LOW, HIGH);
  setMotorSpeed(TURN_SPEED, TURN_SPEED);
  delay(TURN_DURATION);
}

void emergencyStop() {
  setMotorSpeed(0, 0);
}

// LOW-LEVEL MOTOR CONTROL
// =======================
/**
 * @brief Sets motor directions
 * @param r1 Right motor forward (HIGH/LOW)
 * @param r2 Right motor reverse (HIGH/LOW)
 * @param l1 Left motor forward (HIGH/LOW)
 * @param l2 Left motor reverse (HIGH/LOW)
 */
void setMotorDirection(bool r1, bool r2, bool l1, bool l2) {
  digitalWrite(MR1, r1);
  digitalWrite(MR2, r2);
  digitalWrite(ML1, l1);
  digitalWrite(ML2, l2);
}

/**
 * @brief Sets motor speeds using PWM
 * @param rightSpeed Right motor speed (0-255)
 * @param leftSpeed Left motor speed (0-255)
 */
void setMotorSpeed(int rightSpeed, int leftSpeed) {
  analogWrite(ENR, rightSpeed);
  analogWrite(ENL, leftSpeed);
}

/*
 * MIT License
 * 
 * Copyright (c) [2025] [Mirza_TURKYILMAZ]
 * 
 * Permission is hereby granted... [Full license text]
 */
