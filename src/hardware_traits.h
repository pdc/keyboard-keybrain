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
#include "Keyboard.h"


typedef unsigned long millis_t;
typedef uint16_t modifier_flags_t;
typedef uint8_t scancode_t;

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


struct KeyboardTraits {
    /**
     * Set modifier keys. flags is combination of MODIFIERKEY_{CTRL,SHIFT,ALT,GUI}.
     */
    void set_modifier(modifier_flags_t flags) {
        ::Keyboard.set_modifier(flags);
    }

    // The API is rather annoying: 6 functions to set 6 items in a global variable.

    void set_key1(scancode_t scancode) {
        ::Keyboard.set_key1(scancode);
    }
    void set_key2(scancode_t scancode) {
        ::Keyboard.set_key2(scancode);
    }
    void set_key3(scancode_t scancode) {
        ::Keyboard.set_key3(scancode);
    }
    void set_key4(scancode_t scancode) {
        ::Keyboard.set_key4(scancode);
    }
    void set_key5(scancode_t scancode) {
        ::Keyboard.set_key5(scancode);
    }
    void set_key6(scancode_t scancode) {
        ::Keyboard.set_key6(scancode);
    }

    void send_now() {
        ::Keyboard.send_now();
    }
};


#endif // HARDWARE_TRAITS_H
