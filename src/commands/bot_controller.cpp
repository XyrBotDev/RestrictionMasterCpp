#include "../../include/bot_controller.hpp"

#include <sstream>

BotController::BotController(BotManager& botManager)
    : botManager_(botManager) {}

bool BotController::isOwner(std::int64_t userId) const {
    // Owner verification will be connected to AppConfig.
    return userId > 0;
}

int BotController::getNextBotNumber() const {
    const auto bots = botManager_.getAllBots();

    bool used[100] = {};

    for (const auto& bot : bots) {
        if (bot.number >= 1 && bot.number <= 99) {
            used[bot.number] = true;
        }
    }

    for (int i = 1; i <= 99; ++i) {
        if (!used[i]) {
            return i;
        }
    }

    return 0;
}

std::string BotController::addBot(
    std::int64_t requesterId,
    const std::string& token,
    std::int64_t ownerId
) {
    if (!isOwner(requesterId)) {
        return "❌ You are not authorized.";
    }

    if (token.empty() || ownerId <= 0) {
        return "❌ Invalid token or owner ID.";
    }

    const int number = getNextBotNumber();

    if (number == 0) {
        return "❌ Maximum bot limit reached.";
    }

    Bot bot;
    bot.number = number;
    bot.token = token;
    bot.owner_id = ownerId;
    bot.added_by = requesterId;
    bot.enabled = true;
    bot.maintenance = false;

    if (!botManager_.addBot(bot)) {
        return "❌ Failed to add bot.";
    }

    return "✅ Bot added successfully.\n\n"
           "🔢 Bot Number: " + std::to_string(number) + "\n"
           "👤 Owner ID: " + std::to_string(ownerId);
}

std::string BotController::removeBot(
    std::int64_t requesterId,
    int botNumber
) {
    if (!isOwner(requesterId)) {
        return "❌ You are not authorized.";
    }

    if (!botManager_.removeBot(botNumber)) {
        return "❌ Bot not found.";
    }

    return "✅ Bot #" + std::to_string(botNumber) +
           " disconnected successfully.";
}

std::string BotController::botList(
    std::int64_t requesterId
) {
    if (!isOwner(requesterId)) {
        return "❌ You are not authorized.";
    }

    const auto bots = botManager_.getAllBots();

    if (bots.empty()) {
        return "🤖 No bots connected.";
    }

    std::ostringstream out;
    out << "🤖 **CONNECTED BOTS**\n\n";

    for (const auto& bot : bots) {
        out << "🔢 #" << bot.number
            << " — @" 
            << (bot.username.empty() ? "unknown" : bot.username)
            << "\n"
            << "👤 Owner: " << bot.owner_id
            << "\n"
            << "👥 Users: " << bot.users
            << "\n"
            << (bot.maintenance
                    ? "🔧 Maintenance: ON"
                    : "🟢 Active")
            << "\n\n";
    }

    return out.str();
}

std::string BotController::botStats(
    std::int64_t requesterId
) {
    if (!isOwner(requesterId)) {
        return "❌ You are not authorized.";
    }

    const auto bots = botManager_.getAllBots();

    std::int64_t totalUsers = 0;
    std::int64_t totalDownloads = 0;

    for (const auto& bot : bots) {
        totalUsers += bot.users;
        totalDownloads += bot.downloads;
    }

    std::ostringstream out;

    out << "📊 **BOT STATISTICS**\n\n"
        << "🤖 Connected Bots: " << bots.size() << "\n"
        << "👥 Total Users: " << totalUsers << "\n"
        << "📥 Total Downloads: " << totalDownloads;

    return out.str();
}

std::string BotController::botMaintenance(
    std::int64_t requesterId,
    int botNumber,
    bool enabled
) {
    if (!isOwner(requesterId)) {
        return "❌ You are not authorized.";
    }

    if (!botManager_.setMaintenance(botNumber, enabled)) {
        return "❌ Bot not found.";
    }

    return enabled
        ? "🔧 Bot #" + std::to_string(botNumber) +
          " maintenance mode **ON**."
        : "✅ Bot #" + std::to_string(botNumber) +
          " maintenance mode **OFF**.";
}

std::string BotController::botInfo(
    std::int64_t requesterId,
    int botNumber
) {
    if (!isOwner(requesterId)) {
        return "❌ You are not authorized.";
    }

    const auto bot = botManager_.getBot(botNumber);

    if (!bot.has_value()) {
        return "❌ Bot not found.";
    }

    std::ostringstream out;

    out << "🤖 **BOT #" << bot->number << "**\n\n"
        << "👤 Owner ID: " << bot->owner_id << "\n"
        << "👥 Users: " << bot->users << "\n"
        << "📥 Downloads: " << bot->downloads << "\n"
        << (bot->maintenance
                ? "🔧 Maintenance: ON"
                : "🟢 Maintenance: OFF");

    return out.str();
}
