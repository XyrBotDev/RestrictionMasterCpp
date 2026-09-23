#include "../../include/database.hpp"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/make_document.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/index.hpp>
#include <mongocxx/options/replace.hpp>
#include <mongocxx/uri.hpp>

#include <iostream>

using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

Database::Database(
    const std::string& mongoUri,
    const std::string& databaseName
)
    : mongoUri_(mongoUri),
      databaseName_(databaseName) {
}

Database::~Database() = default;

bool Database::connect() {
    try {
        if (
            mongoUri_.empty() ||
            databaseName_.empty()
        ) {
            connected_ = false;
            return false;
        }

        instance_ =
            std::make_unique<mongocxx::instance>();

        mongocxx::uri uri(mongoUri_);

        client_ =
            std::make_unique<mongocxx::client>(uri);

        auto db =
            (*client_)[databaseName_];

        db.run_command(
            make_document(
                kvp("ping", 1)
            )
        );

        auto users =
            db["users"];

        try {
            mongocxx::options::index options;

            options.unique(true);

            users.create_index(
                make_document(
                    kvp("user_id", 1)
                ),
                options
            );
        } catch (...) {
            // Index may already exist.
        }

        connected_ = true;

        std::cout
            << "MongoDB connected successfully."
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

bool Database::saveUser(
    const User& user
) {
    if (
        !connected_ ||
        !client_ ||
        user.user_id == 0
    ) {
        return false;
    }

    try {
        auto db =
            (*client_)[databaseName_];

        auto users =
            db["users"];

        auto existing =
            users.find_one(
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

        auto result =
            users.insert_one(doc.view());

        return static_cast<bool>(result);

    } catch (const std::exception& e) {
        std::cerr
            << "saveUser failed: "
            << e.what()
            << std::endl;

        return false;
    }
}

std::optional<User> Database::getUser(
    std::int64_t userId
) {
    if (
        !connected_ ||
        !client_ ||
        userId == 0
    ) {
        return std::nullopt;
    }

    try {
        auto db =
            (*client_)[databaseName_];

        auto users =
            db["users"];

        auto result =
            users.find_one(
                make_document(
                    kvp("user_id", userId)
                )
            );

        if (!result) {
            return std::nullopt;
        }

        auto view =
            result->view();

        User user;

        if (auto e = view["user_id"])
            user.user_id =
                e.get_int64().value;

        if (auto e = view["name"])
            user.name =
                e.get_string().value.to_string();

        if (auto e = view["username"])
            user.username =
                e.get_string().value.to_string();

        if (auto e = view["phone"])
            user.phone =
                e.get_string().value.to_string();

        if (auto e = view["language"])
            user.language =
                e.get_string().value.to_string();

        if (auto e = view["caption"])
            user.caption =
                e.get_string().value.to_string();

        if (auto e = view["prefix"])
            user.prefix =
                e.get_string().value.to_string();

        if (auto e = view["suffix"])
            user.suffix =
                e.get_string().value.to_string();

        if (auto e = view["thumbnail_id"])
            user.thumbnail_id =
                e.get_string().value.to_string();

        if (auto e = view["downloads"])
            user.downloads =
                e.get_int64().value;

        if (auto e = view["daily_downloads"])
            user.daily_downloads =
                e.get_int64().value;

        if (auto e = view["referral_count"])
            user.referral_count =
                e.get_int64().value;

        if (auto e = view["is_premium"])
            user.is_premium =
                e.get_bool().value;

        if (auto e = view["is_banned"])
            user.is_banned =
                e.get_bool().value;

        if (auto e = view["is_admin"])
            user.is_admin =
                e.get_bool().value;

        if (auto e = view["notifications"])
            user.notifications =
                e.get_bool().value;

        if (auto e = view["silent_mode"])
            user.silent_mode =
                e.get_bool().value;

        return user;

    } catch (const std::exception& e) {
        std::cerr
            << "getUser failed: "
            << e.what()
            << std::endl;

        return std::nullopt;
    }
}

bool Database::updateUser(
    const User& user
) {
    if (
        !connected_ ||
        !client_ ||
        user.user_id == 0
    ) {
        return false;
    }

    try {
        auto db =
            (*client_)[databaseName_];

        auto users =
            db["users"];

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

        auto result =
            users.replace_one(
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

bool Database::deleteUser(
    std::int64_t userId
) {
    if (
        !connected_ ||
        !client_ ||
        userId == 0
    ) {
        return false;
    }

    try {
        auto db =
            (*client_)[databaseName_];

        auto users =
            db["users"];

        auto result =
            users.delete_one(
                make_document(
                    kvp("user_id", userId)
                )
            );

        return result &&
               result->deleted_count() > 0;

    } catch (...) {
        return false;
    }
}

std::vector<User> Database::getAllUsers() {
    std::vector<User> result;

    if (
        !connected_ ||
        !client_
    ) {
        return result;
    }

    try {
        auto db =
            (*client_)[databaseName_];

        auto users =
            db["users"];

        for (auto&& doc : users.find({})) {
            User user;

            if (auto e = doc["user_id"])
                user.user_id =
                    e.get_int64().value;

            if (auto e = doc["name"])
                user.name =
                    e.get_string().value.to_string();

            if (auto e = doc["username"])
                user.username =
                    e.get_string().value.to_string();

            if (auto e = doc["language"])
                user.language =
                    e.get_string().value.to_string();

            if (auto e = doc["downloads"])
                user.downloads =
                    e.get_int64().value;

            if (auto e = doc["daily_downloads"])
                user.daily_downloads =
                    e.get_int64().value;

            if (auto e = doc["referral_count"])
                user.referral_count =
                    e.get_int64().value;

            if (auto e = doc["is_premium"])
                user.is_premium =
                    e.get_bool().value;

            if (auto e = doc["is_banned"])
                user.is_banned =
                    e.get_bool().value;

            if (auto e = doc["is_admin"])
                user.is_admin =
                    e.get_bool().value;

            if (auto e = doc["notifications"])
                user.notifications =
                    e.get_bool().value;

            if (auto e = doc["silent_mode"])
                user.silent_mode =
                    e.get_bool().value;

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
    if (
        !connected_ ||
        !client_
    ) {
        return 0;
    }

    try {
        auto db =
            (*client_)[databaseName_];

        auto users =
            db["users"];

        return static_cast<std::int64_t>(
            users.count_documents({})
        );

    } catch (...) {
        return 0;
    }
}

std::int64_t Database::getTotalDownloads() {
    if (
        !connected_ ||
        !client_
    ) {
        return 0;
    }

    try {
        auto db =
            (*client_)[databaseName_];

        auto users =
            db["users"];

        std::int64_t total = 0;

        for (auto&& doc : users.find({})) {
            if (auto e = doc["downloads"]) {
                total +=
                    e.get_int64().value;
            }
        }

        return total;

    } catch (...) {
        return 0;
    }
}
