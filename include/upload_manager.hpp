#pragma once

#include <cstdint>
#include <string>

struct UploadProgress {
    std::int64_t completed_bytes = 0;
    std::int64_t total_bytes = 0;

    double speed_mb = 0.0;
    double percent = 0.0;

    bool stopped = false;
    bool completed = false;
};

class UploadManager {
public:
    UploadManager() = default;

    bool start(
        std::int64_t userId,
        const std::string& filePath
    );

    bool stop(std::int64_t userId);

    UploadProgress getProgress(
        std::int64_t userId
    ) const;

    bool isUploading(
        std::int64_t userId
    ) const;

private:
    UploadProgress progress_;
    std::int64_t activeUserId_ = 0;
};
