/*
* Fake implementations of fake Arduino.
*/

#include "gtest/gtest.h"
#include "Arduino.h"


/*
* The global stubs are all designed to explode if used.
* The reson for this is they cannot be trusted given that
* the test runner is multithreaded.
* In tests we arrange for members of fixture objects
* to be called instead.
*/

void pinMode(int, int) {
    ADD_FAILURE() << "Cannot call real pinMode function in unit tests";
}

int digitalRead(int) {
    ADD_FAILURE() << "Cannot call real digitalRead function in unit tests";
    return -1;
}

void digitalWrite(int, int) {
    ADD_FAILURE() << "Cannot call real digitalWrite function in unit tests";
}


