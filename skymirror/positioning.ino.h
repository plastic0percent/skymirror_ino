
#ifndef positioning_ino_h
#define positioning_ino_h

#include "definitions.ino.h"

double lat;
double lon;

// Feed the GPS reader
void feed_gps()
{
    while (SerialG.available())
        gps.encode(SerialG.read());
}

#endif