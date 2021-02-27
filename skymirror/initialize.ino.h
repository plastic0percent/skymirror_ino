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

#endif