#pragma once

#include "../include/user.hpp"
#include "../include/bot.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class Database {
public:
    Database(
        const std::string& mongoUri,
        const std::string& databaseName
    );

    bool connect();
    bool isConnected() const;

    bool saveUser(const User& user);
    std::optional<User> getUser(std::int64_t userId);

    bool updateUser(const User& user);
    bool deleteUser(std::int64_t userId);

    std::vector<User> getAllUsers();

    std::int64_t getTotalUsers();
    std::int64_t getTotalDownloads();

private:
    std::string mongoUri_;
    std::string databaseName_;
    bool connected_ = false;
};
