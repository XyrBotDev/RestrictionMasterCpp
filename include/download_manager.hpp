#pragma once

#include <cstdint>
#include <string>

struct DownloadProgress {
    std::int64_t completed_bytes = 0;
    std::int64_t total_bytes = 0;

    double speed_mb = 0.0;
    double percent = 0.0;

    bool stopped = false;
    bool completed = false;
};

class DownloadManager {
public:
    DownloadManager() = default;

    bool start(
        std::int64_t userId,
        const std::string& url
    );

    bool stop(std::int64_t userId);

    DownloadProgress getProgress(
        std::int64_t userId
    ) const;

    bool isDownloading(
        std::int64_t userId
    ) const;

private:
    DownloadProgress progress_;
    std::int64_t activeUserId_ = 0;
};
