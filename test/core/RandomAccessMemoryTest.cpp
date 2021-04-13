#include <doctest.h>

#include "core/Utils.h"
#include "core/RandomAccessMemory.h"

using namespace Core;

TEST_SUITE("RandomAccessMemoryTest") {
    TEST_CASE("ram should work correctly") {
        const std::shared_ptr<Bus> &bus = std::make_shared<Bus>();

        RandomAccessMemory ram(bus);

        auto &mar = dynamic_cast<RegisterListener&>(ram);
        auto &clock = dynamic_cast<ClockListener&>(ram);

        CHECK_EQ(bus->read(), 0);

        SUBCASE("memory should be initialized with 0") {
            for (int i = 0; i < RandomAccessMemory::MEMORY_SIZE; i++) {
                mar.registerValueChanged(i);
                ram.out();

                CHECK_EQ(bus->read(), 0);
            }
        }

        SUBCASE("memory should be programmable manually") {
            // Splitting programming and verification to make sure every location is changed,
            // instead of potentially writing and reading from the same location by mistake
            for (int i = 0; i < RandomAccessMemory::MEMORY_SIZE; i++) {
                mar.registerValueChanged(i);
                ram.program(Utils::to4bits(i), Utils::to4bits(i));
            }

            for (int i = 0; i < RandomAccessMemory::MEMORY_SIZE; i++) {
                mar.registerValueChanged(i);
                ram.out();

                CHECK_EQ(bus->read(), i * 17);
            }
        }

        SUBCASE("reset() should reset address but not memory content") {
            // Set a value of 10 at memory location 0
            ram.program(Utils::to4bits(0), Utils::to4bits(10));
            ram.out();
            CHECK_EQ(bus->read(), 10);

            // Set a value of 15 at memory location 5
            mar.registerValueChanged(5);
            ram.program(Utils::to4bits(0), Utils::to4bits(15));
            ram.out();
            CHECK_EQ(bus->read(), 15);

            // Reset. Should be back at value 10 now, from memory location 0
            ram.reset();
            ram.out();
            CHECK_EQ(bus->read(), 10);

            // And should still have 15 at memory location 5
            mar.registerValueChanged(5);
            ram.out();
            CHECK_EQ(bus->read(), 15);
        }

        SUBCASE("clockTicked() should store value from bus on clock tick") {
            bus->write(8);

            ram.in(); // Prepare to read from bus

            ram.out();
            CHECK_EQ(bus->read(), 0); // Nothing read from the bus yet

            bus->write(6); // This should be stored
            clock.clockTicked();

            bus->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            ram.out();
            CHECK_EQ(bus->read(), 6);
        }

        SUBCASE("clockTicked() should only store on first clock tick") {
            bus->write(8);

            ram.in();
            clock.clockTicked();

            ram.out();
            CHECK_EQ(bus->read(), 8);

            bus->write(6);
            clock.clockTicked();

            ram.out();
            CHECK_EQ(bus->read(), 8); // No change
        }

        SUBCASE("memory should be programmable from the bus") {
            for (int i = 0; i < RandomAccessMemory::MEMORY_SIZE; i++) {
                mar.registerValueChanged(i);
                bus->write(i + 1);
                ram.in();
                clock.clockTicked();
            }

            for (int i = 0; i < RandomAccessMemory::MEMORY_SIZE; i++) {
                mar.registerValueChanged(i);
                ram.out();

                CHECK_EQ(bus->read(), i + 1);
            }
        }

        SUBCASE("ram should throw exception if memory location is out of bounds") {
            CHECK_THROWS_WITH(mar.registerValueChanged(16),
                              "RandomAccessMemory: address out of bounds 16");

        }

        SUBCASE("print() should not fail") {
            ram.print();
        }
    }
}
