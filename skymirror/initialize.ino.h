#ifndef initialize_ino_h
#define initialize_ino_h

#include "definitions.ino.h"
#include "util.ino.h"

// Initialize bluetooth
void init_bluetooth()
{
    log(F("Initializing Bluetooth"));
    SerialB.begin(9600);
    // Wait for bluetooth to be up
    while (!SerialB)
        delay(1);
    log(F("Bluetooth initialized"));
}

// Initialize GPS
void init_gps()
{
    log(F("Initializing GPS"));
    SerialG.begin(9600);
    log(F("Decoder library is"));
    log(TinyGPSPlus::libraryVersion());
    log(F("GPS initialized"));
}

// Initialize MPU6050
void init_mpu()
{
    log(F("Initializing MPU6050"));
    // Try to initialize MPU6050
    if (!mpu.begin())
    {
        log(F("Failed to initialize MPU6050, check wiring"));
        while (1)
            delay(10);
    }
    log(F("MPU6050 initialized"));
}

// Initialize ESC and motor
void init_esc()
{
    log(F("Initializing ESC and motor"));
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
    log(F("ESC and motor initialized"));
}

// Initialize turning servo
void init_servo()
{
    log(F("Initializing turning servo"));
    servo.attach(servo_pwm_pin);
    servo_pos = 15;
    servo.write(servo_pos);
    log(F("Turning servo initialized"));
}

#endif
