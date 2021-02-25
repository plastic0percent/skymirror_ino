#include "Beeper.h"
#include "Pressure.h"
#include "TinyGPS++.h"
#include "Adafruit_MPU6050.h"

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

void log(const char *message) { SerialC.println(message); }

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

void loop()
{
    log("Loop");
    delay(9);
}
