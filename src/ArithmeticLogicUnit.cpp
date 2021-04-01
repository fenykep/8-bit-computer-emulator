#include <iostream>
#include <utility>

#include "Utils.h"

#include "ArithmeticLogicUnit.h"

ArithmeticLogicUnit::ArithmeticLogicUnit(std::shared_ptr<Register> aRegister, std::shared_ptr<Register> bRegister, std::shared_ptr<Bus> bus) {
    std::cout << "ArithmeticLogicUnit in" << std::endl;
    this->aRegister = std::move(aRegister);
    this->bRegister = std::move(bRegister);
    this->bus = std::move(bus);
    this->value = 0;
    this->carry = false;
    this->zero = true;
}

ArithmeticLogicUnit::~ArithmeticLogicUnit() {
    std::cout << "ArithmeticLogicUnit out" << std::endl;
}

void ArithmeticLogicUnit::writeToBus() {
    uint8_t aValue = this->aRegister->readValue();
    uint8_t bValue = this->bRegister->readValue();
    uint16_t result = aValue + bValue;
    uint8_t newValue = result;
    bool newCarry = result > 255;
    bool newZero = newValue == 0; // Both can be active at once if result is 256 (0b100000000) / new value is 0

    std::cout << "ArithmeticLogicUnit: changing value from " << (int) this->value << " to " << (int) result
              << " (" << (int) newValue << ")" << std::endl;
    std::cout << "ArithmeticLogicUnit: changing bits from C=" << this->carry << ", Z=" << this->zero
              << " to C=" << newCarry << ", Z=" << newZero << std::endl;

    this->value = newValue;
    this->carry = newCarry;
    this->zero = newZero;
    this->bus->write(this->value);
}

void ArithmeticLogicUnit::print() const {
    printf("ArithmeticLogicUnit: value - %d / 0x%02X / " BINARY_PATTERN " \n", this->value, this->value, BYTE_TO_BINARY(this->value));
    std::cout << "ArithmeticLogicUnit: bits - C=" << this->carry << ", Z=" << this->zero << std::endl;
}

void ArithmeticLogicUnit::reset() {
    value = 0;
}

void ArithmeticLogicUnit::out() {
    std::cout << "ArithmeticLogicUnit: out" << std::endl;
    writeToBus();
}
