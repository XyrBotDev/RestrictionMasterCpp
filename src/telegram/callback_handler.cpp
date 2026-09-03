#include "../../include/callback_handler.hpp"
#include "../../include/command_handler.hpp"
#include "../../include/bot_manager.hpp"

#include <sstream>
#include <string>

CallbackHandler::CallbackHandler(
    CommandHandler& commandHandler,
    BotManager& botManager
)
    : commandHandler_(commandHandler),
      botManager_(botManager) {}

std::string CallbackHandler::handle(
    std::int64_t userId,
    const std::string& callbackData
) {
    if (callbackData == "main_menu") {
        return handleMainMenu(userId);
    }

    if (callbackData == "premium") {
        return handlePremium(userId);
    }

    if (callbackData == "settings") {
        return handleSettings(userId);
    }

    if (callbackData == "bot_list") {
        return handleBotList(userId);
    }

    const std::string prefix = "bot_control:";

    if (callbackData.rfind(prefix, 0) == 0) {
        try {
            const int botNumber =
                std::stoi(callbackData.substr(prefix.size()));

            return handleBotControl(userId, botNumber);
        } catch (...) {
            return "❌ Invalid bot number.";
        }
    }

    return "❌ Unknown button action.";
}

std::string CallbackHandler::handleMainMenu(
    std::int64_t userId
) {
    return commandHandler_.handle(
        userId,
        "/start",
        ""
    );
}

std::string CallbackHandler::handlePremium(
    std::int64_t userId
) {
    return commandHandler_.handle(
        userId,
        "/premium",
        ""
    );
}

std::string CallbackHandler::handleSettings(
    std::int64_t userId
) {
    return commandHandler_.handle(
        userId,
        "/settings",
        ""
    );
}

std::string CallbackHandler::handleBotList(
    std::int64_t userId
) {
    return commandHandler_.handle(
        userId,
        "/botlist",
        ""
    );
}

std::string CallbackHandler::handleBotControl(
    std::int64_t userId,
    int botNumber
) {
    const auto bot = botManager_.getBot(botNumber);

    if (!bot.has_value()) {
        return "❌ Bot #" + std::to_string(botNumber) +
               " not found.";
    }

    std::ostringstream out;

    out << "🤖 **BOT #" << bot->number << "**\n\n"
        << "Username: @"
        << (bot->username.empty() ? "unknown" : bot->username)
        << "\n"
        << "👥 Users: " << bot->users << "\n"
        << "📥 Downloads: " << bot->downloads << "\n"
        << (bot->maintenance
                ? "🔧 Maintenance: ON"
                : "🟢 Maintenance: OFF");

    return out.str();
}
