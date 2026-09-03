#include "../../include/config_loader.hpp"

#include <fstream>
#include <sstream>

namespace {

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }

    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

std::string extractString(
    const std::string& content,
    const std::string& key
) {
    const std::string search = "\"" + key + "\"";
    const auto keyPos = content.find(search);

    if (keyPos == std::string::npos) {
        return "";
    }

    const auto colon = content.find(':', keyPos + search.size());
    if (colon == std::string::npos) {
        return "";
    }

    const auto firstQuote = content.find('"', colon + 1);
    if (firstQuote == std::string::npos) {
        return "";
    }

    const auto secondQuote = content.find('"', firstQuote + 1);
    if (secondQuote == std::string::npos) {
        return "";
    }

    return content.substr(
        firstQuote + 1,
        secondQuote - firstQuote - 1
    );
}

std::int64_t extractInteger(
    const std::string& content,
    const std::string& key
) {
    const std::string search = "\"" + key + "\"";
    const auto keyPos = content.find(search);

    if (keyPos == std::string::npos) {
        return 0;
    }

    const auto colon = content.find(':', keyPos + search.size());
    if (colon == std::string::npos) {
        return 0;
    }

    auto start = colon + 1;

    while (
        start < content.size() &&
        (content[start] == ' ' ||
         content[start] == '\t' ||
         content[start] == '\r' ||
         content[start] == '\n')
    ) {
        ++start;
    }

    auto end = start;

    while (
        end < content.size() &&
        (content[end] >= '0' && content[end] <= '9')
    ) {
        ++end;
    }

    if (start == end) {
        return 0;
    }

    try {
        return std::stoll(
            trim(content.substr(start, end - start))
        );
    } catch (...) {
        return 0;
    }
}

}

bool ConfigLoader::load(
    const std::string& filePath,
    AppConfig& config
) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    const std::string content = buffer.str();

    config.bot_token = extractString(content, "BOT_TOKEN");
    config.mongo_uri = extractString(content, "MONGO_URI");
    config.database_name = extractString(content, "DATABASE_NAME");
    config.owner_id = extractInteger(content, "OWNER_ID");

    if (config.bot_token.empty() ||
        config.mongo_uri.empty() ||
        config.database_name.empty() ||
        config.owner_id == 0) {
        return false;
    }

    return true;
}
