#pragma once

#include <cstdint>
#include <string>

struct AppConfig {
    std::string bot_token;
    std::string mongo_uri;
    std::string database_name;

    std::int64_t owner_id = 0;
};

class ConfigLoader {
public:
    static bool load(
        const std::string& filePath,
        AppConfig& config
    );
};
