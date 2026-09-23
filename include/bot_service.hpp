#pragma once

#include "bot_manager.hpp"
#include "command_handler.hpp"
#include "callback_handler.hpp"
#include "telegram.hpp"

#include <atomic>
#include <cstdint>
#include <string>
#include <thread>

class AccessControl;

class BotService {
public:
    BotService(
        std::int64_t botNumber,
        const std::string& token,
        BotManager& botManager,
        AccessControl& accessControl
    );

    ~BotService();

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
    void pollingLoop();

    void processUpdate(
        const std::string& update
    );

    bool extractInt(
        const std::string& json,
        const std::string& key,
        std::int64_t& value
    ) const;

    bool extractString(
        const std::string& json,
        const std::string& key,
        std::string& value
    ) const;

    bool extractObject(
        const std::string& json,
        const std::string& key,
        std::string& object
    ) const;

    std::int64_t botNumber_;
    std::string token_;

    std::atomic<bool> running_{false};

    BotManager& botManager_;
    AccessControl& accessControl_;

    CommandHandler commandHandler_;
    CallbackHandler callbackHandler_;

    TelegramClient telegramClient_;

    std::thread pollingThread_;

    std::int64_t updateOffset_ = 0;
};
