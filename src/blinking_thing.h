#ifndef BLINKING_THING_H
#define BLINKING_THING_H

#include "Arduino.h"
#include "hardware_traits.h"


template<typename P = int, class T = PinTraits>
class BlinkingThing {
    P pin;
    unsigned long dark_millis;
    unsigned long lit_millis;

    bool is_lit;
    unsigned long last_modified;

public:
    BlinkingThing(P pin0, unsigned long dark_millis0, unsigned long lit_millis0):
        pin(pin0), dark_millis(dark_millis0), lit_millis(lit_millis0),
        is_lit(false), last_modified(0)
    {
        T::pinMode(pin, OUTPUT);
        T::digitalWrite(pin, LOW);

    }

    void loop(unsigned long millis) {
        unsigned long within_cycle = millis % (dark_millis + lit_millis);
        bool next_is_lit = within_cycle >= dark_millis;
        if (next_is_lit != is_lit) {
            is_lit = next_is_lit;
            T::digitalWrite(pin, is_lit ? HIGH : LOW);
        }
    }
};


#endif // BLINKING_THING_H
