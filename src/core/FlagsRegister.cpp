#include <iostream>

#include "Utils.h"

#include "FlagsRegister.h"

FlagsRegister::FlagsRegister(const std::shared_ptr<ArithmeticLogicUnit> &arithmeticLogicUnit) {
    if (Utils::debugL2()) {
        std::cout << "FlagsRegister construct" << std::endl;
    }

    this->arithmeticLogicUnit = arithmeticLogicUnit;
    this->readOnClock = false;
    this->carryFlag = false;
    this->zeroFlag = false;
}

FlagsRegister::~FlagsRegister() {
    if (Utils::debugL2()) {
        std::cout << "FlagsRegister destruct" << std::endl;
    }
}

void FlagsRegister::print() const {
    std::cout << "FlagsRegister: CF=" << carryFlag << ", ZF=" << zeroFlag << std::endl;
}

void FlagsRegister::reset() {
    carryFlag = false;
    zeroFlag = false;
}

void FlagsRegister::in() {
    if (Utils::debugL2()) {
        std::cout << "FlagsRegister: in - will read from ALU on clock tick" << std::endl;
    }

    readOnClock = true;
}

void FlagsRegister::readFromAlu() {
    bool aluCarry = arithmeticLogicUnit->isCarry();
    bool aluZero = arithmeticLogicUnit->isZero();

    if (Utils::debugL2()) {
        std::cout << "FlagsRegister: read from ALU. Changing values from CF=" << carryFlag << ", ZF=" << zeroFlag
                  << " to CF=" << aluCarry << ", ZF=" << aluZero << std::endl;
    }

    carryFlag = aluCarry;
    zeroFlag = aluZero;
}

void FlagsRegister::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << "FlagsRegister: clock ticked" << std::endl;
    }
    
    if (readOnClock) {
        readFromAlu();
        readOnClock = false;
    }
}

bool FlagsRegister::isCarryFlag() const {
    return carryFlag;
}

bool FlagsRegister::isZeroFlag() const {
    return zeroFlag;
}