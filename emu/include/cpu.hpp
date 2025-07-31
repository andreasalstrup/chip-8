#include <cstdint>

namespace emu {
struct CPU {
  uint16_t index_register[1];
  uint8_t variable_register[16];
  uint16_t pc{0x200};
};
} // namespace emu
