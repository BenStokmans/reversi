#include "Shader.h"

char* readFile(const std::filesystem::path& path) {
    auto wd = std::filesystem::current_path();
    if (std::string(path).find(".app") != std::string::npos) {
        wd = path.parent_path().parent_path().parent_path();
    }
    auto filePath = wd / path;

    std::ifstream f(filePath, std::ios::in | std::ios::binary);
    if (!f.good()) {
        logger.error("SHADER", "error reading file at: " + std::string(filePath));
        return nullptr;
    }
    const auto size = std::filesystem::file_size(filePath);

    char* buf = new char[size];
    f.read(buf, size); // NOLINT(cppcoreguidelines-narrowing-conversions)

    return buf;
}

Shader Shader::fromFile(const std::string& shaderPath) {
    return Shader{
        readFile(shaderPath + std::string(".vert")),
        readFile(shaderPath + std::string(".frag"))
    };
}

Shader Shader::fromFile(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
    return Shader{readFile(vertexPath), readFile(fragmentPath)};
}

Shader::Shader(char *vertexSource, char *fragmentSource) {
    LOG_ASSERT(vertexSource != nullptr)
    LOG_ASSERT(fragmentSource != nullptr)

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);

    // compile the shaders
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    
    // log any potential errors
    LOG_SHADER(vertexShader)
    LOG_SHADER(fragmentShader)

    // link the program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->program, 512, nullptr, infoLog);
        logger.error("GUI", std::string("linking Shader program failed: ") + std::string(infoLog));
    }

    // cache the uniform's location
    GLint numUniforms;
    glGetProgramiv(this->program, GL_ACTIVE_UNIFORMS, &numUniforms);
    for (GLint i = 0; i < numUniforms; i++) {
        const GLsizei bufSize = 32; // maximum uniform name length
        GLchar name[bufSize];       // variable name in source
        GLsizei length;             // source name length
        GLint size;                 // size of the uniform
        GLenum type;                // type of variable

        // get the name for this uniform
        glGetActiveUniform(this->program, (GLuint)i, bufSize, &length, &size, &type, name);

        // add the uniform to the cache
        cachedUniforms[name] = i;
    }

    // clean-up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    // free program memory
    glDeleteProgram(this->program);
}

void Shader::use() const {
    glUseProgram(program);
}

void Shader::set(const char* name, bool value) {
    set(name, value ? 1 : 0);
}

void Shader::set(const char* name, int value) {
    use();
    glUniform1i(cachedUniforms[name], value);
}

void Shader::set(const char* name, float value) {
    use();
    glUniform1f(cachedUniforms[name], value);
}

void Shader::set(const char* name, const glm::vec2 &value) {
    use();
    glUniform2fv(cachedUniforms[name], 1, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::vec3 &value) {
    use();
    glUniform3fv(cachedUniforms[name], 1, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::vec4 &value) {
    use();
    glUniform4fv(cachedUniforms[name], 1, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::mat4 &value) {
    use();
    glUniformMatrix4fv(cachedUniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

GLint Shader::getAttributeLocation(const char* name) const {
    return glGetAttribLocation(program, name);
}
