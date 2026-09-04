#include "../../include/bot_service.hpp"
#include "../../include/access_control.hpp"

BotService::BotService(
    std::int64_t botNumber,
    const std::string& token,
    BotManager& botManager,
    AccessControl& accessControl
)
    : botNumber_(botNumber),
      token_(token),
      botManager_(botManager),
      accessControl_(accessControl),
      commandHandler_(botManager, accessControl),
      callbackHandler_(commandHandler_, botManager) {}

bool BotService::start() {
    if (token_.empty()) {
        return false;
    }

    running_ = true;
    return true;
}

bool BotService::stop() {
    running_ = false;
    return true;
}

bool BotService::isRunning() const {
    return running_;
}

std::string BotService::handleCommand(
    std::int64_t userId,
    const std::string& command,
    const std::string& arguments
) {
    if (!running_) {
        return "❌ Bot is not running.";
    }

    return commandHandler_.handle(
        userId,
        command,
        arguments
    );
}

std::string BotService::handleCallback(
    std::int64_t userId,
    const std::string& callbackData
) {
    if (!running_) {
        return "❌ Bot is not running.";
    }

    return callbackHandler_.handle(
        userId,
        callbackData
    );
}
