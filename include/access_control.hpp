#pragma once

#include <cstdint>

class AccessControl {
public:
    bool isOwner(std::int64_t userId) const;

    bool isAdmin(std::int64_t userId) const;

    bool isPremium(std::int64_t userId) const;

    bool isBanned(std::int64_t userId) const;

    bool isMaintenance() const;

    void setMaintenance(bool enabled);

private:
    bool maintenance_ = false;
};
