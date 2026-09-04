#pragma once

#include "bot.hpp"

#include <optional>
#include <string>
#include <vector>

class BotManager {
public:
    bool addBot(const Bot& bot);
    bool removeBot(int number);

    std::optional<Bot> getBot(int number) const;
    std::vector<Bot> getAllBots() const;

    bool setMaintenance(int number, bool enabled);

    bool setDummyChannel(const std::string& channel);
    bool removeDummyChannel();
    std::string getDummyChannel() const;
    bool hasDummyChannel() const;

private:
    std::vector<Bot> bots_;
    std::string dummyChannel_;
};
