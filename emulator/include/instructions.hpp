#pragma once

#include "chip8.hpp"
#include <cstdint>

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
} // namespace emulator
