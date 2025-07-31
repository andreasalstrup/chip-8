#include "../include/instructions.hpp"
#include <cassert>
#include <fstream>
#include <ios>
#include <iostream>

#define FONT_OFFSET 0x50

uint8_t font_data[] = {
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

namespace emu {
Emulator::Emulator() {
  memcpy(&this->state.memory[FONT_OFFSET], font_data, sizeof(font_data));
}

void Emulator::display() { this->state.display.bitmap.display(); }

void Emulator::loadRom(std::filesystem::path filepath) {
  std::fstream fout;
  fout.open(filepath, std::ios::in | std::ios::binary);
  if (fout) {
    const auto size = std::filesystem::file_size(filepath);
    char buffer[size];
    fout.read(buffer, size);
    fout.close();
    // auto pc = reinterpret_cast<char>(this->state.cpu.pc);
    memcpy(&this->state.memory[0x200], buffer, size);
    // std::cout.write(reinterpret_cast<char*>(this->state.memory + 0x200),
    // size);

    auto *rom_ptr = reinterpret_cast<char *>(&this->state.memory[0x200]);
    for (size_t i = 0; i < size; ++i) {
      std::cout << std::hex << std::setw(2) << std::setfill('0')
                << (static_cast<int>(static_cast<unsigned char>(rom_ptr[i])))
                << ' ';
    }
    std::cout << std::dec << std::endl; // reset to decimal just in case
                                        // std::cout.write(rom_ptr, size);
    // std::cout.write(reinterpret_cast<char*>(&this->state.memory), size);
    // std::cout.write(buffer, size);
  } else {
    std::cout << "Failed to open ROM" << std::endl;
  }
}

void Emulator::update(uint32_t *pixels) {

  // fetch
  auto &pc = state.cpu.pc;
  const uint16_t opcode = pc;
  pc += 2;

  // decode
  std::cout << "opcode: " << opcode << std::endl;
  switch (opcode & 0x000F) {
  case 0x0:
    std::cout << "Clear screen" << std::endl;
    ClearScreen::execute(this->state);
    break;
  case 0x1:
    std::cout << "Jump " << std::endl;
    Jump::execute(this->state);
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
    std::cout << "SetRegisterVX " << std::endl;
    SetRegisterVX::execute(this->state);
    break;
  case 0x7:
    std::cout << "AddValueToRegisterVX" << std::endl;
    AddValueToRegisterVX::execute(this->state);
    break;
  case 0x8:
    break;
  case 0x9:
    break;
  case 0xA:
    std::cout << "SetIndexRegisterI" << std::endl;
    SetIndexRegisterI::execute(this->state);
    break;
  case 0xB:
    break;
  case 0xC:
    break;
  case 0xD:
    std::cout << "Draw" << std::endl;
    Draw::execute(this->state);
    break;
  case 0xE:
    break;
  case 0xF:
    break;
  default:
    break;
  }

  // execute
  this->state.display.bitmap.update(this->state.display.pixels);
}
} // namespace emu
