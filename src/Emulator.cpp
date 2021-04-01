#include <iostream>
#include <thread>

#include "Assembler.h"

#include "Emulator.h"

Emulator::Emulator() {
    std::cout << "Emulator in" << std::endl;

    clock = std::make_shared<Clock>();
    bus = std::make_shared<Bus>();
    aRegister = std::make_shared<Register>("A", bus);
    bRegister = std::make_shared<Register>("B", bus);
    alu = std::make_shared<ArithmeticLogicUnit>(aRegister, bRegister, bus);
    ram = std::make_shared<RandomAccessMemory>(bus);
    mar = std::make_shared<MemoryAddressRegister>(ram, bus);
    pc = std::make_shared<ProgramCounter>(bus);
    instructionRegister = std::make_shared<InstructionRegister>(bus);
    out = std::make_shared<OutputRegister>(bus);
    microcode = std::make_shared<Microcode>(mar, pc, ram, instructionRegister, aRegister, bRegister, alu, out, clock);
    stepCounter = std::make_unique<StepCounter>(microcode);

    // Cyclic dependency - also, setting it here to reuse the shared pointers
    aRegister->setRegisterListener(alu);
    bRegister->setRegisterListener(alu);

    clock->addListener(mar.get());
    clock->addListener(stepCounter.get());
    clock->addListener(instructionRegister.get());
    clock->addListener(pc.get());
    clock->addListener(aRegister.get());
    clock->addListener(bRegister.get());
    clock->addListener(out.get());
    clock->addListener(ram.get());
}

Emulator::~Emulator() {
    std::cout << "Emulator out" << std::endl;
}

void Emulator::run(const std::string &fileName) {
    std::cout << "Emulator: run start" << std::endl;

    reset();
    printValues();

    programMemory(fileName);
    printValues();

    std::cout << "Emulator: run clock" << std::endl;

    clock->setFrequency(20);
    clock->start();
    clock->join();

    printValues();

    std::cout << "Emulator: run stop" << std::endl;
}

void Emulator::programMemory(const std::string &fileName) {
    std::cout << "Emulator: program memory" << std::endl;

    auto assembler = std::make_unique<Assembler>();
    const std::vector<Assembler::Instruction> instructions = assembler->loadInstructions(fileName);

    for (auto instruction : instructions) {
        mar->program(instruction.address);
        ram->program(instruction.opcode, instruction.operand);
    }
}

void Emulator::printValues() {
    std::cout << "Emulator: print current values" << std::endl;

    bus->print();
    aRegister->print();
    bRegister->print();
    alu->print();
    mar->print();
    ram->print();
    pc->print();
    instructionRegister->print();
    out->print();
    stepCounter->print();
}

void Emulator::reset() {
    std::cout << "Emulator: reset" << std::endl;

    bus->reset();
    aRegister->reset();
    bRegister->reset();
    alu->reset();
    mar->reset();
    ram->reset();
    pc->reset();
    instructionRegister->reset();
    out->reset();
    stepCounter->reset();
}
