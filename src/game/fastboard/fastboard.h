#ifndef REVERSI_FASTBOARD_H
#define REVERSI_FASTBOARD_H

#include <string>
#include <cstdint>
#include "moves.h"
#include "../../logger.h"

uint_fast8_t countSetBits(uint64_t n);

namespace AIEnv {
    void Init();
}

struct FastBoard {
public:
    uint64_t player{}, opponent{};
    uint64_t Play(uint_fast8_t x, uint_fast8_t y);
    uint64_t Play(uint_fast8_t place);

    void Reset() {
        player = 0x0000001008000000ULL;
        opponent = 0x0000000810000000ULL;
    }
    [[nodiscard]] int_fast8_t Eval(bool maxPlayer) const;
    [[nodiscard]] uint64_t Moves() const {
        return legalMoves(player, opponent);
    }
    [[nodiscard]] uint64_t Hash() const {
        return ::hash(player, opponent);
    }
    [[nodiscard]] bool GameOver() const {
        return Moves() == 0;
    }
    bool CellIsOpen(uint_fast8_t x, uint_fast8_t y);
    FastBoard Clone() {
        return FastBoard{player, opponent};
    }
    [[nodiscard]] bool GetIsCurrentPlayer(uint_fast8_t x, uint_fast8_t y) const;
    [[nodiscard]] bool GetIsOpponentPlayer(uint_fast8_t x, uint_fast8_t y) const;
    void SwitchTurn() {
        std::swap(player, opponent);
    }

    [[nodiscard]] int PlayerDisks() const;

    [[nodiscard]] int OpponentDisks() const;
};


#endif //REVERSI_FASTBOARD_H
