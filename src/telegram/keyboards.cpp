#include "../../include/keyboards.hpp"

namespace Keyboards {

std::string mainMenu() {
    return "MAIN_MENU";
}

std::string premiumMenu() {
    return "PREMIUM_MENU";
}

std::string botList() {
    return "BOT_LIST";
}

std::string botControl(int botNumber) {
    return "BOT_CONTROL_" + std::to_string(botNumber);
}

std::string maintenanceMenu(int botNumber) {
    return "MAINTENANCE_" + std::to_string(botNumber);
}

std::string rangeDownloadControls() {
    return "RANGE_DOWNLOAD_CONTROLS";
}

}
