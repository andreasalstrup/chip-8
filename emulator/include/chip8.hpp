#pragma once

#include "state.hpp"
#include <filesystem>

namespace emulator {
struct Chip8 {
  State state{};
  // Display screen;
  // uint8_t memory[MEMORY];
  // pc
  // register: (memory 4096 bytes)
  // 1. 16-bit index register.
  // 2. 16 8-bit variable registers.
  // stack
  // delay timer
  // sound timer

  Chip8();
  ~Chip8() = default;
  void display();
  void update();
  void loadRom(const std::filesystem::path &filepath);
};
} // namespace emulator
