#include "../../include/telegram.hpp"

#include <curl/curl.h>

#include <string>

namespace {

size_t writeCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userData
) {
    const size_t totalSize = size * nmemb;

    auto* output =
        static_cast<std::string*>(userData);

    if (output != nullptr) {
        output->append(
            static_cast<char*>(contents),
            totalSize
        );
    }

    return totalSize;
}

}

TelegramClient::TelegramClient(
    const std::string& botToken
)
    : botToken_(botToken) {}

bool TelegramClient::start() {
    if (botToken_.empty()) {
        return false;
    }

    std::string botId;
    std::string username;

    if (!getMe(botId, username)) {
        return false;
    }

    running_ = true;

    return true;
}

bool TelegramClient::stop() {
    running_ = false;
    return true;
}

bool TelegramClient::isRunning() const {
    return running_;
}

std::string TelegramClient::urlEncode(
    const std::string& value
) const {
    CURL* curl = curl_easy_init();

    if (curl == nullptr) {
        return value;
    }

    char* encoded =
        curl_easy_escape(
            curl,
            value.c_str(),
            static_cast<int>(value.size())
        );

    std::string result;

    if (encoded != nullptr) {
        result = encoded;
        curl_free(encoded);
    } else {
        result = value;
    }

    curl_easy_cleanup(curl);

    return result;
}

bool TelegramClient::request(
    const std::string& method,
    const std::string& parameters,
    std::string& response
) const {
    if (botToken_.empty()) {
        return false;
    }

    CURL* curl = curl_easy_init();

    if (curl == nullptr) {
        return false;
    }

    response.clear();

    const std::string url =
        "https://api.telegram.org/bot" +
        botToken_ +
        "/" +
        method +
        (parameters.empty()
            ? ""
            : "?" + parameters);

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        url.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        writeCallback
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response
    );

    curl_easy_setopt(
        curl,
        CURLOPT_TIMEOUT,
        35L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_CONNECTTIMEOUT,
        10L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_FOLLOWLOCATION,
        1L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_USERAGENT,
        "RestrictionMasterCpp/1.0"
    );

    const CURLcode result =
        curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    return result == CURLE_OK;
}

bool TelegramClient::getMe(
    std::string& botId,
    std::string& username
) const {
    std::string response;

    if (!request(
            "getMe",
            "",
            response
        )) {
        return false;
    }

    if (response.find("\"ok\":true") ==
        std::string::npos) {
        return false;
    }

    botId.clear();
    username.clear();

    const std::string idKey = "\"id\":";
    const std::string usernameKey =
        "\"username\":\"";

    const std::size_t idPosition =
        response.find(idKey);

    if (idPosition != std::string::npos) {
        const std::size_t start =
            idPosition + idKey.size();

        const std::size_t end =
            response.find_first_of(
                ",}",
                start
            );

        if (end != std::string::npos) {
            botId =
                response.substr(
                    start,
                    end - start
                );
        }
    }

    const std::size_t usernamePosition =
        response.find(usernameKey);

    if (usernamePosition != std::string::npos) {
        const std::size_t start =
            usernamePosition +
            usernameKey.size();

        const std::size_t end =
            response.find(
                "\"",
                start
            );

        if (end != std::string::npos) {
            username =
                response.substr(
                    start,
                    end - start
                );
        }
    }

    return true;
}

bool TelegramClient::getUpdates(
    std::int64_t offset,
    std::string& response
) const {
    const std::string parameters =
        "offset=" +
        std::to_string(offset) +
        "&timeout=25";

    return request(
        "getUpdates",
        parameters,
        response
    );
}

bool TelegramClient::sendMessage(
    std::int64_t chatId,
    const std::string& text
) const {
    const std::string parameters =
        "chat_id=" +
        std::to_string(chatId) +
        "&text=" +
        urlEncode(text);

    std::string response;

    return request(
        "sendMessage",
        parameters,
        response
    ) &&
    response.find("\"ok\":true") !=
        std::string::npos;
}

bool TelegramClient::sendMessageWithKeyboard(
    std::int64_t chatId,
    const std::string& text,
    const std::string& replyMarkup
) const {
    const std::string parameters =
        "chat_id=" +
        std::to_string(chatId) +
        "&text=" +
        urlEncode(text) +
        "&reply_markup=" +
        urlEncode(replyMarkup);

    std::string response;

    return request(
        "sendMessage",
        parameters,
        response
    ) &&
    response.find("\"ok\":true") !=
        std::string::npos;
}

bool TelegramClient::answerCallbackQuery(
    const std::string& callbackQueryId
) const {
    const std::string parameters =
        "callback_query_id=" +
        urlEncode(callbackQueryId);

    std::string response;

    return request(
        "answerCallbackQuery",
        parameters,
        response
    ) &&
    response.find("\"ok\":true") !=
        std::string::npos;
}

bool TelegramClient::editMessageText(
    std::int64_t chatId,
    std::int64_t messageId,
    const std::string& text
) const {
    const std::string parameters =
        "chat_id=" +
        std::to_string(chatId) +
        "&message_id=" +
        std::to_string(messageId) +
        "&text=" +
        urlEncode(text);

    std::string response;

    return request(
        "editMessageText",
        parameters,
        response
    ) &&
    response.find("\"ok\":true") !=
        std::string::npos;
}

bool TelegramClient::editMessageTextWithKeyboard(
    std::int64_t chatId,
    std::int64_t messageId,
    const std::string& text,
    const std::string& replyMarkup
) const {
    const std::string parameters =
        "chat_id=" +
        std::to_string(chatId) +
        "&message_id=" +
        std::to_string(messageId) +
        "&text=" +
        urlEncode(text) +
        "&reply_markup=" +
        urlEncode(replyMarkup);

    std::string response;

    return request(
        "editMessageText",
        parameters,
        response
    ) &&
    response.find("\"ok\":true") !=
        std::string::npos;
}
