#include "../../include/bot_manager.hpp"

#include <algorithm>

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
