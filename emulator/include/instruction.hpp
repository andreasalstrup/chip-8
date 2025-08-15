#pragma once

#include "opcode.hpp"
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

// 00EE
struct SubroutineCall {
  static int execute(State &state, const uint16_t &opcode);
};

// 2NNN
struct SubroutineReturn {
  static int execute(State &state, const uint16_t &opcode);
};

// 3XNN
struct SkipCondition1 {
  static int execute(State &state, const uint16_t &opcode);
};

// 4XNN
struct SkipCondition2 {
  static int execute(State &state, const uint16_t &opcode);
};

// 5XY0
struct SkipCondition3 {
  static int execute(State &state, const uint16_t &opcode);
};

// 9XY0
struct SkipCondition4 {
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

// 8XY0
struct SetRegToReg {
  static int execute(State &state, const uint16_t &opcode);
};

// 8XY1
struct BinaryOR {
  static int execute(State &state, const uint16_t &opcode);
};

// 8XY2
struct BinaryAND {
  static int execute(State &state, const uint16_t &opcode);
};

// 8XY3
struct BinaryXOR {
  static int execute(State &state, const uint16_t &opcode);
};

// 8XY4
struct AddRegToReg {
  static int execute(State &state, const uint16_t &opcode);
};

// 8XY5
struct SubRegXRegY {
  static int execute(State &state, const uint16_t &opcode);
};

// 8XY7
struct SubRegYRegX {
  static int execute(State &state, const uint16_t &opcode);
};

// 8XY6 - Ambiguous instruction!
struct ShiftRight {
  static int execute(State &state, const uint16_t &opcode);
};

// 8XYE - Ambiguous instruction!
struct ShiftLeft {
  static int execute(State &state, const uint16_t &opcode);
};

// ANNN
struct SetIndexRegisterI {
  static int execute(State &state, const uint16_t &opcode);
};

// BNNN - Ambiguous instruction!
struct JumpOffset {
  static int execute(State &state, const uint16_t &opcode);
};

// CXNN
struct Random {
  static int execute(State &state, const uint16_t &opcode);
};

// DXYN
struct Draw {
  static int execute(State &state, const uint16_t &opcode);
};

// EX9E
struct SkipIfPressed {
  static int execute(State &state, const uint16_t &opcode);
};

// EXA1
struct SkipNotPressed {
  static int execute(State &state, const uint16_t &opcode);
};

// FX07
struct Timer1 {
  static int execute(State &state, const uint16_t &opcode);
};

// FX15
struct Timer2 {
  static int execute(State &state, const uint16_t &opcode);
};

// FX18
struct Timer3 {
  static int execute(State &state, const uint16_t &opcode);
};

// FX1E - Ambiguous instruction!
struct AddToIndex {
  static int execute(State &state, const uint16_t &opcode);
};

// FX0A
struct GetKey {
  static int execute(State &state, const uint16_t &opcode);
};

// FX29
struct FontCharacter {
  static int execute(State &state, const uint16_t &opcode);
};

// FX33
struct BinaryDecimal {
  static int execute(State &state, const uint16_t &opcode);
};

// FX55 - Ambiguous instruction!
struct StoreMemory {
  static int execute(State &state, const uint16_t &opcode);
};

// FX65 - Ambiguous instruction!
struct LoadMemory {
  static int execute(State &state, const uint16_t &opcode);
};

using Fn = std::function<int(State &, const uint16_t &)>;
using Table = std::map<Opcode, Fn>;

inline const Table table = {
    {Opcode::clear_screen, Instruction<ClearScreen>::execute},
    {Opcode::jump, Instruction<Jump>::execute},
    // {Opcode::subroutine_call, Instruction<SubroutineCall>::execute},
    // {Opcode::subroutine_return, Instruction<SubroutineReturn>::execute},
    {Opcode::skip_condition_1, Instruction<SkipCondition1>::execute},
    {Opcode::skip_condition_2, Instruction<SkipCondition2>::execute},
    // {Opcode::skip_condition_3, Instruction<SkipCondition3>::execute},
    {Opcode::skip_condition_4, Instruction<SkipCondition4>::execute},
    {Opcode::set_reg_to_num, Instruction<SetRegisterVX>::execute},
    {Opcode::add, Instruction<AddValueToRegisterVX>::execute},
    {Opcode::set_reg_to_reg, Instruction<SetRegToReg>::execute},
    {Opcode::or_bitwise, Instruction<BinaryOR>::execute},
    {Opcode::and_bitwise, Instruction<BinaryAND>::execute},
    {Opcode::xor_bitwise, Instruction<BinaryXOR>::execute},
    {Opcode::sub_regX_regY, Instruction<SubRegXRegY>::execute},
    {Opcode::sub_regY_regX, Instruction<SubRegYRegX>::execute},
    {Opcode::shift_right, Instruction<ShiftRight>::execute},
    {Opcode::shift_left, Instruction<ShiftLeft>::execute},
    {Opcode::set_index, Instruction<SetIndexRegisterI>::execute},
    {Opcode::jump_offset, Instruction<JumpOffset>::execute},
    {Opcode::random, Instruction<Random>::execute},
    {Opcode::draw, Instruction<Draw>::execute},
    // {Opcode::skip_if_pressed, Instruction<SkipIfPressed>::execute},
    // {Opcode::skip_not_pressed, Instruction<SkipNotPressed>::execute},
    // {Opcode::timer_1, Instruction<Timer1>::execute},
    // {Opcode::timer_2, Instruction<Timer2>::execute},
    // {Opcode::timer_3, Instruction<Timer3>::execute},
    {Opcode::add_to_index, Instruction<AddToIndex>::execute},
    // {Opcode::get_key, Instruction<GetKey>::execute},
    {Opcode::font_character, Instruction<FontCharacter>::execute},
    {Opcode::binary_decimal_conversion, Instruction<BinaryDecimal>::execute},
    {Opcode::store_memory, Instruction<StoreMemory>::execute},
    {Opcode::load_memory, Instruction<LoadMemory>::execute},
};
}; // namespace emulator
