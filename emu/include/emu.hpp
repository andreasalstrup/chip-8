#include "../../platform/include/graphics.hpp"

#define MEMORY 4096

namespace emu
{
    constexpr int display_size = 64 * 32;

    class Display {
        uint32_t pixels[display_size];

        public:
        graphics::Bitmap bitmap;
        Display() : bitmap{pixels} { }
    };

    struct Emulator {
        Display screen;
        uint8_t memory[MEMORY];
        // pc
        // register: (memory 4096 bytes)
        // 1. 16-bit index register.
        // 2. 16 8-bit variable registers. 
        // stack
        // delay timer
        // sound timer

        Emulator();
        ~Emulator() = default;
        void display();
        void update(uint32_t* pixels);
    };
}