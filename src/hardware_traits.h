/*
* Simple dumb way to allow me to replace the global Arduino functions in tests.
*
* Objects that may call gloabl functions have a traits object
* which has no variable memebrs (therefore takes no space)
* and has static functions that call the global ones.
* In tests a different traits class is used that instead
* operates on objects owned by the test fixture.
*/

#ifndef HARDWARE_TRAITS_H
#define HARDWARE_TRAITS_H

#include "Arduino.h"


typedef unsigned long millis_t;

/*
* Defines how to access the I/O pins.
* This version just calls the gloabl functions supplied by Arduino.
* In tests these are replaced with fakes.
*/
struct PinTraits {
    // pin identifies a pin.
    // mode is one of INPUT, INUT_PULLUP, OUTPUT.
    static void pinMode(int pin, int mode) {
        ::pinMode(pin, mode);
    }

    // pin identifies a pin that must have mode OUTOUT.
    // Value is one of HIGH, LOW.
    static void digitalWrite(int pin, int value) {
        ::digitalWrite(pin, value);
    }

    // pin identifies a pin that must have mode INPUT or INPUT_PULLUP.
    // Returns one of HIGH, LOW.
    static int digitalRead(int pin) {
        return ::digitalRead(pin);
    }
};


#endif // HARDWARE_TRAITS_H
