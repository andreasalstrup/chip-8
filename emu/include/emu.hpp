#include "../../platform/include/graphics.hpp"

namespace emu
{
    struct State {
        uint32_t pixels[64 * 32];
        graphics::Bitmap bitmap;

        State() : bitmap{pixels} { }
    };

    class Emulator {
        State state;

        public:
        Emulator() = default;
        ~Emulator() = default;
        void display();
        void update(uint32_t* pixels);
    };
}