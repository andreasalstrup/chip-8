#include "../include/chip8.hpp"
#include "../include/opcode.hpp"
#include <cstdint>
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
  // pc = ROM_OFFSET;
}

void Chip8::display() { this->state.display.bitmap.display(); }

void Chip8::update() {
  // Fetch
  auto &pc = state.cpu.pc;
  const uint16_t opcode_value = (pc[0] << 8) | pc[1];
  // const uint16_t opcode_value = (state.memory[pc] << 8) | state.memory[pc +
  // 1];

  pc += 2;

  // Decode
  auto execute = [&]() {
    std::vector<OpcodeType> opcode_types{
        OpcodeType::high_nibble, OpcodeType::low_nibble,
        OpcodeType::high_and_low_nibble, OpcodeType::high_nibble_and_byte};

    for (auto type : opcode_types) {
      auto type_mask = static_cast<uint16_t>(type);
      auto opcode = static_cast<Opcode>(opcode_value & type_mask);
      std::cout << "mask: " << type_mask << " opcode: " << (int)opcode
                << std::endl;
      auto it = this->instruction.find(opcode);
      if (it != this->instruction.end()) {
        it->second(this->state, opcode_value);
        return true;
      }
    }
    std::cout << "WHAT INSTRUCTION??" << std::endl;
    return false;
  };

  // Execute
  if (!execute())
    return;

  std::clog << "[LOG] Executing instructions " << std::hex << opcode_value
            << std::endl;

  this->state.display.bitmap.update();
}

void Chip8::loadRom(const std::filesystem::path &filepath) {
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
