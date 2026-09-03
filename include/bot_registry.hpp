#pragma once

#include "bot.hpp"
#include "bot_storage.hpp"

#include <optional>
#include <vector>

class BotRegistry {
public:
    explicit BotRegistry(BotStorage& storage);

    bool add(const Bot& bot);
    bool remove(int number);

    std::optional<Bot> get(int number);
    std::vector<Bot> getAll();

    bool setMaintenance(int number, bool enabled);

private:
    BotStorage& storage_;
};
