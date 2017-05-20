/**
 * Represents the matrix circuit.
 */

#ifndef KEYBOARD_MATRIX_H
#define KEYBOARD_MATRIX_H

#include <iostream>
#include <array>
#include <stdexcept>
#include "Arduino.h"
#include "hardware_traits.h"


/**
 * How long to ignore changes after switch changes state.
 */
const millis_t debounce_millis = 50;


/**
 * Identifies a physical switch on the keyboard according to row & column intersection.
 */
struct Switch {
    char col;
    char row;
    millis_t millis;  // When key was pressed.

    Switch(int col_0, int row_0, millis_t millis_0 = 0): col(col_0), row(row_0), millis(millis_0) {}
    Switch(): Switch(0, 0, -1) {}

    bool operator==(Switch other) const {
        return col == other.col && row == other.row;
    }
    bool operator!=(Switch other) const {
        return col != other.col || row != other.row;
    }
};

class Switches {
    const Switch *ptr;
    size_t extent;

public:
    Switches(const Switch *ptr_0, size_t extent_0): ptr(ptr_0), extent(extent_0) {}

    const Switch *begin() const {
        return ptr;
    }
    const Switch *end() const {
        return ptr + extent;
    }
    size_t size() const {
        return extent;
    }
    size_t max_size() const {
        return extent;
    }
    bool empty() const {
        return extent == 0;
    }
    const Switch &operator[](int index) const {
        return ptr[index];
    }
    const Switch &at(int index) const {
        if (index < 0 || static_cast<size_t>(index) >= extent) {
            throw std::out_of_range("index");
        }
        return ptr[index];
    }
};


/**
 * Thing that probes the matrix and records which switches are pressed.
 */
template<int column_count, int row_count, typename Pin_t = int, class Traits_t = PinTraits>
class KeyboardMatrix {
    std::array<const Pin_t, column_count> column_pins;
    std::array<const Pin_t, row_count> row_pins;

    // We record the pressed switches and switches that have been recently released.
    // They share a fixed-size array to avoid dynamic allocation.
    //    switches[0:pressed_count] are pressed switches.
    //    switches[pressed_count:(pressed_count + released_count)] are switches released within debounce_millis.
    std::array<Switch, column_count * row_count> switches;
    int pressed_count;
    int released_count;

public:
    KeyboardMatrix(std::array<const Pin_t, column_count> &&column_pins_0, std::array<const Pin_t, row_count> &&row_pins_0):
        column_pins(column_pins_0), row_pins(row_pins_0), pressed_count(0), released_count(0)
    {
        set_up();
    }

    KeyboardMatrix(const std::array<const Pin_t, column_count> &column_pins_0, const std::array<const Pin_t, row_count> &row_pins_0):
        column_pins(column_pins_0), row_pins(row_pins_0)
    {
        set_up();
    }

    KeyboardMatrix(KeyboardMatrix &&other): column_pins(other.column_pins), row_pins(other.row_pins) {}

    /**
     * Called repeatedly in Arduino's loop.
     */
    void loop(millis_t millis) {
        // Discard expired key-released records.
        int prev = pressed_count;
        for (int k = pressed_count; k < pressed_count + released_count; ++k) {
            if (millis - switches[k].millis >= debounce_millis) {
                // Discard this record.
            } else {
                if (prev != k) {
                    // We are shifting keys down to remove the gaps left by replaced ones.
                    std::swap(switches[prev], switches[k]);
                }
                ++prev;
            }
        }
        released_count = prev - pressed_count;

        // Scan the matrix.
        for (int i = 0; i < column_count; ++i) {
            Traits_t::pinMode(column_pins[i], OUTPUT);
            Traits_t::digitalWrite(column_pins[i], LOW);

            for (int j = 0; j < row_count; ++j) {
                if (Traits_t::digitalRead(row_pins[j]) == LOW) {
                    record_pressed(i, j, millis);
                } else {
                    record_unpressed(i, j, millis);
                }
            }

            Traits_t::pinMode(column_pins[i], INPUT);  // Restore pin to floating state.
        }
    }

    /**
     * Return a collection of Switch objects.
     */
    Switches pressed_switches() const {
        return Switches(&switches[0], pressed_count);
    }

private:
    void set_up() {
        // Start with all column pins FLOATING.
        for (int i = 0; i < column_count; ++i) {
            Traits_t::pinMode(column_pins[i], INPUT);
        }
        // Rows are all inputs with pullup resistor.
        for (int j = 0; j < row_count; ++j) {
            Traits_t::pinMode(row_pins[j], INPUT_PULLUP);
        }
    }

    void record_pressed(int col, int row, millis_t millis) {
        // See if already recoreded as pressed, or released within debounce time.
        for (int k = 0; k < pressed_count + released_count; ++k) {
            if (switches[k].col == col && switches[k].row == row) {
                return;  // Already recorded: nothing to do.
            }
        }
        if (released_count > 0) {
            // Make room!
            std::swap(switches[pressed_count], switches[pressed_count + released_count]);
        }
        switches[pressed_count++] = Switch(col, row, millis);
    }

    void record_unpressed(int col, int row, millis_t millis) {
        for (int k = 0; k < pressed_count; ++k) {
            if (switches[k].col == col && switches[k].row == row) {
                if (millis - switches[k].millis >= debounce_millis) {
                    // Swap this switch to the start of the released list.
                    if (k != pressed_count - 1) {
                        std::swap(switches[k], switches[pressed_count - 1]);
                    }
                    switches[--pressed_count].millis = millis;
                    ++released_count;
                }
                return;
            }
        }
        // Not found, so nothing to do.
    }
};

#endif // KEYBOARD_MATRIX_H
