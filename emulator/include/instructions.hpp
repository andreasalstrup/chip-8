#pragma once

#include "state.hpp"
#include <cstdint>
#include <functional>
#include <map>

namespace emulator {
struct Instruction {
  virtual ~Instruction() = default;
  // virtual int execute(State& state) = 0;
};

// 00E0
struct ClearScreen final : public Instruction {
  static int execute(State &state, const uint16_t &opcode);
};

// 1NNN
struct Jump final : public Instruction {
  static int execute(State &state, const uint16_t &opcode);
};

// 6XNN
struct SetRegisterVX final : public Instruction {
  static int execute(State &state, const uint16_t &opcode);
};

// 7XNN
struct AddValueToRegisterVX final : public Instruction {
  static int execute(State &state, const uint16_t &opcode);
};

// ANNN
struct SetIndexRegisterI final : public Instruction {
  static int execute(State &state, const uint16_t &opcode);
};

// DXYN
struct Draw final : public Instruction {
  static int execute(State &state, const uint16_t &opcode);
};

using instruction = std::function<int(State &, const uint16_t &)>;

const std::map<Opcode, instruction> instruction_table = {
    {Opcode::clear_screen, ClearScreen::execute},
    {Opcode::jump, Jump::execute},
    {Opcode::set_reg_to_num, SetRegisterVX::execute},
    {Opcode::add, AddValueToRegisterVX::execute},
    {Opcode::set_index, SetIndexRegisterI::execute},
    {Opcode::draw, Draw::execute},
};
}; // namespace emulator
