#include "../../include/download_manager.hpp"

#include <algorithm>
#include <chrono>
#include <unordered_map>

namespace {

struct ActiveDownload {
    DownloadProgress progress;
    std::chrono::steady_clock::time_point startTime;
};

std::unordered_map<std::int64_t, ActiveDownload> downloads;

}

bool DownloadManager::start(
    std::int64_t userId,
    const std::string& url
) {
    if (userId == 0 || url.empty()) {
        return false;
    }

    ActiveDownload download;
    download.startTime = std::chrono::steady_clock::now();
    download.progress = {};

    downloads[userId] = download;

    activeUserId_ = userId;
    progress_ = download.progress;

    return true;
}

bool DownloadManager::stop(std::int64_t userId) {
    auto it = downloads.find(userId);

    if (it == downloads.end()) {
        return false;
    }

    it->second.progress.stopped = true;

    if (userId == activeUserId_) {
        progress_ = it->second.progress;
    }

    return true;
}

DownloadProgress DownloadManager::getProgress(
    std::int64_t userId
) const {
    auto it = downloads.find(userId);

    if (it == downloads.end()) {
        return {};
    }

    return it->second.progress;
}

bool DownloadManager::isDownloading(
    std::int64_t userId
) const {
    auto it = downloads.find(userId);

    if (it == downloads.end()) {
        return false;
    }

    const auto& progress = it->second.progress;

    return !progress.stopped && !progress.completed;
}
