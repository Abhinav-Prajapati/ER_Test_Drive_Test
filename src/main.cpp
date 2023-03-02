/*******************************************************************************
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTY AND SUPPORT
 * IS APPLICABLE TO THIS SOFTWARE IN ANY FORM. CYTRON TECHNOLOGIES SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR CONSEQUENTIAL
 * DAMAGES, FOR ANY REASON WHATSOEVER.
 ********************************************************************************
 *
 *
 * This example shows how to drive 2 motors using the PWM and DIR pins with
 * 2-channel motor driver.
 *
 *
 *
 *
 * Arduino D3  - Motor Driver PWM 1 Input  12
 * Arduino D4  - Motor Driver DIR 1 Input  9
 *
 * Arduino D9  - Motor Driver PWM 2 Input  11
 * Arduino D10 - Motor Driver DIR 2 Input  10
 * Arduino GND - Motor Driver GND
 *
 *
 * AUTHOR   : Kong Wai Weng
 * COMPANY  : Cytron Technologies Sdn Bhd
 * WEBSITE  : www.cytron.io
 * EMAIL    : support@cytron.io
 *
 *******************************************************************************/

#include <Arduino.h>
#include "CytronMotorDriver.h"
#include <IBusBM.h>
#define max_pwm 60

IBusBM ibus;

// Configure the motor driver.
CytronMD motor2(PWM_DIR, 10, 9); // PWM 1 = Pin 3, DIR 1 = Pigggg
CytronMD motor1(PWM_DIR, 11, 8); // PWM 2 = Pin 9, DIR 2 = Pin 10.

CytronMD motor3(PWM_DIR, 7, 3); // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor4(PWM_DIR, 5, 2); // PWM 2 = Pin 9, DIR 2 = Pin 10.

// Red the channel and return a boolean value
void convert(int y, int x, int z_rotation)
{

  float WHEEL_GEOMETRY = 0.550 + 0.229; // Hight and width meters
  float WHEEL_RADIUS = 0.0785;          // radius in meters

  float front_left = (x - y - z_rotation * WHEEL_GEOMETRY) / WHEEL_RADIUS;
  float front_right = (x + y + z_rotation * WHEEL_GEOMETRY) / WHEEL_RADIUS;
  float back_left = (x + y - z_rotation * WHEEL_GEOMETRY) / WHEEL_RADIUS;
  float back_right = (x - y + z_rotation * WHEEL_GEOMETRY) / WHEEL_RADIUS;

  int front_left_ = map(front_left, -1000, 1000, -max_pwm, max_pwm);
  int front_right_ = map(front_right, -1000, 1000, -max_pwm, max_pwm);
  int back_left_ = map(back_left, -1000, 1000, -max_pwm, max_pwm);
  int back_right_ = map(back_right, -1000, 1000, -max_pwm, max_pwm);

  char output[100];
  sprintf(output, "Wheel 1 -> %-4d Wheel 2 -> %-4d Wheel 3 -> %-4d Wheel 4 -> %-4d", front_left_, front_right_, back_left_, back_right_);
  Serial.println(output);

  // Serial.println(String(front_left)+" "+String(front_right)+" "+String(back_left)+" "+String(back_right));

  motor1.setSpeed(front_left_);  // Motor 1 stops.
  motor2.setSpeed(front_right_); // Motor 2 stops.
  motor3.setSpeed(back_left_);   // Motor 1 stops.
  motor4.setSpeed(back_right_);  // Motor 2 stops.
}
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue)
{
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 100)
    return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// The setup routine runs once when you press reset.
void setup()
{
  HardwareSerial &ibusRcSerial = Serial1;
  HardwareSerial &aSerial = Serial;
  aSerial.begin(9600);
  ibus.begin(ibusRcSerial);
}

// The loop routine runs over and over again forever.

void loop()
{

  int Y_axis = readChannel(2, -100, 100, 0);
  int X_axis = readChannel(0, -100, 100, 0);
  int z_axis = readChannel(3, -100, 100, 0);
  Y_axis -= 24;
  X_axis -= 22;
  z_axis += 3;
  if (Y_axis < 20 && Y_axis > -20)
  {
    Y_axis = 0;
  }

  char output_1[100];
  sprintf(output_1, "ch 1 -> %-4d ch 2 -> %-4d ch 3 -> %-4d ", Y_axis, X_axis, z_axis);
  // Serial.println(output_1);

  // Serial.println(String(Y_axis)+" "+String(X_axis)+" "+String(z_axis));
  convert(X_axis, Y_axis, z_axis);
  delay(10);
}