#include "../include/instruction.hpp"
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <utility>

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
  const uint16_t address = opcode & 0x0FFF;
  state.cpu.pc = &state.memory[address];
  // state.cpu.pc = address;
  return 0;
}

// 2NNN
int SubroutineCall::execute(State &state, const uint16_t &opcode) {
  const uint16_t address = opcode & 0x0FFF;
  uint8_t *subroutine = &state.memory[address];
  // state.call_stack.push_back(state.cpu.pc + 2);
  state.cpu.pc = subroutine;
  state.call_stack.push_back(state.cpu.pc);

  // state.call_stack.push_back(state.cpu.pc + 2);
  // state.cpu.pc = address;

  // state.cpu.stack[state.cpu.sp] = state.cpu.pc;
  // ++state.cpu.sp;
  // state.cpu.pc = address;
  return 0;
}

// 00EE
int SubroutineReturn::execute(State &state, const uint16_t &opcode) {
  if (state.call_stack.empty())
    return 1;

  uint8_t *subroutine = state.call_stack.back();
  state.call_stack.pop_back();
  state.cpu.pc = subroutine;

  // uint16_t address = state.call_stack.back();
  // state.call_stack.pop_back();
  // state.cpu.pc = address;

  // --state.cpu.sp;
  // state.cpu.pc = state.cpu.stack[state.cpu.sp];
  return 0;
}

// 3XNN
int SkipCondition1::execute(State &state, const uint16_t &opcode) {
  const uint8_t value = (opcode & 0x00FF);
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const auto &reg_value = state.cpu.variable_register[reg];
  if (value == reg_value) {
    state.cpu.pc += 2;
    return 0;
  }
  return 1;
}

// 4XNN
int SkipCondition2::execute(State &state, const uint16_t &opcode) {
  const uint8_t value = (opcode & 0x00FF);
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const auto &reg_value = state.cpu.variable_register[reg];
  if (value != reg_value) {
    state.cpu.pc += 2;
    return 0;
  }
  return 1;
}

// 5XY0
int SkipCondition3::execute(State &state, const uint16_t &opcode) {
  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  const auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];
  if (reg_x_value == reg_y_value) {
    state.cpu.pc += 2;
    return 0;
  }
  return 1;
}

// 9XY0
int SkipCondition4::execute(State &state, const uint16_t &opcode) {
  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  const auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];
  if (reg_x_value != reg_y_value) {
    state.cpu.pc += 2;
    return 0;
  }
  return 1;
}

// 6XNN
int SetRegisterVX::execute(State &state, const uint16_t &opcode) {
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const uint8_t value = opcode & 0x00FF;
  state.cpu.variable_register[reg] = value;
  return 0;
}

// 7XNN
int AddValueToRegisterVX::execute(State &state, const uint16_t &opcode) {
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const uint8_t value = opcode & 0x00FF;
  state.cpu.variable_register[reg] += value;
  return 0;
}

// 8XY0
int SetRegToReg::execute(State &state, const uint16_t &opcode) {
  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];
  reg_x_value = reg_y_value;
  return 0;
}

// 8XY1
int BinaryOR::execute(State &state, const uint16_t &opcode) {
  const uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
  const uint8_t reg_y = (opcode & 0x00F0u) >> 4u;
  auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];
  // reg_x_value = reg_x_value | reg_y_value;
  reg_x_value |= reg_y_value;

  return 0;
}

// 8XY2
int BinaryAND::execute(State &state, const uint16_t &opcode) {
  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];
  reg_x_value = reg_x_value & reg_y_value;
  return 0;
}

// 8XY3
int BinaryXOR::execute(State &state, const uint16_t &opcode) {
  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];
  reg_x_value = reg_x_value ^ reg_y_value;
  return 0;
}

// 8XY4
int AddRegToReg::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];
  const uint16_t res = reg_x_value + reg_y_value;

  if (res > 0xFF) {
    registers[0xF] = 1;
  } else {
    registers[0xF] = 0;
  }
  // reg_x_value += reg_y_value;
  reg_x_value = res & 0xFF;
  return 0;
}

// 8XY5
int SubRegXRegY::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];

  if (reg_x_value > reg_y_value) {
    registers[0xF] = 1;
  } else {
    registers[0xF] = 0;
  }
  reg_x_value = reg_x_value - reg_y_value;
  return 0;
}

// 8XY7
int SubRegYRegX::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];

  if (reg_y_value > reg_x_value) {
    registers[0xF] = 1;
  } else {
    registers[0XF] = 0;
  }
  reg_x_value = reg_y_value - reg_x_value;
  return 0;
}

// 8XY6 - Ambiguous instruction!
int ShiftRight::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];

  // COSMAC VIP instruction
  // reg_x_value = reg_y_value;

  if (reg_x_value & 0x0F) {
    registers[0xF] = 1;
  } else {
    registers[0xF] = 0;
  }
  reg_x_value = reg_x_value >> 1;
  return 0;
}

// 8XYE - Ambiguous instruction!
int ShiftLeft::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg_x = (opcode & 0x0F00) >> 8;
  const uint8_t reg_y = (opcode & 0x00F0) >> 4;
  auto &reg_x_value = state.cpu.variable_register[reg_x];
  const auto &reg_y_value = state.cpu.variable_register[reg_y];

  // COSMAC VIP instruction
  // reg_x_value = reg_y_value;

  if (reg_x_value & 0xF0) {
    registers[0xF] = 1;
  } else {
    registers[0xF] = 0;
  }
  reg_x_value = reg_x_value << 1;
  return 0;
}

// ANNN
int SetIndexRegisterI::execute(State &state, const uint16_t &opcode) {
  const uint16_t value = opcode & 0x0FFF;
  state.cpu.index_register = value;
  return 0;
}

// BNNN - Ambiguous instruction!
int JumpOffset::execute(State &state, const uint16_t &opcode) {
  // COSMAC VIP interpreter
  const auto &registers = state.cpu.variable_register;
  const auto &reg_value = registers[0x0];
  const uint16_t address = (opcode & 0x0FFF);
  state.cpu.pc = &state.memory[address] + reg_value;
  // state.cpu.pc = address + reg_value;
  return 0;
}

// CXNN
int Random::execute(State &state, const uint16_t &opcode) {
  auto &registers = state.cpu.variable_register;
  const uint8_t reg = (opcode & 0xF00) >> 8;
  const uint8_t value = (opcode & 0x0FF);
  srand(std::time(0));
  const auto random = std::rand() & value;
  auto &reg_value = registers[reg];
  reg_value = random & value;
  return 0;
}

// DXYN
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
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const auto reg_value = registers[reg];
  state.cpu.index_register += reg_value;
  return 0;
}

// FX0A
int GetKey::execute(State &state, const uint16_t &opcode) { return 0; }

// FX29
int FontCharacter::execute(State &state, const uint16_t &opcode) {
  const auto &registers = state.cpu.variable_register;
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const uint8_t reg_value = registers[reg]; // & 0xF0 >> 4;
  // state.cpu.index_register = state.memory[0x200 + reg_value];
  state.cpu.index_register = 0x200 + (5 * reg_value);
  return 0;
}

// FX33
int BinaryDecimal::execute(State &state, const uint16_t &opcode) {
  const auto &registers = state.cpu.variable_register;
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const auto reg_value = registers[reg];
  const auto address = state.cpu.index_register;
  auto memory = &state.memory[address];
  uint8_t a = reg_value / 100;
  uint8_t b = (reg_value % 100) / 10;
  uint8_t c = reg_value % 10;
  memory[0] = a;
  memory[1] = b;
  memory[2] = c;
  return 0;
}

// FX55 - Ambiguous instruction!
int StoreMemory::execute(State &state, const uint16_t &opcode) {
  const auto &registers = state.cpu.variable_register;
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const auto address = state.cpu.index_register;
  auto memory = &state.memory[address];

  for (auto i = 0; i < reg; i++) {
    const auto reg_value = registers[i];
    memory[i] = reg_value;
  }

  return 0;
};

// FX65 - Ambiguous instruction!
int LoadMemory::execute(State &state, const uint16_t &opcode) {
  // CHIP48 and SUPER-CHIP
  auto &registers = state.cpu.variable_register;
  const uint8_t reg = (opcode & 0x0F00) >> 8;
  const auto address = state.cpu.index_register;
  const auto memory = &state.memory[address];

  for (auto i = 0; i < reg; i++) {
    const auto value = memory[i];
    registers[i] = value;
  }

  return 0;
};
} // namespace emulator
