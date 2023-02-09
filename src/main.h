#ifndef REVERSI_MAIN_H
#define REVERSI_MAIN_H

#include "logger.h"
#include "ui/shader.h"
#include "ui/ui.h"
#include "state.h"
#include "ui/helper.h"
#include "game/reversi.h"

int main();

// when building with WIN32 in cmake we need to define a WinMain as that will be the entrypoint MSVC is looking for
#ifdef WIN32
#include <Windows.h>
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    main();
}
#endif

#endif //REVERSI_MAIN_H
