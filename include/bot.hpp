#pragma once

#include <cstdint>
#include <string>

struct Bot {
    int number = 0;

    std::string bot_id;
    std::string username;
    std::string token;

    std::int64_t owner_id = 0;
    std::int64_t added_by = 0;

    bool enabled = true;
    bool maintenance = false;

    std::int64_t users = 0;
    std::int64_t downloads = 0;
};
