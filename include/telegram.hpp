#pragma once

#include <cstdint>
#include <string>

class TelegramClient {
public:
    explicit TelegramClient(const std::string& bot_token);

    bool start();
    bool stop();
    bool isRunning() const;

    // Telegram Bot API
    bool getMe(
        std::string& botId,
        std::string& username
    ) const;

    bool getUpdates(
        std::int64_t offset,
        std::string& response
    ) const;

private:
    std::string bot_token_;
    bool running_ = false;

    bool request(
        const std::string& method,
        const std::string& parameters,
        std::string& response
    ) const;
};
