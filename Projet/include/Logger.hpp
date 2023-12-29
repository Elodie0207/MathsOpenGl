//
// Created by ianpo on 29/12/2023.
//

#pragma once

#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <memory>
#include <filesystem>
#include "Macros.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


class Log {
public:
    static void Init();
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};




#ifdef REI_DEBUG
//  spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__ )

#define REI_TRACE(...)       ::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, REI_FUNC}, spdlog::level::trace, __VA_ARGS__)
#define REI_INFO(...)        ::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, REI_FUNC}, spdlog::level::info, __VA_ARGS__)
#define REI_WARNING(...)     ::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, REI_FUNC}, spdlog::level::warn, __VA_ARGS__)
#define REI_ERROR(...)       ::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, REI_FUNC}, spdlog::level::err, __VA_ARGS__)
#define REI_CRITICAL(...)    ::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, REI_FUNC}, spdlog::level::critical, __VA_ARGS__)

//
//#define REI_TRACE(...)       ::Log::GetClientLogger()->trace(__VA_ARGS__)
//#define REI_INFO(...)        ::Log::GetClientLogger()->info(__VA_ARGS__)
//#define REI_WARNING(...)     ::Log::GetClientLogger()->warn(__VA_ARGS__)
//#define REI_ERROR(...)       ::Log::GetClientLogger()->error(__VA_ARGS__)
//#define REI_CRITICAL(...)    ::Log::GetClientLogger()->critical(__VA_ARGS__)

#else

#define REI_TRACE(...)
#define REI_INFO(...)
#define REI_WARNING(...)
#define REI_ERROR(...)
#define REI_CRITICAL(...)

#endif
