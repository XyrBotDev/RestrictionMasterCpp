#include "../../include/bot_manager.hpp"

#include <algorithm>
#include <string>

bool BotManager::addBot(const Bot& bot) {
    if (bot.number < 1 || bot.number > 99) {
        return false;
    }

    for (const auto& existing : bots_) {
        if (existing.number == bot.number) {
            return false;
        }

        if (!bot.bot_id.empty() && existing.bot_id == bot.bot_id) {
            return false;
        }
    }

    bots_.push_back(bot);

    std::sort(
        bots_.begin(),
        bots_.end(),
        [](const Bot& a, const Bot& b) {
            return a.number < b.number;
        }
    );

    return true;
}

bool BotManager::removeBot(int number) {
    const auto oldSize = bots_.size();

    bots_.erase(
        std::remove_if(
            bots_.begin(),
            bots_.end(),
            [number](const Bot& bot) {
                return bot.number == number;
            }
        ),
        bots_.end()
    );

    return bots_.size() != oldSize;
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

bool BotManager::setDummyChannel(const std::string& channel) {
    if (channel.empty()) {
        return false;
    }

    dummyChannel_ = channel;
    return true;
}

bool BotManager::removeDummyChannel() {
    if (dummyChannel_.empty()) {
        return false;
    }

    dummyChannel_.clear();
    return true;
}

std::string BotManager::getDummyChannel() const {
    return dummyChannel_;
}

bool BotManager::hasDummyChannel() const {
    return !dummyChannel_.empty();
}
