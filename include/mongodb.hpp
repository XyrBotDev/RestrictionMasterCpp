#pragma once

#include <string>

class MongoDB {
public:
    MongoDB(
        const std::string& uri,
        const std::string& database_name
    );

    bool connect();
    bool isConnected() const;

private:
    std::string uri_;
    std::string database_name_;
    bool connected_ = false;
};
