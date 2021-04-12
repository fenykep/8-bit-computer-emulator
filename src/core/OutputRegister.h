#ifndef INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H

#include <memory>

#include "Bus.h"
#include "ClockListener.h"

namespace Core {

    class OutputRegister: public ClockListener {

    public:
        explicit OutputRegister(const std::shared_ptr<Bus> &bus);
        ~OutputRegister();

        void print() const;
        void reset();
        virtual void in();

    private:
        std::shared_ptr<Bus> bus;
        uint8_t value;
        bool readOnClock;

        void readFromBus();

        void clockTicked() override;
        void invertedClockTicked() override {}; // Not implemented
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H
