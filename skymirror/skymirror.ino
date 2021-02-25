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
// Main motor ESC at pin 43
const int esc_pwm_pin = 43;
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

void log(const char *message)
{
    SerialC.println(message);
    SerialB.println(message);
}

// Initialize bluetooth
void init_bluetooth()
{
    log("Initializing Bluetooth");
    SerialB.begin(9600);
    // Wait for bluetooth to be up
    while (!SerialB)
        delay(1);
    log("Bluetooth initialized");
}

void init_mpu()
{
    // Try to initialize MPU6050
    if (!mpu.begin())
    {
        log("Failed to initialize MPU6050, check wiring");
        while (1)
            delay(10);
    }
    log("MPU6050 initialized");
}

void setup()
{
    SerialC.begin(115200);
    init_bluetooth();
    init_mpu();
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
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x10:
        case 0x20:
        case 0x30:
        case 0x40:
            log("Stub");
            break;
        default:
            log("Unrecognized command received from bluetooth");
    }
}

void loop()
{
    log("Loop");
    fish_repeller.beep(1000, 1.0);
    if (SerialB.available())
    {
        exec_bluetooth_cmd();
    }
    delay(9);
}
