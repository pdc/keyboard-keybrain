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


void Keyboard_t::set_modifier(uint16_t) {
    ADD_FAILURE() << "Cannot call real Keyboard_t::set_modifier in unit tests";
}

void Keyboard_t::set_key1(uint8_t) {
    ADD_FAILURE() << "Cannot call real Keyboard_t::set_key1 in unit tests";
}

void Keyboard_t::set_key2(uint8_t) {
    ADD_FAILURE() << "Cannot call real Keyboard_t::set_key2 in unit tests";
}

void Keyboard_t::set_key3(uint8_t) {
    ADD_FAILURE() << "Cannot call real Keyboard_t::set_key3 in unit tests";
}

void Keyboard_t::set_key4(uint8_t) {
    ADD_FAILURE() << "Cannot call real Keyboard_t::set_key4 in unit tests";
}

void Keyboard_t::set_key5(uint8_t) {
    ADD_FAILURE() << "Cannot call real Keyboard_t::set_key5 in unit tests";
}

void Keyboard_t::set_key6(uint8_t) {
    ADD_FAILURE() << "Cannot call real Keyboard_t::set_key6 in unit tests";
}

void Keyboard_t::send_now() {
    ADD_FAILURE() << "Cannot call real Keyboard_t::send_now in unit tests";
}

Keyboard_t Keyboard;
