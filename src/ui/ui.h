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

#include "../logger.h"
#include "../game/input.h"
#include "../state.h"
#include "../game/ai.h"

#include "game.h"
#include "tabs/local.h"
#include "tabs/theme.h"
#include "tabs/debug.h"
#include "tabs/ai.h"

namespace UI {
    void DrawGame(
            Shader* gridShader, GLuint gridVAO,
            Shader* cellShader, GLuint cellVAO,
            Shader* diskShader, GLuint diskVAO
    );
    void DrawUI();
    void Init();
    void Destroy();
}

#endif //REVERSI_UI_H
