/* Tests for keyboard_cortex. */

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>
#include "Arduino.h"
#include "gtest/gtest.h"
#include "keyboard_cortex.h"

using namespace std;


KeyboardCortex<2, 4, 3> default_cortex = {{{
    {{
        {{ KEY_TAB, KEY_Q, KEY_W, KEY_E }},
        {{ MODIFIERKEY_CTRL, KEY_A, KEY_S, KEY_D }},
        {{ MODIFIERKEY_SHIFT, KEY_Z, KEY_X, KEY_C }},
    }},
}}};

class KeyboardContextTest: public ::testing::Test {

public:
    static const int layer_count = 2;
    static const int column_count = 4;
    static const int row_count = 3;
    KeyboardCortex<layer_count, column_count, row_count> cortex;
    keyboard_record result;

    KeyboardContextTest(): cortex(default_cortex) {}

    void given_spec(const KeyboardCortex<layer_count, column_count, row_count> &next_cortext) {
        cortex = next_cortext;
    }

    void when_applied_to_switches(const Switches &switches) {
        result = cortex.record_from_switches(switches);
    }
    void when_applied_to_switches(vector<Switch> switches) {
        // Convert from vector to the more primitive representation we use.
        Switch *data = new Switch[switches.size()];
        copy(switches.begin(), switches.end(), data);
        result = cortex.record_from_switches(Switches(data, switches.size()));
    }

    void then_result_should_be(modifier_flags_t expected_modifier_flags, array<unsigned, 6> expected_keys) {
        EXPECT_EQ(result.modifier_flags, expected_modifier_flags);
        for (int i = 0; i < 6; ++i) {
            EXPECT_EQ(result.keys[i], expected_keys[i] & 0xFF) << "Wrong key in position " << i;
        }
    }
};


TEST_F(KeyboardContextTest, ReturnsEmptyRecordForNoSwitches) {
    when_applied_to_switches(Switches(nullptr, 0));

    then_result_should_be(0, {{}});
}

TEST_F(KeyboardContextTest, AddsScancodeForSingleKey) {
    when_applied_to_switches({Switch(1, 1)});

    then_result_should_be(0, {{KEY_A}});
}

TEST_F(KeyboardContextTest, AddsScanCodesFor2Keys) {
    when_applied_to_switches({Switch(1, 1), Switch(1, 0)});

    then_result_should_be(0, {{KEY_A, KEY_Q}});
}

TEST_F(KeyboardContextTest, AddsScanCodesFor3Keys) {
    when_applied_to_switches({Switch(1, 1), Switch(1, 0), Switch(0, 0)});

    then_result_should_be(0, {{KEY_A, KEY_Q, KEY_TAB}});
}

TEST_F(KeyboardContextTest, AddsScanCodesForAtMost6Keys) {
    when_applied_to_switches({
        Switch(0, 0), Switch(1, 0), Switch(2, 0), Switch(3, 0),
        Switch(1, 1), Switch(2, 1), Switch(3, 1),
    });

    then_result_should_be(0, {{KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_A, KEY_S}});
}

TEST_F(KeyboardContextTest, SetsModiferForModifierKeys) {
    when_applied_to_switches({
        Switch(0, 1), // CTRL
        Switch(3, 2), // C
    });

    then_result_should_be(MODIFIERKEY_CTRL, {{KEY_C}});
}

TEST_F(KeyboardContextTest, CombinesModifers) {
    when_applied_to_switches({
        Switch(0, 1), Switch(0, 2), // CTRL SHift
        Switch(1, 1), // A
    });

    then_result_should_be(MODIFIERKEY_CTRL | MODIFIERKEY_SHIFT, {{KEY_A}});
}
