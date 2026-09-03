#include "../../include/bot_registry.hpp"

BotRegistry::BotRegistry(BotStorage& storage)
    : storage_(storage) {}

bool BotRegistry::add(const Bot& bot) {
    if (bot.number < 1 || bot.number > 99) {
        return false;
    }

    return storage_.saveBot(bot);
}

bool BotRegistry::remove(int number) {
    if (number < 1 || number > 99) {
        return false;
    }

    return storage_.removeBot(number);
}

std::optional<Bot> BotRegistry::get(int number) {
    if (number < 1 || number > 99) {
        return std::nullopt;
    }

    return storage_.getBot(number);
}

std::vector<Bot> BotRegistry::getAll() {
    return storage_.getAllBots();
}

bool BotRegistry::setMaintenance(
    int number,
    bool enabled
) {
    auto bot = storage_.getBot(number);

    if (!bot.has_value()) {
        return false;
    }

    bot->maintenance = enabled;

    return storage_.saveBot(*bot);
}
