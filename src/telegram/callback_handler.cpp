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

    if (callbackData == "bot_stats") {
        return commandHandler_.handle(
            userId,
            "/botstats",
            ""
        );
    }

    if (callbackData == "settings_notifications") {
        return
            "🔔 **Notifications**\n\n"
            "Notification settings are ready.";
    }

    if (callbackData == "settings_silent") {
        return
            "🌙 **Silent Mode**\n\n"
            "Silent mode settings are ready.";
    }

    if (callbackData == "settings_language") {
        return
            "🌐 **Language**\n\n"
            "Language settings are ready.";
    }

    if (callbackData == "dummy_refresh") {
        return handleDummyRefresh(userId);
    }

    if (callbackData == "range_stop") {
        return
            "⏹ **Range Download**\n\n"
            "No active range download is running.";
    }

    const std::string botControlPrefix =
        "bot_control:";

    if (
        callbackData.rfind(
            botControlPrefix,
            0
        ) == 0
    ) {
        try {
            const int botNumber =
                std::stoi(
                    callbackData.substr(
                        botControlPrefix.size()
                    )
                );

            return handleBotControl(
                userId,
                botNumber
            );
        } catch (...) {
            return "❌ Invalid bot number.";
        }
    }

    const std::string maintenanceOnPrefix =
        "maintenance_on:";

    if (
        callbackData.rfind(
            maintenanceOnPrefix,
            0
        ) == 0
    ) {
        try {
            const int botNumber =
                std::stoi(
                    callbackData.substr(
                        maintenanceOnPrefix.size()
                    )
                );

            if (
                !botManager_.setMaintenance(
                    botNumber,
                    true
                )
            ) {
                return "❌ Bot number not found.";
            }

            return
                "🔧 Maintenance **ON** for bot #" +
                std::to_string(botNumber);

        } catch (...) {
            return "❌ Invalid bot number.";
        }
    }

    const std::string maintenanceOffPrefix =
        "maintenance_off:";

    if (
        callbackData.rfind(
            maintenanceOffPrefix,
            0
        ) == 0
    ) {
        try {
            const int botNumber =
                std::stoi(
                    callbackData.substr(
                        maintenanceOffPrefix.size()
                    )
                );

            if (
                !botManager_.setMaintenance(
                    botNumber,
                    false
                )
            ) {
                return "❌ Bot number not found.";
            }

            return
                "✅ Maintenance **OFF** for bot #" +
                std::to_string(botNumber);

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
    (void)userId;

    const auto bot =
        botManager_.getBot(botNumber);

    if (!bot.has_value()) {
        return
            "❌ Bot #" +
            std::to_string(botNumber) +
            " not found.";
    }

    std::ostringstream out;

    out
        << "🤖 **BOT #"
        << bot->number
        << "**\n\n"

        << "Username: @"
        << (
            bot->username.empty()
                ? "unknown"
                : bot->username
        )
        << "\n"

        << "👥 Users: "
        << bot->users
        << "\n"

        << "📥 Downloads: "
        << bot->downloads
        << "\n"

        << (
            bot->maintenance
                ? "🔧 Maintenance: ON"
                : "🟢 Maintenance: OFF"
        );

    return out.str();
}

std::string CallbackHandler::handleDummyRefresh(
    std::int64_t userId
) {
    (void)userId;

    return
        "🔄 **Dummy Channel Refresh**\n\n"
        "Membership verification is not connected yet. "
        "The Telegram channel membership check will be "
        "added in the next step.";
}
