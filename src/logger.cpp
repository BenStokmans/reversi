#include <unistd.h>
#include "logger.h"

Logger logger;

bool stdOutAvailable() {
#ifdef __APPLE__
    return isatty(fileno(stdout));
#elif WIN32
    DWORD tmp;
    return !GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
#endif
}

void Logger::create(const std::string& verbosity) {
    int length = wai_getExecutablePath(nullptr, 0, nullptr);
    char* rawPath = (char*)malloc(length + 1);
    int dirnameLen;
    wai_getExecutablePath(rawPath, length, &dirnameLen);
    rawPath[length] = '\0';

    auto path = std::filesystem::path(rawPath);
#ifdef __APPLE__
    path = path.parent_path().parent_path().parent_path().parent_path();
#endif
    const auto logDirPath = path / "logs";
    if(!std::filesystem::exists(logDirPath)) {
        LOG_ASSERT(std::filesystem::create_directory(logDirPath))
    }
    LOG_ASSERT(std::filesystem::is_directory(logDirPath))

    time_t now = time(nullptr);
    struct tm* time = localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "reversi-%m.%d.%Y-%H.%M.log", time);

    this->filePath = logDirPath / buf;

    this->levels = { "TRACE", "DEBUG", "INFO", "ERROR", "FATAL" };
    auto it = std::find(this->levels.begin(), this->levels.end(), verbosity);
    if (it == this->levels.end()) {
        this->currentVerbosityIndex = 2;
        logger.error("LOGGER", "invalid verbosity level, defaulting to info");
    } else {
        this->currentVerbosityIndex = it - this->levels.begin(); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }
    if (!stdOutAvailable()) {
        this->stdOut = false;
        logger.info("LOGGER", "stdout not available, disabling stdout logging");
    }
    logger.info(std::string("started logger with file: ") + this->filePath.string());
}

void Logger::write(std::string_view verbosity, std::string_view source, std::string_view message) {
    auto it = std::find(this->levels.begin(), this->levels.end(), verbosity);

    time_t now = time(nullptr);
    struct tm* time = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "[%EX]", time);

    std::stringstream ss;
    ss << buf;
    ss << "[" << verbosity << " - " << source << "] " << message << std::endl;
    if (it != this->levels.end() && it - Logger::levels.begin() >= this->currentVerbosityIndex && stdOut) {
        std::cout << ss.str();
    }

    if (this->filePath == "") return;
    std::ofstream file(this->filePath, std::ios_base::app);
    file << ss.str();
    file.close();
}

void Logger::trace(std::string_view source, std::string_view message) {
    write("TRACE", source, message);
}

void Logger::debug(std::string_view source, std::string_view message) {
    write("DEBUG", source, message);
}

void Logger::error(std::string_view source, std::string_view message) {
    write("ERROR", source, message);
}

void Logger::info(std::string_view source, std::string_view message) {
    write("INFO", source, message);
}

void Logger::fatal(std::string_view source, std::string_view message) {
    write("FATAL", source, message);
}

void Logger::trace(std::string_view message) {
    trace("REVERSI", message);
}

void Logger::debug(std::string_view message) {
    debug("REVERSI", message);
}

void Logger::error(std::string_view message) {
    error("REVERSI", message);
}

void Logger::fatal(std::string_view message) {
    fatal("REVERSI", message);
}

void Logger::info(std::string_view message) {
    info("REVERSI", message);
}

void Logger::logShaderInfo(std::string_view source, GLuint shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success) return;

    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::ostringstream ss;
    ss << "'" << source <<  "' Shader compilation failed: " << infoLog;
    error("OPENGL", ss.str());
}