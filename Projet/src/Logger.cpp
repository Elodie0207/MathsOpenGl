//
// Created by ianpo on 29/12/2023.
//

#include "Logger.hpp"

std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init()
{
    // Setting Pattern
    spdlog::set_pattern("%^[%T] [%l] %n (%s:%#->%!): %v%$");

    // Creating Client logger
    s_ClientLogger = spdlog::stdout_color_mt("REI");
    s_ClientLogger->set_level(spdlog::level::trace);
}
