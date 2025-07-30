#include "emu.hpp"

namespace emu
{
    struct Instruction {
        virtual ~Instruction() = default;
        virtual int execute(State& state) = 0;
    };

    // 00E0
    struct ClearScreen final : public Instruction {
        int execute(State& state) override;
    };

    // 1NNN
    struct Jump final : public Instruction {
        int execute(State& state) override;
    };

    // 6XNN
    struct SetRegisterVX final : public Instruction {
        int execute(State& state) override;
    };

    // 7XNN
    struct AddValueToRegisterVX final : public Instruction {
        int execute(State& state) override;
    };

    // ANNN
    struct SetIndexRegisterI final : public Instruction {
        int execute(State& state) override;
    };

    // DXYN
    struct Draw final : public Instruction {
        int execute(State& state) override;
    };
}
