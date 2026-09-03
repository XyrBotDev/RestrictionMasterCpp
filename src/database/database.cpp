#include "../../include/database.hpp"

Database::Database(
    const std::string& mongoUri,
    const std::string& databaseName
)
    : mongoUri_(mongoUri),
      databaseName_(databaseName) {}

bool Database::connect() {
    if (mongoUri_.empty() || databaseName_.empty()) {
        connected_ = false;
        return false;
    }

    // MongoDB driver integration will be added here.
    connected_ = true;
    return true;
}

bool Database::isConnected() const {
    return connected_;
}

bool Database::saveUser(const User& user) {
    if (!connected_ || user.user_id == 0) {
        return false;
    }

    return true;
}

std::optional<User> Database::getUser(std::int64_t userId) {
    if (!connected_ || userId == 0) {
        return std::nullopt;
    }

    return std::nullopt;
}

bool Database::updateUser(const User& user) {
    if (!connected_ || user.user_id == 0) {
        return false;
    }

    return true;
}

bool Database::deleteUser(std::int64_t userId) {
    if (!connected_ || userId == 0) {
        return false;
    }

    return true;
}

std::vector<User> Database::getAllUsers() {
    return {};
}

std::int64_t Database::getTotalUsers() {
    return 0;
}

std::int64_t Database::getTotalDownloads() {
    return 0;
}
