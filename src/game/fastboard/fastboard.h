#ifndef REVERSI_FASTBOARD_H
#define REVERSI_FASTBOARD_H

#include <string>
#include <cstdint>
#include "moves.h"
#include "../../state.h"

namespace AIEnv {
    void Init();
}

struct FastBoard {
public:
    uint64_t player{}, opponent{};
    void Play(uint_fast8_t x, uint_fast8_t y);
    void Play(uint_fast8_t place);

    [[nodiscard]] int Eval(bool max) const;
    [[nodiscard]] uint64_t Moves() const {
        return legalMoves(player, opponent);
    }
    [[nodiscard]] uint64_t Hash() const {
        return ::hash(player, opponent);
    }
    [[nodiscard]] bool GameOver() const {
        return Moves() == 0;
    }
    FastBoard Clone() {
        return FastBoard{player, opponent};
    }
    void SwitchTurn() {
        std::swap(player, opponent);
    }
};


#endif //REVERSI_FASTBOARD_H
