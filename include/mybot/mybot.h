// Some common definitions that could be potentially used in multiple places.

#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace mybot
{

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#  define WINDOWS 1
#  define WIN32_LEAN_AND_MEAN
#  define VC_EXTRALEAN
#  include <SDKDDKVer.h> // Silence "Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately".
#  include <windows.h>
#else
#  define WINDOWS 0
#endif

extern std::shared_ptr<spdlog::logger> g_logger;

} // namespace mybot
