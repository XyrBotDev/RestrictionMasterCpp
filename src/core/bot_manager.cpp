#include "../../include/bot_manager.hpp"

bool BotManager::addBot(const Bot& bot) {
    for (const auto& existing : bots_) {
        if (existing.number == bot.number) {
            return false;
        }
    }

    bots_.push_back(bot);
    return true;
}

bool BotManager::removeBot(int number) {
    for (auto it = bots_.begin(); it != bots_.end(); ++it) {
        if (it->number == number) {
            bots_.erase(it);
            return true;
        }
    }

    return false;
}

std::optional<Bot> BotManager::getBot(int number) const {
    for (const auto& bot : bots_) {
        if (bot.number == number) {
            return bot;
        }
    }

    return std::nullopt;
}

std::vector<Bot> BotManager::getAllBots() const {
    return bots_;
}

bool BotManager::setMaintenance(int number, bool enabled) {
    for (auto& bot : bots_) {
        if (bot.number == number) {
            bot.maintenance = enabled;
            return true;
        }
    }

    return false;
}
