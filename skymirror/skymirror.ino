// Place for refs:
// Motor/ESC:
// Drivers:
// https://www.arduino.cc/reference/en/libraries/rc_esc/
// https://www.robotshop.com/community/blog/show/rc-speed-controller-esc-arduino-library
//
// OV7670:
// Datasheets:
// https://www.elecrow.com/download/OV7670_DS_(1_4).pdf
// https://www.elecrow.com/download/AL422b.pdf
// http://e-structshop.com/structwp/wp-content/uploads/2014/12/SCCBSpec_AN.pdf
// http://www.therandomlab.com/2016/06/arduvision-ii-ov7670-fifo-module-and.html
//
// Kalman filter:
// Tutorial:
// http://www.bzarg.com/p/how-a-kalman-filter-works-in-pictures/
// Explanation:
// https://www.cs.cmu.edu/~motionplanning/papers/sbp_papers/kalman/kleeman_understanding_kalman.pdf
// Implementation:
// https://www.arduino.cc/reference/en/libraries/kalman-filter-library/
// https://github.com/rfetick/Kalman
// Caveats:
// https://electronics.stackexchange.com/questions/86102/kalman-filter-on-linear-acceleration-for-distance
// http://www.chrobotics.com/library/accel-position-velocity
//
// Other things about MPU6050:
// Datasheet: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
// Runge-Kutta Methods: https://en.wikipedia.org/wiki/Runge–Kutta_methods
//
// GPS:
// NMEA Sentences:
// https://www.nps.gov/gis/gps/NMEA_sentences.html
// http://aprs.gids.nl/nmea/
// https://www.sparkfun.com/datasheets/GPS/NMEA%20Reference%20Manual-Rev2.1-Dec07.pdf

#include "Adafruit_MPU6050.h"
#include "Beeper.h"
#include "Pressure.h"
#include "TinyGPS++.h"
#include "Servo.h"

#include "definitions.ino.h"
#include "initialize.ino.h"
#include "util.ino.h"

void setup()
{
    SerialC.begin(115200);
    init_bluetooth();
    init_mpu();
    init_gps();
    esc.arm();
    servo.attach(6);
    // TODO: Camera
}

// Bluetooth commands:
// All commands take 2 bytes even if they do not require an argument.
// - 0x01 $1: receive $1 frames from the camera
// - 0x02   : receive current speed
// - 0x03   : receive current depth
// - 0x04   : receive current gps location
// - 0x00   : calibrate sensors
// - 0x10 $1: set speed to $1*0.02 m/s
// - 0x20 $1: set direction to $i degrees N (cw)
// - 0x21 $1: set direction to $i degrees N (ccw)
// - 0x30 $1: set depth to $1 m
// - 0x40 $1: set fish repeller frequency to $1*10 Hz
// - 0x50 $1: (raw) set esc speed to $1
// - 0x60 $1: (raw) set turning angle to $1
void exec_bluetooth_cmd()
{
    int cmd = SerialB.read();
    int arg = SerialB.read();
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
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x04:
        case 0x10:
        case 0x20:
        case 0x21:
        case 0x30:
            log(F("Stub"));
            break;
        case 0x40:
            log(F("Setting fish_repeller to: "));
            log(arg * 10);
            fish_repeller.beep_forever(arg * 10);
            break;
        case 0x50:
            esc_speed = arg;
            log(F("Setting ESC speed to: "));
            log(esc_speed);
            esc.speed(esc_speed);
            break;
        case 0x60:
            servo_pos = arg;
            log(F("Setting servo position to: "));
            log(servo_pos);
            servo.write(servo_pos);
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
