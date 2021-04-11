
#ifndef positioning_ino_h
#define positioning_ino_h

#include "Pos_Algor.hpp"

#include "definitions.ino.h"

#define FULL_READ()                                                            \
    motion_state.add_data(mpu_a.acceleration.x, mpu_a.acceleration.y,          \
                          mpu_a.acceleration.z, mpu_gy.gyro.x, mpu_gy.gyro.y,  \
                          mpu_gy.gyro.z, millis())
double lat;
double lon;

// Feed the GPS reader
void feed_gps()
{
    while (SerialG.available())
        gps.encode(SerialG.read());
}

// Feed the motion state
void feed_motion()
{
    mpu.getEvent(&mpu_a, &mpu_gy, &mpu_temp);
    FULL_READ();
}

// Read the MPU three times to calibrate
void read_mpu_rest()
{
    feed_motion();
    delay(10);
    feed_motion();
    delay(10);
    feed_motion();
}
#endif
