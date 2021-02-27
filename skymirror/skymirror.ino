// Place for refs:
// Motor/ESC:
// https://www.arduino.cc/reference/en/libraries/rc_esc/
// https://www.robotshop.com/community/blog/show/rc-speed-controller-esc-arduino-library
//
// OV7670:
// https://www.elecrow.com/download/OV7670_DS_(1_4).pdf
// https://www.elecrow.com/download/AL422b.pdf
// http://e-structshop.com/structwp/wp-content/uploads/2014/12/SCCBSpec_AN.pdf
// http://www.therandomlab.com/2016/06/arduvision-ii-ov7670-fifo-module-and.html
//
// Kalman filter:
// Tutorial: http://www.bzarg.com/p/how-a-kalman-filter-works-in-pictures/
// Explanation:
// https://www.cs.cmu.edu/~motionplanning/papers/sbp_papers/kalman/kleeman_understanding_kalman.pdf
// Implementation:
// https://www.arduino.cc/reference/en/libraries/kalman-filter-library/

#include "Adafruit_MPU6050.h"
#include "Beeper.h"
#include "Pressure.h"
#include "TinyGPS++.h"

#include "definitions.ino.h"
#include "initialize.ino.h"
#include "util.ino.h"

void setup()
{
    SerialC.begin(115200);
    init_bluetooth();
    init_mpu();
    init_gps();
    // TODO: Camera, Motor
}

// Bluetooth commands:
// - 0x01 $1: receive $1 frames from the camera
// - 0x02   : receive current speed
// - 0x03   : receive current depth
// - 0x04   : receive current gps location
// - 0x10 $1: set speed to $1*0.02 m/s
// - 0x20 $1 $2: set direction to $i degrees N, cw if $2 is 0, ccw is $2 is 1
// - 0x30 $1: set depth to $1 m
// - 0x40 $1: set fish repeller frequency to $1 Hz
void exec_bluetooth_cmd()
{
    int cmd = SerialB.read();
    switch (cmd)
    {
        case 0x03:
            SerialB.print(F("Depth: "));
            SerialB.print(pressure_sens.get_depth_mm());
            SerialB.println(F("mm"));
            SerialC.print(F("Depth: "));
            SerialC.print(pressure_sens.get_depth_mm());
            SerialC.println(F("mm"));
            break;
        case 0x01:
        case 0x02:
        case 0x04:
        case 0x10:
        case 0x20:
        case 0x30:
        case 0x40:
            log(F("Stub"));
            break;
        default:
            log(F("Unrecognized command received from bluetooth"));
    }
}

void loop()
{
    fish_repeller.beep(100, 0.1);
    if (SerialB.available())
    {
        exec_bluetooth_cmd();
    }
    delay(1000);
}
