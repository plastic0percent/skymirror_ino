#ifndef definition_ino_h
#define definition_ino_h

#include "Adafruit_MPU6050.h"
#include "Beeper.h"
#include "ESC.h"
#include "Pressure.h"
#include "Servo.h"
#include "TinyGPS++.h"

// Constants
// GPS on Serial2
#define SerialG Serial2
// Bluetooth on Serial1
#define SerialB Serial1
// Computer on Serial0
#define SerialC Serial

// Beeper at pin 3
const int fish_repeller_pin = 3;
// Main motor ESC at pin 7
const int esc_pwm_pin = 7;
const int speed_max = 2000;
const int speed_min = 1000;
// Turning servo at pin 6;
const int servo_pwm_pin = 6;
// Pressure sensor at pin A0
const int pressure_sens_pin = A0;

// Global objects
// The fish repeller is a beeper
Beeper fish_repeller(fish_repeller_pin);
// Pressure sensor
DFRobot_Pressure pressure_sens(pressure_sens_pin);
// MPU6050 motion sensor on I2C
Adafruit_MPU6050 mpu;
// GPS Decoder
TinyGPSPlus gps;
// Turing Servo
Servo servo;
// range is 23-54, in HEX: 0x17-0x36
int servo_pos = 38;
// Main motor ESC
ESC esc(7, speed_min, speed_max, 500);
// speed, 1501-2000 are forward, 1000-1499 are backward, 1500 is stop
int esc_speed = 1500;
#endif
