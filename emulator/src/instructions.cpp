#include "../include/instructions.hpp"
#include <cstdint>

namespace emulator {
int ClearScreen::execute(State &state, const uint16_t &opcode) {
  for (int i = 0; i < platform::DISPLAY_SIZE; i++) {
    state.display.bitmap.setPixel(i, platform::action::on);
  }
  return 0;
}

int Jump::execute(State &state, const uint16_t &opcode) {
  const uint16_t address = opcode & 0x0FFF;
  state.cpu.pc = &state.memory[address];
  return 0;
}

int SetRegisterVX::execute(State &state, const uint16_t &opcode) {
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const uint8_t value = opcode & 0x00FF;
  state.cpu.variable_register[reg] = value;
  return 0;
}

int AddValueToRegisterVX::execute(State &state, const uint16_t &opcode) {
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const uint8_t value = opcode & 0x00FF;
  state.cpu.variable_register[reg] += value;
  return 0;
}

int SetIndexRegisterI::execute(State &state, const uint16_t &opcode) {
  const uint16_t value = opcode & 0x0FFF;
  state.cpu.index_register = value;
  return 0;
}

int Draw::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const auto &address = state.cpu.index_register;
  const auto &memory = state.memory;
  const uint8_t rows = opcode & 0x000F;

  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  const auto start_x = registers[reg_x] % 64;
  const auto start_y = registers[reg_y] % 32;

  registers[0xF] = 0;

  for (int row = 0; row < rows; row++) {
    const uint8_t cols = memory[address + row];

    for (int col = 0; col < 8; col++) {
      if (const bool current_pixel = (cols & (0x80 >> col))) {
        const uint8_t current_x = (start_x + col);
        const uint8_t current_y = (start_y + row);

        if (current_x >= platform::WIDTH || current_y >= platform::HEIGHT)
          continue;

        const auto &screen_pixel =
            state.display.bitmap.getPixel(current_x, current_y);
        if (screen_pixel == 0xFF000000) {
          state.display.bitmap.setPixel(current_x, current_y,
                                        platform::action::off);
          registers[0xF] = 1;
        } else {
          state.display.bitmap.setPixel(current_x, current_y,
                                        platform::action::on);
        }
      }
    }
  }

  return 0;
}
} // namespace emulator
