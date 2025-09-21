#include "../include/instruction.hpp"
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>

namespace emulator {
// 00E0
int ClearScreen::execute(State &state, const uint16_t &opcode) {
  for (int i = 0; i < platform::DISPLAY_SIZE; i++) {
    state.display.bitmap.setPixel(i, platform::action::on);
  }
  return 0;
}

// 1NNN
int Jump::execute(State &state, const uint16_t &opcode) {
  const uint16_t address = opcode & 0x0FFFu;
  state.cpu.pc = &state.memory[address];
  return 0;
}

// 2NNN
int SubroutineCall::execute(State &state, const uint16_t &opcode) {
  const uint16_t address = opcode & 0x0FFFu;
  uint8_t *subroutine = &state.memory[address];
  state.call_stack.push_back(state.cpu.pc);
  state.cpu.pc = subroutine;
  return 0;
}

// 00EE
int SubroutineReturn::execute(State &state, const uint16_t &opcode) {
  if (state.call_stack.empty())
    return 1;

  uint8_t *subroutine = state.call_stack.back();
  state.call_stack.pop_back();
  state.cpu.pc = subroutine;
  return 0;
}

// 3XNN
int SkipCondition1::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t value = (opcode & 0x00FFu);
  const auto &vx = registers[reg_x];

  if (vx == value) {
    state.cpu.pc += 2;
    return 0;
  }

  return 1;
}

// 4XNN
int SkipCondition2::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t value = (opcode & 0x00FFu);
  const auto &vx = registers[reg_x];

  if (vx != value) {
    auto &pc = state.cpu.pc;
    pc += 2;
    return 0;
  }

  return 1;
}

// 5XY0
int SkipCondition3::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  const auto &vx = registers[reg_x];
  const auto &vy = registers[reg_y];

  if (vx == vy) {
    state.cpu.pc += 2;
    return 0;
  }

  return 1;
}

// 9XY0
int SkipCondition4::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  const auto &vx = registers[reg_x];
  const auto &vy = registers[reg_y];

  if (vx != vy) {
    state.cpu.pc += 2;
    return 0;
  }

  return 1;
}

// 6XNN
int SetRegisterVX::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t value = opcode & 0x00FFu;
  auto &vx = registers[reg_x];

  vx = value;
  return 0;
}

// 7XNN
int AddValueToRegisterVX::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t value = opcode & 0x00FFu;
  auto &vx = registers[reg_x];

  vx += value;
  return 0;
}

// 8XY0
int SetRegToReg::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  auto &vx = registers[reg_x];
  const auto &vy = registers[reg_y];

  vx = vy;
  return 0;
}

// 8XY1
int BinaryOR::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4u;
  auto &vx = registers[reg_x];
  const auto &vy = registers[reg_y];

  vx |= vy;
  return 0;
}

// 8XY2
int BinaryAND::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  auto &vx = registers[reg_x];
  const auto &vy = registers[reg_y];

  vx &= vy;
  return 0;
}

// 8XY3
int BinaryXOR::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  auto &vx = registers[reg_x];
  const auto &vy = registers[reg_y];

  vx ^= vy;
  return 0;
}

// 8XY4
int AddRegToReg::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  uint8_t &vx = registers[reg_x];
  uint8_t &vy = registers[reg_y];
  uint8_t &vf = registers[0xFu];

  const uint16_t result = vx + vy;
  vx = static_cast<uint8_t>(result);
  vf = (result > 0xFFu) ? 1 : 0;
  return 0;
}

// 8XY5
int SubRegXRegY::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  uint8_t &vx = registers[reg_x];
  const uint8_t vx_tmp = registers[reg_x];
  const uint8_t &vy = registers[reg_y];
  uint8_t &vf = registers[0xFu];

  const uint16_t result = vx - vy;
  vx = static_cast<uint8_t>(result);
  vf = (vx_tmp >= vy) ? 1 : 0;
  return 0;
}

// 8XY7
int SubRegYRegX::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  uint8_t &vx = registers[reg_x];
  const uint8_t vx_tmp = registers[reg_x];
  const uint8_t &vy = registers[reg_y];
  uint8_t &vf = registers[0xFu];

  const uint16_t result = vy - vx;
  vx = static_cast<uint8_t>(result);
  vf = (vx_tmp <= vy) ? 1 : 0;
  return 0;
}

// 8XY6 - Ambiguous instruction!
int ShiftRight::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  uint8_t &vx = registers[reg_x];
  const uint8_t &vy = registers[reg_y];
  uint8_t &vf = registers[0xFu];

  const uint8_t lsb = vx & 0x01u;
  vx >>= 1;
  vf = lsb;
  return 0;
}

// 8XYE - Ambiguous instruction!
int ShiftLeft::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  uint8_t &vx = registers[reg_x];
  const uint8_t &vy = registers[reg_y];
  uint8_t &vf = registers[0xFu];

  const uint8_t msb = (vx & 0x80u) >> 7;
  vx <<= 1;
  vf = msb;
  return 0;
}

// ANNN
int SetIndexRegisterI::execute(State &state, const uint16_t &opcode) {
  const uint16_t value = opcode & 0x0FFFu;
  state.cpu.index_register = value;
  return 0;
}

// BNNN - Ambiguous instruction!
int JumpOffset::execute(State &state, const uint16_t &opcode) {
  // COSMAC VIP interpreter
  const auto &registers = state.cpu.variable_register;
  const auto &reg_value = registers[0x0u];
  const uint16_t address = (opcode & 0x0FFFu);

  state.cpu.pc = &state.memory[address] + reg_value;
  return 0;
}

// CXNN
int Random::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0xF00u) >> 8;
  const uint8_t value = (opcode & 0x0FFu);
  auto &vx = registers[reg_x];

  srand(std::time(0));
  const auto random = std::rand() & value;
  vx = random & value;
  return 0;
}

// DXYN
int Draw::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const auto &address = state.cpu.index_register;
  const auto &memory = state.memory;
  const uint8_t rows = opcode & 0x000Fu;

  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4;
  const auto start_x = registers[reg_x] % 64;
  const auto start_y = registers[reg_y] % 32;

  registers[0xFu] = 0;

  for (int row = 0; row < rows; row++) {
    const uint8_t cols = memory[address + row];

    for (int col = 0; col < 8; col++) {
      if (const bool current_pixel = (cols & (0x80u >> col))) {
        const uint8_t current_x = (start_x + col);
        const uint8_t current_y = (start_y + row);

        if (current_x >= platform::WIDTH || current_y >= platform::HEIGHT)
          continue;

        const auto &screen_pixel =
            state.display.bitmap.getPixel(current_x, current_y);
        if (screen_pixel == 0xFF000000u) {
          state.display.bitmap.setPixel(current_x, current_y,
                                        platform::action::off);
          registers[0xFu] = 1;
        } else {
          state.display.bitmap.setPixel(current_x, current_y,
                                        platform::action::on);
        }
      }
    }
  }

  return 0;
}

// EX9E
int SkipIfPressed::execute(State &state, const uint16_t &opcode) { return 0; }

// EXA1
int SkipNotPressed::execute(State &state, const uint16_t &opcode) { return 0; }

// FX07
int Timer1::execute(State &state, const uint16_t &opcode) { return 0; }

// FX15
int Timer2::execute(State &state, const uint16_t &opcode) { return 0; }

// FX18
int Timer3::execute(State &state, const uint16_t &opcode) { return 0; }

// FX1E - Ambiguous instruction!
int AddToIndex::execute(State &state, const uint16_t &opcode) {
  // COSMAC VIP interpreter
  const auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const auto &vx = registers[reg_x];

  state.cpu.index_register += vx;
  return 0;
}

// FX0A
int GetKey::execute(State &state, const uint16_t &opcode) { return 0; }

// FX29
int FontCharacter::execute(State &state, const uint16_t &opcode) {
  const auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const auto &vx = registers[reg_x];

  state.cpu.index_register = 0x200u + (5 * vx);
  return 0;
}

// FX33
int BinaryDecimal::execute(State &state, const uint16_t &opcode) {
  const auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const auto &vx = registers[reg_x];
  const auto address = state.cpu.index_register;
  auto memory = &state.memory[address];

  uint8_t a = vx / 100;
  uint8_t b = (vx % 100) / 10;
  uint8_t c = vx % 10;
  memory[0] = a;
  memory[1] = b;
  memory[2] = c;
  return 0;
}

// FX55 - Ambiguous instruction!
int StoreMemory::execute(State &state, const uint16_t &opcode) {
  const auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const auto address = state.cpu.index_register;
  auto memory = &state.memory[address];

  for (auto i = 0; i <= reg_x; i++) {
    const auto reg_value = registers[i];
    memory[i] = reg_value;
  }

  return 0;
};

// FX65 - Ambiguous instruction!
int LoadMemory::execute(State &state, const uint16_t &opcode) {
  // CHIP48 and SUPER-CHIP
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8;
  const auto address = state.cpu.index_register;
  const auto memory = &state.memory[address];

  for (auto i = 0; i <= reg_x; i++) {
    const auto value = memory[i];
    registers[i] = value;
  }

  return 0;
};
} // namespace emulator
