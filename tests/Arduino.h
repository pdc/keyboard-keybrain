/*
 * Fake Arduino environment.
 *
 * Not intended to be complete: just enough for my tests.
 */

#ifndef FAKE_ARDUINO_H
#define FAKE_ARDUINO_H


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


/*
* Fake pin that can be interrogated afterwards to check
* the system under test fiddles with the pins correctly.
* Use pointer to this class as 1st template argument.
*/
struct FakePin {
    int mode;
    int value;

public:
    FakePin(): mode(-1), value(-1) {}
};

/*
* Use this as 2nd tempalte argument.
*/
struct FakePinTraits {
    static void pinMode(FakePin *pin_ptr, int mode) {
        pin_ptr->mode = mode;
    }

    // pin identifies a pin that must have mode OUTOUT.
    // Value is one of HIGH, LOW.
    static void digitalWrite(FakePin *pin_ptr, int value) {
        pin_ptr->value = value;
    }

    // pin identifies a pin that must have mode INPUT ir INPUT_PULLUP.
    // Returns one of HIGH, LOW.
    static int digitalRead(const FakePin *pin_ptr) {
        return pin_ptr->value;
    }
};



#endif // FAKE_ARDUINO_H
