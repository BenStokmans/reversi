#ifndef REVERSI_MAIN_H
#define REVERSI_MAIN_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "logger.h"
#include "ui/shader.h"
#include "ui/shaders.h"
#include "ui/ui.h"
#include "state.h"
#include "ui/helper.h"
#include "game/reversi.h"
#include "online/client.h"

int main();

// when building with WIN32 in cmake we need to define a WinMain as that will be the entrypoint MSVC is looking for
#ifdef WIN32
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    main();
    return 0;
}
#endif

#endif //REVERSI_MAIN_H
