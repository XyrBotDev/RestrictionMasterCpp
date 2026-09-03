#include "../../include/telegram.hpp"

TelegramClient::TelegramClient(const std::string& bot_token)
    : bot_token_(bot_token) {}

bool TelegramClient::start() {
    if (bot_token_.empty()) {
        return false;
    }

    running_ = true;
    return true;
}

bool TelegramClient::stop() {
    running_ = false;
    return true;
}

bool TelegramClient::isRunning() const {
    return running_;
}
