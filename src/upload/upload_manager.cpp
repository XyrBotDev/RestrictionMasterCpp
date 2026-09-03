#include "../../include/upload_manager.hpp"

#include <chrono>
#include <unordered_map>

namespace {

struct ActiveUpload {
    UploadProgress progress;
    std::chrono::steady_clock::time_point startTime;
};

std::unordered_map<std::int64_t, ActiveUpload> uploads;

}

bool UploadManager::start(
    std::int64_t userId,
    const std::string& filePath
) {
    if (userId == 0 || filePath.empty()) {
        return false;
    }

    ActiveUpload upload;
    upload.startTime = std::chrono::steady_clock::now();
    upload.progress = {};

    uploads[userId] = upload;

    activeUserId_ = userId;
    progress_ = upload.progress;

    return true;
}

bool UploadManager::stop(std::int64_t userId) {
    auto it = uploads.find(userId);

    if (it == uploads.end()) {
        return false;
    }

    it->second.progress.stopped = true;

    if (userId == activeUserId_) {
        progress_ = it->second.progress;
    }

    return true;
}

UploadProgress UploadManager::getProgress(
    std::int64_t userId
) const {
    auto it = uploads.find(userId);

    if (it == uploads.end()) {
        return {};
    }

    return it->second.progress;
}

bool UploadManager::isUploading(
    std::int64_t userId
) const {
    auto it = uploads.find(userId);

    if (it == uploads.end()) {
        return false;
    }

    const auto& progress = it->second.progress;

    return !progress.stopped && !progress.completed;
}
