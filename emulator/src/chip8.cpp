#include "../include/instructions.hpp"
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>

namespace emulator {
constexpr uint16_t FONT_DATA[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
constexpr int FONT_OFFSET = 0x050;
constexpr int ROM_OFFSET = 0x200;

Chip8::Chip8() {
  auto &memory = this->state.memory;
  auto &pc = this->state.cpu.pc;
  memcpy(&memory[FONT_OFFSET], FONT_DATA, sizeof(FONT_DATA));
  pc = &memory[ROM_OFFSET];
}

void Chip8::display() { this->state.display.bitmap.display(); }

void Chip8::update() {
  // Fetch
  auto &pc = state.cpu.pc;
  const uint16_t opcode = (pc[0] << 8) | pc[1];
  pc += 2;

  // Decode
  switch ((opcode & 0xF000) >> 12) {
  case 0x0:
    std::clog << "[LOG] Clear screen" << std::endl;
    ClearScreen::execute(this->state, opcode);
    break;
  case 0x1:
    std::clog << "[LOG] Jump" << std::endl;
    Jump::execute(this->state, opcode);
    break;
  case 0x2:
    break;
  case 0x3:
    break;
  case 0x4:
    break;
  case 0x5:
    break;
  case 0x6:
    std::clog << "[LOG] SetRegisterVX" << std::endl;
    SetRegisterVX::execute(this->state, opcode);
    break;
  case 0x7:
    std::clog << "[LOG] AddValueToRegisterVX" << std::endl;
    AddValueToRegisterVX::execute(this->state, opcode);
    break;
  case 0x8:
    break;
  case 0x9:
    break;
  case 0xA:
    std::clog << "[LOG] SetIndexRegisterI" << std::endl;
    SetIndexRegisterI::execute(this->state, opcode);
    break;
  case 0xB:
    break;
  case 0xC:
    break;
  case 0xD:
    std::clog << "[LOG] Draw" << std::endl;
    Draw::execute(this->state, opcode);
    break;
  case 0xE:
    break;
  case 0xF:
    break;
  default:
    break;
  }

  // Execute
  this->state.display.bitmap.update();
}

void Chip8::loadRom(const std::filesystem::path& filepath) {
  std::fstream fout;
  fout.open(filepath, std::ios::in | std::ios::binary);

  if (fout) {
    const auto size = std::filesystem::file_size(filepath);
    char buffer[size];
    fout.read(buffer, size);
    fout.close();
    memcpy(&this->state.memory[ROM_OFFSET], buffer, size);

    // Log rom data
    auto *rom_ptr = reinterpret_cast<char *>(&this->state.memory[ROM_OFFSET]);
    std::clog << "[LOG] Rom: ";
    for (size_t i = 0; i < size; ++i) {
      std::cout << std::hex << std::setw(2) << std::setfill('0')
                << (static_cast<int>(static_cast<unsigned char>(rom_ptr[i])))
                << ' ';
    }
    std::cout << std::dec << std::endl;
  } else {
    std::cerr << "[ERROR] Emulator::loadRom: Failed to open ROM." << std::endl;
  }
}
} // namespace emulator
