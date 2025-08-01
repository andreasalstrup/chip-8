#include <cstdint>

namespace emu {
struct CPU {
  uint16_t index_register;
  uint8_t variable_register[16];
  uint8_t* pc;
};
} // namespace emu
