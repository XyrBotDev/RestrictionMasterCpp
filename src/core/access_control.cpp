#include "../../include/access_control.hpp"

AccessControl::AccessControl(std::int64_t ownerId)
    : ownerId_(ownerId) {}

bool AccessControl::isOwner(std::int64_t userId) const {
    return userId == ownerId_;
}

bool AccessControl::isAdmin(std::int64_t userId) const {
    return isOwner(userId) ||
           admins_.find(userId) != admins_.end();
}

bool AccessControl::isPremium(std::int64_t userId) const {
    return premiumUsers_.find(userId) != premiumUsers_.end();
}

bool AccessControl::isBanned(std::int64_t userId) const {
    return bannedUsers_.find(userId) != bannedUsers_.end();
}

bool AccessControl::isMaintenance() const {
    return maintenance_;
}

void AccessControl::setMaintenance(bool enabled) {
    maintenance_ = enabled;
}

bool AccessControl::addAdmin(std::int64_t userId) {
    if (userId == ownerId_) {
        return false;
    }

    return admins_.insert(userId).second;
}

bool AccessControl::removeAdmin(std::int64_t userId) {
    return admins_.erase(userId) > 0;
}
