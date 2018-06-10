#pragma once

#include <spdlog/spdlog.h>

namespace roboclaw
{

inline auto get_roboclaw_logger()
{
    auto lg = spdlog::get("roboclaw");
    if (!lg)
    {
        lg = spdlog::stdout_logger_mt("roboclaw");
        lg->warn(
                "Could not get the 'roboclaw' logger. Created a logger with name "
                "'roboclaw'");
    }
    return lg;
}

}  // namespace roboclaw
