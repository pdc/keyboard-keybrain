#ifndef KEYBOARD_CORTEX_H
#define KEYBOARD_CORTEX_H

#include <iostream>
#include <utility>
// #include "Keyboard.h"
#include "hardware_traits.h"
#include "keyboard_matrix.h"

const int usb_rollover_max = 6;

struct keyboard_record {
    modifier_flags_t modifier_flags;
    std::array<scancode_t, usb_rollover_max> keys;

    keyboard_record():
        modifier_flags(0), keys({})
    {}
    keyboard_record(modifier_flags_t modifier_flags, std::array<scancode_t, usb_rollover_max> initial_keys):
        modifier_flags(modifier_flags), keys(initial_keys)
    {}
};


template<int layer_count, int column_count, int row_count>
class KeyboardCortex {
    std::array<std::array<std::array<uint16_t, column_count>, row_count>, layer_count> layers;

public:
    KeyboardCortex(std::array<std::array<std::array<uint16_t, column_count>, row_count>, layer_count> initial_layers):
        layers(initial_layers)
    {}

public:
    keyboard_record record_from_switches(const Switches &switches) const {
        keyboard_record result;
        int layer = 0;
        int k = 0;
        for (Switch switch_ : switches) {
            uint16_t code = layers[layer][switch_.row][switch_.col];
            if ((code & 0xFF00) == 0xE000) {
                // Modifer key.
                result.modifier_flags |= code;
            } else {
                // Regular key.
                if (k < usb_rollover_max) {
                    result.keys.at(k++) = code & 0xFF;
                }
            }
        }
        return result;
    }
};


#endif // KEYBOARD_CORTEX_H
