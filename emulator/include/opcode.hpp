#pragma once

namespace emulator {
enum class Opcode {
  clear_screen = 0x0000,              // 00E0 (First nibble)
  jump = 0x1000,                      // 1NNN
  subroutine_call = 0x2000,           // 2NNN
  subroutine_return = 0x00EE,         // 00EE (First and last nibble)
  skip_condition_1 = 0x3000,          // 3XNN (First nibble)
  skip_condition_2 = 0x4000,          // 4XNN
  skip_condition_3 = 0x5000,          // 5XY0
  skip_condition_4 = 0x9000,          // 9XN0
  set_reg_to_num = 0x6000,            // 6XNN
  add = 0x7000,                       // 7XNN
  set_reg_to_reg = 0x8000,            // 8XY0 (First and last nibble)
  or_bitwise = 0x8001,                // 8XY1
  and_bitwise = 0x8002,               // 8XY2
  xor_bitwise = 0x8003,               // 8XY3
  add_reg_to_reg = 0x8004,            // 8XY4
  sub_regX_regY = 0x8005,             // 8XY5
  sub_regY_regX = 0x8007,             // 8XY7
  shift_right = 0x8006,               // 8XY6
  shift_left = 0x800E,                // 8XYE
  set_index = 0xA000,                 // ANNN (First nibble)
  jump_offset = 0xB000,               // BNNN
  random = 0xC000,                    // CXNN
  draw = 0xD000,                      // DXYN
  skip_if_pressed = 0xE09E,           // EX9E (First nibble and last byte)
  skip_not_pressed = 0xE0A1,          // EXA1
  timer_1 = 0xF007,                   // FX07
  timer_2 = 0xF015,                   // FX15
  timer_3 = 0xF018,                   // FX18
  add_to_index = 0xF01E,              // FX1E
  get_key = 0xF00A,                   // FX0A
  font_character = 0xF029,            // FX29
  binary_decimal_conversion = 0xF033, // FX33
  store_memory = 0xF055,              // FX55
  load_memory = 0xF065,               // FX65
};

enum class OpcodeType {
  high_nibble = 0xF000,
  low_nibble = 0x000F,
  high_and_low_nibble = 0xF000F,
  high_nibble_and_byte = 0xF0FF,
};
} // namespace emulator
