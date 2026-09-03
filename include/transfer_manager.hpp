#pragma once

#include "download_manager.hpp"
#include "upload_manager.hpp"

#include <cstdint>
#include <string>

enum class TransferType {
    Download,
    Upload
};

struct TransferStatus {
    TransferType type = TransferType::Download;
    std::int64_t userId = 0;

    std::int64_t completedBytes = 0;
    std::int64_t totalBytes = 0;

    double speedMb = 0.0;
    double percent = 0.0;

    bool active = false;
    bool stopped = false;
    bool completed = false;
};

class TransferManager {
public:
    bool startDownload(
        std::int64_t userId,
        const std::string& url
    );

    bool stopDownload(std::int64_t userId);

    bool startUpload(
        std::int64_t userId,
        const std::string& filePath
    );

    bool stopUpload(std::int64_t userId);

    TransferStatus getStatus(
        std::int64_t userId,
        TransferType type
    ) const;

    bool isActive(
        std::int64_t userId,
        TransferType type
    ) const;

private:
    DownloadManager downloadManager_;
    UploadManager uploadManager_;
};
