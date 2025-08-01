#include "../../platform/include/graphics.hpp"
#include "../include/cpu.hpp"
#include <filesystem>

namespace emulator {
constexpr int MEMORY = 4096;

class Display {
  uint32_t pixels[platform::DISPLAY_SIE];

public:
  platform::Bitmap bitmap;
  Display() : bitmap{pixels} {}
};

struct State {
  Display display{};
  CPU cpu;
  uint8_t memory[MEMORY];
};

struct Chip8 {
  State state;
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
  void loadRom(std::filesystem::path filepath);
};
} // namespace emulator
