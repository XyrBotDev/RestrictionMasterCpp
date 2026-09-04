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

    output->append(
        static_cast<char*>(contents),
        totalSize
    );

    return totalSize;
}

}

TelegramClient::TelegramClient(
    const std::string& bot_token
)
    : bot_token_(bot_token) {}

bool TelegramClient::start() {
    if (bot_token_.empty()) {
        return false;
    }

    std::string response;

    if (!getMe(
            response,
            response
        )) {
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

bool TelegramClient::request(
    const std::string& method,
    const std::string& parameters,
    std::string& response
) const {
    if (bot_token_.empty()) {
        return false;
    }

    CURL* curl = curl_easy_init();

    if (!curl) {
        return false;
    }

    std::string url =
        "https://api.telegram.org/bot" +
        bot_token_ +
        "/" +
        method;

    if (!parameters.empty()) {
        url += "?" + parameters;
    }

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
        30L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_FOLLOWLOCATION,
        1L
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

    botId = response;
    username = response;

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
