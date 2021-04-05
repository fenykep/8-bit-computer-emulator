#include "include/doctest.h"
#include "include/fakeit.hpp"

#include "StepCounter.h"

TEST_SUITE("StepCounterTest") {
    TEST_CASE("inverted clock ticks should notify listener of incrementing steps and wrap at 4") {
        fakeit::Mock<StepListener> stepListenerMock;
        auto stepListenerMockSharedPtr = std::shared_ptr<StepListener>(&stepListenerMock(), [](...) {});
        StepCounter stepCounter(stepListenerMockSharedPtr);

        fakeit::When(Method(stepListenerMock, stepReady)).AlwaysReturn();

        auto &clock = dynamic_cast<ClockListener&>(stepCounter);

        clock.invertedClockTicked();
        fakeit::Verify(Method(stepListenerMock, stepReady).Using(0)).Once();

        clock.invertedClockTicked();
        fakeit::Verify(Method(stepListenerMock, stepReady).Using(1)).Once();

        clock.invertedClockTicked();
        fakeit::Verify(Method(stepListenerMock, stepReady).Using(2)).Once();

        clock.invertedClockTicked();
        fakeit::Verify(Method(stepListenerMock, stepReady).Using(3)).Once();

        clock.invertedClockTicked();
        fakeit::Verify(Method(stepListenerMock, stepReady).Using(4)).Once();

        clock.invertedClockTicked();
        fakeit::Verify(Method(stepListenerMock, stepReady).Using(0)).Twice();

        clock.invertedClockTicked();
        fakeit::Verify(Method(stepListenerMock, stepReady).Using(1)).Twice();
    }

    TEST_CASE("reset() should start counting steps at 0 again") {
        fakeit::Mock<StepListener> stepListenerMock;
        auto stepListenerMockSharedPtr = std::shared_ptr<StepListener>(&stepListenerMock(), [](...) {});
        StepCounter stepCounter(stepListenerMockSharedPtr);

        fakeit::When(Method(stepListenerMock, stepReady)).AlwaysReturn();

        auto &clock = dynamic_cast<ClockListener&>(stepCounter);

        clock.invertedClockTicked();
        clock.invertedClockTicked();

        fakeit::Verify(Method(stepListenerMock, stepReady).Using(0)).Once();
        fakeit::Verify(Method(stepListenerMock, stepReady).Using(1)).Once();

        stepCounter.reset();

        clock.invertedClockTicked();
        fakeit::Verify(Method(stepListenerMock, stepReady).Using(0)).Twice();
    }
}
