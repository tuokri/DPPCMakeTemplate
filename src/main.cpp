#include <cstdlib>
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <string>

#include <dpp/dpp.h>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "mybot/mybot.h"

std::shared_ptr<spdlog::logger> mybot::g_logger;

namespace
{

std::string get_env_var(const std::string_view key)
{
#if WINDOWS
    char* value;
    std::size_t len;
    const errno_t err = _dupenv_s(&value, &len, key.data());
    if (err)
    {
        throw std::runtime_error(std::format("_dupenv_s error: {}", err));
    }
    if (!value)
    {
        free(value);
        throw std::runtime_error(std::format("unable to get env var: {}", key));
    }
    const auto ret = std::string{value};
    free(value);
    return ret;
#else
    const char* value = std::getenv(key.data());
    if (!value)
    {
        throw std::runtime_error(std::format("unable to get env var: {}", key));
    }
    return std::string{value};
#endif // WINDOWS
}

#ifndef NDEBUG

bool debugger_present()
{
#if WINDOWS
    return IsDebuggerPresent();
#else
    // TODO: Linux version?
    return false;
#endif // WINDOWS
}

#else

constexpr bool debugger_present()
{
    return false;
}

#endif // NDEBUG

#define FLUSH_SHUTDOWN_LOGGER() \
if (mybot::g_logger)            \
{                               \
    mybot::g_logger->flush();   \
    spdlog::shutdown();         \
}                               \

// Helper macro to make debugging easier when debugger is attached.
#define THROW_IF_DEBUGGING()    \
FLUSH_SHUTDOWN_LOGGER();        \
if (debugger_present())         \
{                               \
    throw;                      \
}                               \

} // namespace

// Example bot program with some boilerplate that I personally
// like to use in my projects. Remove or modify it based on your needs.
int main()
{
    using namespace mybot;

    int rc = EXIT_SUCCESS;

    // Could also be a raw pointer, whatever you prefer.
    std::shared_ptr<dpp::cluster> bot;

    try
    {
        // An example on setting up spdlog to log to a rotating file and stdout.
        constexpr auto default_log_level = spdlog::level::debug;
        spdlog::init_thread_pool(8192, 2);
        constexpr auto max_log_size = 1024 * 1024 * 10;
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "DPPExampleBot.log", max_log_size, 3);
        const auto sinks = std::vector<spdlog::sink_ptr>{stdout_sink, rotating_sink};
        g_logger = std::make_shared<spdlog::async_logger>(
            "DPPExampleBot", sinks.cbegin(), sinks.cend(), spdlog::thread_pool(),
            spdlog::async_overflow_policy::block);
        spdlog::register_logger(g_logger);
        g_logger->set_level(default_log_level);
        g_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e%z] [%n] [%^%l%$] [th#%t]: %v");

        // Make sure to set DISCORD_BOT_TOKEN in your environment variables.
        // Alternatively, hard-code the token here (NEVER COMMIT A HARD-CODED TOKEN!).
        // const auto bot_token = "dangerous_hard_coded_token_goes_here";
        const auto bot_token = get_env_var("DISCORD_BOT_TOKEN");
        if (bot_token.empty())
        {
            throw std::runtime_error("invalid bot token");
        }

        bot = std::make_shared<dpp::cluster>(bot_token);

        // Forward D++ logs to spdlog logger.
        bot->on_log(
            [bot, &rc](const dpp::log_t& event)
            {
                switch (event.severity)
                {
                    case dpp::ll_trace:
                        g_logger->trace("{}", event.message);
                        break;
                    case dpp::ll_debug:
                        g_logger->debug("{}", event.message);
                        break;
                    case dpp::ll_info:
                        g_logger->info("{}", event.message);
                        break;
                    case dpp::ll_warning:
                        g_logger->warn("{}", event.message);
                        break;
                    case dpp::ll_error:
                        g_logger->error("{}", event.message);
                        break;
                    case dpp::ll_critical:
                    default:
                        g_logger->critical("{}", event.message);
                        // NOTE: Assuming if we get here, the program is in
                        // unrecoverable state anyway, so just bail.
                        bot->shutdown();
                        rc = EXIT_FAILURE;
                        break;
                }
            });

        // Example stub coroutine slashcommand handler.
        bot->on_slashcommand(
            [](const dpp::slashcommand_t& event) -> dpp::task<void>
            {
                const auto cmd_name = event.command.get_command_name();
                g_logger->info("got slash command: {}", cmd_name);

                // TODO: build your bot and do something neat here.
                if (cmd_name == "ping")
                {
                    co_await event.co_reply("pong");
                }

                co_return;
            });

        bot->on_ready(
            [bot](const dpp::ready_t& event) -> dpp::task<void>
            {
                if (dpp::run_once<struct register_bot_commands>())
                {
                    dpp::slashcommand ping_cmd{"ping", "Pong!", bot->me.id};

                    bot->global_bulk_command_create({ping_cmd});
                }

                g_logger->info("bot is ready and initialized");

                co_return;
            });

        bot->start(dpp::st_wait);

        g_logger->info("exiting");
    }
    catch (const std::exception& ex)
    {
        if (g_logger)
        {
            g_logger->error("unhandled exception: {}", ex.what());
        }
        if (bot)
        {
            bot->shutdown();
        }
        THROW_IF_DEBUGGING();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        if (g_logger)
        {
            g_logger->error("unhandled error");
        }
        if (bot)
        {
            bot->shutdown();
        }
        THROW_IF_DEBUGGING();
        return EXIT_FAILURE;
    }

    FLUSH_SHUTDOWN_LOGGER();
    return rc;
}
