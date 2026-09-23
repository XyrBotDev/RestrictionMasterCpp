#include "../../include/config_loader.hpp"

#include <cstdlib>
#include <iostream>

namespace {

std::string getEnv(const char* name) {
    const char* value = std::getenv(name);

    if (!value) {
        return {};
    }

    return std::string(value);
}

}

bool ConfigLoader::load(Config& config) {
    config.main_bot_token = getEnv("BOT_TOKEN");

    config.mongodb_uri = getEnv("MONGODB_URI");

    if (config.mongodb_uri.empty()) {
        config.mongodb_uri = getEnv("MONGO_URL");
    }

    config.database_name = getEnv("DATABASE_NAME");

    const std::string ownerId = getEnv("OWNER_ID");

    if (!ownerId.empty()) {
        try {
            config.owner_id = std::stoll(ownerId);
        } catch (...) {
            config.owner_id = 0;
        }
    }

    if (config.main_bot_token.empty()) {
        std::cerr << "BOT_TOKEN is missing." << std::endl;
        return false;
    }

    if (config.mongodb_uri.empty()) {
        std::cerr
            << "MONGODB_URI or MONGO_URL is missing."
            << std::endl;

        return false;
    }

    if (config.database_name.empty()) {
        std::cerr
            << "DATABASE_NAME is missing."
            << std::endl;

        return false;
    }

    return true;
}
