#pragma once

#include "bot.hpp"
#include <vector>
#include <optional>

class BotManager {
public:
    bool addBot(const Bot& bot);
    bool removeBot(int number);

    std::optional<Bot> getBot(int number) const;
    std::vector<Bot> getAllBots() const;

    bool setMaintenance(int number, bool enabled);

private:
    std::vector<Bot> bots_;
};
