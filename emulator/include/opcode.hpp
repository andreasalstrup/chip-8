#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>

namespace emulator {
enum class Opcode {
  clear_screen = 0x0000u,              // 00E0 (First nibble)
  jump = 0x1000u,                      // 1NNN
  subroutine_call = 0x2000u,           // 2NNN
  subroutine_return = 0x00EEu,         // 00EE (First and last nibble)
  skip_condition_1 = 0x3000u,          // 3XNN (First nibble)
  skip_condition_2 = 0x4000u,          // 4XNN
  skip_condition_3 = 0x5000u,          // 5XY0
  skip_condition_4 = 0x9000u,          // 9XN0
  set_reg_to_num = 0x6000u,            // 6XNN
  add = 0x7000u,                       // 7XNN
  set_reg_to_reg = 0x8000u,            // 8XY0 (First and last nibble)
  or_bitwise = 0x8001u,                // 8XY1
  and_bitwise = 0x8002u,               // 8XY2
  xor_bitwise = 0x8003u,               // 8XY3
  add_reg_to_reg = 0x8004u,            // 8XY4
  sub_regX_regY = 0x8005u,             // 8XY5
  sub_regY_regX = 0x8007u,             // 8XY7
  shift_right = 0x8006u,               // 8XY6
  shift_left = 0x800Eu,                // 8XYE
  set_index = 0xA000u,                 // ANNN (First nibble)
  jump_offset = 0xB000u,               // BNNN
  random = 0xC000u,                    // CXNN
  draw = 0xD000u,                      // DXYN
  skip_if_pressed = 0xE09Eu,           // EX9E (First nibble and last byte)
  skip_not_pressed = 0xE0A1u,          // EXA1
  timer_1 = 0xF007u,                   // FX07
  timer_2 = 0xF015u,                   // FX15
  timer_3 = 0xF018u,                   // FX18
  add_to_index = 0xF01Eu,              // FX1E
  get_key = 0xF00Au,                   // FX0A
  font_character = 0xF029u,            // FX29
  binary_decimal_conversion = 0xF033u, // FX33
  store_memory = 0xF055u,              // FX55
  load_memory = 0xF065u,               // FX65
};

enum class OpcodeType {
  high_nibble = 0xF000u,
  low_nibble = 0x000Fu,
  high_and_low_nibble = 0xF00Fu,
  high_nibble_and_byte = 0xF0FFu,
};

inline void print_hex(const uint16_t &opcode) {
  std::cout << "Instru:\t 0x" << std::setw(4) << std::setfill('0') << std::hex
            << std::uppercase << opcode << std::endl; // << std::endl;
};

inline void print_hex(const Opcode &opcode) {
  std::cout << "Opcode:\t 0x" << std::setw(4) << std::setfill('0') << std::hex
            << std::uppercase << static_cast<uint16_t>(opcode) << std::endl;
};

inline void print_hex(const OpcodeType &opcode_type) {
  std::cout << "Type:\t 0x" << std::setw(4) << std::setfill('0') << std::hex
            << std::uppercase << static_cast<uint16_t>(opcode_type)
            << std::endl;
};
} // namespace emulator
