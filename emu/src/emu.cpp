#include "../include/emu.hpp"

namespace emu
{
    void Emulator::display() {
        this->state.bitmap.display();
    }

    void Emulator::update(uint32_t* pixels) {
        this->state.bitmap.update(pixels);
    }
}