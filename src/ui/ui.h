#ifndef REVERSI_UI_H
#define REVERSI_UI_H

#ifdef __APPLE
#define GL_SILENCE_DEPRECATION
#endif
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "../game/input.h"
#include "../game/ai.h"

#include "game.h"
#include "tabs/game.h"
#include "tabs/theme.h"
#include "tabs/debug.h"
#include "tabs/ai.h"
#include "tabs/online.h"

namespace UI {
    void DrawGame(
            Shader* evalShader, GLuint evalVAO,
            Shader* gridShader, GLuint gridVAO,
            Shader* cellShader, GLuint cellVAO,
            Shader* diskShader, GLuint diskVAO
    );
    void DrawUI();
    void Init();
    void Destroy();
}

#endif //REVERSI_UI_H
