#include "../../include/bot_storage.hpp"

#include <algorithm>
#include <vector>

// Temporary in-memory implementation.
// MongoDB driver integration will replace this storage layer.

class MemoryBotStorage : public BotStorage {
public:
    bool saveBot(const Bot& bot) override {
        if (bot.number <= 0 || bot.number > 99) {
            return false;
        }

        for (auto& existing : bots_) {
            if (existing.number == bot.number) {
                existing = bot;
                return true;
            }
        }

        bots_.push_back(bot);
        return true;
    }

    bool removeBot(int number) override {
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

    std::optional<Bot> getBot(int number) override {
        for (const auto& bot : bots_) {
            if (bot.number == number) {
                return bot;
            }
        }

        return std::nullopt;
    }

    std::vector<Bot> getAllBots() override {
        return bots_;
    }

private:
    std::vector<Bot> bots_;
};
