#include "../include/instructions.hpp"

namespace emu
{
    int ClearScreen::execute(Emulator& emulator) {
        for (int i = 0; i <= display_size; i++) {
            emulator.screen.bitmap.setPixel(i, graphics::action::off);
        }

        return 0;
    }
}