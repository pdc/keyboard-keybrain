/* Tests for blinking_thing. */

#include "Arduino.h"
#include "gtest/gtest.h"
#include "blinking_thing.h"

class BlinkingThingTest: public ::testing::Test {
public:
    BlinkingThingTest(): blinking_thing(&pin, 500, 1000) {
    }

    void given_blinking_thing_timing(unsigned long off_millis, unsigned long on_millis) {
        blinking_thing = BlinkingThing<FakePin *, FakePinTraits>(&pin, off_millis, on_millis);
    }

    FakePin pin;
    BlinkingThing<FakePin *, FakePinTraits> blinking_thing;
};


TEST_F(BlinkingThingTest, StartsLow) {
    EXPECT_EQ(pin.get_mode(), OUTPUT);
    EXPECT_EQ(pin.get_value(), LOW);
}

TEST_F(BlinkingThingTest, After500MillisSetsHigh) {
    given_blinking_thing_timing(500, 1000);

    blinking_thing.loop(500);

    EXPECT_EQ(pin.get_value(), HIGH);
}


TEST_F(BlinkingThingTest, AlternatesLowAndHigh) {
    given_blinking_thing_timing(500, 1000);

    blinking_thing.loop(555);
    EXPECT_EQ(pin.get_value(), HIGH);
    blinking_thing.loop(666);
    EXPECT_EQ(pin.get_value(), HIGH);
    blinking_thing.loop(500 + 1000);
    EXPECT_EQ(pin.get_value(), LOW);

    blinking_thing.loop(500 + 1000 + 500);
    EXPECT_EQ(pin.get_value(), HIGH);

    blinking_thing.loop(500 + 1000 + 500 + 1000);
    EXPECT_EQ(pin.get_value(), LOW);
}

TEST_F(BlinkingThingTest, SetsScheduleFromparams) {
    given_blinking_thing_timing(333, 667);

    blinking_thing.loop(332);
    EXPECT_EQ(pin.get_value(), LOW);

    blinking_thing.loop(333);
    EXPECT_EQ(pin.get_value(), HIGH);

    blinking_thing.loop(333 + 667);
    EXPECT_EQ(pin.get_value(), LOW);

    blinking_thing.loop(333 + 667 + 333);
    EXPECT_EQ(pin.get_value(), HIGH);
}

// Old algorithm switched from LOW to HIGH first time it notices it is past 1st transition time.
// Instead we want to be synchrinbized with a platonic blinker that starts at time 0.
// This way all blinking things will be in sync.
TEST_F(BlinkingThingTest, CanSkipFirstCycle) {
    given_blinking_thing_timing(333, 667);

    blinking_thing.loop(333 + 667);
    EXPECT_EQ(pin.get_value(), LOW);
}
