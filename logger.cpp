#include "logger.h"

Logger logger;

void Logger::create(const std::string& verbosity) {
    auto path = std::filesystem::current_path();
    if (std::string(path).find(".app") != std::string::npos) {
        path = path.parent_path().parent_path().parent_path();
    }
    const auto logDirPath = path / "logs";
    if(!std::filesystem::exists(logDirPath)) {
        LOG_ASSERT(std::filesystem::create_directory(logDirPath))
    }
    LOG_ASSERT(std::filesystem::is_directory(logDirPath))

    time_t now = time(nullptr);
    struct tm* time = localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "reversi-%m.%d.%Y-%H.%M", time);

    this->filePath = logDirPath / buf;

    this->levels = { "TRACE", "DEBUG", "INFO", "ERROR", "FATAL" };
    auto it = std::find(this->levels.begin(), this->levels.end(), verbosity);
    if (it == this->levels.end()) {
        logger.error("LOGGER", "invalid verbosity level, defaulting to info");
        this->currentVerbosityIndex = 2;
    } else {
        this->currentVerbosityIndex = it - this->levels.begin(); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }
}

void Logger::write(std::string_view verbosity, std::string_view source, std::string_view message) {
    auto it = std::find(this->levels.begin(), this->levels.end(), verbosity);

    std::stringstream ss;

    time_t now = time(nullptr);
    struct tm* time = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "[%EX]", time);

    ss << buf;
    ss << "[" << verbosity << " - " << source << "] " << message << std::endl;
    if (it != this->levels.end() && it - Logger::levels.begin() >= this->currentVerbosityIndex) {
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
