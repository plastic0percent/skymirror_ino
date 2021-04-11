#ifndef initialize_ino_h
#define initialize_ino_h

#include "Pos_Algor.hpp"
#include "TimerThree.h"

#include "definitions.ino.h"
#include "positioning.ino.h"
#include "util.ino.h"

// Initialize bluetooth
void init_bluetooth()
{
    logger(F("Initializing Bluetooth"));
    SerialB.begin(9600);
    // Wait for bluetooth to be up
    while (!SerialB)
        delay(1);
    logger(F("Bluetooth initialized"));
}

// Initialize GPS
void init_gps()
{
    logger(F("Initializing GPS"));
    SerialG.begin(9600);
    logger(F("Decoder library is"));
    logger(TinyGPSPlus::libraryVersion());
    logger(F("GPS initialized"));
}

// Initialize MPU6050
void init_mpu()
{
    logger(F("Initializing MPU6050"));
    // Try to initialize MPU6050
    if (!mpu.begin())
    {
        logger(F("Failed to initialize MPU6050, check wiring"));
        while (1)
            delay(10);
    }
    motion_state.start_rest_test();
    read_mpu_rest();
    motion_state.end_rest_test();
    logger(F("MPU6050 initialized"));
}

// Initialize ESC and motor
void init_esc()
{
    logger(F("Initializing ESC and motor"));
    esc_speed = 1500;
    esc.arm();
    // This cycle is used to calibrate some models
    for (int speed = 1000; speed <= 2000; ++speed)
    {
        esc.speed(speed);
        delay(10);
    }
    for (int speed = 2000; speed >= 1000; --speed)
    {
        esc.speed(speed);
        delay(10);
    }
    esc.speed(1500);
    // Stoping at 1500 and wait for a while to start
    delay(500);
    logger(F("ESC and motor initialized"));
}

// Initialize turning servo
void init_servo()
{
    logger(F("Initializing turning servo"));
    servo.attach(servo_pwm_pin);
    servo_pos = 15;
    servo.write(servo_pos);
    logger(F("Turning servo initialized"));
}

// Initialize fish repeller beeper
void init_beeper()
{
    logger(F("Initializing fish repeller beeper"));
    pinMode(beeper_pin, OUTPUT);
    Timer3.initialize(40);
    logger(F("Fish repeller beeper initialized"));
}

#endif
