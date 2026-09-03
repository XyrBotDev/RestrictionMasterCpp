#include "../../include/progress_formatter.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace {

double calculatePercent(
    std::int64_t completedBytes,
    std::int64_t totalBytes
) {
    if (totalBytes <= 0) {
        return 0.0;
    }

    return std::clamp(
        static_cast<double>(completedBytes) * 100.0 /
        static_cast<double>(totalBytes),
        0.0,
        100.0
    );
}

std::string createBar(double percent) {
    constexpr int width = 20;

    const int filled = static_cast<int>(
        (percent / 100.0) * width
    );

    std::string bar;

    for (int i = 0; i < width; ++i) {
        bar += (i < filled) ? "█" : "░";
    }

    return bar;
}

std::string formatMb(std::int64_t bytes) {
    const double mb =
        static_cast<double>(bytes) / (1024.0 * 1024.0);

    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << mb << " MB";

    return out.str();
}

std::string buildProgress(
    const std::string& title,
    std::int64_t completedBytes,
    std::int64_t totalBytes,
    double speedMb
) {
    const double percent =
        calculatePercent(completedBytes, totalBytes);

    std::ostringstream out;

    out << title << "\n\n"
        << createBar(percent) << " "
        << std::fixed << std::setprecision(1)
        << percent << "%\n\n"
        << "📦 "
        << formatMb(completedBytes)
        << " / "
        << formatMb(totalBytes)
        << "\n"
        << "⚡ Speed: "
        << std::fixed << std::setprecision(2)
        << speedMb
        << " MB/s";

    return out.str();
}

}

namespace ProgressFormatter {

std::string formatDownload(
    std::int64_t completedBytes,
    std::int64_t totalBytes,
    double speedMb
) {
    return buildProgress(
        "📥 Downloading...",
        completedBytes,
        totalBytes,
        speedMb
    );
}

std::string formatUpload(
    std::int64_t completedBytes,
    std::int64_t totalBytes,
    double speedMb
) {
    return buildProgress(
        "📤 Uploading...",
        completedBytes,
        totalBytes,
        speedMb
    );
}

}
