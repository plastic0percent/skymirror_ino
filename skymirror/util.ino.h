#ifndef util_ino_h
#define util_ino_h

#include "definitions.ino.h"

template <class MsgType>
// Log a message to both Serial and Bluetooth
void logger(MsgType message)
{
    SerialC.println(message);
    SerialB.println(message);
}

// Update a counter and return the number of milliseconds passed
// after last update
unsigned long update_counter(unsigned long *counter)
{
    // Modulus add is closed so we do not need to worry about overflow
    // unless two overflows occurred
    unsigned long old_cnt = *counter;
    unsigned long new_cnt = millis();
    *counter = new_cnt;
    return new_cnt - old_cnt;
}

// Update a counter and return the number of microseconds passed
// after last update
unsigned long update_counter_micros(unsigned long *counter)
{
    // Modulus add is closed so we do not need to worry about overflow
    // unless two overflows occurred
    unsigned long old_cnt = *counter;
    unsigned long new_cnt = micros();
    *counter = new_cnt;
    return new_cnt - old_cnt;
}
#endif
