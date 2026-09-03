#pragma once

#include "../include/bot_manager.hpp"

#include <cstdint>
#include <string>

class BotController {
public:
    explicit BotController(BotManager& botManager);

    std::string addBot(
        std::int64_t requesterId,
        const std::string& token,
        std::int64_t ownerId
    );

    std::string removeBot(
        std::int64_t requesterId,
        int botNumber
    );

    std::string botList(
        std::int64_t requesterId
    );

    std::string botStats(
        std::int64_t requesterId
    );

    std::string botMaintenance(
        std::int64_t requesterId,
        int botNumber,
        bool enabled
    );

    std::string botInfo(
        std::int64_t requesterId,
        int botNumber
    );

private:
    BotManager& botManager_;

    bool isOwner(std::int64_t userId) const;
    int getNextBotNumber() const;
};
