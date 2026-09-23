#include "../../include/database.hpp"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/make_document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/options/index.hpp>
#include <mongocxx/options/replace.hpp>

#include <iostream>

using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

Database::Database(const std::string& mongoUri,
                   const std::string& databaseName)
    : mongoUri_(mongoUri),
      databaseName_(databaseName) {
}

Database::~Database() = default;

bool Database::connect() {
    try {
        if (mongoUri_.empty() || databaseName_.empty()) {
            connected_ = false;
            return false;
        }

        instance_ = std::make_unique<mongocxx::instance>();

        mongocxx::uri uri{mongoUri_};

        client_ = std::make_unique<mongocxx::client>(uri);

        auto db = (*client_)[databaseName_];

        db.run_command(make_document(
            kvp("ping", 1)
        ));

        auto users = db["users"];

        try {
            mongocxx::options::index indexOptions;
            indexOptions.unique(true);

            users.create_index(
                make_document(
                    kvp("user_id", 1)
                ),
                indexOptions
            );
        } catch (...) {
            // Index may already exist.
        }

        connected_ = true;

        std::cout << "MongoDB connected successfully."
                  << std::endl;

        return true;

    } catch (const std::exception& e) {
        connected_ = false;

        std::cerr
            << "MongoDB connection failed: "
            << e.what()
            << std::endl;

        return false;
    }
}

bool Database::isConnected() const {
    return connected_;
}

bool Database::saveUser(const User& user) {
    if (!connected_ || !client_ || user.user_id == 0) {
        return false;
    }

    try {
        auto db = (*client_)[databaseName_];
        auto users = db["users"];

        auto existing = users.find_one(
            make_document(
                kvp("user_id", user.user_id)
            )
        );

        if (existing) {
            return updateUser(user);
        }

        document doc;

        doc.append(
            kvp("user_id", user.user_id),
            kvp("name", user.name),
            kvp("username", user.username),
            kvp("phone", user.phone),
            kvp("language", user.language),
            kvp("caption", user.caption),
            kvp("prefix", user.prefix),
            kvp("suffix", user.suffix),
            kvp("thumbnail_id", user.thumbnail_id),
            kvp("downloads", user.downloads),
            kvp("daily_downloads", user.daily_downloads),
            kvp("referral_count", user.referral_count),
            kvp("is_premium", user.is_premium),
            kvp("is_banned", user.is_banned),
            kvp("is_admin", user.is_admin),
            kvp("notifications", user.notifications),
            kvp("silent_mode", user.silent_mode)
        );

        auto result = users.insert_one(doc.view());

        return static_cast<bool>(result);

    } catch (const std::exception& e) {
        std::cerr
            << "saveUser failed: "
            << e.what()
            << std::endl;

        return false;
    }
}

std::optional<User> Database::getUser(std::int64_t userId) {
    if (!connected_ || !client_ || userId == 0) {
        return std::nullopt;
    }

    try {
        auto db = (*client_)[databaseName_];
        auto users = db["users"];

        auto result = users.find_one(
            make_document(
                kvp("user_id", userId)
            )
        );

        if (!result) {
            return std::nullopt;
        }

        auto view = result->view();

        User user;

        if (auto element = view["user_id"]) {
            user.user_id = element.get_int64().value;
        }

        if (auto element = view["name"]) {
            user.name = element.get_string().value.to_string();
        }

        if (auto element = view["username"]) {
            user.username = element.get_string().value.to_string();
        }

        if (auto element = view["phone"]) {
            user.phone = element.get_string().value.to_string();
        }

        if (auto element = view["language"]) {
            user.language = element.get_string().value.to_string();
        }

        if (auto element = view["caption"]) {
            user.caption = element.get_string().value.to_string();
        }

        if (auto element = view["prefix"]) {
            user.prefix = element.get_string().value.to_string();
        }

        if (auto element = view["suffix"]) {
            user.suffix = element.get_string().value.to_string();
        }

        if (auto element = view["thumbnail_id"]) {
            user.thumbnail_id =
                element.get_string().value.to_string();
        }

        if (auto element = view["downloads"]) {
            user.downloads =
                element.get_int64().value;
        }

        if (auto element = view["daily_downloads"]) {
            user.daily_downloads =
                element.get_int64().value;
        }

        if (auto element = view["referral_count"]) {
            user.referral_count =
                element.get_int64().value;
        }

        if (auto element = view["is_premium"]) {
            user.is_premium =
                element.get_bool().value;
        }

        if (auto element = view["is_banned"]) {
            user.is_banned =
                element.get_bool().value;
        }

        if (auto element = view["is_admin"]) {
            user.is_admin =
                element.get_bool().value;
        }

        if (auto element = view["notifications"]) {
            user.notifications =
                element.get_bool().value;
        }

        if (auto element = view["silent_mode"]) {
            user.silent_mode =
                element.get_bool().value;
        }

        return user;

    } catch (const std::exception& e) {
        std::cerr
            << "getUser failed: "
            << e.what()
            << std::endl;

        return std::nullopt;
    }
}

bool Database::updateUser(const User& user) {
    if (!connected_ || !client_ || user.user_id == 0) {
        return false;
    }

    try {
        auto db = (*client_)[databaseName_];
        auto users = db["users"];

        document replacement;

        replacement.append(
            kvp("user_id", user.user_id),
            kvp("name", user.name),
            kvp("username", user.username),
            kvp("phone", user.phone),
            kvp("language", user.language),
            kvp("caption", user.caption),
            kvp("prefix", user.prefix),
            kvp("suffix", user.suffix),
            kvp("thumbnail_id", user.thumbnail_id),
            kvp("downloads", user.downloads),
            kvp("daily_downloads", user.daily_downloads),
            kvp("referral_count", user.referral_count),
            kvp("is_premium", user.is_premium),
            kvp("is_banned", user.is_banned),
            kvp("is_admin", user.is_admin),
            kvp("notifications", user.notifications),
            kvp("silent_mode", user.silent_mode)
        );

        mongocxx::options::replace options;
        options.upsert(true);

        auto result = users.replace_one(
            make_document(
                kvp("user_id", user.user_id)
            ),
            replacement.view(),
            options
        );

        return static_cast<bool>(result);

    } catch (const std::exception& e) {
        std::cerr
            << "updateUser failed: "
            << e.what()
            << std::endl;

        return false;
    }
}

bool Database::deleteUser(std::int64_t userId) {
    if (!connected_ || !client_ || userId == 0) {
        return false;
    }

    try {
        auto db = (*client_)[databaseName_];
        auto users = db["users"];

        auto result = users.delete_one(
            make_document(
                kvp("user_id", userId)
            )
        );

        return result && result->deleted_count() > 0;

    } catch (const std::exception& e) {
        std::cerr
            << "deleteUser failed: "
            << e.what()
            << std::endl;

        return false;
    }
}

std::vector<User> Database::getAllUsers() {
    std::vector<User> result;

    if (!connected_ || !client_) {
        return result;
    }

    try {
        auto db = (*client_)[databaseName_];
        auto users = db["users"];

        for (auto&& document : users.find({})) {
            auto view = document;

            User user;

            if (auto e = view["user_id"])
                user.user_id = e.get_int64().value;

            if (auto e = view["name"])
                user.name = e.get_string().value.to_string();

            if (auto e = view["username"])
                user.username = e.get_string().value.to_string();

            if (auto e = view["language"])
                user.language = e.get_string().value.to_string();

            if (auto e = view["downloads"])
                user.downloads = e.get_int64().value;

            if (auto e = view["daily_downloads"])
                user.daily_downloads = e.get_int64().value;

            if (auto e = view["referral_count"])
                user.referral_count = e.get_int64().value;

            if (auto e = view["is_premium"])
                user.is_premium = e.get_bool().value;

            if (auto e = view["is_banned"])
                user.is_banned = e.get_bool().value;

            if (auto e = view["is_admin"])
                user.is_admin = e.get_bool().value;

            if (auto e = view["notifications"])
                user.notifications = e.get_bool().value;

            if (auto e = view["silent_mode"])
                user.silent_mode = e.get_bool().value;

            result.push_back(user);
        }

    } catch (const std::exception& e) {
        std::cerr
            << "getAllUsers failed: "
            << e.what()
            << std::endl;
    }

    return result;
}

std::int64_t Database::getTotalUsers() {
    if (!connected_ || !client_) {
        return 0;
    }

    try {
        auto db = (*client_)[databaseName_];
        auto users = db["users"];

        return static_cast<std::int64_t>(
            users.count_documents({})
        );

    } catch (...) {
        return 0;
    }
}

std::int64_t Database::getTotalDownloads() {
    if (!connected_ || !client_) {
        return 0;
    }

    try {
        auto db = (*client_)[databaseName_];
        auto users = db["users"];

        std::int64_t total = 0;

        for (auto&& document : users.find({})) {
            if (auto e = document["downloads"]) {
                total += e.get_int64().value;
            }
        }

        return total;

    } catch (...) {
        return 0;
    }
}
