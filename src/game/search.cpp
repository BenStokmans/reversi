#include "search.h"


// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if(size_s <= 0) throw std::runtime_error("Error during formatting.");
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf( buf.get(), size, format.c_str(), args...);
    return {buf.get(), buf.get() + size - 1}; // We don't want the '\0' inside
}

void calcBestForNext(int depth, bool* cancel, int* e) {
    bestMoveNext.clear();

    auto moves = gameBoard.Moves();
    for (uint_fast8_t y = 0; y < 8; y++) {
        for (uint_fast8_t x = 0; x < 8; x++) {
            if (*cancel) {
                return;
            }
            uint_fast8_t place = y * 8 + x;
            uint64_t cell = 1ULL << place;
            if ((cell & moves) == 0) continue;

            FastBoard local = gameBoard.Clone();
            local.Play(place);

            Move m = getMoveMinMax(local, depth - 1);
            bestMoveNext.insert({local.Hash(), m});
        }
    }
    *cancel = true;
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    *e = minmax(gameBoard, alpha, beta, depth, true);
}

void searchLoop() {
    int depth = 2;
    auto lastHash = gameBoard.Hash();
    while (quiescenceSearchEnabled) {
        if (lastHash != gameBoard.Hash()) {
            depth = 2;
            lastHash = gameBoard.Hash();
            bestMoveNext.clear();
        }

        if (depth > quiescenceSearchLim) {
            // limit reached sleep thread until next move is made
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        std::string text;
        bool cancel = false;
        int eval = 0;

        std::thread t(calcBestForNext, depth, &cancel, &eval);
        t.detach();

        while(lastHash == gameBoard.Hash() && !cancel) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        if (lastHash != gameBoard.Hash() && !cancel) {
            cancel = true;
            continue;
        }

        if (!quiescenceSearchEnabled) {
            break; // we have this here to make sure the thread is stopped even if the eval calculation takes very long
        }
        uint64_t covered = gameBoard.player | gameBoard.opponent;
        int maxEval =
                countSetBits(covered) * aiDiskMul +
                countSetBits(covered & 0x3C0081818181003CULL) * aiEdgeDiskMul +
                countSetBits(covered & 0x8100000000000081ULL) * aiCornerDiskMul -
                countSetBits(covered & 0x4281000000008142ULL) * aiAdjacentCornerDiskMul;

        if (eval == std::numeric_limits<int>::max() || eval == std::numeric_limits<int>::min()) {
            int d = depth - Game::AI::GetEndGame(depth);
            const char* color = eval == std::numeric_limits<int>::max() ? "white" : "black";
            if (d == 0) {
                text = string_format("Eval: %s has won", color, d, depth);
            } else {
                text = string_format("Eval: %s wins in %d moves (depth: %d)", color, d, depth);
            }
            eval = eval == std::numeric_limits<int>::max() ? -maxEval : maxEval;
        } else {
            eval = Game::AI::GetEval(depth) + aiDiskMul*2;
            text = string_format("Eval: %d (depth: %d)", Game::AI::GetEval(depth) + aiDiskMul*2, depth);
        }
        evalBarValueGoal = (float)eval/(float)maxEval;

        if (!gameStarted) {
            evalBarValueGoal = 0.f;
            text = string_format("Eval: 0 (depth: %d)", depth);
        }
        currentEvalText = text;
        depth++;
    }
    currentEvalText = "Eval: please enable quiescence search for eval";
}

void Game::AI::StartSearch() {
    std::thread t(searchLoop);
    t.detach();
}


void animateLoop() {
    while (enableEvalBar) {
        if (evalBarValue == evalBarValueGoal) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        float diff = evalBarValueGoal - evalBarValue;
        if (diff < 0) diff *= -1;
        if (diff < evalBarAnimationRate) {
            evalBarValue = evalBarValueGoal;
            continue;
        }
        diff = evalBarValueGoal - evalBarValue;
        if (diff < 0) {
            evalBarValue -= evalBarAnimationRate;
        } else {
            evalBarValue += evalBarAnimationRate;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Game::AI::StartEvalBarAnimation() {
    std::thread t(animateLoop);
    t.detach();
}