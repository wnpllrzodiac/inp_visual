#include "logging.h"
#include "pathutil.h"
#include <iostream>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Impl {
Logger& Logger::instance()
{
    static Logger ins;
    return ins;
}

Logger::Logger()
{
    try {
        auto logger = std::make_shared<spdlog::logger>("robotic_arm_control");
#if defined(DEBUG)
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        stdout_sink->set_level(spdlog::level::debug);
        logger->sinks().push_back(stdout_sink);
#endif

        auto file_sink = std::make_shared<spdlog::sinks::daily_file_format_sink_mt>(
            PathUtil::getUserFilePath("logs/log-%Y%m%d_%H%M%S.txt", true), 0, 0);
        file_sink->set_level(spdlog::level::info);

        logger->sinks().push_back(file_sink);
        logger->set_level(spdlog::level::trace);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %^[%5l]%$ [%7t] %v");
        logger->flush_on(spdlog::level::err);

        spdlog::flush_every(std::chrono::seconds(3));
        spdlog::set_default_logger(logger);
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Initialized Logger Exception: " << ex.what() << std::endl;
        // PathUtil::saveToFile(
        //     PathUtil::getUserFilePath("logs/spdlog_ex_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss").toStdString() +
        //     ".txt"), ex.what());
    }
}

} // namespace Impl