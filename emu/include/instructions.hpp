#include "emu.hpp"

namespace emu
{
    struct Instruction {
        virtual ~Instruction() = default;
        virtual int execute(Emulator& emulator) = 0;
    };

    // 00E0
    struct ClearScreen final : public Instruction {
        int execute(Emulator& emulator) override;
    };

    // 1NNN
    struct Jump final : public Instruction {
        int execute(Emulator& emulator) override;
    };

}