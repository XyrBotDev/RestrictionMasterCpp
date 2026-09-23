#pragma once

#include <string>

namespace Keyboards {

std::string mainMenu();

std::string premiumMenu();

std::string settingsMenu();

std::string botList();

std::string botControl(
    int botNumber
);

std::string maintenanceMenu(
    int botNumber
);

std::string dummyRefresh();

std::string rangeDownloadControls();

}
