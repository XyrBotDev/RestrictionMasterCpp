#pragma once

#include "../include/bot_manager.hpp"
#include "../include/command_handler.hpp"
#include "../include/callback_handler.hpp"

#include <cstdint>
#include <string>

class BotService {
public:
    BotService(
        std::int64_t botNumber,
        const std::string& token,
        BotManager& botManager
    );

    bool start();
    bool stop();

    bool isRunning() const;

    std::string handleCommand(
        std::int64_t userId,
        const std::string& command,
        const std::string& arguments
    );

    std::string handleCallback(
        std::int64_t userId,
        const std::string& callbackData
    );

private:
    std::int64_t botNumber_;
    std::string token_;

    bool running_ = false;

    BotManager& botManager_;
    CommandHandler commandHandler_;
    CallbackHandler callbackHandler_;
};
