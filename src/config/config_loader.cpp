#include "../../include/config_loader.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

namespace {

std::string getEnv(const char* name) {
    const char* value = std::getenv(name);

    if (value == nullptr) {
        return "";
    }

    return value;
}

bool checkVariable(
    const char* name,
    const std::string& value
) {
    if (value.empty()) {
        std::cerr << "Missing environment variable: "
                  << name
                  << std::endl;
        return false;
    }

    return true;
}

}

bool ConfigLoader::load(
    const std::string& filePath,
    AppConfig& config
) {
    (void)filePath;

    config.bot_token = getEnv("BOT_TOKEN");
    config.mongo_uri = getEnv("MONGO_URI");
    config.database_name = getEnv("DATABASE_NAME");

    const std::string ownerId = getEnv("OWNER_ID");

    bool valid = true;

    valid &= checkVariable(
        "BOT_TOKEN",
        config.bot_token
    );

    valid &= checkVariable(
        "MONGO_URI",
        config.mongo_uri
    );

    valid &= checkVariable(
        "DATABASE_NAME",
        config.database_name
    );

    valid &= checkVariable(
        "OWNER_ID",
        ownerId
    );

    if (!valid) {
        return false;
    }

    try {
        config.owner_id = std::stoll(ownerId);
    } catch (...) {
        std::cerr << "Invalid OWNER_ID."
                  << std::endl;

        config.owner_id = 0;
        return false;
    }

    if (config.owner_id == 0) {
        std::cerr << "Invalid OWNER_ID."
                  << std::endl;
        return false;
    }

    return true;
}
