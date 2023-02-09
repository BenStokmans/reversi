#ifndef REVERSI_SHADER_H
#define REVERSI_SHADER_H

#include "../logger.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../state.h"


class Shader {
public:
    Shader() {}
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();

    void use() const;
    void destroy() const;

    void set(const char* name, int value);
    void set(const char* name, bool value);
    void set(const char* name, float value);
    void set(const char* name, const glm::vec2& value);
    void set(const char* name, const glm::vec3& value);
    void set(const char* name, const glm::vec4& value);
    void set(const char* name, const glm::mat4& value);
    void set(const char* name, const Color3& value);
    void set(const char* name, const Color4& value);

    GLint getAttributeLocation(const char* name) const;
private:
    GLuint program;
    std::unordered_map<std::string, GLint> cachedUniforms;
};


#endif //REVERSI_SHADER_H
