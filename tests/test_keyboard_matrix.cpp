/* Tests for keyboard_matrix. */

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>
#include "Arduino.h"
#include "gtest/gtest.h"
#include "keyboard_matrix.h"

using namespace std;


/**
 * Test fixture with a keyboard matrix and some controllable pins.
 */
class KeyboardMatrixTest: public ::testing::Test, protected FakePinListener {
public:
    static const int column_count = 2;
    static const int row_count = 2;

    array<FakePin, column_count> column_pins;
    array<FakePin, row_count> row_pins;
    KeyboardMatrix<column_count, row_count, FakePin *, FakePinTraits> keyboard_matrix;

    vector<pair<int, int> > closed_switches;
    millis_t next_millis = 13;

    KeyboardMatrixTest():
        keyboard_matrix{ {{&column_pins[0], &column_pins[1]}}, {{&row_pins[0], &row_pins[1]}} }
    {
        for (auto &pin : column_pins) {
            pin.add_listener(this);
        }
    }

    void when_loop_called_with_closed_switches(vector<pair<int, int> > next_closed_switches) {
        closed_switches = next_closed_switches;
        keyboard_matrix.loop(next_millis++);
    }

    void when_loop_called_with_closed_switches(millis_t millis, vector<pair<int, int> > next_closed_switches) {
        closed_switches = next_closed_switches;
        next_millis = millis;
        keyboard_matrix.loop(next_millis++);
    }

    void given_loop_called_with_closed_switches(millis_t millis, vector<pair<int, int> > next_closed_switches) {
        when_loop_called_with_closed_switches(millis, next_closed_switches);
    }

    void then_pressed_switches_should_contain_in_any_order(vector<Switch> expected) {
        Switches actual = keyboard_matrix.pressed_switches();
        for (Switch x : expected) {
            EXPECT_TRUE(any_of(actual.begin(), actual.end(), [=](Switch y){ return x == y; }))
                << "Expected to find switch {" << (int) x.col << ", " << (int) x.row << "}";
        }
        for (Switch y : actual) {
            EXPECT_TRUE(any_of(expected.begin(), expected.end(), [=](Switch x){ return x == y; }))
                << "Did not expect to find switch {" << (int) y.col << ", " << (int) y.row << "}";
        }
        EXPECT_EQ(actual.size(), expected.size());
    }

protected:
    void on_value_changed(int) override {
        update();
    }

private:
    /**
     * Every time we set the value of a pin
     * we update the values of pins that are affected by it.
     */
    void update() {
        for (int j = 0; j < row_count; ++j) {
            int value = HIGH;
            for (auto pr : closed_switches) {
                int col = pr.first;
                int row = pr.second;
                if (row == j) {
                    auto &pin = column_pins[col];
                    if (!pin.is_floating()) {
                        EXPECT_EQ(pin.get_value(), LOW);
                        value = LOW;
                        break;
                    }
                }
            }
            row_pins[j].set_value(value);
        }
    }
};


/* Tests */


TEST_F(KeyboardMatrixTest, StartsWithAllColumnsFloating) {
    for (int i = 0; i < column_count; ++i) {
        EXPECT_TRUE(column_pins[i].is_floating());
    }
    for (int i = 0; i < row_count; ++i) {
        EXPECT_EQ(row_pins[i].get_mode(), INPUT_PULLUP);
    }
}

TEST_F(KeyboardMatrixTest, RecordsKeyPressInLoop) {
    closed_switches = { {0, 1} };

    keyboard_matrix.loop(13);
    const Switches result = keyboard_matrix.pressed_switches();

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].col, 0);
    EXPECT_EQ(result[0].row, 1);
}

TEST_F(KeyboardMatrixTest, DoesntRecordKepressesMoreThanOnce) {
    closed_switches = { {1, 0} };

    keyboard_matrix.loop(13);
    keyboard_matrix.loop(17);
    keyboard_matrix.loop(21);

    then_pressed_switches_should_contain_in_any_order({ {1, 0} });
}

TEST_F(KeyboardMatrixTest, MultipleKeypressesReocrded) {
    given_loop_called_with_closed_switches(140, { {1, 1} });

    when_loop_called_with_closed_switches(150, { {1, 1}, {0, 1} });

    then_pressed_switches_should_contain_in_any_order({ {1, 1}, {0, 1} });
}

TEST_F(KeyboardMatrixTest, KeypressRemovedWhenNoLongerPressed) {
    const millis_t start = 1313;
    given_loop_called_with_closed_switches(start, { {1, 1}, {0, 1} });

    when_loop_called_with_closed_switches(start + debounce_millis, { {0, 1} });

    then_pressed_switches_should_contain_in_any_order({ {0, 1} });
}

TEST_F(KeyboardMatrixTest, KeypressIsDebounced) {
    const millis_t start = 69;
    given_loop_called_with_closed_switches(start, { {1, 1}, {0, 1} });

    when_loop_called_with_closed_switches(start + debounce_millis - 1, { {0, 1} });

    then_pressed_switches_should_contain_in_any_order({ {1, 1}, {0, 1} });
}

TEST_F(KeyboardMatrixTest, KeyReleaseIsDebounced) {
    const millis_t start = 69;
    given_loop_called_with_closed_switches(start, { {1, 1}, {0, 1} });
    given_loop_called_with_closed_switches(start + debounce_millis, { {0, 1} });
    given_loop_called_with_closed_switches(start + debounce_millis + 1, { {0, 1} });

    when_loop_called_with_closed_switches(start + 2 * debounce_millis - 1, { {1, 1}, {0, 1} });

    then_pressed_switches_should_contain_in_any_order({ {0, 1} });
}

TEST_F(KeyboardMatrixTest, KeyReleaseIsNotDebouncedForever) {
    const millis_t start = 69;
    given_loop_called_with_closed_switches(start, { {1, 1}, {0, 1} });
    given_loop_called_with_closed_switches(start + debounce_millis, { {0, 1} });

    when_loop_called_with_closed_switches(start + 2 * debounce_millis, { {1, 1}, {0, 1} });

    then_pressed_switches_should_contain_in_any_order({ {1, 1}, {0, 1} });
}

