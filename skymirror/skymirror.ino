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
// Datasheet:
// https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
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
#include "Servo.h"
#include "TinyGPS++.h"

#include "definitions.ino.h"
#include "initialize.ino.h"
#include "positioning.ino.h"
#include "util.ino.h"

void setup()
{
    SerialC.begin(115200);
    init_bluetooth();
    init_mpu();
    init_gps();
    init_esc();
    servo.attach(servo_pwm_pin);
    servo.write(servo_pos);
    // TODO: Camera
}

// Bluetooth commands:
// All commands take 2 bytes even if they do not require an argument.
// - 0x00   : calibrate sensors
// - 0x01 $1: receive $1 frames from the camera (JSON between STX and ETX)
/* {
    "image": "$base64-image"
} */
// - 0x02   : query status (JSON between STX and ETX)
/* {
    "time": "",                   // UTC time, yyyy-mm-ddThh:mm:ss
                                  // (ISO8601 w/o timezone)
    "accel": [0.0, 0.0, 0.0],     // x,y,z m/s2
    "speed": [0.0, 0.0, 0.0],     // x,y,z m/s
    "displ": [0.0, 0.0, 0.0],     // x,y,z m
    "pressure": 10.0,             // kPa
    "depth": 10.0,                // m
    "lat": 0.0,                   // degrees
    "lon": 0.0                    // degrees
}*/
// - 0x10 $1: set speed to $1*0.02 m/s
// - 0x20 $1: set direction to $i degrees N (cw)
// - 0x21 $1: set direction to $i degrees N (ccw)
// - 0x30 $1: set depth to $1 m
// - 0x40 $1: set fish repeller frequency to $1 * 30 Hz
// - 0x50 $1: (raw) set esc speed to $1 * 10
// - 0x60 $1: (raw) set turning angle to $1
// - 0xff   : (raw) re-run init
void exec_bluetooth_cmd()
{
    int cmd = SerialB.read();
    int arg = SerialB.read();
    switch (cmd)
    {
        case 0x02: {
            // Print available information
            TinyGPSDate d = gps.date;
            TinyGPSTime t = gps.time;
            SerialB.print(F("\002{\"time\":\""));
            if (d.isValid() && d.month() != 0)
            {
                char dateBuf[11];
                sprintf(dateBuf, "%04d-%02d-%02d", d.year(), d.month(),
                        d.day());
                SerialB.print(dateBuf);
                SerialC.print(dateBuf);
            }
            else
                SerialB.print(F("2005-01-06"));
            SerialB.print(F("T"));
            if (t.isValid())
            {
                char timeBuf[9];
                sprintf(timeBuf, "%02d:%02d:%02d", t.hour(), t.minute(),
                        t.second());
                SerialC.print(timeBuf);
                SerialB.print(timeBuf);
            }
            else
                SerialB.print(F("00:00:00"));
            SerialB.print(F("\",\"accel\":[0.0,0.0,0.0],\"speed\":[0.0,0.0,0.0]"
                            ",\"displ\":[0.0,0.0,0.0],\"pressure\":"));
            SerialB.print(pressure_sens.read_kpa());
            SerialB.print(F(",\"depth\":"));
            SerialB.print(pressure_sens.get_depth_mm());
            if (gps.location.isValid())
            {
                SerialB.print(F(",\"lat\":"));
                SerialB.print(gps.location.lat());
                SerialB.print(F(",\"lon\":"));
                SerialB.print(gps.location.lng());
            }
            else
                SerialB.print(F(",\"lat\":0.0,\"lon\":0.0"));
            SerialB.println(F("}\003"));
            break;
        }
        case 0x00:
        case 0x01:
        case 0x10:
        case 0x20:
        case 0x21:
        case 0x30:
            log(F("Stub"));
            break;
        case 0x40:
            log(F("Setting fish_repeller to: "));
            log(arg * 30);
            fish_repeller.beep_forever(arg * 30);
            break;
        case 0x50:
            esc_speed = arg;
            log(F("Setting ESC speed to: "));
            log(esc_speed * 10);
            esc.speed(esc_speed * 10);
            break;
        case 0x60:
            servo_pos = arg;
            log(F("Setting servo position to: "));
            log(servo_pos);
            servo.write(servo_pos);
            break;
        case 0xff:
            setup();
        default:
            log(F("Unrecognized command received from bluetooth"));
    }
}

void loop()
{
    if (SerialB.available())
    {
        exec_bluetooth_cmd();
    }
    feed_gps();
    delay(10);
}
