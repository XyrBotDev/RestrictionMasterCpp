#pragma once

#include <cstdint>
#include <string>

class TelegramClient {
public:
    explicit TelegramClient(const std::string& botToken);

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

    bool sendMessage(
        std::int64_t chatId,
        const std::string& text
    ) const;

    bool answerCallbackQuery(
        const std::string& callbackQueryId
    ) const;

    bool editMessageText(
        std::int64_t chatId,
        std::int64_t messageId,
        const std::string& text
    ) const;

private:
    std::string botToken_;
    bool running_ = false;

    bool request(
        const std::string& method,
        const std::string& parameters,
        std::string& response
    ) const;

    bool requestPost(
        const std::string& method,
        const std::string& parameters,
        std::string& response
    ) const;

    std::string urlEncode(
        const std::string& value
    ) const;
};
