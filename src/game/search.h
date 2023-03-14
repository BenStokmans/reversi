#ifndef REVERSI_SEARCH_H
#define REVERSI_SEARCH_H

#include "../state.h"
#include "ai.h"
#include <memory>
#include <string>
#include <stdexcept>
#include <thread>
#include <chrono>

namespace Game::AI {
    void StartSearch();
    void StartEvalBarAnimation();
}

#endif //REVERSI_SEARCH_H
