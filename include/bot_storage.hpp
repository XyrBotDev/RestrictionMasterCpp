#pragma once

#include "bot.hpp"

#include <optional>
#include <string>
#include <vector>

class BotStorage {
public:
    virtual ~BotStorage() = default;

    virtual bool saveBot(const Bot& bot) = 0;

    virtual bool removeBot(int number) = 0;

    virtual std::optional<Bot> getBot(int number) = 0;

    virtual std::vector<Bot> getAllBots() = 0;
};
