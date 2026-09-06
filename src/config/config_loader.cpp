#include "../../include/config_loader.hpp"

#include <cstdlib>
#include <string>

namespace {

std::string getEnv(const char* name) {
    const char* value = std::getenv(name);

    if (value == nullptr) {
        return "";
    }

    return value;
}

}

bool ConfigLoader::load(
    const std::string& filePath,
    AppConfig& config
) {
    // filePath is kept for compatibility with the existing interface.
    (void)filePath;

    config.bot_token = getEnv("BOT_TOKEN");
    config.mongo_uri = getEnv("MONGO_URI");
    config.database_name = getEnv("DATABASE_NAME");

    const std::string ownerId = getEnv("OWNER_ID");

    if (config.bot_token.empty() ||
        config.mongo_uri.empty() ||
        config.database_name.empty() ||
        ownerId.empty()) {
        return false;
    }

    try {
        config.owner_id = std::stoll(ownerId);
    } catch (...) {
        config.owner_id = 0;
        return false;
    }

    return config.owner_id != 0;
}
