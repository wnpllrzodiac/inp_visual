#pragma once

#include "utils/formatter.h"
#include <iostream>
#include <spdlog/spdlog.h>

namespace Impl {
class Logger {
public:
    static Logger& instance();

    template <typename... Args> void debug(fmt::format_string<Args...> fmt, Args&&... args)
    {
#if defined(DEBUG)
        try {
            spdlog::debug(fmt, std::forward<Args>(args)...);
        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Write Log Exception: " << ex.what() << std::endl;
        }
#endif
    }

    template <typename... Args> void info(fmt::format_string<Args...> fmt, Args&&... args)
    {
        try {
            spdlog::info(fmt, std::forward<Args>(args)...);
        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Write Log Exception: " << ex.what() << std::endl;
        }
    }

    template <typename... Args> void warn(fmt::format_string<Args...> fmt, Args&&... args)
    {
        try {
            spdlog::warn(fmt, std::forward<Args>(args)...);
        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Write Log Exception: " << ex.what() << std::endl;
        }
    }

    template <typename... Args> void error(fmt::format_string<Args...> fmt, Args&&... args)
    {
        try {
            spdlog::error(fmt, std::forward<Args>(args)...);
        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Write Log Exception: " << ex.what() << std::endl;
        }
    }

private:
    Logger();
};
} // namespace Impl

class Logging {
public:
    template <typename... Args> static void debug(fmt::format_string<Args...> fmt, Args&&... args)
    {
        Impl::Logger::instance().debug(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args> static void info(fmt::format_string<Args...> fmt, Args&&... args)
    {
        Impl::Logger::instance().info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args> static void warn(fmt::format_string<Args...> fmt, Args&&... args)
    {
        Impl::Logger::instance().warn(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args> static void error(fmt::format_string<Args...> fmt, Args&&... args)
    {
        Impl::Logger::instance().error(fmt, std::forward<Args>(args)...);
    }
};