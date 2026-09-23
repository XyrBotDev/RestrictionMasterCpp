#include "../../include/bot_service.hpp"
#include "../../include/access_control.hpp"
#include "../../include/keyboards.hpp"

#include <chrono>
#include <cctype>
#include <iostream>
#include <thread>

namespace {

std::string trim(
    const std::string& value
) {
    std::size_t start = 0;

    while (
        start < value.size() &&
        std::isspace(
            static_cast<unsigned char>(
                value[start]
            )
        )
    ) {
        ++start;
    }

    std::size_t end = value.size();

    while (
        end > start &&
        std::isspace(
            static_cast<unsigned char>(
                value[end - 1]
            )
        )
    ) {
        --end;
    }

    return value.substr(
        start,
        end - start
    );
}

std::string keyboardForCommand(
    const std::string& command,
    const std::string& arguments
) {
    (void)arguments;

    if (command == "/start") {
        return Keyboards::mainMenu();
    }

    if (command == "/premium") {
        return Keyboards::premiumMenu();
    }

    if (command == "/settings") {
        return Keyboards::settingsMenu();
    }

    if (command == "/botlist") {
        return Keyboards::botList();
    }

    if (command == "/dummy") {
        return Keyboards::dummyRefresh();
    }

    return "";
}

std::string keyboardForCallback(
    const std::string& callbackData
) {
    if (callbackData == "main_menu") {
        return Keyboards::mainMenu();
    }

    if (callbackData == "premium") {
        return Keyboards::premiumMenu();
    }

    if (
        callbackData == "settings" ||
        callbackData == "settings_notifications" ||
        callbackData == "settings_silent" ||
        callbackData == "settings_language"
    ) {
        return Keyboards::settingsMenu();
    }

    if (callbackData == "bot_list") {
        return Keyboards::botList();
    }

    if (
        callbackData.rfind(
            "bot_control:",
            0
        ) == 0
    ) {
        try {
            return Keyboards::maintenanceMenu(
                std::stoi(
                    callbackData.substr(12)
                )
            );
        } catch (...) {
            return "";
        }
    }

    if (
        callbackData.rfind(
            "maintenance_on:",
            0
        ) == 0 ||
        callbackData.rfind(
            "maintenance_off:",
            0
        ) == 0
    ) {
        const std::size_t separator =
            callbackData.find(':');

        if (separator != std::string::npos) {
            try {
                return Keyboards::maintenanceMenu(
                    std::stoi(
                        callbackData.substr(
                            separator + 1
                        )
                    )
                );
            } catch (...) {
                return "";
            }
        }
    }

    if (callbackData == "dummy_refresh") {
        return Keyboards::dummyRefresh();
    }

    return "";
}

}

BotService::BotService(
    std::int64_t botNumber,
    const std::string& token,
    BotManager& botManager,
    AccessControl& accessControl
)
    : botNumber_(botNumber),
      token_(token),
      botManager_(botManager),
      accessControl_(accessControl),
      commandHandler_(
          botManager,
          accessControl
      ),
      callbackHandler_(
          commandHandler_,
          botManager
      ),
      telegramClient_(token) {}

BotService::~BotService() {
    stop();
}

bool BotService::start() {
    if (running_) {
        return true;
    }

    if (token_.empty()) {
        std::cerr
            << "Bot token is empty."
            << std::endl;

        return false;
    }

    if (!telegramClient_.start()) {
        std::cerr
            << "Telegram client failed to start."
            << std::endl;

        return false;
    }

    std::string botId;
    std::string username;

    if (
        !telegramClient_.getMe(
            botId,
            username
        )
    ) {
        std::cerr
            << "Unable to verify Telegram bot."
            << std::endl;

        telegramClient_.stop();

        return false;
    }

    std::cout
        << "Telegram bot verified."
        << std::endl;

    if (!botId.empty()) {
        std::cout
            << "Bot ID: "
            << botId
            << std::endl;
    }

    if (!username.empty()) {
        std::cout
            << "Bot username: @"
            << username
            << std::endl;
    }

    updateOffset_ = 0;
    running_ = true;

    pollingThread_ =
        std::thread(
            &BotService::pollingLoop,
            this
        );

    return true;
}

bool BotService::stop() {
    running_ = false;

    telegramClient_.stop();

    if (pollingThread_.joinable()) {
        pollingThread_.join();
    }

    return true;
}

bool BotService::isRunning() const {
    return running_;
}

std::string BotService::handleCommand(
    std::int64_t userId,
    const std::string& command,
    const std::string& arguments
) {
    if (!running_) {
        return "Bot is not running.";
    }

    return commandHandler_.handle(
        userId,
        command,
        arguments
    );
}

std::string BotService::handleCallback(
    std::int64_t userId,
    const std::string& callbackData
) {
    if (!running_) {
        return "Bot is not running.";
    }

    return callbackHandler_.handle(
        userId,
        callbackData
    );
}

bool BotService::extractInt(
    const std::string& json,
    const std::string& key,
    std::int64_t& value
) const {
    const std::string search =
        "\"" + key + "\":";

    const std::size_t position =
        json.find(search);

    if (position == std::string::npos) {
        return false;
    }

    std::size_t start =
        position + search.size();

    while (
        start < json.size() &&
        std::isspace(
            static_cast<unsigned char>(
                json[start]
            )
        )
    ) {
        ++start;
    }

    bool negative = false;

    if (
        start < json.size() &&
        json[start] == '-'
    ) {
        negative = true;
        ++start;
    }

    const std::size_t numberStart = start;

    while (
        start < json.size() &&
        std::isdigit(
            static_cast<unsigned char>(
                json[start]
            )
        )
    ) {
        ++start;
    }

    if (start == numberStart) {
        return false;
    }

    try {
        value =
            std::stoll(
                json.substr(
                    numberStart,
                    start - numberStart
                )
            );

        if (negative) {
            value = -value;
        }

        return true;

    } catch (...) {
        return false;
    }
}

bool BotService::extractString(
    const std::string& json,
    const std::string& key,
    std::string& value
) const {
    const std::string search =
        "\"" + key + "\":\"";

    const std::size_t position =
        json.find(search);

    if (position == std::string::npos) {
        return false;
    }

    const std::size_t start =
        position + search.size();

    std::string result;
    bool escaped = false;

    for (
        std::size_t i = start;
        i < json.size();
        ++i
    ) {
        const char character = json[i];

        if (escaped) {
            switch (character) {
                case 'n':
                    result += '\n';
                    break;

                case 'r':
                    result += '\r';
                    break;

                case 't':
                    result += '\t';
                    break;

                case '"':
                    result += '"';
                    break;

                case '\\':
                    result += '\\';
                    break;

                default:
                    result += character;
                    break;
            }

            escaped = false;
            continue;
        }

        if (character == '\\') {
            escaped = true;
            continue;
        }

        if (character == '"') {
            value = result;
            return true;
        }

        result += character;
    }

    return false;
}

bool BotService::extractObject(
    const std::string& json,
    const std::string& key,
    std::string& object
) const {
    const std::string search =
        "\"" + key + "\":";

    const std::size_t keyPosition =
        json.find(search);

    if (keyPosition == std::string::npos) {
        return false;
    }

    std::size_t start =
        keyPosition + search.size();

    while (
        start < json.size() &&
        std::isspace(
            static_cast<unsigned char>(
                json[start]
            )
        )
    ) {
        ++start;
    }

    if (
        start >= json.size() ||
        json[start] != '{'
    ) {
        return false;
    }

    int depth = 0;
    bool inString = false;
    bool escaped = false;

    for (
        std::size_t i = start;
        i < json.size();
        ++i
    ) {
        const char character = json[i];

        if (escaped) {
            escaped = false;
            continue;
        }

        if (
            inString &&
            character == '\\'
        ) {
            escaped = true;
            continue;
        }

        if (character == '"') {
            inString = !inString;
            continue;
        }

        if (inString) {
            continue;
        }

        if (character == '{') {
            ++depth;
        } else if (character == '}') {
            --depth;

            if (depth == 0) {
                object =
                    json.substr(
                        start,
                        i - start + 1
                    );

                return true;
            }
        }
    }

    return false;
}

void BotService::processUpdate(
    const std::string& update
) {
    std::int64_t updateId = 0;

    if (
        !extractInt(
            update,
            "update_id",
            updateId
        )
    ) {
        return;
    }

    if (updateId >= updateOffset_) {
        updateOffset_ =
            updateId + 1;
    }

    std::string fullMessage;

    if (
        extractObject(
            update,
            "message",
            fullMessage
        )
    ) {
        std::int64_t userId = 0;
        std::int64_t chatId = 0;

        std::string from;

        if (
            extractObject(
                fullMessage,
                "from",
                from
            )
        ) {
            if (
                !extractInt(
                    from,
                    "id",
                    userId
                )
            ) {
                return;
            }
        } else {
            return;
        }

        std::string chat;

        if (
            extractObject(
                fullMessage,
                "chat",
                chat
            )
        ) {
            if (
                !extractInt(
                    chat,
                    "id",
                    chatId
                )
            ) {
                return;
            }
        } else {
            return;
        }

        std::string text;

        if (
            !extractString(
                fullMessage,
                "text",
                text
            )
        ) {
            return;
        }

        text = trim(text);

        if (text.empty()) {
            return;
        }

        std::string command = text;
        std::string arguments;

        const std::size_t space =
            text.find_first_of(
                " \t\r\n"
            );

        if (space != std::string::npos) {
            command =
                text.substr(
                    0,
                    space
                );

            arguments =
                trim(
                    text.substr(
                        space + 1
                    )
                );
        }

        if (
            !command.empty() &&
            command[0] == '/'
        ) {
            const std::size_t at =
                command.find('@');

            if (at != std::string::npos) {
                command =
                    command.substr(
                        0,
                        at
                    );
            }

            const std::string reply =
                handleCommand(
                    userId,
                    command,
                    arguments
                );

            if (!reply.empty()) {
                const std::string keyboard =
                    keyboardForCommand(
                        command,
                        arguments
                    );

                const bool sent =
                    keyboard.empty()
                        ? telegramClient_.sendMessage(
                            chatId,
                            reply
                        )
                        : telegramClient_.sendMessageWithKeyboard(
                            chatId,
                            reply,
                            keyboard
                        );

                if (!sent) {
                    std::cerr
                        << "Failed to send command response."
                        << std::endl;
                }
            }
        }

        return;
    }

    std::string callback;

    if (
        extractObject(
            update,
            "callback_query",
            callback
        )
    ) {
        std::int64_t userId = 0;

        std::string callbackData;
        std::string callbackId;

        std::string from;

        if (
            extractObject(
                callback,
                "from",
                from
            )
        ) {
            if (
                !extractInt(
                    from,
                    "id",
                    userId
                )
            ) {
                return;
            }
        } else {
            return;
        }

        if (
            !extractString(
                callback,
                "id",
                callbackId
            )
        ) {
            return;
        }

        extractString(
            callback,
            "data",
            callbackData
        );

        telegramClient_.answerCallbackQuery(
            callbackId
        );

        std::string callbackMessage;

        if (
            !extractObject(
                callback,
                "message",
                callbackMessage
            )
        ) {
            return;
        }

        std::int64_t messageId = 0;

        if (
            !extractInt(
                callbackMessage,
                "message_id",
                messageId
            )
        ) {
            extractInt(
                callbackMessage,
                "id",
                messageId
            );
        }

        std::int64_t chatId = 0;

        std::string chat;

        if (
            extractObject(
                callbackMessage,
                "chat",
                chat
            )
        ) {
            extractInt(
                chat,
                "id",
                chatId
            );
        }

        if (callbackData.empty()) {
            return;
        }

        const std::string reply =
            handleCallback(
                userId,
                callbackData
            );

        if (
            !reply.empty() &&
            chatId != 0 &&
            messageId != 0
        ) {
            const std::string keyboard =
                keyboardForCallback(
                    callbackData
                );

            const bool edited =
                keyboard.empty()
                    ? telegramClient_.editMessageText(
                        chatId,
                        messageId,
                        reply
                    )
                    : telegramClient_.editMessageTextWithKeyboard(
                        chatId,
                        messageId,
                        reply,
                        keyboard
                    );

            if (!edited) {
                std::cerr
                    << "Failed to edit callback message."
                    << std::endl;
            }
        }
    }
}

void BotService::pollingLoop() {
    std::cout
        << "Telegram polling started."
        << std::endl;

    while (running_) {
        std::string response;

        if (
            telegramClient_.getUpdates(
                updateOffset_,
                response
            )
        ) {
            if (
                response.find("\"ok\":true") !=
                std::string::npos
            ) {
                std::size_t position = 0;

                while (running_) {
                    const std::size_t updateStart =
                        response.find(
                            "{\"update_id\":",
                            position
                        );

                    if (
                        updateStart ==
                        std::string::npos
                    ) {
                        break;
                    }

                    int depth = 0;
                    bool inString = false;
                    bool escaped = false;

                    std::size_t updateEnd =
                        std::string::npos;

                    for (
                        std::size_t i =
                            updateStart;
                        i < response.size();
                        ++i
                    ) {
                        const char character =
                            response[i];

                        if (escaped) {
                            escaped = false;
                            continue;
                        }

                        if (
                            inString &&
                            character == '\\'
                        ) {
                            escaped = true;
                            continue;
                        }

                        if (character == '"') {
                            inString =
                                !inString;
                            continue;
                        }

                        if (inString) {
                            continue;
                        }

                        if (character == '{') {
                            ++depth;
                        } else if (
                            character == '}'
                        ) {
                            --depth;

                            if (depth == 0) {
                                updateEnd =
                                    i + 1;
                                break;
                            }
                        }
                    }

                    if (
                        updateEnd ==
                        std::string::npos
                    ) {
                        break;
                    }

                    processUpdate(
                        response.substr(
                            updateStart,
                            updateEnd -
                                updateStart
                        )
                    );

                    position = updateEnd;
                }
            }
        } else {
            if (running_) {
                std::cerr
                    << "Telegram polling request failed."
                    << std::endl;

                std::this_thread::sleep_for(
                    std::chrono::seconds(2)
                );
            }
        }
    }

    std::cout
        << "Telegram polling stopped."
        << std::endl;
}
