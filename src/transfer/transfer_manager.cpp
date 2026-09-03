#include "../../include/transfer_manager.hpp"

bool TransferManager::startDownload(
    std::int64_t userId,
    const std::string& url
) {
    return downloadManager_.start(userId, url);
}

bool TransferManager::stopDownload(std::int64_t userId) {
    return downloadManager_.stop(userId);
}

bool TransferManager::startUpload(
    std::int64_t userId,
    const std::string& filePath
) {
    return uploadManager_.start(userId, filePath);
}

bool TransferManager::stopUpload(std::int64_t userId) {
    return uploadManager_.stop(userId);
}

TransferStatus TransferManager::getStatus(
    std::int64_t userId,
    TransferType type
) const {
    TransferStatus status;
    status.userId = userId;
    status.type = type;

    if (type == TransferType::Download) {
        const auto progress = downloadManager_.getProgress(userId);

        status.completedBytes = progress.completed_bytes;
        status.totalBytes = progress.total_bytes;
        status.speedMb = progress.speed_mb;
        status.percent = progress.percent;
        status.active = downloadManager_.isDownloading(userId);
        status.stopped = progress.stopped;
        status.completed = progress.completed;
    } else {
        const auto progress = uploadManager_.getProgress(userId);

        status.completedBytes = progress.completed_bytes;
        status.totalBytes = progress.total_bytes;
        status.speedMb = progress.speed_mb;
        status.percent = progress.percent;
        status.active = uploadManager_.isUploading(userId);
        status.stopped = progress.stopped;
        status.completed = progress.completed;
    }

    return status;
}

bool TransferManager::isActive(
    std::int64_t userId,
    TransferType type
) const {
    if (type == TransferType::Download) {
        return downloadManager_.isDownloading(userId);
    }

    return uploadManager_.isUploading(userId);
}
