#pragma once

#include <cstdint>
#include <string>

class CommandHandler;
class BotManager;

class CallbackHandler {
public:
    CallbackHandler(
        CommandHandler& commandHandler,
        BotManager& botManager
    );

    std::string handle(
        std::int64_t userId,
        const std::string& callbackData
    );

private:
    CommandHandler& commandHandler_;
    BotManager& botManager_;

    std::string handleMainMenu(std::int64_t userId);
    std::string handlePremium(std::int64_t userId);
    std::string handleSettings(std::int64_t userId);
    std::string handleBotList(std::int64_t userId);
    std::string handleBotControl(
        std::int64_t userId,
        int botNumber
    );

    std::string handleDummyRefresh(std::int64_t userId);
};
