#pragma once

#include <cstdint>
#include <unordered_set>

class AccessControl {
public:
    explicit AccessControl(std::int64_t ownerId = 0);

    bool isOwner(std::int64_t userId) const;
    bool isAdmin(std::int64_t userId) const;
    bool isPremium(std::int64_t userId) const;
    bool isBanned(std::int64_t userId) const;

    bool isMaintenance() const;
    void setMaintenance(bool enabled);

    bool addAdmin(std::int64_t userId);
    bool removeAdmin(std::int64_t userId);

private:
    std::int64_t ownerId_ = 0;

    std::unordered_set<std::int64_t> admins_;
    std::unordered_set<std::int64_t> premiumUsers_;
    std::unordered_set<std::int64_t> bannedUsers_;

    bool maintenance_ = false;
};
