#pragma once

#include <cstdint>
#include <string>

namespace ProgressFormatter {

std::string formatDownload(
    std::int64_t completedBytes,
    std::int64_t totalBytes,
    double speedMb
);

std::string formatUpload(
    std::int64_t completedBytes,
    std::int64_t totalBytes,
    double speedMb
);

}
