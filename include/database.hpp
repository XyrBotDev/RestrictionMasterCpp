#pragma once

#include "user.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace mongocxx {
class instance;
class client;
}

class Database {
public:
    Database(const std::string& mongoUri,
             const std::string& databaseName);

    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

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

    std::unique_ptr<mongocxx::instance> instance_;
    std::unique_ptr<mongocxx::client> client_;
};
