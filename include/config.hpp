#pragma once

#include <string>

struct Config {
    std::string mongodb_uri;
    std::string database_name;
    std::string main_bot_token;
    long long owner_id = 0;
};
