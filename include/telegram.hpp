#pragma once

#include <string>

class TelegramClient {
public:
    explicit TelegramClient(const std::string& bot_token);

    bool start();
    bool stop();
    bool isRunning() const;

private:
    std::string bot_token_;
    bool running_ = false;
};
