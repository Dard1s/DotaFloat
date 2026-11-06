#pragma once
#include "../../shared/common.hpp"
#include <string>
#include <format>
#include <iostream>
#include <chrono>

namespace horizon::utils {

enum class LogLevel { Debug, Info, Warn, Error };

class Logger {
public:
    static Logger& instance() { static Logger l; return l; }
    void set_level(LogLevel lvl) { level_ = lvl; }
    template<typename... Args>
    void log(LogLevel lvl, std::string_view fmt, Args&&... args) {
        if (lvl < level_) return;
        auto msg = std::vformat(fmt, std::make_format_args(args...));
        const char* prefix = lvl==LogLevel::Debug?"[DBG]": lvl==LogLevel::Info?"[INF]": lvl==LogLevel::Warn?"[WRN]":"[ERR]";
        std::cout << prefix << " " << msg << "\n";
    }
private:
    LogLevel level_{LogLevel::Info};
};

#define HORZ_LOG_INFO(...) horizon::utils::Logger::instance().log(horizon::utils::LogLevel::Info, __VA_ARGS__)
#define HORZ_LOG_WARN(...) horizon::utils::Logger::instance().log(horizon::utils::LogLevel::Warn, __VA_ARGS__)
#define HORZ_LOG_ERR(...)  horizon::utils::Logger::instance().log(horizon::utils::LogLevel::Error, __VA_ARGS__)

}
