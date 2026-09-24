#include "../include/config_loader.hpp"
#include "../include/database.hpp"
#include "../include/access_control.hpp"
#include "../include/bot_manager.hpp"
#include "../include/bot_service.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>

namespace {

void runHealthServer() {
    const char* portEnv = std::getenv("PORT");

    int port = 8080;

    if (portEnv != nullptr) {
        try {
            port = std::stoi(portEnv);
        } catch (...) {
            port = 8080;
        }
    }

    std::cout
        << "Health server configured on port "
        << port
        << std::endl;

    // Render service remains alive through the bot process.
    // Telegram polling is handled by BotService.
}

}

int main() {

    std::cout
        << "========================================"
        << std::endl;

    std::cout
        << "     RestrictionMasterCpp"
        << std::endl;

    std::cout
        << "========================================"
        << std::endl;

    AppConfig config;

    if (!ConfigLoader::load("", config)) {
        std::cerr
            << "Configuration loading failed."
            << std::endl;

        return 1;
    }

    std::cout
        << "Configuration loaded."
        << std::endl;

    Database database(
        config.mongo_uri,
        config.database_name
    );

    if (!database.connect()) {
        std::cerr
            << "MongoDB connection failed."
            << std::endl;

        return 1;
    }

    std::cout
        << "MongoDB connection successful."
        << std::endl;

    std::cout
        << "Total users: "
        << database.getTotalUsers()
        << std::endl;

    std::cout
        << "Total downloads: "
        << database.getTotalDownloads()
        << std::endl;

    AccessControl accessControl(
        config.owner_id
    );

    BotManager botManager;

    BotService botService(
        1,
        config.bot_token,
        botManager,
        accessControl
    );

    runHealthServer();

    if (!botService.start()) {
        std::cerr
            << "Failed to start Telegram bot."
            << std::endl;

        return 1;
    }

    std::cout
        << "Telegram bot started successfully."
        << std::endl;

    std::cout
        << "RestrictionMasterCpp is running."
        << std::endl;

    while (botService.isRunning()) {
        std::this_thread::sleep_for(
            std::chrono::seconds(5)
        );
    }

    std::cout
        << "Stopping bot..."
        << std::endl;

    botService.stop();

    std::cout
        << "Bot stopped."
        << std::endl;

    return 0;
}
