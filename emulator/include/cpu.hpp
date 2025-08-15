#pragma once

#include <cstdint>

namespace emulator {
struct CPU {
  uint16_t index_register{};
  uint8_t variable_register[16]{};
  uint8_t *pc{};
  // uint8_t sp{};
  // uint16_t stack[16]{};
};
} // namespace emulator
