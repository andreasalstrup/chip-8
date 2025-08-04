#pragma once

#include "state.hpp"
#include <cstdint>
#include <functional>
#include <map>
#include <sys/types.h>

namespace emulator {

template <typename C>
concept InstructionConcept = requires(State &state, const uint16_t &opcode) {
  { C::execute(state, opcode) } -> std::same_as<int>;
};

template <InstructionConcept T> struct Instruction {
  static int execute(State &state, const uint16_t &opcode) {
    return T::execute(state, opcode);
  }
};

// 00E0
struct ClearScreen {
  static int execute(State &state, const uint16_t &opcode);
};

// 1NNN
struct Jump {
  static int execute(State &state, const uint16_t &opcode);
};

// 6XNN
struct SetRegisterVX {
  static int execute(State &state, const uint16_t &opcode);
};

// 7XNN
struct AddValueToRegisterVX {
  static int execute(State &state, const uint16_t &opcode);
};

// ANNN
struct SetIndexRegisterI {
  static int execute(State &state, const uint16_t &opcode);
};

// DXYN
struct Draw {
  static int execute(State &state, const uint16_t &opcode);
};

using Fn = std::function<int(State &, const uint16_t &)>;
using Table = std::map<Opcode, Fn>;

inline const Table table = {
    {Opcode::clear_screen, Instruction<ClearScreen>::execute},
    {Opcode::jump, Instruction<Jump>::execute},
    {Opcode::set_reg_to_num, Instruction<SetRegisterVX>::execute},
    {Opcode::add, Instruction<AddValueToRegisterVX>::execute},
    {Opcode::set_index, Instruction<SetIndexRegisterI>::execute},
    {Opcode::draw, Instruction<Draw>::execute},
};
}; // namespace emulator
