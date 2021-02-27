#ifndef definition_ino_h
#define definition_ino_h

#include "Adafruit_MPU6050.h"
#include "Beeper.h"
#include "Pressure.h"
#include "TinyGPS++.h"

// Constants
// GPS on Serial2
#define SerialG Serial2
// Bluetooth on Serial1
#define SerialB Serial1
// Computer on Serial0
#define SerialC Serial

// Beeper at pin 42
const int fish_repeller_pin = 42;
// Main motor ESC at pin 10
const int esc_pwm_pin = 10;
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
#endif
