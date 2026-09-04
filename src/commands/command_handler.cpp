#include "../../include/command_handler.hpp"
#include "../../include/bot_manager.hpp"
#include "../../include/access_control.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

CommandHandler::CommandHandler(
    BotManager& botManager,
    AccessControl& accessControl
)
    : botManager_(botManager),
      accessControl_(accessControl) {}

std::string CommandHandler::handle(
    std::int64_t userId,
    const std::string& command,
    const std::string& arguments
) {
    std::string cmd = command;

    if (!cmd.empty() && cmd[0] == '/') {
        cmd.erase(0, 1);
    }

    std::transform(
        cmd.begin(),
        cmd.end(),
        cmd.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        }
    );

    if (cmd == "start") {
        return handleStart(userId);
    }

    if (cmd == "help") {
        return handleHelp(userId);
    }

    if (cmd == "premium") {
        return handlePremium(userId);
    }

    if (cmd == "settings") {
        return handleSettings(userId);
    }

    if (cmd == "botlist") {
        return handleBotList(userId);
    }

    if (cmd == "botstats") {
        return handleBotStats(userId);
    }

    if (cmd == "botmin") {
        if (!accessControl_.isAdmin(userId)) {
            return "❌ Admin access required.";
        }

        return handleBotMaintenance(userId, arguments);
    }

    if (cmd == "dummy") {
        if (!accessControl_.isAdmin(userId)) {
            return "❌ Admin access required.";
        }

        return handleDummy(userId, arguments);
    }

    if (cmd == "rdummy") {
        if (!accessControl_.isAdmin(userId)) {
            return "❌ Admin access required.";
        }

        return handleRemoveDummy(userId);
    }

    return "❌ Unknown command. Use /help";
}

std::string CommandHandler::handleStart(std::int64_t userId) {
    (void)userId;

    return
        "🤖 **Welcome!**\n\n"
        "📥 Send a Telegram link to download content.\n\n"
        "Use /help to see available commands.";
}

std::string CommandHandler::handleHelp(std::int64_t userId) {
    (void)userId;

    return
        "📖 **HELP MENU**\n\n"
        "🔐 /login — Login\n"
        "🚪 /logout — Logout\n"
        "📊 /mystats — Your statistics\n"
        "ℹ️ /myinfo — Your information\n"
        "📜 /history — Download history\n"
        "⚙️ /settings — Settings\n"
        "💎 /premium — Premium information\n\n"
        "📥 Send a Telegram link to start downloading.";
}

std::string CommandHandler::handlePremium(std::int64_t userId) {
    (void)userId;

    return
        "💎 **PREMIUM BENEFITS**\n\n"
        "✅ Unlimited downloads\n"
        "✅ 2 GB file size\n"
        "✅ Faster download speed\n"
        "✅ Priority support\n"
        "✅ No ads\n\n"
        "👑 Contact the owner for Premium.";
}

std::string CommandHandler::handleSettings(std::int64_t userId) {
    (void)userId;

    return
        "⚙️ **SETTINGS**\n\n"
        "🔔 Notifications\n"
        "🌙 Silent Mode\n"
        "🌐 Language\n"
        "🔄 Reset Settings";
}

std::string CommandHandler::handleBotList(std::int64_t userId) {
    (void)userId;

    const auto bots = botManager_.getAllBots();

    if (bots.empty()) {
        return "🤖 **BOT LIST**\n\nNo bots connected.";
    }

    std::ostringstream out;

    out << "🤖 **CONNECTED BOTS**\n\n";

    for (const auto& bot : bots) {
        out << "🔢 **"
            << bot.number
            << "** — @"
            << (bot.username.empty() ? "unknown" : bot.username)
            << "\n";

        out << "👥 Users: "
            << bot.users
            << "\n";

        out << (bot.enabled ? "🟢 Active" : "🔴 Disabled")
            << "\n\n";
    }

    return out.str();
}

std::string CommandHandler::handleBotStats(std::int64_t userId) {
    (void)userId;

    const auto bots = botManager_.getAllBots();

    if (bots.empty()) {
        return "📊 **BOT STATS**\n\nNo bots connected.";
    }

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
        << "📥 Total Downloads: " << totalDownloads << "\n";

    return out.str();
}

std::string CommandHandler::handleBotMaintenance(
    std::int64_t userId,
    const std::string& arguments
) {
    (void)userId;

    std::istringstream input(arguments);

    int botNumber = 0;
    std::string mode;

    input >> botNumber >> mode;

    if (botNumber <= 0 || mode.empty()) {
        return
            "🔧 **BOT MAINTENANCE**\n\n"
            "Usage:\n"
            "/botmin <bot_number> on\n"
            "/botmin <bot_number> off";
    }

    std::transform(
        mode.begin(),
        mode.end(),
        mode.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        }
    );

    if (mode != "on" && mode != "off") {
        return "❌ Use `on` or `off`.";
    }

    const bool enabled = mode == "on";

    if (!botManager_.setMaintenance(botNumber, enabled)) {
        return "❌ Bot number not found.";
    }

    return enabled
        ? "🔧 Maintenance **ON** for bot #" + std::to_string(botNumber)
        : "✅ Maintenance **OFF** for bot #" + std::to_string(botNumber);
}

std::string CommandHandler::handleDummy(
    std::int64_t userId,
    const std::string& arguments
) {
    (void)userId;

    if (arguments.empty()) {
        return
            "📢 **Dummy Channel Setup**\n\n"
            "Please add me to your authorized channel and then press Refresh.";
    }

    std::string channel = arguments;

    if (channel[0] != '@') {
        channel = "@" + channel;
    }

    if (!botManager_.setDummyChannel(channel)) {
        return "❌ Failed to set dummy channel.";
    }

    return
        "✅ **Dummy Channel Set**\n\n"
        "Channel: " + channel;
}

std::string CommandHandler::handleRemoveDummy(
    std::int64_t userId
) {
    (void)userId;

    if (!botManager_.removeDummyChannel()) {
        return "❌ No dummy channel is configured.";
    }

    return "✅ Dummy channel removed.";
}
