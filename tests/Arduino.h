/*
 * Fake Arduino environment.
 *
 * Not intended to be complete: just enough for my tests.
 */

#ifndef FAKE_ARDUINO_H
#define FAKE_ARDUINO_H

#include <vector>

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3
#define OUTPUT_OPENDRAIN 4
#define INPUT_DISABLE 5



/*
* The global stubs are all designed to explode if used.
* The reson for this is they cannot be trusted given that
* the test runner is multithreaded.
* In tests we arrange for members of fixture objects
* to be called instead.
*/

void pinMode(int pin, int mode);
int digitalRead(int pin);
void digitalWrite(int pin, int value);


class FakePinListener {
public:
    virtual void on_mode_changed(int) {}
    virtual void on_value_changed(int) {}
};

/*
* Fake pin that can be interrogated afterwards to check
* the system under test fiddles with the pins correctly.
* Use pointer to this class as 1st template argument.
*/
class FakePin {
    int mode;
    int value;
    std::vector<FakePinListener *> listeners;

public:
    FakePin(): mode(-1), value(-1) {}

    void add_listener(FakePinListener *listener) {
        listeners.push_back(listener);
    }

    virtual int get_mode() const { return mode; }

    virtual void set_mode(int next_mode) {
        mode = next_mode;
        for (auto listener : listeners) {
            listener->on_mode_changed(next_mode);
        }
    }

    virtual int get_value() const { return value; }

    virtual void set_value(int next_value) {
        value = next_value;
        for (auto listener : listeners) {
            listener->on_value_changed(next_value);
        }
    }

    int is_low() { return value == LOW; }
    int is_high() { return value == HIGH; }
    int is_floating() { return mode == INPUT; }
};

/*
* Use this as 2nd tempalte argument.
*/
struct FakePinTraits {
    static void pinMode(FakePin *pin_ptr, int mode) {
        pin_ptr->set_mode(mode);
    }

    // pin identifies a pin that must have mode OUTOUT.
    // Value is one of HIGH, LOW.
    static void digitalWrite(FakePin *pin_ptr, int value) {
        pin_ptr->set_value(value);
    }

    // pin identifies a pin that must have mode INPUT ir INPUT_PULLUP.
    // Returns one of HIGH, LOW.
    static int digitalRead(const FakePin *pin_ptr) {
        return pin_ptr->get_value();
    }
};



#endif // FAKE_ARDUINO_H
