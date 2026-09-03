#pragma once

#include <cstdint>
#include <string>

struct User {
    std::int64_t user_id = 0;

    std::string name;
    std::string username;

    std::string phone;
    std::string language = "en";

    std::string caption;
    std::string prefix;
    std::string suffix;
    std::string thumbnail_id;

    std::int64_t downloads = 0;
    std::int64_t daily_downloads = 0;
    std::int64_t referral_count = 0;

    bool is_premium = false;
    bool is_banned = false;
    bool is_admin = false;

    bool notifications = true;
    bool silent_mode = false;
};
