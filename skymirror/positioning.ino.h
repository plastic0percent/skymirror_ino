#ifndef positioning_ino_h
#define positioning_ino_h

#include "definitions.ino.h"

// Feed the GPS reader
void feed_gps()
{
    while (SerialG.available())
        gps.encode(SerialG.read());
}

double lat;
double lon;
// Time counter for acceleration integration
unsigned long cnt_a = 0;

#endif
