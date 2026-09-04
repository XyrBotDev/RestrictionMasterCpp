#include <iostream>

#include "../include/config_loader.hpp"
#include "../include/access_control.hpp"
#include "../include/bot_manager.hpp"
#include "../include/bot_service.hpp"

int main() {
    AppConfig config;

    if (!ConfigLoader::load("config.json", config)) {
        std::cerr << "Failed to load configuration." << std::endl;
        return 1;
    }

    AccessControl accessControl(config.owner_id);
    BotManager botManager;

    Bot mainBot;
    mainBot.number = 1;
    mainBot.bot_id = "main";
    mainBot.token = config.bot_token;
    mainBot.owner_id = config.owner_id;
    mainBot.added_by = config.owner_id;

    if (!botManager.addBot(mainBot)) {
        std::cerr << "Failed to register main bot." << std::endl;
        return 1;
    }

    BotService botService(
        mainBot.number,
        mainBot.token,
        botManager,
        accessControl
    );

    if (!botService.start()) {
        std::cerr << "Failed to start bot service." << std::endl;
        return 1;
    }

    std::cout
        << "RestrictionMasterCpp started successfully!"
        << std::endl;

    std::cout
        << "Main bot service is running."
        << std::endl;

    return 0;
}
