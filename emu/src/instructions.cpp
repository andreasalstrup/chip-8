#include "../include/instructions.hpp"

namespace emu {
        int ClearScreen::execute(State &state) {
                for (int i = 0; i <= display_size; i++) {
                        state.display.bitmap.setPixel(i, graphics::action::off);
                }

                return 0;
        }

        int Jump::execute(State &state) {
                auto &pc = state.cpu.pc;
                auto address = state.memory[pc] & 0x000;
                pc = address;
                return 0;
        }

        int SetRegisterVX::execute(State &state) {
                const auto &pc = state.cpu.pc;
                uint8_t reg = pc & 0xFF;
                uint8_t value = pc >> 8;
                state.cpu.variable_register[reg] = value;
                return 0;
        }

        int AddValueToRegisterVX::execute(State &state) {
                const auto &pc = state.cpu.pc;
                uint8_t reg = pc & 0xFF;
                uint8_t value = pc >> 8;
                state.cpu.variable_register[reg] += value;
                return 0;
        }

        int SetIndexRegisterI::execute(State &state) {
                const auto &pc = state.cpu.pc;
                uint16_t value = pc & 0xFFF;
                state.cpu.index_register[1] = value;
                return 0;
        }

        int Draw::execute(State &state) {
                const auto &pc = state.cpu.pc;
                auto &registers = state.cpu.variable_register;

                uint8_t reg_x = pc & 0x0F0;
                uint8_t reg_y = pc & 0xF00;
                auto value_x = registers[reg_x] % 64;
                auto value_y = registers[reg_y] % 32;
                registers[sizeof(registers) / sizeof(registers[0])] = 0;

                size_t data = pc >> 12;

                auto offset_address = state.cpu.index_register[1];
                auto &memory = state.memory;
                for (int i = 0; i <= data; i++) {
                        uint8_t pixel = memory[offset_address + i];
                        for (int j = 0; i <= pixel; i++) {
                                // Right edge of screen
                                if (value_x >= 63)
                                        break;

                                if (pixel && state.display.bitmap.getPixel(value_x, value_y)) {
                                        state.display.bitmap.setPixel(value_x, value_y, graphics::action::off);
                                        registers[sizeof(registers) / sizeof(registers[0])] = 0;
                                } else if (pixel && !state.display.bitmap.getPixel(value_x, value_y)) {
                                        state.display.bitmap.setPixel(value_x, value_y, graphics::action::on);
                                }

                                value_x += 1;
                        }
                        value_y += 1;
                        if (value_y >= 31) {
                                break;
                        }
                }
                return 0;
        }
}
