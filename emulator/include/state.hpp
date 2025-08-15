#pragma once

#include "../../platform/include/graphics.hpp"
#include "../include/cpu.hpp"
#include <cstdint>
#include <vector>

namespace emulator {
constexpr int MEMORY = 4096;

class Display {
  uint32_t pixels[platform::DISPLAY_SIZE]{};

public:
  platform::Bitmap bitmap;
  Display() : bitmap{pixels} {}
};

struct State {
  Display display{};
  CPU cpu{};
  uint8_t memory[MEMORY]{};
  std::vector<uint8_t*> call_stack{};
};
} // namespace emulator
