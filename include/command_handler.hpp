#pragma once

#include <cstdint>
#include <string>

class BotManager;
class AccessControl;

class CommandHandler {
public:
    CommandHandler(
        BotManager& botManager,
        AccessControl& accessControl
    );

    std::string handle(
        std::int64_t userId,
        const std::string& command,
        const std::string& arguments
    );

private:
    BotManager& botManager_;
    AccessControl& accessControl_;

    std::string handleStart(std::int64_t userId);
    std::string handleHelp(std::int64_t userId);
    std::string handlePremium(std::int64_t userId);
    std::string handleSettings(std::int64_t userId);
    std::string handleBotList(std::int64_t userId);
    std::string handleBotStats(std::int64_t userId);

    std::string handleBotMaintenance(
        std::int64_t userId,
        const std::string& arguments
    );

    std::string handleDummy(
        std::int64_t userId,
        const std::string& arguments
    );

    std::string handleRemoveDummy(
        std::int64_t userId
    );
};
